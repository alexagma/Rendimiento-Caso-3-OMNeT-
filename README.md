# Rendimiento-Caso-3-OMNeT-

El objetivo de este trabajo consiste en la simulación de una red de colas utilizando el programa OMNeT ++.

## 1) Contenido del paquete

Formado por 6 campos:

- Ruta del paquete (route).
- Identificador del paquete (seq). 
- Número de saltos desde el origen al destino (hops).
- Número de errores durante el trayecto (errors).
- Tiempo que ha tardado el paquete en llegar al destino (lifetime).
- Habilitador del rutado del paquete (enableRoute).

## 2) Topología de la red

La red hace uso del protocolo SW distinguiendo 3 módulos donde se van a tener 3 tipos de paquetes: de datos (1), acks (2) y nacks (3). 

- Source: genera el flujo de paquetes.

  - Función initialize(): inicializa la lambda y el número de muestras y hace la llamada y crea el flujo de los paquetes haciendo uso de las funciones getFlow() y getPacket(). Con scheduleAt() se programan los paquetes creados para los tiempos de llegada obtenidos.
  - Función getFlow(): es la encargada de obtener el tiempo entre llegadas de los paquetes utilizando la distrbibucción exponencial.
  - Función getPacket(): crea el paquete asignándole un tamaño de 1024 bits e inicializa los campos de la ruta e identificador del paquete. Por otra parte, habilita el rutado y asigna como tipo de paquete el de datos con el método setKind() a 1.    
  - Función handleMessage(): cada vez que recibe un mensaje lo castea a la clase del paquete y lo envía con send().

- Sender: nodos intermedios de la red con colas para almacenar los paquetes. Cada nodo tiene asignada una probabilidad para cada salida. Son los nodos 1,2 y 5.

  - Función initialize(): inicializa los canales, colas y probabilidades de cada nodo.
  - Función sendPacket(): comprueba si el canal está ocupado y si no lo está envía el paquete al siguiente salto actualizando los campos de la ruta y saltos. El envío se hace duplicando el paquete que está en la cola mediante la función dup().
  - Función continueRoute(): elige cual va a ser el siguiente salto utilizando las probabilidades de los enlaces y el índice de la puerta. Si la cola esta vacía inserta con insert() el paquete y hace la llamada a sendPacket() y si no, lo inserta y espera su turno para enviarlo.
  - Función getFirstQueuePacket(): en el caso de que la cola no esté vacía obtiene el primer paquete mediante front() y si ha habido errores actualiza los campos de saltos y errores y finaliza haciendo la llamada a sendPacket().
  - Función handleMessage(): en primer lugar recibe el mensaje, lo castea a la clase del paquete y obtiene la puerta del canal con getArrivalGate() y su índice con getIndex() para el envío de los paquetes. Si el paquete tiene habilitada la ruta ya puede reenviarse, se pone el campo a false y se hace la llamada a continueRoute() y se sale de la función con return. En función del tipo de paquete que recibe se hacen la siguientes acciones:
  
    - Datos: comprueba si tiene fallos con hasbitError() y si no tiene envía una ack y hace la llamada a continueRoute(). En el caso contrario, se envía un nack.
    - Ack: si la cola no está vacía borra el paquete de la cola con pop() y llama a getFirstQueuePacket() para obtener el siguiente. 
    - Nack: obtiene el primer paquete de la cola con getFirstQueuePacket() e inicializa el campo de los errores.
        
- Receiver: nodos finales de la red siendo el 3 y 4.

  - Función createFile(): añade los campos del paquete recibido al archivo results.txt.
  - Función sendAck(): envía un ack, actualiza el campo de la ruta, establece el campo del tiempo de duración, añade sus valores de errores y satlos a los histogramas de los errores y saltos y hace la llamada a createFile().
  - Función sendNack(): envía un nack.
  - Función handleMessage(): si el paquete recibido es de datos ejecuta la función sendAck() o sendNack() en función de si tiene error o no.
  - Función finish(): se ejecuta al finalizar la simulación y guarda los histogramas.

## 3) Simulación

Los parámetros utilizados para la simulación son los siguientes:

- λ = 2 paquetes/s
- 100 muestras
- Capacidad del enlace: 9600 bps
- Delay del enlace: 100 µs
- PER = 0.05

Tras simularlo se observa que el 75,33 % de los paquetes (226 de 300) llegan al nodo 4, esto es debido a que el nodo 4 está conectado con los nodos 2 y 5 mientras que el nodo 3 solo está conectado al 2 con una probabilidad de llegada de un 1/3. En cuánto al número de saltos que necesitan los paquetes para llegar a los nodos destino se han obtenido los siguientes resultados:

| Destino | 1 salto | 2 saltos | 3 saltos | 4 saltos | 5 saltos | Media |
| :------------ | :-----: | :------: | :------: | :------: | :------: | ----: |
| Nodo 3        | 36       | 21        | 14        | 3        | 0        | 1,78  |
| Nodo 4        | 113       | 76        | 29        | 7        | 1        | 1,70  |

Tras observar la tabla se concluye que el número medio de saltos es cercano 2 pero ha habido varios errores en la transmisión ya que el número de saltos máximo tendría que ser 3 debido a la ruta 1-5-2-4. Así pues, analizando los valores obtenidos del campo de los errores se concluye que ha habido 29 durante la simulación (9,66%):

| Destino | 0 errores | 1 error | 2 errores | Media |
| :------------ | :-----: | :------: | :------: | ----: |
| Nodo 3        | 66       | 8        | 0        | 0,10  |
| Nodo 4        | 205       | 19        | 2          | 0,10  |

Por último, en la siguiente tabla se tienen las estadísticas de los primeros y últimos paquetes que llegan a los nodos destino:

| Destino | Ruta | Paquete | Saltos | Errores | Tiempo de llegada |
| :------------ | :-----: | :------: | :------: |  :------: | ----: |
| Nodo 3        | Fuente 2 - Nodo 2        | 4        | 1 | 0  | 0,10  |
| Nodo 3        | Fuente 1 - Nodo 1 - Nodo 5 - Nodo 2       | 97        | 4 | 1  | 9,72 s |
| Nodo 4        | Fuente 2 - Nodo 2       | 1       | 1 | 0  | 0,10  |
| Nodo 4        | Fuente 1 - Nodo 1 - Nodo 5 - Nodo 2       | 99        | 3 | 0  | 15,17 s  |

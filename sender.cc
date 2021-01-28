
#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include "paquete_m.h"

using namespace omnetpp;

class Sender : public cSimpleModule{
private:
    cChannel *channel[2];
    cQueue *queue[2];
    double probability;
protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void continueRoute(Paquete *packet);
    virtual void getFirstQueuePacket(int gateIndex,int error);
    virtual void sendPacket(Paquete *packet, int gateIndex);
};

Define_Module(Sender);

void Sender::initialize(){
     channel[0] = gate("out", 0) -> getTransmissionChannel();
     channel[1] = gate("out", 1) -> getTransmissionChannel();
     queue[0] = new cQueue("queueZero");
     queue[1] = new cQueue("queueOne");
     probability = (double) par("probability");
}

void Sender::handleMessage(cMessage *msg){
     Paquete *packet = check_and_cast<Paquete *> (msg);
     cGate *arrivalGate = packet -> getArrivalGate();
     int arrivalGateIndex = arrivalGate -> getIndex();
     if (packet->getEnableRoute()) {
           packet -> setEnableRoute(false);
           continueRoute(packet);
           return;
     }
     if (packet -> getKind() == 1) {
         if (packet -> hasBitError()) {
             bubble("ERROR! , ENVIAR NACK");
             Paquete *nak = new Paquete("nack");
             nak -> setKind(3);
             send(nak, "out", arrivalGateIndex);
         }
         else {
             bubble("CORRECTO! , ENVIAR ACK");
             Paquete *ack = new Paquete("ack");
             ack -> setKind(2);
             send(ack, "out", arrivalGateIndex);
             continueRoute(packet);
         }
     }
     else if (packet -> getKind() == 2) {
         if (queue[arrivalGateIndex] -> isEmpty())
             bubble("ACK RECIBIDO , COLA VACÍA");
         else {
             bubble("ACK RECIBIDO , BORRAR PAQUETE COLA");
             queue[arrivalGateIndex] -> pop();
             getFirstQueuePacket(arrivalGateIndex,0);
         }
     }
     else {
         bubble("NACK RECIBIDO");
         getFirstQueuePacket(arrivalGateIndex,1);
     }
}

void Sender::continueRoute(Paquete *packet) {
    int gateIndex;
    if (uniform(0, 1) < probability)
        gateIndex = 0;
    else
        gateIndex = 1;
    if (queue[gateIndex] -> isEmpty()) {
        bubble("COLA VACÍA , ENVIAR PAQUETE");
        queue[gateIndex] -> insert(packet);
        sendPacket(packet, gateIndex);
    } else {
        bubble("COLA OCUPADA , AGREGAR PAQUETE");
        queue[gateIndex] -> insert(packet);
    }
}

void Sender::getFirstQueuePacket(int gateIndex,int error) {
    if (queue[gateIndex] -> isEmpty())
        bubble("COLA VACÍA");
    else {
        Paquete *packet = check_and_cast<Paquete *> (queue[gateIndex] -> front());
        packet -> setErrors(packet -> getErrors() + error);
        packet -> setHops(packet -> getHops() + error);
        bubble("OBTENER PAQUETE COLA");
        sendPacket(packet, gateIndex);
    }
}

void Sender::sendPacket(Paquete *packet, int gateIndex) {
    if (channel[gateIndex] -> isBusy()) {
        bubble("CANAL OCUPADO");
    } else {
        Paquete *newpacket = check_and_cast<Paquete *> (packet -> dup());
        std::string route1 = newpacket -> getRoute();
        std::string route2 = route1.append("-");
        route2.append(getName());
        newpacket -> setRoute(route2.data());
        newpacket -> setHops(newpacket -> getHops() + 1);
        bubble("CONTINUAR RUTA");
        send(newpacket, "out", gateIndex);
    }
}



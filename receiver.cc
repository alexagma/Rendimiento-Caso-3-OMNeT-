
#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include "paquete_m.h"

using namespace omnetpp;

class Receiver : public cSimpleModule{
protected:
    virtual void handleMessage(cMessage *msg);
    virtual void sendNack(int arrivalGateIndex);
    virtual void sendAck(int arrivalGateIndex,Paquete *packet);
};

Define_Module(Receiver);

void Receiver::handleMessage(cMessage *msg){
    Paquete * packet = check_and_cast<Paquete *>(msg);
    cGate *arrivalGate = packet -> getArrivalGate();
    int arrivalGateIndex = arrivalGate -> getIndex();
    if (packet -> getKind() == 1) {
        if (packet -> hasBitError()) {
            sendNack(arrivalGateIndex);
        }
        else {
            sendAck(arrivalGateIndex,packet);
        }
    }
    else{
        bubble("ERROR! FORMATO PAQUETE INCORRECTO");
    }
}

void Receiver::sendAck(int arrivalGateIndex,Paquete *packet){
    bubble("TRAYECTO FINALIZADO!, ENVIAR ACK");
    Paquete * ack = new Paquete("ack");
    ack -> setKind(2);
    packet -> setLifeTime(simTime().dbl());
    std::string route1 = packet -> getRoute();
    std::string route2 = route1.append("-");
    route2.append(getName());
    packet -> setRoute(route2.data());
    EV << "Ruta: " + std::string(packet -> getRoute()) + ", Paquete:" + std::to_string(packet -> getSeq()) + "\n";
    EV << "Duracion: " + std::to_string(packet -> getLifeTime()) + "\n";
    EV << "Saltos: " + std::to_string(packet -> getHops()) + "\n";
    EV << "Errores: " + std::to_string(packet -> getErrors()) + "\n";
    send(ack, "out",arrivalGateIndex);
}

void Receiver::sendNack(int arrivalGateIndex){
    bubble("ERROR! , ENVIAR NACK");
    Paquete * nack = new Paquete("nack");
    nack -> setKind(3);
    send(nack, "out",arrivalGateIndex);
}




#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <omnetpp.h>
#include "paquete_m.h"

using namespace omnetpp;

class Receiver : public cSimpleModule{
    private:
        cLongHistogram hops;
        cLongHistogram errors;

    protected:
        virtual void handleMessage(cMessage *msg) override;
        virtual void sendNack(int arrivalGateIndex);
        virtual void sendAck(int arrivalGateIndex,Paquete *packet);
        virtual void createFile(Paquete *packet);
        virtual void finish() override;
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
    hops.collect(packet->getHops());
    errors.collect(packet->getErrors());
    createFile(packet);
    send(ack, "out",arrivalGateIndex);
}

void Receiver::sendNack(int arrivalGateIndex){
    bubble("ERROR! , ENVIAR NACK");
    Paquete * nack = new Paquete("nack");
    nack -> setKind(3);
    send(nack, "out",arrivalGateIndex);
}

void Receiver::createFile(Paquete *packet){
    std::ofstream outfile ("results.txt",std::ios_base::app);
    outfile << "Ruta: " << std::string(packet->getRoute()) << endl;
    outfile << "Paquete: " << packet->getSeq() << endl;
    outfile << "Saltos: " << packet->getHops() << endl;
    outfile << "Errores: " << packet->getErrors() << endl;
    outfile << "Tiempo de llegada: " << packet->getLifeTime() << endl;
    outfile << "\n" << endl;
    outfile.close();
}

void Receiver::finish(){
    hops.recordAs("Hops count");
    errors.recordAs("Errors count");
}



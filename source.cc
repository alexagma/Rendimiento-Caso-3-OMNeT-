
#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include <random>
#include "paquete_m.h"

using namespace omnetpp;

class Source: public cSimpleModule
{
    private:
        int lambda;
        int samples;
        int seq = 0;

    protected:
        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;
        virtual std::vector<double> getFlow(int lambda, int samples);
        virtual Paquete * getPacket();
};

Define_Module(Source);



void Source::initialize(){
    lambda = (int) par("lambda");
    samples =  (int) par("samples");
    std::vector<double> flow = getFlow(lambda,samples);
    for(int i = 0;i <flow.size();i++)
    {
        Paquete *packet = getPacket();
        scheduleAt(flow[i], packet);
    }
}

void Source::handleMessage(cMessage * msg){
    Paquete *packet = check_and_cast<Paquete *> (msg);
    send(packet,"out");
}

std::vector<double> Source::getFlow(int lambda,int samples){
    std::vector<double> flow(samples);
    std::default_random_engine generator;
    std::uniform_real_distribution<> distribution(0.0, 1.0);
    for(int i = 0;i <flow.size();i++)
    {
        double random = distribution(generator);
        double nextdata = (-1/lambda) * log(random);
        if(i!=0)
            flow[i] = flow[i-1] + nextdata;
        else
            flow[i] = nextdata;
    }
    return flow;
}

Paquete * Source::getPacket(){
    char nombrePaquete[15];
    seq++;
    sprintf(nombrePaquete, "%s msg-%d",getName(),seq);
    Paquete * packet = new Paquete(nombrePaquete);
    packet -> setBitLength(1024);
    packet -> setKind(1);
    packet -> setSeq(seq);
    packet -> setEnableRoute(true);
    packet -> setRoute(getName());
    return packet;
}


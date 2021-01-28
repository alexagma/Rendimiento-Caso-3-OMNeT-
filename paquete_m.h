//
// Generated file, do not edit! Created by nedtool 5.6 from paquete.msg.
//

#ifndef __PAQUETE_M_H
#define __PAQUETE_M_H

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0506
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



/**
 * Class generated from <tt>paquete.msg:17</tt> by nedtool.
 * <pre>
 * packet Paquete
 * {
 *     int seq;
 *     int enableRoute = false;
 *     int hops;
 *     int errors;
 *     string route;
 *     double lifeTime;
 * }
 * </pre>
 */
class Paquete : public ::omnetpp::cPacket
{
  protected:
    int seq;
    int enableRoute;
    int hops;
    int errors;
    ::omnetpp::opp_string route;
    double lifeTime;

  private:
    void copy(const Paquete& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const Paquete&);

  public:
    Paquete(const char *name=nullptr, short kind=0);
    Paquete(const Paquete& other);
    virtual ~Paquete();
    Paquete& operator=(const Paquete& other);
    virtual Paquete *dup() const override {return new Paquete(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual int getSeq() const;
    virtual void setSeq(int seq);
    virtual int getEnableRoute() const;
    virtual void setEnableRoute(int enableRoute);
    virtual int getHops() const;
    virtual void setHops(int hops);
    virtual int getErrors() const;
    virtual void setErrors(int errors);
    virtual const char * getRoute() const;
    virtual void setRoute(const char * route);
    virtual double getLifeTime() const;
    virtual void setLifeTime(double lifeTime);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const Paquete& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, Paquete& obj) {obj.parsimUnpack(b);}


#endif // ifndef __PAQUETE_M_H

#ifndef __INETWORK_H__
#define __INETWORK_H__

#include <QtNetwork>
#include "Protocole.h"

class                       INetwork
{
public:
    virtual ~INetwork() {}
    virtual void            setServer(QString const&) = 0;
    virtual QString         getServer() const = 0;
    virtual void            setTcpPort(quint16) = 0;
    virtual void            setUdpPort(quint16) = 0;
    virtual quint16         getTcpPort() const = 0;
    virtual quint16         getUdpPort() const = 0;
    virtual void            setTcpSocket(QTcpSocket *) = 0;
    virtual void            setUdpSocket(QUdpSocket *) = 0;
    virtual QTcpSocket      *getTcpSocket() const = 0;
    virtual QUdpSocket      *getUdpSocket() const = 0;
    virtual void            sendDataTcp(type_CS, std::string const&, std::string const&) = 0;
    virtual void            sendDataUdp(type_CC _action, std::string const&, std::string const&, QHostAddress const&) = 0;
    virtual void            connectServer() const = 0;
};

#endif // INETWORK_H

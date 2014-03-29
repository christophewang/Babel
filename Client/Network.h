#ifndef __NETWORK_H__
#define __NETWORK_H__

#include "INetwork.h"

class                       Network: public INetwork
{
private:
    QTcpSocket              *_tcpSocket;
    QUdpSocket              *_udpSocket;
    QString                 _serverInfo;
    quint16                 _tcpPort;
    quint16                 _udpPort;
protected:
    ProtocoleSC             *_dataSC;
    ProtocoleCC             *_dataCC;
public:
    Network();
    virtual ~Network();
public:
    virtual void            setServer(QString const &);
    virtual QString         getServer() const;
    virtual void            setTcpPort(quint16);
    virtual void            setUdpPort(quint16);
    virtual quint16         getTcpPort() const;
    virtual quint16         getUdpPort() const;
    virtual void            setTcpSocket(QTcpSocket *);
    virtual void            setUdpSocket(QUdpSocket *);
    virtual QTcpSocket      *getTcpSocket() const;
    virtual QUdpSocket      *getUdpSocket() const;
    virtual void            sendDataTcp(type_CS, std::string const&, std::string const&);
    virtual void            sendDataUdp(type_CC _action, std::string const&, std::string const&, QHostAddress const&);
    virtual void            connectServer() const;
};

#endif // NETWORK_H

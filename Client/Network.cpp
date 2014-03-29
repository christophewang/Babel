#include <iostream>
#include "Network.h"

Network::Network()
    : _serverInfo("127.0.0.1"), _tcpPort(6666), _udpPort(6667)
{
    this->_tcpSocket = new QTcpSocket;
    this->_udpSocket = new QUdpSocket;
    this->_dataSC = new ProtocoleSC;
    this->_dataCC = new ProtocoleCC;
}

Network::~Network()
{
    this->_tcpSocket->close();
    this->_udpSocket->close();

    delete this->_dataCC;
    delete this->_dataSC;
}

void                Network::setServer(QString const &_serv)
{
    this->_serverInfo = _serv;
}

void                Network::setTcpPort(quint16 _port)
{
    this->_tcpPort = _port;
}

void                Network::setUdpPort(quint16 _port)
{
    this->_udpPort = _port;
}

QString             Network::getServer() const
{
    return this->_serverInfo;
}

quint16             Network::getTcpPort() const
{
    return this->_tcpPort;
}

quint16             Network::getUdpPort() const
{
    return this->_udpPort;
}

void                Network::setTcpSocket(QTcpSocket *_socket)
{
    this->_tcpSocket = _socket;
}

void                Network::setUdpSocket(QUdpSocket *_socket)
{
    this->_udpSocket = _socket;
}

QTcpSocket*         Network::getTcpSocket() const
{
    return this->_tcpSocket;
}

QUdpSocket*         Network::getUdpSocket() const
{
    return this->_udpSocket;
}

void                Network::connectServer() const
{
    this->_tcpSocket->abort();
    this->_tcpSocket->connectToHost(_serverInfo, _tcpPort);
}

void                Network::sendDataTcp(type_CS _action, std::string const& _id, std::string const& _infos)
{
    QByteArray      block;
    QDataStream     out(&block, QIODevice::WriteOnly);
    ProtocoleCS     _data;

    out.setVersion(QDataStream::Qt_4_7);
    out.setByteOrder(QDataStream::ByteOrder(QSysInfo::ByteOrder));
    _data._type = _action;
    memset(_data._id, 0, sizeof(_data._id));
    memset(_data._infos, 0, sizeof(_data._infos));
    copy(_id.begin(), _id.end(), _data._id);
    copy(_infos.begin(), _infos.end(), _data._infos);
    out << _data._type;
    out.writeRawData(_data._id, sizeof(_data._id));
    out.writeRawData(_data._infos, sizeof(_data._infos));
    this->_tcpSocket->write(block, sizeof(ProtocoleCS));
    this->_tcpSocket->waitForBytesWritten();
}

void                Network::sendDataUdp(type_CC _action, std::string const& _id, std::string const& _infos, QHostAddress const& _address)
{
    QByteArray      datagram;
    QDataStream     out(&datagram, QIODevice::WriteOnly);
    ProtocoleCC     _data;

    out.setVersion(QDataStream::Qt_4_7);
    out.setByteOrder(QDataStream::ByteOrder(QSysInfo::ByteOrder));
    _data._type = _action;
    memset(_data._id, 0, sizeof(_data._id));
    memset(_data._infos, 0, sizeof(_data._infos));
    copy(_id.begin(), _id.end(), _data._id);
    copy(_infos.begin(), _infos.end(), _data._infos);
    out << _data._type;
    out.writeRawData(_data._id, sizeof(_data._id));
    out.writeRawData(_data._infos, sizeof(_data._infos));
    this->_udpSocket->writeDatagram(datagram, _address, _udpPort);
}

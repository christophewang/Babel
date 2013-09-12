#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <QString>
#include <QStringList>
#include "Network.h"

class               Client : public Network
{
private:
    QString         _clientId;
    QString         _clientIp;
    QStringList     _contactsOn;
    QStringList     _contactsOff;
    QStringList     _contactsList;
public:
    Client();
    ~Client(){}
public:
    void            setContacts(QString const&);
    void            addContactList(QString const&);
    void            refreshContacts(QString const&);
    void            removeContactList(QString const&);
    void            setClientId(QString const&);
    void            setClientIp(QString const&);
    QString         getClientId() const;
    QString         getClientIp() const;
    QStringList     getContactOn() const;
    QStringList     getContactOff() const;
    void            call(QString const&, QHostAddress const&);
    void            hangup(QString const&, QHostAddress const&);
};

#endif // CLIENT_H

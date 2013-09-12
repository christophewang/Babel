#include <iostream>
#include <QString>
#include <QStringList>
#include "Client.h"

Client::Client()
    : _clientId("Anonymous"), _clientIp("127.0.0.1")
{
}

void                Client::setClientId(QString const& _id)
{
    this->_clientId = _id;
}

void                Client::setClientIp(QString const& _ip)
{
    this->_clientIp = _ip;
}

QString             Client::getClientId() const
{
    return this->_clientId;
}

QString             Client::getClientIp() const
{
    return this->_clientIp;
}

void                Client::setContacts(QString const& contactList)
{
    QStringList     clientTmp;

    this->_contactsList = contactList.split(";", QString::SkipEmptyParts);
    clientTmp = this->_contactsList.at(0).split("=", QString::SkipEmptyParts);
    this->_clientId = clientTmp.at(0);
    this->_clientIp = clientTmp.at(1);
    for (int i = 1; i < this->_contactsList.size(); ++i)
    {
        if (this->_contactsList.at(i).contains("=0"))
            this->_contactsOff << this->_contactsList.at(i).left(this->_contactsList.at(i).indexOf("="));
        else
            this->_contactsOn << this->_contactsList.at(i).left(this->_contactsList.at(i).indexOf("="));
    }
}

void                Client::addContactList(QString const& contact)
{
    this->_contactsList << contact;
    if (this->_contactsList.last().contains("=0"))
        this->_contactsOff << this->_contactsList.last().left(this->_contactsList.last().indexOf("="));
    else
        this->_contactsOn << this->_contactsList.last().left(this->_contactsList.last().indexOf("="));
}

void                Client::removeContactList(QString const& contact)
{
    for (int i = 1; i < this->_contactsList.size(); ++i)
    {
        if (this->_contactsList.at(i).left(this->_contactsList.at(i).indexOf("=")) == contact)
            this->_contactsList.removeAt(i);
    }
    this->_contactsOff.clear();
    this->_contactsOn.clear();
    for (int i = 1; i < this->_contactsList.size(); ++i)
    {
        if (this->_contactsList.at(i).contains("=0"))
            this->_contactsOff << this->_contactsList.at(i).left(this->_contactsList.at(i).indexOf("="));
        else
            this->_contactsOn << this->_contactsList.at(i).left(this->_contactsList.at(i).indexOf("="));
    }
}

void                Client::refreshContacts(QString const& contact)
{
    QStringList     tmp;

    tmp = contact.split("=", QString::SkipEmptyParts);
    for (int i = 1; i < this->_contactsList.size(); ++i)
    {
        if (this->_contactsList.at(i).left(this->_contactsList.at(i).indexOf("=")) == tmp.at(0))
        {
            this->_contactsList.removeAt(i);
            this->_contactsList << contact;
        }
    }
    this->_contactsOff.clear();
    this->_contactsOn.clear();
    for (int i = 1; i < this->_contactsList.size(); ++i)
    {
        if (this->_contactsList.at(i).contains("=0"))
            this->_contactsOff << this->_contactsList.at(i).left(this->_contactsList.at(i).indexOf("="));
        else
            this->_contactsOn << this->_contactsList.at(i).left(this->_contactsList.at(i).indexOf("="));
    }
}

QStringList         Client::getContactOn() const
{
    return this->_contactsOn;
}

QStringList         Client::getContactOff() const
{
    return this->_contactsOff;
}

void                Client::call(QString const& _name, QHostAddress const& _address)
{
    this->sendDataUdp(CALL, this->_clientId.toStdString(), _name.toStdString(), _address);
}

void                Client::hangup(QString const& _name, QHostAddress const& _address)
{
    this->sendDataUdp(FINISHCALL, this->_clientId.toStdString(), _name.toStdString(), _address);
}

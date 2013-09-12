#include <string>
#include <list>
#include <fstream>
#include "SClient.h"

SClient::SClient()
	: _connected(false), _client_manager(NULL)
{
	return ;
}

SClient::~SClient()
{
	return ;
}

void					SClient::addContactInit(std::string const & login)
{
	this->_contactList.push_back(login);
	return ;
}

std::string const &		SClient::getLogin()
{
	return (this->_login);
}

void					SClient::setLogin(std::string const & login)
{
	this->_login = login;
	return ;
}

std::string const &		SClient::getPassword()
{
	return (this->_password);
}

void					SClient::setPassword(std::string const & password)
{
	this->_password = password;
	return ;
}

void					SClient::clearContactList()
{
	this->_contactList.clear();
	return ;
}

void					SClient::setClientManager(ClientManager * client_manager)
{
	this->_client_manager = client_manager;
	return ;
}

ClientManager * const	SClient::getClientManager()
{
	return (this->_client_manager);
}

void					SClient::setConnected(bool connect)
{
	this->_connected = connect;
	return ;
}

bool					SClient::getConnected()
{
	return (this->_connected);
}

std::list<std::string> *	SClient::getContactList()
{
	return (&this->_contactList);
}

void					SClient::setIp(std::string const & ip)
{
	this->_ip = ip;
	return ;
}

std::string	const &		SClient::getIp()
{
	return (this->_ip);
}

void					SClient::addContact(std::string const & login)
{
	std::fstream		contact_db;

	this->_contactList.push_back(login);
	contact_db.open("contact.db", std::ios::in | std::ios::out | std::fstream::app);
	if (contact_db.is_open())
	{
		contact_db << login << std::endl;
		contact_db.close();
	}
	return ;
}

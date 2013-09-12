#ifndef					SCLIENT_H__
# define				SCLIENT_H__

#include <string>
#include "ClientManager.h"

class					ClientManager;

class					SClient
{
public :
	SClient();
	~SClient();
	void				addContactInit(std::string const & login);
	void				addContact(std::string const & login);
	std::string const &	getLogin();
	void				setLogin(std::string const & login);
	std::string const & getPassword();
	void				setPassword(std::string const & password);
	void				clearContactList();
	void				setClientManager(ClientManager * client_manager);
	ClientManager * const	getClientManager();
	void				setConnected(bool connect);
	bool				getConnected();
	std::list<std::string> *	getContactList();
	void				setIp(std::string const & ip);
	std::string	const &	getIp();

private :
	std::string			_login;
	std::string			_ip;
	std::string			_password;
	std::list<std::string>	_contactList;
	bool				_connected;
	ClientManager		*_client_manager;
};

#endif					/* SCLIENT_H__ */
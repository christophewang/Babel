#include <string>
#include <iostream>
#include <list>
#include <fstream>
#include <utility>
#include "SClient.h"
#include "QueryManager.h"
#include "ClientManager.h"
#include "Protocole.h"
#include "Server.h"

class					ClientManager;

QueryManager::QueryManager(std::list<SClient> & clientList, std::list<std::pair<std::string, std::string> > & waitingList)
	: _clientList(clientList), _waitingList(waitingList)
{
	_cmd_map[CONNECT] = &QueryManager::execConnect;
	_cmd_map[DISCONNECT] = &QueryManager::execDisconnect;
	_cmd_map[REGISTER] = &QueryManager::execRegister;
	_cmd_map[SEARCHFRIEND] = &QueryManager::execSearchFriend;
	_cmd_map[ASKTOADD] = &QueryManager::execAskToAdd;
	_cmd_map[ACCEPTFRIEND] = &QueryManager::execAcceptFriend;
	_cmd_map[DENIFRIEND] = &QueryManager::execDenyFriend;
	_cmd_map[DELFRIEND] = &QueryManager::execDelFriend;
	return ;
}

QueryManager::~QueryManager()
{
	return ;
}

void					QueryManager::execCmd(ClientManager * const client_manager, ProtocoleCS const & cmd)
{
	bool				success;

	success = false;
	for (std::map<type_CS, cmd_ptr>::iterator it = this->_cmd_map.begin(); it != this->_cmd_map.end(); ++it)
	{
		if (it->first == cmd._type)
		{
			(this->*(*it).second)(client_manager, cmd);
			success = true;
			break ;
		}
	}
	if (!success)
		this->execInvalidCommand(client_manager, cmd);
	return ;
}

std::string				QueryManager::_check_connection(std::string const & login, std::string const & from)
{
	std::string			tmp;
	std::list<std::string>	*tmp_contact_list;

	for (std::list<SClient>::iterator it = this->_clientList.begin(); it != this->_clientList.end(); ++it)
	{
		if (it->getLogin() == login)
		{
			if (it->getConnected())
			{
				tmp_contact_list = it->getContactList();
				for (std::list<std::string>::iterator it_contact = tmp_contact_list->begin(); it_contact != tmp_contact_list->end(); ++it_contact)
				{
					if (*it_contact == from)
						return (it->getIp());
				}
				return ("0");
			}
			else
				tmp = "0";
			break ;
		}
	}
	return (tmp);
}

void					QueryManager::_eraseWaiting(std::string const & login)
{
	for (std::list<std::pair<std::string, std::string> >::iterator it = this->_waitingList.begin(); it != this->_waitingList.end(); ++it)
	{
		if (it->second == login)
		{
			this->_waitingList.erase(it);
			it = this->_waitingList.begin();
		}
	}
	this->_saveWaitingList();
	return ;
}

void					QueryManager::_saveWaitingList()
{
	std::fstream		query_db;

	query_db.open("query.db", std::ios::in | std::ios::out | std::ios::trunc);
	if (query_db.is_open())
	{
		for (std::list<std::pair<std::string, std::string> >::iterator it = this->_waitingList.begin(); it != this->_waitingList.end(); ++it)
		{
			query_db << it->first << "\t" << it->second << std::endl;
		}
		query_db.close();
	}
	return ;
}

void					QueryManager::execConnect(ClientManager * const client_manager, ProtocoleCS const & cmd)
{
	std::string			login(cmd._id);
	std::string			pass(cmd._infos);
	std::string			buff;
	std::list<std::string>	*tmp_contact_list;
	std::string			ip;
	std::string			other_tmp;

	for (std::list<SClient>::iterator it = this->_clientList.begin(); it != this->_clientList.end(); ++it)
	{
		if (it->getLogin() == login && it->getPassword() == pass && it->getConnected() == false)
		{
			it->setConnected(true);
			it->setIp(client_manager->socket().remote_endpoint().address().to_string());
			it->setClientManager(client_manager);
			tmp_contact_list = it->getContactList();
			buff += login;
			buff += "=";
			buff += it->getIp();
			buff += ";";
			other_tmp += login;
			other_tmp += "=";
			other_tmp += it->getIp();
			for (std::list<std::string>::iterator it_contact = tmp_contact_list->begin(); it_contact != tmp_contact_list->end(); ++it_contact)
			{
				buff += *it_contact;
				buff += "=";
				ip = this->_check_connection(*it_contact, login);
				buff += ip;
				buff += ";";
				if (ip != "0")
				{
					for (std::list<SClient>::iterator it_other = this->_clientList.begin(); it_other != this->_clientList.end(); ++it_other)
					{
						if (it_other->getLogin() == *it_contact && it_other->getConnected() == true)
						{
							it_other->getClientManager()->send(REFRESH, other_tmp);
							break ;
						}
					}
				}
			}
			client_manager->send(CONNECT_SUCCES, buff);
			for (std::list<std::pair<std::string, std::string> >::iterator it_query = this->_waitingList.begin(); it_query != this->_waitingList.end(); ++it_query)
			{
				if (it_query->second == login)
					client_manager->send(ASKTOADD, it_query->first);
			}
			this->_eraseWaiting(login);
			return ;
		}
	}
	client_manager->send(CONNECT_FAILED, "");
	return ;
}

void					QueryManager::execDisconnect(ClientManager * const client_manager, ProtocoleCS const & cmd)
{
	std::string			ip(client_manager->socket().remote_endpoint().address().to_string());
	std::list<std::string>	*tmp_contact_list;
	std::string			other_tmp;

	for (std::list<SClient>::iterator it = this->_clientList.begin(); it != this->_clientList.end(); ++it)
	{
		if (it->getIp() == ip && it->getConnected() == true)
		{
			other_tmp += it->getLogin();
			other_tmp += "=0";
			it->setConnected(false);
			it->setIp("0");
			it->setClientManager(NULL);
			tmp_contact_list = it->getContactList();
			for (std::list<std::string>::iterator it_contact = tmp_contact_list->begin(); it_contact != tmp_contact_list->end(); ++it_contact)
			{
				for (std::list<SClient>::iterator it_other = this->_clientList.begin(); it_other != this->_clientList.end(); ++it_other)
				{
					if (it_other->getLogin() == *it_contact)
					{
						if (it_other->getConnected() == false)
							break ;
						it_other->getClientManager()->send(REFRESH, other_tmp);
						break ;
					}
				}
			}
		}
	}
	return ;
}

void					QueryManager::execRegister(ClientManager * const client_manager, ProtocoleCS const & cmd)
{
	std::string			login(cmd._id);
	std::string			password(cmd._infos);
	bool				success;
	SClient				tmp;
	std::fstream		user_db;

	success = true;
	for (std::list<SClient>::iterator it_client = this->_clientList.begin(); it_client != this->_clientList.end(); ++it_client)
	{
		if (it_client->getLogin() == login)
		{
			success = false;
			break ;
		}
	}
	if (success)
	{
		tmp.clearContactList();
		tmp.setLogin(login);
		tmp.setPassword(password);
		this->_clientList.push_back(tmp);
		user_db.open("user.db", std::ios::in | std::ios::out | std::fstream::app);
		if (user_db.is_open())
		{
			user_db << login << "\t" << password << std::endl;
			client_manager->send(REGISTER_SUCCES, "");
			user_db.close();
		}
		else
			client_manager->send(REGISTER_FAILED, "");
	}
	else
		client_manager->send(REGISTER_FAILED, "");
	return ;
}

void					QueryManager::execSearchFriend(ClientManager * const client_manager, ProtocoleCS const & cmd)
{
	std::string			id_search(cmd._infos);
	std::list<SClient>::iterator	it;

	it = this->_clientList.begin();
	while (it != this->_clientList.end())
	{
		if (it->getLogin() == id_search)
		{
			client_manager->send(SEARCHFRIEND_SUCCES, "");
			break ;
		}
		++it;
	}
	if (it == this->_clientList.end())
		client_manager->send(SEARCHFRIEND_FAILED, "");
	return ;
}

void					QueryManager::execAskToAdd(ClientManager * const client_manager, ProtocoleCS const & cmd)
{
	std::string			from(cmd._id);
	std::string			to(cmd._infos);
	std::fstream		query_db;

	for (std::list<SClient>::iterator it_client = this->_clientList.begin(); it_client != this->_clientList.end(); ++it_client)
	{
		if (it_client->getLogin() == from)
		{
			it_client->addContact(from);
			it_client->getClientManager()->send(ASKTOADD_SUCCES, "");
			break ;
		}
	}
	for (std::list<SClient>::iterator it_client = this->_clientList.begin(); it_client != this->_clientList.end(); ++it_client)
	{
		if (it_client->getLogin() == to)
		{
			if (it_client->getConnected())
				it_client->getClientManager()->send(ASKTOADD, from);
			else
			{
				query_db.open("query.db", std::ios::in | std::ios::out | std::fstream::app);
				if (query_db.is_open())
				{
					query_db << from << "\t" << to << std::endl;
					query_db.close();
				}
			}
			break ;
		}
	}
	return ;
}

void					QueryManager::execAcceptFriend(ClientManager * const client_manager, ProtocoleCS const & cmd)
{
	std::string			from;
	std::string			to;
	std::string			tmp_buff;

	tmp_buff = from;
	tmp_buff += "=";
	for (std::list<SClient>::iterator it_client = this->_clientList.begin(); it_client != this->_clientList.end(); ++it_client)
	{
		if (it_client->getLogin() == from)
		{
			tmp_buff += it_client->getIp();
			it_client->addContact(to);
			break ;
		}
	}
	for (std::list<SClient>::iterator it_client = this->_clientList.begin(); it_client != this->_clientList.end(); ++it_client)
	{
		if (it_client->getLogin() == to)
		{
			if (it_client->getConnected())
				it_client->getClientManager()->send(ADDFRIEND_SUCCES, tmp_buff);
			break ;
		}
	}
	return ;
}

void					QueryManager::execDenyFriend(ClientManager * const client_manager, ProtocoleCS const & cmd)
{
	std::string			from(cmd._id);
	std::string			to(cmd._infos);

	for (std::list<SClient>::iterator it_client = this->_clientList.begin(); it_client != this->_clientList.end(); ++it_client)
	{
		if (it_client->getLogin() == to)
		{
			if (it_client->getConnected())
				it_client->getClientManager()->send(ADDFRIEND_FAILED, from);
			break ;
		}
	}
	return ;
}

void					QueryManager::execDelFriend(ClientManager * const client_manager, ProtocoleCS const & cmd)
{
	std::string			login(cmd._id);
	std::string			to_suppress(cmd._infos);
	std::string			buff_disc;
	std::list<std::string>	*tmp_contact_list;
	bool				success;

	success = false;
	buff_disc = login;
	buff_disc = "=0;";
	for (std::list<SClient>::iterator it_client = this->_clientList.begin(); it_client != this->_clientList.end(); ++it_client)
	{
		if (it_client->getLogin() == login)
		{
			tmp_contact_list = it_client->getContactList();
			for (std::list<std::string>::iterator it_contact = tmp_contact_list->begin(); it_contact != tmp_contact_list->end(); ++it_contact)
			{
				if (*it_contact == to_suppress)
				{
					success = true;
					tmp_contact_list->erase(it_contact);
					for (std::list<SClient>::iterator it_disconnect = this->_clientList.begin(); it_disconnect != this->_clientList.end(); ++it_disconnect)
					{
						if (it_disconnect->getLogin() == to_suppress)
						{
							if (it_disconnect->getConnected())
								it_disconnect->getClientManager()->send(REFRESH, buff_disc);
							else
								break ;
						}
					}
					break ;
				}
			}
		}
	}
	if (success)
	{
		client_manager->send(DELFRIEND_SUCCES, "");
		this->_saveContactList();
	}
	else
		client_manager->send(DELFRIEND_FAILED, "");
	return ;
}

void					QueryManager::execInvalidCommand(ClientManager * const client_manager, ProtocoleCS const & cmd)
{
	client_manager->send(INVALIDCOMMAND, "");
	return ;
}

void					QueryManager::_saveContactList()
{
	std::fstream		contact_db;
	std::string			buff;
	std::list<std::string>	*tmp_contact_list;

	contact_db.open("contact.db", std::ios::in | std::ios::out | std::ios::trunc);
	if (contact_db.is_open())
	{
		for (std::list<SClient>::iterator it_client = this->_clientList.begin(); it_client != this->_clientList.end(); ++ it_client)
		{
			tmp_contact_list = it_client->getContactList();
			for (std::list<std::string>::iterator it_contact = tmp_contact_list->begin(); it_contact != tmp_contact_list->end(); ++ it_contact)
			{
				buff = it_client->getLogin();
				buff += "\t";
				buff += *it_contact;
				contact_db << buff << std::endl;
			}
		}
		contact_db.close();
	}
	return ;
}

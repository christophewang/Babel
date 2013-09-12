#include <iostream>
#include <fstream>
#include <list>
#include "Server.h"
#include "QueryManager.h"

Server::Server(const int port)
	: _port(port)
{
	return ;
}

Server::~Server()
{
	return ;
}

void					Server::launch()
{
	std::cout << "Launching server on port " << this->_port << std::endl;
	this->_loadDB();
	QueryManager		query_manager(this->_clientList, this->_waitingList);
	NetworkManager		network_manager(this->_port, query_manager);
	std::cout << "Server Ready" << std::endl;
	network_manager.run();
	return ;
}

void					Server::_loadDB()
{
	std::fstream			user_db;
	std::fstream			contact_db;
	std::fstream			query_db;
	std::list<s_load_db>	user_list;
	std::list<s_load_db>	contact_list;
	s_load_db				tmp;
	std::pair<std::string, std::string>	waiting_tmp;

	std::cout << "Loading contact...";
	user_db.open("user.db", std::ios::in | std::fstream::app);
	contact_db.open("contact.db", std::ios::in | std::fstream::app);
	query_db.open("query.db", std::ios::in | std::fstream::app);
	if (user_db.is_open())
	{
		while (user_db)
		{
			user_db >> tmp.first;
			user_db >> tmp.second;
			if (user_db)
				user_list.push_back(tmp);
		}
		user_db.close();
	}
	if (contact_db.is_open())
	{
		while (contact_db)
		{
			contact_db >> tmp.first;
			contact_db >> tmp.second;
			if (contact_db)
				contact_list.push_back(tmp);
		}
		contact_db.close();
	}
	this->_fillClientList(user_list, contact_list);
	std::cout << "Loading pending task...";
	if (query_db.is_open())
	{
		while (query_db)
		{
			query_db >> waiting_tmp.first;
			query_db >> waiting_tmp.second;
			if (query_db)
				this->_waitingList.push_back(waiting_tmp);
		}
		query_db.close();
	}
	std::cout << " Done" << std::endl;
	return ;
}

void				Server::_fillClientList(std::list<s_load_db> & user_list, std::list<s_load_db> & contact_list)
{
	SClient			tmp;

	std::cout << " Done" << std::endl << "Creating contact list...";
	for (std::list<s_load_db>::iterator it_user = user_list.begin(); it_user != user_list.end(); ++it_user)
	{
		tmp.clearContactList();
		tmp.setLogin(it_user->first);
		tmp.setPassword(it_user->second);
		for (std::list<s_load_db>::iterator it_contact = contact_list.begin(); it_contact != contact_list.end(); ++it_contact)
		{
			if (it_contact->first == it_user->first)
				tmp.addContactInit(it_contact->second);
		}
		this->_clientList.push_back(tmp);
	}
	std::cout << " Done" << std::endl;
	return ;
}
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include "NetworkManager.h"
#include "QueryManager.h"

NetworkManager::NetworkManager(const int port, QueryManager & query_manager)
: _io_service(),_acceptor(_io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)), _query_manager(query_manager)
{
	startAccept();
	return ;
}

NetworkManager::~NetworkManager()
{
	return ;
}

void					NetworkManager::startAccept()
{
	_new_connection.reset(new ClientManager(_io_service, this->_query_manager));

	this->_acceptor.async_accept(_new_connection->socket(), boost::bind(&NetworkManager::handleAccept, this, boost::asio::placeholders::error));
	return ;
}

void					NetworkManager::handleAccept(const boost::system::error_code & error)
{
	if (!error)
	{
		std::cout << "Client connecte" << std::endl;
		_new_connection->start();
		this->startAccept();
	}
	return ;
}

void					NetworkManager::run()
{
	this->_io_service.run();
	return ;
}
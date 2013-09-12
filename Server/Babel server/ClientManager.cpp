#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include "ClientManager.h"
#include "QueryManager.h"
#include "Protocole.h"

ClientManager::ClientManager(boost::asio::io_service & io_service, QueryManager & query_manager)
	: _socket(io_service), _query_manager(query_manager)
{
	return ;
}

ClientManager::~ClientManager()
{
	return ;
}

void					ClientManager::start()
{
	this->_socket.async_read_some(boost::asio::buffer(_buffer),
		boost::bind(&ClientManager::handleRead, shared_from_this(),
		boost::asio::placeholders::error,
		boost::asio::placeholders::bytes_transferred));
	return ;
}

void					ClientManager::handleRead(const boost::system::error_code& error, std::size_t bytes_transferred)
{
	ProtocoleCS		*proto_tmp;

	if (!error)
	{
		proto_tmp = static_cast<ProtocoleCS *>(static_cast<void *>(&this->_buffer));
		this->_query_manager.execCmd(this, *proto_tmp);
		this->_socket.async_read_some(boost::asio::buffer(_buffer),
			boost::bind(&ClientManager::handleRead, shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
	}
	else
	{
		this->_query_manager.execDisconnect(this, *proto_tmp);
		this->_socket.close();
	}
}

void					ClientManager::handleWrite(const boost::system::error_code & error)
{
	if (error)
	{
		std::cout << "Send Error" << std::endl;
	}
}

boost::asio::ip::tcp::socket &			ClientManager::socket()
{
	return (_socket);
}

void					ClientManager::send(type_CS type, std::string const & msg)
{
	ProtocoleSC			proto;
	char				*tmp;

	std::cout << "SEND : " << type << " " << msg << std::endl;
	proto._type = type;
	memset(proto._infos, 0, 1024);
	memcpy(proto._infos, msg.c_str(), 1024);
	tmp = static_cast<char *>(static_cast<void *>(&proto));
	boost::asio::async_write(this->_socket, boost::asio::buffer(tmp, sizeof(proto)),
			boost::bind(&ClientManager::handleWrite, shared_from_this(),
			boost::asio::placeholders::error));
	return ;
}

#ifndef					NETWORKMANAGER_H__
# define				NETWORKMANAGER_H__

#include <boost/asio.hpp>
#include <boost/smart_ptr/enable_shared_from_this.hpp>
#include "ClientManager.h"
#include "QueryManager.h"

class					ClientManager;
class					QueryManager;

typedef boost::shared_ptr<ClientManager>	ClientManager_ptr;

class					NetworkManager : public boost::enable_shared_from_this<NetworkManager>
{
public :
	NetworkManager(const int port, QueryManager & query_manager);
	~NetworkManager();

	void				run();

private :
	boost::asio::io_service		_io_service;
	boost::asio::ip::tcp::acceptor	_acceptor;
	ClientManager_ptr				_new_connection;
	QueryManager &					_query_manager;

	void				startAccept();
	void				handleAccept(const boost::system::error_code & error);
};

#endif					/* NETWORKMANAGER_H__ */
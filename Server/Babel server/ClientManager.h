#ifndef					CLIENTMANAGER_H__
# define				CLIENTMANAGER_H__

#include <boost/asio.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/enable_shared_from_this.hpp>
#include <boost/array.hpp>
#include "QueryManager.h"
#include "Protocole.h"

class					ClientManager : public boost::enable_shared_from_this<ClientManager>
{
public :
	ClientManager(boost::asio::io_service & io_service, QueryManager & query_manager);
	~ClientManager();

	void				start();
	boost::asio::ip::tcp::socket &				socket();
	void				send(type_CS, std::string const &);

private :
	boost::asio::ip::tcp::socket	_socket;
	boost::array<char, 1080>		_buffer;
	QueryManager &					_query_manager;

	void							handleRead(const boost::system::error_code & error, std::size_t bytes_transferred);
	void							handleWrite(const boost::system::error_code & error);
};

typedef boost::shared_ptr<ClientManager>	ClientManager_ptr;

#endif					/* CLIENTMANAGER_H__ */
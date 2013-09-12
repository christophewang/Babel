#define	SERVER_PORT		6666
#define _WIN32_WINNT	0x0501

#include <boost/asio.hpp>
#include <iostream>
#include "Server.h"

int					main()
{
	Server			server(SERVER_PORT);

	std::cout << "-- Babel Server --" << std::endl;
	server.launch();
	return (0);
}
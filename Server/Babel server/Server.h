#ifndef					SERVER_H__
# define				SERVER_H__

#include <string>
#include <list>
#include <utility>
#include "NetworkManager.h"
#include "SClient.h"

class					SClient;

struct					s_load_db
{
	std::string			first;
	std::string			second;
};

class					Server
{
public :
	Server(const int port);
	~Server();

	void				launch();

private :
	int					_port;

	void				_loadDB();
	void				_fillClientList(std::list<s_load_db> &, std::list<s_load_db> &);
	std::list<SClient>	_clientList;
	std::list<std::pair<std::string, std::string> >	_waitingList;
};

#endif					/* SERVER_H__ */

#ifndef					QUERYMANAGER_H__
# define				QUERYMANAGER_H__

#include <string>
#include <list>
#include <utility>
#include "ClientManager.h"
#include "SClient.h"
#include "Protocole.h"
#include "Server.h"

class					SClient;
class					ClientManager;

class					QueryManager
{
public :
	QueryManager(std::list<SClient> &, std::list<std::pair<std::string, std::string> > &);
	~QueryManager();

	void				execCmd(ClientManager * const client_manager, ProtocoleCS const & cmd);
	void				execDisconnect(ClientManager * const client_manager, ProtocoleCS const & cmd);

private :
	typedef void (QueryManager::*cmd_ptr)(ClientManager * const client_manager, ProtocoleCS const & cmd);

	std::list<SClient> &		_clientList;
	std::list<std::pair<std::string, std::string> > &			_waitingList;
	std::map<type_CS, cmd_ptr>	_cmd_map;
	std::string					_check_connection(std::string const & login, std::string const & from);
	void						_saveContactList();
	void						_eraseWaiting(std::string const & login);
	void						_saveWaitingList();

	void				execConnect(ClientManager * const client_manager, ProtocoleCS const & cmd);
	void				execRegister(ClientManager * const client_manager, ProtocoleCS const & cmd);
	void				execSearchFriend(ClientManager * const client_manager, ProtocoleCS const & cmd);
	void				execAskToAdd(ClientManager * const client_manager, ProtocoleCS const & cmd);
	void				execAcceptFriend(ClientManager * const client_manager, ProtocoleCS const & cmd);
	void				execDenyFriend(ClientManager * const client_manager, ProtocoleCS const & cmd);
	void				execDelFriend(ClientManager * const client_manager, ProtocoleCS const & cmd);
	void				execInvalidCommand(ClientManager * const client_manager, ProtocoleCS const & cmd);
};

#endif					/* QUERYMANAGER_H__ */

//#pragma once

#ifndef _PROTOCOLE_H_
# define _PROTOCOLE_H_

#include <list>

 enum type_CS
{
	CONNECT,
	CONNECT_SUCCES,
	CONNECT_FAILED,
	DISCONNECT,
	REGISTER,
	REGISTER_SUCCES = 5,
	REGISTER_FAILED,
	SEARCHFRIEND, // le client A cherche le client B et envoie au serveur le ID du client B
	SEARCHFRIEND_SUCCES, // le serveur trouve le ID et renvoie SEARCHFRIEND_SUCCES
	SEARCHFRIEND_FAILED, // le serveur ne trouve pas le ID et renvoie SEARCHFRIEND_FAILED
	ASKTOADD = 10,
	ASKTOADD_SUCCES,
	ASKTOADD_FAILED,// le client A fait une demande d'ajout d'amie il envoie au serveur le ID du client B, le serveur envoie au client B ASKTOADD : ID (client A)
	ACCEPTFRIEND, // le client B envoie ACCEPTFRIEND au serveur si il accepte
	DENIFRIEND, // le client B envoie DENIFRIEND au serveur si il refuse
	ADDFRIEND_SUCCES = 15, // le serveur envoie au client A si demande accepter
	ADDFRIEND_FAILED, // le serveur envoie au client A si demande refuser
	DELFRIEND,
	DELFRIEND_SUCCES,
	DELFRIEND_FAILED,
	REFRESH = 20,
	INVALIDCOMMAND
	//BLOCKFRIEND,
	//BLOCKFRIEND_SUCCES,
	//BLOCKFRIEND_FAILED,
	//UNBLOCKFRIEND,
	//UNBLOCKFRIEND_SUCCES,
	//UNBLOCKFRIEND_FAILED,	
	//CHANGESTATUE,
	//CHANGESTATUE_SUCCES,
	//CHANGESTATUE_FAILED,	
};

struct ProtocoleCS // cest la structure envoye par le client
{
	type_CS _type;
	char	_id[50];
	char	_infos[1024];
};

struct ProtocoleSC // cest la structure qui contient la reponse de serveur au client
{
	type_CS	_type;
	char	_infos[1024];
};

#endif // _PROTOCOLE_H_
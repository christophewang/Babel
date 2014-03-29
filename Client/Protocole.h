#ifndef __PROTOCOLE_H__
#define __PROTOCOLE_H__

#include <list>

enum type_CS
{
        CONNECT = 0,// _id[] contient id de client, et  _infos[] contient le mot de passe
        CONNECT_SUCCES = 1, // ip de client;ami=0;ami1=ip_amis2;
        CONNECT_FAILED = 2,
        DISCONNECT = 3, // _id[] contient id de client , rien dans _infos[]
        REGISTER = 4, // _id[] contient id de client, _infos[] le pwd
        REGISTER_SUCCES = 5,
        REGISTER_FAILED = 6,
        SEARCHFRIEND = 7, // le client A cherche le client B et envoie au serveur le ID du client B
        SEARCHFRIEND_SUCCES = 8, // le serveur trouve le ID et renvoie SEARCHFRIEND_SUCCES
        SEARCHFRIEND_FAILED = 9, // le serveur ne trouve pas le ID et renvoie SEARCHFRIEND_FAILED
        ASKTOADD = 10, // le client A fait une demande d'ajout d'amie il envoie au serveur le ID du client B, le serveur envoie au client B ASKTOADD : ID (client A)
        ASKTOADD_SUCCES = 11,
        ASKTOADD_FAILED = 12,
        ACCEPTFRIEND = 13, // le client B envoie ACCEPTFRIEND au serveur si il accepte
        DENIFRIEND = 14, // le client B envoie DENIFRIEND au serveur si il refuse
        ADDFRIEND_SUCCES = 15, // le serveur envoie au client A si demande accepter info = ip
        ADDFRIEND_FAILED = 16, // le serveur envoie au client A si demande refuser
        DELFRIEND = 17, // _id[] id de client , _infos[] id de ami a supprimer
        DELFRIEND_SUCCES = 18,
        DELFRIEND_FAILED = 19,
        REFRESH = 20,
        INVALIDCOMMAND = 21
        //SOUNDTOCLIENT = 22,
        //MESSAGETOCLIENT = 23,
        //MESSAGETOCLIENTFAILED = 24
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

enum type_CC
{
        CALL = 0,
        ACCEPTCALL = 1,
        DENIEDCALL = 2,
        CANCELCALL = 3,
        FINISHCALL = 4,
        INVALID = 5
};

struct  ProtocoleCC
{
        type_CC _type;
        char    _id[50];
        char    _infos[1024];
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


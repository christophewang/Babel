#ifndef __LOGINWIN_H__
#define __LOGINWIN_H__

#include "IGUI.h"
#include "UserWin.h"
#include "RegisterWin.h"

class           LoginWin : public QWidget, public IGUI
{
    Q_OBJECT
 private:
     QLabel          _logo;
     QLineEdit       _id;
     QLineEdit       _pass;
     QLineEdit       _server;
     QSpinBox        _port;
     QPushButton     _login;
     QPushButton     _register;
     QHBoxLayout     _buttonLayout;
     QHBoxLayout     _logoLayout;
     QFormLayout     _userLayout;
     QFormLayout     _serverLayout;
     QGroupBox       _userGroup;
     QGroupBox       _serverGroup;
     QVBoxLayout     _mainLayout;
     UserWin         *userWin;
     RegisterWin     *registerWin;
 public:
     LoginWin();
     virtual        ~LoginWin();
     virtual void    manageWin();
 private slots:
     void            checkLogin();
     void            connectAccount();
     void            loginSuccess(std::string const&);
     void            launchRegister();
     void            connectFail(QAbstractSocket::SocketError);
     void            receivedTcpData();
};

#endif // LOGINWIN_H

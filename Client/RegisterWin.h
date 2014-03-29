#ifndef __REGISTERWIN_H__
#define __REGISTERWIN_H__

#include "IGUI.h"
#include <QDialog>
#include <QtWidgets>

class               RegisterWin : public QDialog, public IGUI
{
    Q_OBJECT
private:
    QLineEdit       _id;
    QLineEdit       _pass;
    QLineEdit       _repass;
    QLineEdit       _server;
    QSpinBox        _port;
    QPushButton     _register;
    QPushButton     _cancel;
    QHBoxLayout     _buttonLayout;
    QFormLayout     _serverLayout;
    QFormLayout     _userLayout;
    QGroupBox       _serverGroup;
    QGroupBox       _userGroup;
    QVBoxLayout     _mainLayout;
public:
    RegisterWin();
    virtual         ~RegisterWin() {}
    virtual void    manageWin();
private slots:
    void            sendInfo();
    void            doRegister();
    void            receivedTcpData();
    void            cancelRegister();
    void            connectFail(QAbstractSocket::SocketError);
};

#endif // REGISTERWIN_H

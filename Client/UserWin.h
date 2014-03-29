#ifndef __USERWIN_H__
#define __USERWIN_H__

#include "IGUI.h"
#include <QMainWindow>
#include <QtWidgets>

class                       UserWin : public QMainWindow, public IGUI
{
    Q_OBJECT
private:
    QMenu                   *_mContact;
    QMenu                   *_mAccount;
    QMenu                   *_mHelp;
    QAction                 *_aAddContact;
    QAction                 *_aDelContact;
    QAction                 *_aSearchContact;
    QAction                 *_aQuit;
    QAction                 *_aDisconnect;
    QAction                 *_aHelp;
    QAction                 *_aProfile;
    QVBoxLayout             _layout;
    QListView               _viewOn;
    QListView               _viewOff;
    QStringListModel        _modelOn;
    QStringListModel        _modelOff;
    QWidget                 _core;
    QPushButton             _call;
    QItemSelectionModel     *_selectedModel;
    QModelIndex             _index;
    QVariant                _selected;
    QLabel                  _infoLabel;
    QLabel                  _idLabel;
    QLabel                  _online;
    QLabel                  _offline;
    QString                 _serverText;
    QString                 _portText;
    QString                 _idText;
    QString                 _connectedContact;
    QDialog                 _callWin;
    QDialog                 _receiveWin;
    QDialog                 _acceptedWin;
    QTimer                  timer;
    QString                 _scontact;
public:
    UserWin();
    virtual ~UserWin();
    virtual void            manageWin();
public slots:
    void                    showProfile();
    void                    showHelp();
    void                    addContact();
    void                    delContact();
    void                    searchContact();
    void                    selectContact();
    void                    callContact(QString const &);
    void                    receiveCall(QString);
    void                    acceptedCall();
    void                    aDisconnect();
    void                    receivedTcpData();
    void                    receivedUdpData();
    void                    receivedVoiceData();
    void                    sendVoiceData();
    void                    askToAddClient();
    void                    refreshList();
    void                    cancelCall();
    void                    acceptCall();
    void                    deniedCall();
    void                    finishCall();
};

#endif // USER_WIN

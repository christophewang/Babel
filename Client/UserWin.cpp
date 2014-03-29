#include <iostream>
#include "IGUI.h"
#include "UserWin.h"

UserWin::UserWin()
{
    this->_mContact = menuBar()->addMenu("&Contacts");
    this->_mAccount = menuBar()->addMenu("&Comptes");
    this->_mHelp = menuBar()->addMenu("&Aide");
    this->_aAddContact = new QAction("&Ajouter un contact", this);
    this->_aDelContact = new QAction("&Supprimer un contact", this);
    this->_aSearchContact = new QAction("&Rechercher un contact", this);
    this->_aDisconnect = new QAction("&Se deconnecter", this);
    this->_aQuit = new QAction("&Quitter", this);
    this->_aHelp = new QAction("&A propos de", this);
    this->_aProfile = new QAction("&Voir son profil", this);
}

UserWin::~UserWin()
{
    delete this->_mContact;
    delete this->_mAccount;
    delete this->_mHelp;
    delete this->_aAddContact;
    delete this->_aDelContact;
    delete this->_aSearchContact;
    delete this->_aDisconnect;
    delete this->_aQuit;
    delete this->_aHelp;
    delete this->_aProfile;
    delete this->_selectedModel;
}

void                UserWin::manageWin()
{
    this->_serverText = "Serveur: ";
    this->_serverText.append(this->getServer());
    this->_portText = " | Port: ";
    this->_portText.append(QString::number(this->getTcpPort()));
    this->_idText = "Identifiant: ";
    this->_idText.append(this->getClientId());
    this->_call.setText("Appeler");
    this->_mContact->addAction(this->_aAddContact);
    this->_mContact->addAction(this->_aDelContact);
    this->_mContact->addAction(this->_aSearchContact);
    this->_mContact->addAction(this->_aDisconnect);
    this->_mContact->addAction(this->_aQuit);
    this->_mAccount->addAction(this->_aProfile);
    this->_mHelp->addAction(this->_aHelp);
    this->_online.setText("Contacts en ligne");
    this->_offline.setText("Contacts hors ligne");
    this->_idLabel.setText(this->_idText);
    this->_infoLabel.setText(this->_serverText.append(this->_portText));
    this->_call.setFont(QFont("Default", 14));
    this->_call.setCursor(Qt::PointingHandCursor);
    this->_call.setIcon(QIcon(QCoreApplication::applicationDirPath() + "/ressources/Logo.png"));
    this->_modelOn.setStringList(this->getContactOn());
    this->_modelOff.setStringList(this->getContactOff());
    this->_viewOn.setModel(&this->_modelOn);
    this->_viewOn.setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->_viewOff.setModel(&this->_modelOff);
    this->_viewOff.setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->_viewOff.setSelectionMode(QAbstractItemView::NoSelection);
    this->_layout.addWidget(&this->_infoLabel);
    this->_layout.addWidget(&this->_idLabel);
    this->_layout.addWidget(&this->_online, Qt::AlignCenter);
    this->_layout.addWidget(&this->_viewOn);
    this->_layout.addWidget(&this->_offline, Qt::AlignCenter);
    this->_layout.addWidget(&this->_viewOff);
    this->_layout.addWidget(&this->_call);
    this->_core.setLayout(&this->_layout);
    this->getUdpSocket()->bind(this->getUdpPort());
    this->setCentralWidget(&this->_core);
    this->setWindowIcon(QIcon(QCoreApplication::applicationDirPath() + "/ressources/babybel.png"));
    connect(&this->_call, SIGNAL(clicked()), this, SLOT(selectContact()));
    connect(this->_aQuit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(this->_aAddContact, SIGNAL(triggered()), this, SLOT(addContact()));
    connect(this->_aDelContact, SIGNAL(triggered()), this, SLOT(delContact()));
    connect(this->_aSearchContact, SIGNAL(triggered()), this, SLOT(searchContact()));
    connect(this->_aDisconnect, SIGNAL(triggered()), this, SLOT(aDisconnect()));
    connect(this->_aHelp, SIGNAL(triggered()), this, SLOT(showHelp()));
    connect(this->_aProfile, SIGNAL(triggered()), this, SLOT(showProfile()));
    connect(this->getTcpSocket(), SIGNAL(readyRead()), this, SLOT(receivedTcpData()));
    connect(this->getUdpSocket(), SIGNAL(readyRead()), this, SLOT(receivedUdpData()));
}

void                UserWin::selectContact()
{
    this->_selectedModel = this->_viewOn.selectionModel();
    this->_index = this->_selectedModel->currentIndex();
    this->_selected = this->_modelOn.data(this->_index, Qt::DisplayRole);
    this->_connectedContact = "";
    this->callContact(this->_selected.toString());
}

void                UserWin::callContact(QString const& _contact)
{
    QLabel          _logo;
    QLabel          _text;
    QPushButton     _cancel;
    QVBoxLayout     _mainLayout;
    QHBoxLayout     _logoLayout;
    QHBoxLayout     _buttonLayout;
    QHBoxLayout     _textLayout;

    if (!_selected.isNull())
    {
        this->_connectedContact = _contact;
        this->_callWin.setFixedSize(300, 350);
        _cancel.setText("Annuler");
        _logo.setPixmap(QPixmap(QCoreApplication::applicationDirPath() + "/ressources/Lcall.png"));
        _logoLayout.setAlignment(Qt::AlignCenter);
        _logoLayout.addWidget(&_logo);
        this->_connectedContact.append(": Appel en cours ...");
        _text.setText(this->_connectedContact);
        _text.setAlignment(Qt::AlignCenter);
        _textLayout.addWidget(&_text);
        _cancel.setCursor(Qt::PointingHandCursor);
        _buttonLayout.setAlignment(Qt::AlignBottom);
        _buttonLayout.addWidget(&_cancel);
        _mainLayout.addLayout(&_logoLayout);
        _mainLayout.addLayout(&_textLayout);
        _mainLayout.addLayout(&_buttonLayout);
        this->_callWin.setLayout(&_mainLayout);
        this->_callWin.setWindowTitle("Appel en cours ...");
        this->_callWin.setWindowIcon(QIcon(QCoreApplication::applicationDirPath() + "/ressources/babybel.png"));
        this->call(this->getClientId(), QHostAddress ("127.0.0.1"));
        connect(&_cancel, SIGNAL(clicked()), this, SLOT(cancelCall()));
        this->_callWin.exec();
    }
}

void                UserWin::cancelCall()
{
    this->sendDataUdp(CANCELCALL, QString(this->_dataCC->_id).toStdString(), QString (this->_dataCC->_infos).toStdString(), QHostAddress ("127.0.0.1"));
    this->_callWin.close();
}

void                UserWin::receiveCall(QString _contact)
{
    QLabel          _logo;
    QLabel          _text;
    QPushButton     _accept;
    QPushButton     _cancel;
    QVBoxLayout     _mainLayout;
    QHBoxLayout     _logoLayout;
    QHBoxLayout     _buttonLayout;
    QHBoxLayout     _textLayout;

    this->_connectedContact = _contact;
    this->_receiveWin.setFixedSize(300, 350);
    _accept.setText("Accepter");
    _cancel.setText("Annuler");
    _logo.setPixmap(QPixmap(QCoreApplication::applicationDirPath() + "/ressources/Lreceive.png"));
    _logoLayout.setAlignment(Qt::AlignCenter);
    _logoLayout.addWidget(&_logo);
    _contact.append(" cherche à vous joindre ...");
    _text.setText(_contact);
    _text.setAlignment(Qt::AlignCenter);
    _textLayout.addWidget(&_text);
    _cancel.setCursor(Qt::PointingHandCursor);
    _buttonLayout.setAlignment(Qt::AlignBottom);
    _buttonLayout.addWidget(&_accept);
    _buttonLayout.addWidget(&_cancel);
    _mainLayout.addLayout(&_logoLayout);
    _mainLayout.addLayout(&_textLayout);
    _mainLayout.addLayout(&_buttonLayout);
    this->_receiveWin.setLayout(&_mainLayout);
    this->_receiveWin.setWindowTitle("Appel en cours ...");
    this->_receiveWin.setWindowIcon(QIcon(QCoreApplication::applicationDirPath() + "/ressources/babybel.png"));
    connect(&_accept, SIGNAL(clicked()), this, SLOT(acceptCall()));
    connect(&_cancel, SIGNAL(clicked()), this, SLOT(deniedCall()));
    this->_receiveWin.exec();
}

void                    UserWin::acceptCall()
{
    this->_receiveWin.close();
    this->sendDataUdp(ACCEPTCALL, QString(this->_dataCC->_id).toStdString(), QString (this->_dataCC->_infos).toStdString(), QHostAddress ("127.0.0.1"));
    this->acceptedCall();
}

void                    UserWin::deniedCall()
{
    this->_receiveWin.close();
    this->sendDataUdp(DENIEDCALL, QString(this->_dataCC->_id).toStdString(), QString (this->_dataCC->_infos).toStdString(), QHostAddress ("127.0.0.1"));
}

void                    UserWin::acceptedCall()
{
    QLabel              _logo;
    QLabel              _text;
    QPushButton         _hangup;
    QVBoxLayout         _mainLayout;
    QHBoxLayout         _logoLayout;
    QHBoxLayout         _buttonLayout;
    QHBoxLayout         _textLayout;

    this->_acceptedWin.setFixedSize(300, 350);
    _hangup.setText("Raccrocher");
    _logo.setPixmap(QPixmap(QCoreApplication::applicationDirPath() + "/ressources/Lphone.png"));
    _logoLayout.setAlignment(Qt::AlignCenter);
    _logoLayout.addWidget(&_logo);
    this->_connectedContact.append(": Connecté !!");
    _text.setText(this->_connectedContact);
    _text.setAlignment(Qt::AlignCenter);
    _textLayout.addWidget(&_text);
    _hangup.setCursor(Qt::PointingHandCursor);
    _buttonLayout.setAlignment(Qt::AlignBottom);
    _buttonLayout.addWidget(&_hangup);
    _mainLayout.addLayout(&_logoLayout);
    _mainLayout.addLayout(&_textLayout);
    _mainLayout.addLayout(&_buttonLayout);
    this->_acceptedWin.setLayout(&_mainLayout);
    this->_acceptedWin.setWindowTitle("Connecté !!");
    this->_acceptedWin.setWindowIcon(QIcon(QCoreApplication::applicationDirPath() + "/ressources/babybel.png"));
    connect(&_hangup, SIGNAL(clicked()), this, SLOT(finishCall()));
    disconnect(this->getUdpSocket(), SIGNAL(readyRead()), this, SLOT(receivedUdpData()));
    connect(&this->timer, SIGNAL(timeout()), this, SLOT(receivedVoiceData()));
    connect(&this->timer, SIGNAL(timeout()), this, SLOT(sendVoiceData()));
    this->timer.start(1000);
    this->_acceptedWin.exec();
}

void                    UserWin::finishCall()
{
    disconnect(&this->timer, SIGNAL(timeout()), this, SLOT(receivedVoiceData()));
    disconnect(&this->timer, SIGNAL(timeout()), this, SLOT(sendVoiceData()));
    connect(this->getUdpSocket(), SIGNAL(readyRead()), this, SLOT(receivedUdpData()));
    this->sendDataUdp(FINISHCALL, QString(this->_dataCC->_id).toStdString(), QString (this->_dataCC->_infos).toStdString(), QHostAddress ("127.0.0.1"));
    this->_acceptedWin.close();
}


void                    UserWin::sendVoiceData()
{
    QByteArray          datagram;
    QDataStream         out(&datagram, QIODevice::WriteOnly);
    QString             data("HELLO");

    out.setVersion(QDataStream::Qt_4_1);
    out << data;
    /*Envoie des données en UDP Client to Client*/
    this->getUdpSocket()->writeDatagram(datagram, QHostAddress ("127.0.0.1"), this->getUdpPort());
    this->getUdpSocket()->waitForBytesWritten();
}

void                    UserWin::receivedVoiceData()
{
    QByteArray          datagram;
    QDataStream         in(&datagram, QIODevice::ReadOnly);
    QString             data;

    do {
    datagram.resize(this->getUdpSocket()->pendingDatagramSize());
    this->getUdpSocket()->readDatagram(datagram.data(), datagram.size());
    } while (this->getUdpSocket()->hasPendingDatagrams());

    in.setVersion(QDataStream::Qt_4_1);
    in >> data;
    /*Reception des données en UDP et lecture des données*/
    std::cout << data.toStdString() << std::endl;
}

void                    UserWin::aDisconnect()
{
    this->sendDataTcp(DISCONNECT, this->getClientId().toStdString(), "");
    this->getTcpSocket()->close();
    this->close();
}

void                    UserWin::showProfile()
{
    QDialog             profile(this);
    QVBoxLayout         layout;
    QLabel              title("Mon Profil");
    QLabel              id("Login: ");
    QLabel              ip("Adresse IP: ");
    QLabel              idValue;
    QLabel              ipValue;

    idValue.setText(this->getClientId());
    ipValue.setText(this->getClientIp());
    title.setAlignment(Qt::AlignHCenter);
    idValue.setAlignment(Qt::AlignCenter);
    ipValue.setAlignment(Qt::AlignCenter);
    layout.addWidget(&title);
    layout.addWidget(&id);
    layout.addWidget(&idValue);
    layout.addWidget(&ip);
    layout.addWidget(&ipValue);
    profile.setLayout(&layout);
    profile.setFixedSize(200, 125);
    profile.setWindowIcon(QIcon(QCoreApplication::applicationDirPath() + "/ressources/babybel.png"));
    profile.setWindowTitle("Profil");
    profile.exec();
}

void                    UserWin::showHelp()
{
    QDialog             help(this);
    QVBoxLayout         layout;
    QLabel              copyright;

    copyright.setText("Projet Babel: phol_r, thai_j, yos_d, sok_f, wang_c");
    copyright.setAlignment(Qt::AlignCenter);
    layout.addWidget(&copyright);
    help.setLayout(&layout);
    help.setWindowIcon(QIcon(QCoreApplication::applicationDirPath() + "/ressources/babybel.png"));
    help.setWindowTitle("A Propos de");
    help.exec();
}

void                    UserWin::addContact()
{
    bool                ok = false;
    QString             contact = QInputDialog::getText(this, "Ajout", "Indiquez l'ID du contact à ajouter:", QLineEdit::Normal, QString(), &ok);

    if (ok && !contact.isEmpty() && contact != getClientId())
        this->sendDataTcp(ASKTOADD, this->getClientId().toStdString(), contact.toStdString());
    else
        QMessageBox::warning(this, "Ajout de Contact", "Votre demande a échouée.", QMessageBox::Close);
}

void                    UserWin::delContact()
{
    bool                ok = false;

    _scontact = QInputDialog::getText(this, "Suppresion", "Indiquez l'ID du contact à supprimer:", QLineEdit::Normal, QString(), &ok);
    if (ok && !_scontact.isEmpty())
        this->sendDataTcp(DELFRIEND, this->getClientId().toStdString(), _scontact.toStdString());
}

void                    UserWin::searchContact()
{
    bool                ok = false;
    QString             contact = QInputDialog::getText(this, "Recherche", "Indiquez l'ID du contact:", QLineEdit::Normal, QString(), &ok);

    if (ok && !contact.isEmpty())
        this->sendDataTcp(SEARCHFRIEND, this->getClientId().toStdString(), contact.toStdString());
}

void                    UserWin::askToAddClient()
{
    int                 response;
    QString             question("Acceptez vous la demande d'ajout du contact ");
    QString             contact(this->_dataSC->_infos);

    question.append(contact);
    response = QMessageBox::question(this, "Demande d'ajout", question, QMessageBox::Yes | QMessageBox::No);
    if (response == QMessageBox::Yes)
        {
            this->addContactList(contact);
            this->_modelOn.setStringList(this->getContactOn());
            this->_modelOff.setStringList(this->getContactOff());
            this->sendDataTcp(ACCEPTFRIEND, this->getClientId().toStdString(), contact.toStdString());
        }
    else if (response == QMessageBox::No)
        this->sendDataTcp(DENIFRIEND, this->getClientId().toStdString(), contact.toStdString());
}

void                    UserWin::refreshList()
{
    std::string         contacts(_dataSC->_infos);

    this->refreshContacts(QString::fromStdString(contacts));
    this->_modelOn.setStringList(this->getContactOn());
    this->_modelOff.setStringList(this->getContactOff());
}

void                    UserWin::receivedUdpData()
{
    char                buffer[sizeof(ProtocoleCC)];
    QByteArray          datagram;
    QDataStream         in(&datagram, QIODevice::ReadOnly);

    this->_dataCC->_type = INVALID;
    memset(this->_dataCC->_id, 0, sizeof(this->_dataCC->_id));
    memset(this->_dataCC->_infos, 0, sizeof(this->_dataCC->_infos));
    do {
        datagram.resize(this->getUdpSocket()->pendingDatagramSize());
        this->getUdpSocket()->readDatagram(datagram.data(), datagram.size());
    } while (this->getUdpSocket()->hasPendingDatagrams());
    in.setVersion(QDataStream::Qt_4_7);
    in.setByteOrder(QDataStream::ByteOrder(QSysInfo::ByteOrder));
    in.readRawData(buffer, sizeof(buffer));
    this->_dataCC = static_cast<ProtocoleCC *>(static_cast<void*>(buffer));
    switch (this->_dataCC->_type)
    {
        case CALL:
            this->receiveCall(QString(this->_dataCC->_infos));
            break;
        case ACCEPTCALL:
            {
                this->_callWin.close();
                this->acceptedCall();
                break;
            }
        case DENIEDCALL:
            {
                this->_callWin.close();
                QMessageBox::information(this, "Appel", "Votre appel a ete rejete.", QMessageBox::Close);
                break;
            }
        case CANCELCALL:
            this->_receiveWin.close();
            break;
        case FINISHCALL:
            this->_acceptedWin.close();
            break;
        default:
            std::cout << "Type non reconnu CC" << this->_dataCC << std::endl;
    }
}

void                    UserWin::receivedTcpData()
{
    char                buffer[sizeof(ProtocoleSC)];
    QDataStream         in(this->getTcpSocket());

    this->_dataSC->_type = INVALIDCOMMAND;
    memset(this->_dataSC->_infos, 0, sizeof(this->_dataSC->_infos));
    in.setVersion(QDataStream::Qt_4_7);
    in.setByteOrder(QDataStream::ByteOrder(QSysInfo::ByteOrder));
    in.readRawData(buffer, sizeof(buffer));
    this->_dataSC = static_cast<ProtocoleSC *>(static_cast<void*>(buffer));
    switch (this->_dataSC->_type)
    {
        case SEARCHFRIEND_SUCCES:
            QMessageBox::information(this, "Recherche de Contact", "Votre contact est bien enregistre sur le serveur.", QMessageBox::Close);
            break;
        case SEARCHFRIEND_FAILED:
            QMessageBox::warning(this, "Recherche de Contact", "Votre contact est introuvable.", QMessageBox::Close);
            break;
        case ASKTOADD_FAILED:
            QMessageBox::warning(this, "Ajout de Contact", "Votre demande a échouée.", QMessageBox::Close);
            break;
        case ASKTOADD:
            this->askToAddClient();
            break;
        case ADDFRIEND_SUCCES:
            {
                std::string         contact(_dataSC->_infos);
                this->addContactList(QString::fromStdString(contact));
                this->_modelOn.setStringList(this->getContactOn());
                this->_modelOff.setStringList(this->getContactOff());
                QMessageBox::information(this, "Ajout de Contact", "Votre demande d'ajout a reussi.", QMessageBox::Close);
                break;
            }
        case ADDFRIEND_FAILED:
            QMessageBox::warning(this, "Ajout de Contact", "Votre demande d'ajout a été refusée.", QMessageBox::Close);
            break;
        case DELFRIEND_SUCCES:
            {
                this->removeContactList(_scontact);
                this->_modelOn.setStringList(this->getContactOn());
                this->_modelOff.setStringList(this->getContactOff());
                QMessageBox::information(this, "Suppression de Contact", "Votre contact a été supprimé.", QMessageBox::Close);
                break;
            }
        case DELFRIEND_FAILED:
            QMessageBox::warning(this, "Suppression de Contact", "Votre suppression a échouée.", QMessageBox::Close);
            break;
        case REFRESH:
            this->refreshList();
            break;
        default:
            std::cout << "Type non reconnu SC:" << _dataSC->_type << std::endl;
    }
}

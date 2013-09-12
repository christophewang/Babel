#include "IGUI.h"
#include "RegisterWin.h"
#include "UserWin.h"
#include "LoginWin.h"

LoginWin::LoginWin()
{
    this->manageWin();
    this->userWin = new UserWin;
    this->registerWin = new RegisterWin;
    connect(this->getTcpSocket(), SIGNAL(connected()), this, SLOT(connectAccount()));
    connect(this->getTcpSocket(), SIGNAL(readyRead()), this, SLOT(receivedTcpData()));
    connect(this->getTcpSocket(), SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectFail(QAbstractSocket::SocketError)));
    connect(&this->_login, SIGNAL(clicked()), this, SLOT(checkLogin()));
    connect(&this->_register, SIGNAL(clicked()), this, SLOT(launchRegister()));
}

LoginWin::~LoginWin()
{
    delete  this->userWin;
    delete  this->registerWin;
}

void                LoginWin::manageWin()
{
    this->setFixedSize(300, 500);
    this->_id.setValidator(new QRegExpValidator(QRegExp("[A-Za-z0-9_]+"), this));
    this->_pass.setEchoMode(QLineEdit::Password);
    this->_pass.setValidator(new QRegExpValidator(QRegExp("[A-Za-z0-9_]+"), this));
    this->_server.setValidator(new QRegExpValidator(QRegExp("[0-9.]+"), this));
    this->_server.setMaxLength(16);
    this->_port.setRange(0, 99999);
    this->_serverLayout.addRow("Serveur", &this->_server);
    this->_serverLayout.addRow("Port", &this->_port);
    this->_serverLayout.setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
    this->_serverLayout.setFormAlignment(Qt::AlignHCenter);
    this->_serverGroup.setTitle("Information Serveur");
    this->_serverGroup.setLayout(&this->_serverLayout);
    this->_userLayout.addRow("Identifiant :", &this->_id);
    this->_userLayout.addRow("Mot de passe :", &this->_pass);
    this->_userLayout.setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
    this->_userLayout.setFormAlignment(Qt::AlignHCenter);
    this->_userGroup.setTitle("Information Utilisateur");
    this->_userGroup.setLayout(&this->_userLayout);
    this->_login.setText("Se connecter");
    this->_login.setCursor(Qt::PointingHandCursor);
    this->_register.setText("S'enregister");
    this->_register.setCursor(Qt::PointingHandCursor);
    this->_buttonLayout.setAlignment(Qt::AlignCenter);
    this->_buttonLayout.addWidget(&this->_login);
    this->_buttonLayout.addWidget(&this->_register);
    this->_logo.setPixmap(QPixmap(QCoreApplication::applicationDirPath() + "/ressources/babybel.png"));
    this->_logoLayout.setAlignment(Qt::AlignCenter);
    this->_logoLayout.addWidget(&this->_logo);
    this->_mainLayout.addLayout(&this->_logoLayout);
    this->_mainLayout.addWidget(&this->_serverGroup);
    this->_mainLayout.addWidget(&this->_userGroup);
    this->_mainLayout.addLayout(&this->_buttonLayout);
    this->setLayout(&this->_mainLayout);
    this->setWindowTitle("Babel");
    this->setWindowIcon(QIcon(QCoreApplication::applicationDirPath() + "/ressources/babybel.png"));
}

void                LoginWin::checkLogin()
{
    if (this->_id.text().isEmpty() || this->_pass.text().isEmpty() || this->_server.text().isEmpty())
        QMessageBox::critical(this, "Authentification", "Champs incomplets", QMessageBox::Close);
    else
        {
            this->_login.setEnabled(false);
            this->_register.setEnabled(false);
            this->setServer(this->_server.text());
            this->setTcpPort(this->_port.value());
            this->connectServer();
        }
}

void                LoginWin::connectAccount()
{
    this->sendDataTcp(CONNECT, this->_id.text().toStdString(), this->_pass.text().toStdString());
}

void                LoginWin::receivedTcpData()
{
    char            buffer[sizeof(ProtocoleSC)];
    QDataStream     in(this->getTcpSocket());
    std::string     contacts;

    in.setVersion(QDataStream::Qt_4_7);
    in.setByteOrder(QDataStream::ByteOrder(QSysInfo::ByteOrder));
    in.readRawData(buffer, sizeof(buffer));
    this->_dataSC = static_cast<ProtocoleSC *>(static_cast<void*>(buffer));
    this->_login.setEnabled(true);
    this->_register.setEnabled(true);
    if (this->_dataSC->_type == CONNECT_SUCCES)
        {
            contacts = this->_dataSC->_infos;
            this->loginSuccess(contacts);
        }
    else if (this->_dataSC->_type == CONNECT_FAILED)
        {
            QMessageBox::warning(this, "Authentification", "Mauvais nom d'utilisateur ou mauvais mot de passe", QMessageBox::Close);
            this->getTcpSocket()->close();
        }
}

void                LoginWin::loginSuccess(std::string const& contacts)
{
    this->userWin->setServer(this->_server.text());
    this->userWin->setTcpPort(this->_port.value());
    this->userWin->setClientId(this->_id.text());
    this->userWin->setContacts(QString::fromStdString(contacts));
    this->userWin->setTcpSocket(this->getTcpSocket());
    this->userWin->manageWin();
    this->close();
    disconnect(this->getTcpSocket(), SIGNAL(readyRead()), this, SLOT(receivedTcpData()));
    this->userWin->show();
}

void                LoginWin::connectFail(QAbstractSocket::SocketError error)
{
    if (error == QAbstractSocket::HostNotFoundError)
        QMessageBox::warning(this, "Connexion", "Le serveur n'a pas pu être trouvé, Vérifiez l'IP et le port.", QMessageBox::Close);
    else if (error == QAbstractSocket::RemoteHostClosedError)
    {
        QMessageBox::warning(this, "Connexion", "Vous avez été déconnecté du serveur.", QMessageBox::Close);
        this->userWin->close();
        disconnect(this->getTcpSocket(), SIGNAL(readyRead()), this->userWin, SLOT(receivedTcpData()));
        connect(this->getTcpSocket(), SIGNAL(readyRead()), this, SLOT(receivedTcpData()));
        this->show();
    }
    else
        QMessageBox::warning(this, "Connexion", "Le serveur est injoignable", QMessageBox::Close);
    this->_login.setEnabled(true);
    this->_register.setEnabled(true);
}

void                LoginWin::launchRegister()
{
    this->hide();
    this->registerWin->exec();
    this->show();
}

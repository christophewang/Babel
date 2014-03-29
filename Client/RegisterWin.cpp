#include "IGUI.h"
#include "RegisterWin.h"

RegisterWin::RegisterWin()
{
    this->manageWin();
    connect(&this->_register, SIGNAL(clicked()), this, SLOT(sendInfo()));
    connect(&this->_cancel, SIGNAL(clicked()), this, SLOT(cancelRegister()));
    connect(this->getTcpSocket(), SIGNAL(connected()), this, SLOT(doRegister()));
    connect(this->getTcpSocket(), SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectFail(QAbstractSocket::SocketError)));
    connect(this->getTcpSocket(), SIGNAL(readyRead()), this, SLOT(receivedTcpData()));
}

void                RegisterWin::manageWin()
{
    this->setFixedSize(400, 400);
    this->_id.setValidator(new QRegExpValidator(QRegExp("[A-Za-z0-9_]+"), this));
    this->_pass.setEchoMode(QLineEdit::Password);
    this->_pass.setValidator(new QRegExpValidator(QRegExp("[A-Za-z0-9_]+"), this));
    this->_repass.setEchoMode(QLineEdit::Password);
    this->_repass.setValidator(new QRegExpValidator(QRegExp("[A-Za-z0-9_]+"), this));
    this->_server.setValidator(new QRegExpValidator(QRegExp("[0-9.]+"), this));
    this->_server.setMaxLength(16);
    this->_port.setRange(0, 99999);
    this->_serverLayout.addRow("Serveur", &this->_server);
    this->_serverLayout.addRow("Port", &this->_port);
    this->_serverLayout.setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
    this->_serverLayout.setFormAlignment(Qt::AlignHCenter);
    this->_serverGroup.setTitle("Serveur Babel");
    this->_serverGroup.setLayout(&this->_serverLayout);
    this->_userLayout.addRow("Identifiant :", &this->_id);
    this->_userLayout.addRow("Mot de passe :", &this->_pass);
    this->_userLayout.addRow("Retaper votre Mot de passe :", &this->_repass);
    this->_userLayout.setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
    this->_userLayout.setFormAlignment(Qt::AlignHCenter);
    this->_userGroup.setTitle("Créer un compte");
    this->_userGroup.setLayout(&this->_userLayout);
    this->_register.setText("Envoyer");
    this->_register.setCursor(Qt::PointingHandCursor);
    this->_cancel.setText("Annuler");
    this->_cancel.setCursor(Qt::PointingHandCursor);
    this->_buttonLayout.setAlignment(Qt::AlignRight);
    this->_buttonLayout.addWidget(&this->_register);
    this->_buttonLayout.addWidget(&this->_cancel);
    this->_mainLayout.addWidget(&this->_serverGroup);
    this->_mainLayout.addWidget(&this->_userGroup);
    this->_mainLayout.addLayout(&this->_buttonLayout);
    this->setLayout(&this->_mainLayout);
    this->setWindowTitle("Création d'un compte");
    this->setWindowIcon(QIcon(QCoreApplication::applicationDirPath() + "/ressources/babybel.png"));
}

void                RegisterWin::doRegister()
{
    this->sendDataTcp(REGISTER, this->_id.text().toStdString(), this->_pass.text().toStdString());
}

void                RegisterWin::receivedTcpData()
{
    char            buffer[sizeof(ProtocoleSC)];
    QDataStream     in(this->getTcpSocket());

    in.setVersion(QDataStream::Qt_4_7);
    in.setByteOrder(QDataStream::ByteOrder(QSysInfo::ByteOrder));
    in.readRawData(buffer, sizeof(buffer));
    this->_dataSC = static_cast<ProtocoleSC *>(static_cast<void*>(buffer));
    this->getTcpSocket()->close();
    this->_register.setEnabled(true);
    if (this->_dataSC->_type == REGISTER_SUCCES)
        {
            QMessageBox::information(this, "Compte", "Votre compte a bien été crée", QMessageBox::Close);
            this->close();
        }
    else if (this->_dataSC->_type == REGISTER_FAILED)
        QMessageBox::warning(this, "Compte", "Echec lors de la creation du compte", QMessageBox::Close);
}

void                RegisterWin::sendInfo()
{
    if (this->_id.text().isEmpty() || this->_pass.text().isEmpty() || this->_server.text().isEmpty())
        QMessageBox::warning(this, "Compte", "Champs incomplets", QMessageBox::Close);
    else if (this->_pass.text() != this->_repass.text())
        QMessageBox::warning(this, "Mot de Passe", "Reconfirmation du mot de passe échouée.", QMessageBox::Close);
    else
    {
        this->_register.setEnabled(false);
        this->setServer(this->_server.text());
        this->setTcpPort(this->_port.value());
        this->connectServer();
    }
}

void                RegisterWin::cancelRegister()
{
    this->getTcpSocket()->close();
    this->close();
    this->_register.setEnabled(true);
}

void                RegisterWin::connectFail(QAbstractSocket::SocketError error)
{
    if (error == QAbstractSocket::HostNotFoundError)
        QMessageBox::warning(this, "Connexion", "Le serveur n'a pas pu être trouvé, Vérifiez l'IP et le port.", QMessageBox::Close);
    else
        QMessageBox::warning(this, "Connexion", "Le serveur est injoignable", QMessageBox::Close);
    this->_register.setEnabled(true);
}

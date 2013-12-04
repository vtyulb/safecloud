#include "serverconnection.h"

ServerConnection::ServerConnection(QObject *parent)
{
    socket = new QTcpSocket();
    isConnected = false;
    isReadyToAuthorizate = false;

    QObject::connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
                                    this, SLOT(handleSocketError(QAbstractSocket::SocketError)));
    QObject::connect(socket, SIGNAL(connected()),
                                    this, SLOT(connectedToServer()));
    QObject::connect(socket, SIGNAL(disconnected()),
                                    this, SLOT(disconnectedFromServer()));
    QObject::connect(socket, SIGNAL(readyRead()),
                                    this, SLOT(recieveMessage()));
}

ServerConnection::~ServerConnection()
{
    delete socket;
}

void ServerConnection::connect(int ip, int port)
{
    socket->connectToHost(QHostAddress(ip), port);
}

void ServerConnection::connectedToServer()
{
    isConnected = true;
}

void ServerConnection::disconnectedFromServer()
{
    isConnected = false;
    isReadyToAuthorizate = false;
}

void ServerConnection::receiveMessage()
{
    if (!isConnected)
    {
        qDebug() << "What's going on?";
        return;
    }
    QByteArray line = socket->readLine();
    if (line.startsWith("auth"))
        isReadyToAuthorizate = true;
}

void ServerConnection::authorization(const QString &login, const QString &password)
{
    if (!isConnected)
    {
        newLogMessage("You aren't connected to the server!");
        return;
    }

    if (isReadyToAuthorizate)
    {
        QByteArray loginHash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha512);
        qDebug() << loginHash;
        socket->write((login + "\n").toUtf8());
        socket->write(loginHash + "\n");
    }
    else
    {
        newLogMessage("Server isn't ready for authorization.");
    }
}

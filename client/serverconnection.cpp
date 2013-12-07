#include <QSslSocket>
#include <QSslKey>
#include <QSslConfiguration>
#include <QFile>
#include "serverconnection.h"

ServerConnection::ServerConnection(QObject *parent) :
    QObject(parent)
{
    socket = new QSslSocket();
    socket->setProtocol(QSsl::SslV3);
    socket->setPeerVerifyMode(QSslSocket::VerifyNone);

    isConnected = false;
    isReadyToAuthorizate = false;

    QObject::connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
                                    this, SLOT(handleSocketError(QAbstractSocket::SocketError)));

    QObject::connect(socket, SIGNAL(encrypted()),
                                    this, SLOT(connectedToServer()));

    QObject::connect(socket, SIGNAL(disconnected()),
                                    this, SLOT(disconnectedFromServer()));

    QObject::connect(socket, SIGNAL(readyRead()),
                                    this, SLOT(receiveMessage()));
}

ServerConnection::~ServerConnection()
{
    delete socket;
}

void ServerConnection::connect(const QString &address, int port)
{
    socket->connectToHostEncrypted(address, port);
}

void ServerConnection::connectedToServer()
{
    isConnected = true;
    emit newLogMessage("You were connected to the server.");
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
    QByteArray line = socket->readAll();
    qDebug() << "message " << line;
    if (line.startsWith("auth"))
        isReadyToAuthorizate = true;
    else
        switch (line[0]) {
        case 1:
            emit newLogMessage("You were authorizated.");
            return;
        case 11:
            emit newLogMessage("Authorization timeout.");
            return;
        case 12:
            emit newLogMessage("Internal server error.");
            return;
        case 13:
            emit newLogMessage("Password incorrect / Login not found.");
            return;
        default:
            emit newLogMessage(QString("Unknown message: ") + QString::number(line[0]));
        }
}

void ServerConnection::handleSocketError(QAbstractSocket::SocketError error)
{
    qDebug() << error;
    emit newLogMessage(QString::number(error));
}

void ServerConnection::authorization(const QString &login, const QString &password)
{
    if (!isConnected)
    {
        emit newLogMessage("You aren't connected to the server!");
        return;
    }

    if (isReadyToAuthorizate)
    {
        QByteArray passwordHash = password.toUtf8() + login.toUtf8();
        passwordHash = QCryptographicHash::hash(passwordHash, QCryptographicHash::Md5).toHex();

        qDebug() << passwordHash;
        socket->write((login + "\n").toUtf8());
        socket->write(passwordHash + "\n");
        socket->flush();
    }
    else
    {
        emit newLogMessage("Server isn't ready for authorization.");
    }
}

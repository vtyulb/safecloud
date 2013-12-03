#include "tcpserver.h"

TcpServer::TcpServer(QObject *parent): QTcpServer(parent) {
    QSettings settings("config.ini", QSettings::IniFormat);
    login = settings.value("login").toString();
    password = settings.value("password").toString();
}

void TcpServer::incomingConnection(qintptr handle) {
    Connection *connection = new Connection(handle, login, password, this);
    QObject::connect(connection, SIGNAL(finished()), connection, SLOT(deleteLater()));
    connection->start();
}

#include "tcpserver.h"

TcpServer::TcpServer(QObject *parent): QTcpServer(parent) {
    QSettings settings("config.ini", QSettings::IniFormat);
    login = settings.value("login").toString();
    password = settings.value("password").toString();

    qDebug() << QSqlDatabase::isDriverAvailable("QMYSQL");
}

void TcpServer::incomingConnection(qintptr handle) {
    Connection *connection = new Connection(handle, login, password);
    QObject::connect(connection, SIGNAL(finished()), connection, SLOT(deleteLater()));
    connection->start();
}

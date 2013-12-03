#include "tcpserver.h"

TcpServer::TcpServer(QObject *parent): QTcpServer(parent) {
}

void TcpServer::incomingConnection(qintptr handle) {
    Connection *connection = new Connection(handle, this);
    connection->start();
}

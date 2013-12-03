#include "connection.h"

Connection::Connection(qintptr handle, QObject *parent): QThread(parent) {
    socketHandle = handle;
    state = Auth;
}

void Connection::run() {
    socket = new QTcpSocket(this);

    if (!socket->setSocketDescriptor(socketHandle)) {
        qDebug() << "Can't establish connection with socket" << socketHandle;
        return;
    }

    //TODO: version protocol

    socket->write("auth\n");
    QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(timeToRead()));
    QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(quit()));
    exec();
}

void Connection::timeToRead() {
    qDebug() << socket->readLine();
}

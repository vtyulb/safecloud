#include "connection.h"

Connection::Connection(qintptr handle, QString login, QString password, QObject *parent): QThread(parent) {
    socketHandle = handle;
    state = Auth;
    mySqlLogin = login;
    mySqlPassword = password;
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

void Connection::authorization() {
    QByteArray login = socket->readLine();
    if (!socket->waitForReadyRead(MAX_TIMEOUT)) {
        qDebug() << "Too slow!";
        return;
    }
    QByteArray hash = socket->readLine();

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("safecloud");
    db.setUserName(mySqlLogin);
    db.setPassword(mySqlPassword);
    if (!db.open()) {
        qDebug() << "Failed to connect to database";
        socket->write("Corrupted database, please contact support\n");
        return;
    } else
        qDebug() << "success";
}

void Connection::timeToRead() {
    if (state == Auth)
        authorization();
    else {

    }
}

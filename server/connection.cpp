#include "connection.h"
#include <QSqlError>
#include <QStringList>

Connection::Connection(qintptr handle, QString login, QString password, QObject *parent): QThread(parent) {
    socketHandle = handle;
    state = Auth;
    mySqlLogin = login;
    mySqlPassword = password;
}

Connection::~Connection() {
    delete socket;
}

void Connection::run() {
    socket = new QTcpSocket();

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

    qDebug() << mySqlLogin << mySqlPassword;
    qDebug() << QSqlDatabase::drivers();

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("safecloud");
    db.setUserName(mySqlLogin);
    db.setPassword(mySqlPassword);

    qDebug() << db.lastError();
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

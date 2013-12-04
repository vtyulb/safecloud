#include "connection.h"
#include <QSqlError>
#include <QStringList>
#include <QSqlQuery>
#include <QHostAddress>

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

    socket->write("auth?v=1\n");
    QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(timeToRead()));
    QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(quit()));
    exec();
}

void Connection::authorization() {
    QByteArray login = socket->readLine();
    if (!socket->waitForReadyRead(MAX_TIMEOUT)) {
        socket->write(TIMEOUT_ERROR);
        qDebug() << "Too slow!";
        return;
    }
    QByteArray hash = socket->readLine();

    login.resize(login.size() - 1);
    hash.resize(hash.size() - 1);

    qDebug() << "Checking: " << QString(login) << QString(hash);

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("safecloud");
    db.setUserName(mySqlLogin);
    db.setPassword(mySqlPassword);

    if (!db.open()) {
        qDebug() << "Failed to connect to database" << db.lastError();
        socket->write(INTERNAL_SERVER_ERROR);
        return;
    } else {
        QSqlQuery query(QString("SELECT PASSWORD FROM `main` WHERE (LOGIN=%1)").arg(QString(login)), db);
        query.exec();
        if (query.next()) {
            if (query.value(0).toString() == hash) {
                socket->write(OK);
                state = Normal;
                qDebug() << "Authorized from" << socket->peerAddress();
            } else {
                socket->write(FAIL);
                qDebug() << "Password incorrect" << socket->peerAddress();
            }
        } else {
            socket->write(FAIL);
            qDebug() << "Login not found" << socket->peerAddress();
        }
    }
}

void Connection::timeToRead() {
    if (state == Auth)
        authorization();
    else {

    }
}

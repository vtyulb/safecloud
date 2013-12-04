#include "connection.h"
#include <QSqlError>
#include <QStringList>
#include <QSqlQuery>

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
        socket->write("Corrupted database, please contact support\n");
        return;
    } else {
        QSqlQuery query(QString("SELECT PASSWORD FROM `main` WHERE (LOGIN=%1)").arg(QString(login)), db);
        query.exec();
        if (query.next()) {
            if (query.value(0).toString() == hash) {
                socket->write("authorized\n");
                state = Normal;
            } else
                socket->write("Access denied\n");
        } else {
            socket->write("Login not found\n");
        }
    }
}

void Connection::timeToRead() {
    if (state == Auth)
        authorization();
    else {

    }
}

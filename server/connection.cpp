#include "connection.h"
#include <QSqlError>
#include <QStringList>
#include <QSqlQuery>
#include <QSslSocket>
#include <QHostAddress>
#include <QSqlDatabase>
#include <QDebug>
#include <QSslConfiguration>
#include <QSslKey>
#include <QFile>

Connection::Connection(qintptr handle, QString login, QString password, QObject *parent): QThread(parent) {
    socketHandle = handle;
    state = Auth;
    mySqlLogin = login;
    mySqlPassword = password;
}

Connection::~Connection() {
    delete socket;
    qDebug() << "Connection closed";
}

void Connection::run() {
    socket = new QSslSocket();

    socket->setPeerVerifyMode(QSslSocket::VerifyNone);
    socket->setProtocol(QSsl::SslV3);

    if (!socket->setSocketDescriptor(socketHandle)) {
        qDebug() << "Can't establish connection with socket" << socketHandle;
        return;
    }

    socket->setPrivateKey("safecloud-server.pem", QSsl::Rsa);
    socket->setLocalCertificate("safecloud-server.pem");
    socket->startServerEncryption();

    if (!socket->waitForEncrypted(1000)) {
        qDebug() << "Can't create encrypted tunnel";
        qDebug() << socket->sslErrors();
        return;
    }
        qDebug() << "Encrypted tunnel created";

    QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(quit()));
    QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(timeToRead()));

    socket->write("auth?v=1\n");
    exec();
}

void Connection::authorization() {
    QByteArray login = socket->readLine();
    if (!socket->canReadLine() && !socket->waitForReadyRead(MAX_TIMEOUT)) {
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
    } else {
        QSqlQuery query(QString("SELECT PASSWORD FROM `main` WHERE (USER_LOGIN='%1')").arg(QString(login)), db);
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
    socket->flush();
}

void Connection::timeToRead() {
    if (!socket->canReadLine())
        return;

    if (state == Auth)
        authorization();
    else {
        qDebug() << socket->readLine();
    }
}

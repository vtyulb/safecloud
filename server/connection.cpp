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

    socket->setPrivateKey("private-key.pem", QSsl::Rsa);
    socket->setLocalCertificate("private-key.pem");
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
    login = socket->readLine();
    checkSQLInjection(QString(login));
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
                QSqlQuery query(QString("SELECT BITTORRENT_READWRITE FROM `main` WHERE (USER_LOGIN='%1')").arg(QString(login)), db);
                if (!query.next()) {
                    qDebug() << Q_FUNC_INFO << "fatal";
                    socket->write(FAIL);
                } else if (query.value(0).toString() == "UNDEFINED") {
                    socket->write(GENERATE_KEYS);
                    state = NoKeys;
                    qDebug() << "First authorization from" << socket->peerAddress();
                } else {
                    socket->write(OK);
                    state = Normal;
                    qDebug() << "Authorized from" << socket->peerAddress();
                }
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
    else if (state == NoKeys) {
        QByteArray encrypted = socket->readLine();
        QByteArray readwrite = socket->readLine();
        QByteArray readonly = socket->readLine();

        encrypted.resize(encrypted.size() - 1);
        readwrite.resize(readwrite.size() - 1);
        readonly.resize(readonly.size() - 1);

        checkSQLInjection(QString(encrypted));
        checkSQLInjection(QString(readwrite));
        checkSQLInjection(QString(readonly));

        QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
        db.setHostName("localhost");
        db.setDatabaseName("safecloud");
        db.setUserName(mySqlLogin);
        db.setPassword(mySqlPassword);
        db.open();

        QSqlQuery query(QString("UPDATE `main` SET `BITTORRENT_ENCRYPTED`='%1', `BITTORRENT_READWRITE`='%2', `BITTORRENT_READONLY`='%3' WHERE `USER_LOGIN`='%4'")
                        .arg(QString(encrypted)).arg(QString(readwrite)).arg(QString(readonly)).arg(QString(login)), db);

        query.exec();
        state = Normal;
        socket->write(OK);
    }
}

void Connection::checkSQLInjection(QString query) {
    for (int i = 0; i < query.length(); i++)
        if (query[i] == '\'' || query[i] == '"' || query[i] == '`') {
            qDebug() << "hack attempt from" << socket->peerAddress() << "with string: " << query;
            socket->close();
            terminate();
        }
}

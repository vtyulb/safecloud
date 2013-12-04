#ifndef CONNECTION_H
#define CONNECTION_H

#include <QThread>
#include <QTcpSocket>
#include <QDebug>
#include <QSqlDatabase>

const int MAX_TIMEOUT = 30000;
const QByteArray TIMEOUT_ERROR(1, 11);
const QByteArray INTERNAL_SERVER_ERROR(1, 12);
const QByteArray OK(1, 200);
const QByteArray FAIL(1, 13);


enum State {
    Auth,
    Normal
};

class Connection : public QThread {
    Q_OBJECT
public:
    explicit Connection(qintptr handle, QString login, QString password, QObject *parent = 0);
    ~Connection();

private:
    qintptr socketHandle;
    QTcpSocket *socket;
    QString mySqlLogin;
    QString mySqlPassword;
    State state;

    void run();
    void authorization();

private slots:
    void timeToRead();
};

#endif // CONNECTION_H

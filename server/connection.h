#ifndef CONNECTION_H
#define CONNECTION_H

#include <QThread>

const int MAX_TIMEOUT = 30000;
const QByteArray TIMEOUT_ERROR(1, 11);
const QByteArray INTERNAL_SERVER_ERROR(1, 12);
const QByteArray OK(1, 1);
const QByteArray FAIL(1, 13);
const QByteArray GENERATE_KEYS(1, 2);


enum State {
    Auth,
    NoKeys,
    Normal
};

class QSslSocket;

class Connection : public QThread {
    Q_OBJECT
public:
    explicit Connection(qintptr handle, QString login, QString password, QObject *parent = 0);
    ~Connection();

private:
    qintptr socketHandle;
    QSslSocket *socket;
    QString mySqlLogin;
    QString mySqlPassword;
    State state;

    QByteArray login;

    void run();
    void authorization();
    void checkSQLInjection(QString);

private slots:
    void timeToRead();
};

#endif // CONNECTION_H

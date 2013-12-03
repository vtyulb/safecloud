#ifndef CONNECTION_H
#define CONNECTION_H

#include <QThread>
#include <QTcpSocket>
#include <QDebug>
#include <QSqlDatabase>

#define MAX_TIMEOUT 3000

enum {
    Auth,
    Normal
} state;

class Connection : public QThread {
    Q_OBJECT
public:
    explicit Connection(qintptr handle, QString login, QString password, QObject *parent = 0);

private:
    qintptr socketHandle;
    QTcpSocket *socket;
    QString mySqlLogin;
    QString mySqlPassword;

    void run();
    void authorization();

private slots:
    void timeToRead();
};

#endif // CONNECTION_H

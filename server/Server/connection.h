#ifndef CONNECTION_H
#define CONNECTION_H

#include <QThread>
#include <QTcpSocket>
#include <QDebug>

enum {
    Auth,
    Normal
} state;

class Connection : public QThread {
    Q_OBJECT
public:
    explicit Connection(qintptr handle, QObject *parent = 0);

private:
    qintptr socketHandle;
    void run();
    QTcpSocket *socket;

private slots:
    void timeToRead();
};

#endif // CONNECTION_H

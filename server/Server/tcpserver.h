#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>
#include <connection.h>

class TcpServer : public QTcpServer {
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = 0);
protected:
    void incomingConnection(qintptr handle);
signals:

public slots:

};

#endif // TCPSERVER_H

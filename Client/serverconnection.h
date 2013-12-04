#ifndef SERVERCONNECTION_H
#define SERVERCONNECTION_H

#include <QString>
#include <QTcpSocket>
#include <QHostAddress>
#include <QCryptographicHash>

class ServerConnection : public QObject
{
    Q_OBJECT

public:
    explicit ServerConnection(QObject *parent = 0);
    ~ServerConnection();
    void connect(int ip, int port);
    void authorization(const QString &login, const QString &password);

private:
    bool isConnected;
    bool isReadyToAuthorizate;
    QTcpSocket *socket;

private slots:
    void handleSocketError(QAbstractSocket::SocketError error);
    void connectedToServer();
    void disconnectedFromServer();
    void receiveMessage();

signals:
    void newLogMessage(const QString &s);

};

#endif // SERVERCONNECTION_H

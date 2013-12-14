#ifndef SERVERCONNECTION_H
#define SERVERCONNECTION_H

#include <QString>
#include <QHostAddress>
#include <QSslSocket>
#include <QCryptographicHash>\

class QNetworkReply;

class ServerConnection : public QObject
{
    Q_OBJECT

public:
    explicit ServerConnection(QObject *parent = 0);
    ~ServerConnection();
    void connect(const QString &address, int port);
    void authorization(const QString &login, const QString &password);

private:
    bool isConnected;
    bool isReadyToAuthorizate;
    QSslSocket *socket;
    QString password;

    void generateKeys();

private slots:
    void handleSocketError(QAbstractSocket::SocketError error);
    void connectedToServer();
    void disconnectedFromServer();
    void receiveMessage();
    void keysGenerated(QNetworkReply*);

signals:
    void newLogMessage(const QString &s);

};

#endif // SERVERCONNECTION_H

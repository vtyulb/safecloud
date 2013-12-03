#include <QCoreApplication>
#include <QTcpServer>
#include <tcpserver.h>

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    TcpServer server;
    if (!server.listen(QHostAddress::Any, 43718)) {
        qDebug() << "Failed to start server" << server.errorString();
        return 1;
    }

    return a.exec();
}

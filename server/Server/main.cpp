#include <QCoreApplication>
#include <QTcpServer>
#include <tcpserver.h>

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    TcpServer server;
    server.listen(QHostAddress::Any, 43718);

    return a.exec();
}

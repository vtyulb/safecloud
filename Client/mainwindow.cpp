#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    serverConnection = new ServerConnection();
    serverConnection->connect("vtyulb.ru", 43718);

    QObject::connect(ui->loginButton, SIGNAL(clicked()), this, SLOT(logIn()));
    QObject::connect(serverConnection, SIGNAL(newLogMessage(QString)), this, SLOT(displayLogMessage(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::logIn()
{
    serverConnection->authorization(ui->loginEdit->text(), ui->passwordEdit->text());
}

void MainWindow::displayLogMessage(const QString &message)
{
    statusBar()->showMessage(message, 5000);
    qDebug() << "StatusBar::" << message;
}

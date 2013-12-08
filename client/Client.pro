#-------------------------------------------------
#
# Project created by QtCreator 2013-12-04T17:24:25
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Client
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    serverconnection.cpp \
    crypter.cpp

HEADERS  += mainwindow.h \
    serverconnection.h \
    crypter.h

FORMS    += mainwindow.ui

LIBS += -lpthread -lcryptopp

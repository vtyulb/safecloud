#-------------------------------------------------
#
# Project created by QtCreator 2013-12-03T22:08:48
#
#-------------------------------------------------

QT       += core
QT       -= gui
QT       += network sql

TARGET = Server
CONFIG   += console

TEMPLATE = app


SOURCES += main.cpp \
    tcpserver.cpp \
    connection.cpp

HEADERS += \
    tcpserver.h \
    connection.h

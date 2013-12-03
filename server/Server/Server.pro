#-------------------------------------------------
#
# Project created by QtCreator 2013-12-03T22:08:48
#
#-------------------------------------------------

QT       += core
QT       -= gui
QT       += network

TARGET = Server
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    tcpserver.cpp \
    connection.cpp

HEADERS += \
    tcpserver.h \
    connection.h

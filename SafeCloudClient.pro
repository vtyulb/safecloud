#-------------------------------------------------
#
# Project created by QtCreator 2013-11-25T13:20:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SafeCloudClient
TEMPLATE = app

LIBS += \
       -lboost_system\
       -ltorrent-rasterbar\


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

PKGCONFIG += libtorrent-rasterbar

DEFINES += BOOST_ASIO_DYN_LINK TORRENT_USE_OPENSSL BOOST_ASIO_HASH_MAP_BUCKETS=1021 BOOST_EXCEPTION_DISABLE BOOST_ASIO_ENABLE_CANCELIO BOOST_ASIO_DYN_LINK TORRENT_LINKING_SHARED

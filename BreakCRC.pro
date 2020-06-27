#-------------------------------------------------
#
# Project created by QtCreator 2019-07-25T08:54:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CXXFLAGS += -std=c++0x

TARGET = BreakCRC
TEMPLATE = app


SOURCES += main.cpp\
    crc8saej1850.cpp \
        mainwindow.cpp \
    packcrc.cpp \
    crc8.cpp

HEADERS  += mainwindow.h \
    crc8saej1850.h \
    packcrc.h \
    crc8.h

FORMS    += mainwindow.ui

RC_ICONS = "car.ico"
VERSION = 0.0.0.1
QMAKE_TARGET_COMPANY = Home
QMAKE_TARGET_PRODUCT = BreakCRC
QMAKE_TARGET_DESCRIPTION = Program to find CRC settings for BIN packet
QMAKE_TARGET_COPYRIGHT = Kompot

#RC_FILE = "resources.rc"

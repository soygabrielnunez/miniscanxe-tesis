#-------------------------------------------------
#
# Project created by QtCreator 2015-07-07T21:22:17
#
#-------------------------------------------------

QT       += core gui
QT += axcontainer
QT += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = Spectrasoft
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    spectralops.cpp \
    miniscanxe.cpp \
    dlgfototipo.cpp \
    dlggrafica.cpp \
    dlgdatosadicionales.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    spectralops.h \
    miniscanxe.h \
    dlgfototipo.h \
    dlggrafica.h \
    dlgdatosadicionales.h

FORMS    += mainwindow.ui \
    dlgfototipo.ui \
    dlggrafica.ui \
    dlgdatosadicionales.ui

RESOURCES += \
    recursos.qrc

RC_ICONS += img/logo_icon.ico

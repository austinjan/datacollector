#-------------------------------------------------
#
# Project created by QtCreator 2018-05-21T15:44:18
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

G++-arm-hf {
    QMAKE_LFLAGS *= -Wl,-rpath=/home/dest:/opt/qt_arm48_p
    QMAKE_LFLAGS *= -Wl,--dynamic-linker=/lib/ld-linux.so.3
    QMAKE_LFLAGS += -Wl,-rpath-link=/tslib/lib
}

#unix:QMAKE_RPATHDIR += /opt/qt_arm48_p

TARGET = RT
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    startform.cpp \
    loginform.cpp \
    barcodeinputform.cpp \
    producthistoryform.cpp \
    settingsform.cpp \
    fileoperation.cpp \
    inputpanelcontext.cpp \
    keyhandler.cpp \
    departmentselectform.cpp \
    information.cpp \
    filedataloader.cpp \
    inputpanel.cpp \
    ftpservice.cpp \
    hardwareinterface.cpp \
    downloadform.cpp \
    fixinputform.cpp

HEADERS += \
        mainwindow.h \
    startform.h \
    loginform.h \
    barcodeinputform.h \
    producthistoryform.h \
    settingsform.h \
    fileoperation.h \
    inputpanelcontext.h \
    keyhandler.h \
    departmentselectform.h \
    information.h \
    filedataloader.h \
    inputpanel.h \
    ftpservice.h \
    hardwareinterface.h \
    downloadform.h \
    fixinputform.h

FORMS += \
        mainwindow.ui \
    startform.ui \
    loginform.ui \
    barcodeinputform.ui \
    producthistoryform.ui \
    settingsform.ui \
    departmentselectform.ui \
    inputpanel.ui \
    downloadform.ui \
    fixinputform.ui

RESOURCES += \
    resource.qrc



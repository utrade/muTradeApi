#-------------------------------------------------
#
# Project created by QtCreator 2013-11-06T12:18:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = apiTest
TEMPLATE = app

DEFINES += CUTELOGGER_LIBRARY
SOURCES += main.cpp\
        mainwindow.cpp \
        testStrategy.cpp \
        sgtables.cpp \
        processapiresponses.cpp \
        dialoglogin.cpp \
        orderbook.cpp \
        tradebook.cpp \
        netpositions.cpp \
        buysell.cpp \
        dockwidgetlogger.cpp \
        Logger/src/stringlistmodelappender.cpp \
        Logger/src/Logger.cpp \
        Logger/src/FileAppender.cpp \
        Logger/src/ConsoleAppender.cpp \
        Logger/src/AbstractStringAppender.cpp \
        Logger/src/AbstractAppender.cpp \
        formtwolegstrategy.cpp

HEADERS  += mainwindow.h \
            testStrategy.h \
            sgtables.h \
            processapiresponses.h \
            dialoglogin.h \
            orderbook.h \
            tradebook.h \
            netpositions.h \
            buysell.h \
            dockwidgetlogger.h \
            Logger/src/stringlistmodelappender.h \
            formtwolegstrategy.h

FORMS    += mainwindow.ui \
    sgtables.ui \
    dialoglogin.ui \
    orderbook.ui \
    tradebook.ui \
    netpositions.ui \
    buysell.ui \
    dockwidgetlogger.ui \
    formtwolegstrategy.ui


DEFINES += API_CLIENT
unix:!macx: LIBS += -L$$PWD/../muTradeApiTest/sampleStrategy/bin/ -lmuTradeAPI -lssl -lcrypto

INCLUDEPATH += $$PWD/../muTradeApiTest/sampleStrategy/include
DEPENDPATH += $$PWD/../muTradeApiTest/sampleStrategy/include

unix:!macx: PRE_TARGETDEPS += $$PWD/../muTradeApiTest/sampleStrategy/bin/libmuTradeAPI.a

INCLUDEPATH += $$PWD/Logger/include


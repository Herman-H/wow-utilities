#-------------------------------------------------
#
# Project created by QtCreator 2015-08-09T13:14:57
#
#-------------------------------------------------

QT       += core gui
QT       += sql

CONFIG += c++17

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = xp_path
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    action.cpp \
    actionselecterdialog.cpp \
    questselecter.cpp \
    actionitemmodel.cpp \
    questsetmodel.cpp \
    hearthstonelocationselecter.cpp \
    questitemrewardselecter.cpp \
    flightmasterselecter.cpp \
    vendorconditionmodel.cpp \
    itemselecter.cpp \
    vendorconditionselecter.cpp \
    luastatementedit.cpp \
    generateaddon.cpp \
    xpbreakpointselecter.cpp \
    xprewardviewer.cpp \
    banktransactionselecter.cpp \
    trainertablemodel.cpp

HEADERS  += mainwindow.h \
    action.h \
    questdata.h \
    simulation.h \
    actionselecterdialog.h \
    questselecter.h \
    actionitemmodel.h \
    questsetmodel.h \
    hearthstonelocationselecter.h \
    questitemrewardselecter.h \
    flightmasterselecter.h \
    vendorconditionmodel.h \
    itemselecter.h \
    vendorconditionselecter.h \
    luastatementedit.h \
    generateaddon.h \
    levels.h \
    xpbreakpointselecter.h \
    xprewardviewer.h \
    banktransactionselecter.h \
    trainertablemodel.h

FORMS    += mainwindow.ui \
    actionselecterdialog.ui \
    questselecter.ui \
    hearthstonelocationselecter.ui \
    questitemrewardselecter.ui \
    flightmasterselecter.ui \
    itemselecter.ui \
    vendorconditionselecter.ui \
    luastatementedit.ui \
    xpbreakpointselecter.ui \
    xprewardviewer.ui \
    banktransactionselecter.ui

TEPLATE = app
TARGET = BugSearch_GUI

QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    main.cpp \
    cmainwidget.cpp \
    SocketClassClient.cpp

HEADERS += \
    cmainwidget.h \
    SocketClassClient.h \
    connstates.h

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -pthread
SOURCES += main.cpp \
    client.cpp \
    server.cpp \
    protocol.cpp \
    logger.cpp

HEADERS += \
    client.h \
    common.h \
    server.h \
    protocol.h \
    serialization.h \
    logger.h \
    inputargreader.h

TEMPLATE = lib
CONFIG += staticlib

QT += sql

DEPENDPATH += $$PWD
INCLUDEPATH += $$PWD/..

MOC_DIR = .moc
OBJECTS_DIR = .obj

QMAKE_CXXFLAGS += -O2 -g2 -Wextra

DEFINES += QT_NO_DEBUG_OUTPUT QT_NO_WARNING_OUTPUT
#DEFINES += DEBUG_QUERY
#DEFINES += DEBUG_TRACE

# Input
HEADERS += $$PWD/Table.h \
           $$PWD/Tools.h

SOURCES += $$PWD/Table.cpp \
           $$PWD/Tools.cpp

QMAKE_CLEAN += $$OUT_PWD/libmysql.a


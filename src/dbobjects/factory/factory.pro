TEMPLATE = lib
CONFIG += staticlib

DEPENDPATH += $$PWD
INCLUDEPATH += $$PWD/..

MOC_DIR = .moc

OBJECTS_DIR = .obj

QMAKE_CXXFLAGS += -O2 -g2 -Wextra

DEFINES += QT_NO_DEBUG_OUTPUT QT_NO_WARNING_OUTPUT
#DEFINES += DEBUG_QUERY
#DEFINES += DEBUG_TRACE

# Input
HEADERS += $$PWD/Index.h \
           $$PWD/Language.h \
           $$PWD/Procedure.h \
           $$PWD/Role.h \
           $$PWD/Table.h \
           $$PWD/Trigger.h \
           $$PWD/View.h \
           $$PWD/../psql/Procedure.h \
           $$PWD/../psql/View.h

SOURCES += $$PWD/Index.cpp \
           $$PWD/Language.cpp \
           $$PWD/Procedure.cpp \
           $$PWD/Role.cpp \
           $$PWD/Table.cpp \
           $$PWD/Trigger.cpp \
           $$PWD/View.cpp

QMAKE_CLEAN += $$OUT_PWD/libfactory.a


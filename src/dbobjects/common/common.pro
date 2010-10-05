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
HEADERS += $$PWD/Database.h \
           $$PWD/DbIndex.h \
           $$PWD/DbLanguage.h \
           $$PWD/DbObject.h \
           $$PWD/DbProcedure.h \
           $$PWD/DbRole.h \
           $$PWD/DbTable.h \
           $$PWD/DbTrigger.h \
           $$PWD/DbView.h \
           $$PWD/DbSchema.h

SOURCES += $$PWD/Database.cpp \
           $$PWD/DbIndex.cpp \
           $$PWD/DbLanguage.cpp \
           $$PWD/DbObject.cpp \
           $$PWD/DbProcedure.cpp \
           $$PWD/DbRole.cpp \
           $$PWD/DbTable.cpp \
           $$PWD/DbTrigger.cpp \
           $$PWD/DbView.cpp \
           $$PWD/DbSchema.cpp

QMAKE_CLEAN += $$OUT_PWD/libcommon.a


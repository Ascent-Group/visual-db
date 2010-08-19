message("We advice you run 'make -j3'")

QT += testlib sql network xml
#CONFIG += release

# We need to override old TARGET and DESTDIR
TARGET = tests
DESTDIR = ./

MOC_DIR = .moc
OBJECTS_DIR = .obj

DEPENDPATH = . dbobjects $$TOP_SRC_DIR $$TOP_SRC_DIR/dbobjects
INCLUDEPATH = . dbobjects $$TOP_SRC_DIR $$TOP_SRC_DIR/dbobjects

QMAKE_CXXFLAGS = -g -Wextra

LIBS += -L$$TOP_SRC_DIR/dbobjects -ldbobjects

DEFINES -= QT_NO_DEBUG_OUTPUT QT_NO_WARNING_OUTPUT

#DEFINES += TEST_BEHAVIOUR
#DEFINES += TEST_GUI
DEFINES += TEST_CONNECT
DEFINES += TEST_DBOBJECTS

SOURCES = main.cpp

contains(DEFINES, TEST_BEHAVIOUR) {
    HEADERS += gui/behaviour/*.h \
               $$TOP_SRC_DIR/gui/behaviour/*.h

    SOURCES += gui/behaviour/*.cpp \
               $$TOP_SRC_DIR/gui/behaviour/*.cpp
}

contains(DEFINES, TEST_GUI) {
    HEADERS += gui/*.h \
               $$TOP_SRC_DIR/gui/*.h

    SOURCES += gui/*.cpp \
               $$TOP_SRC_DIR/gui/*.cpp
}

contains(DEFINES, TEST_CONNECT) {
    HEADERS += connect/*.h \
               $$TOP_SRC_DIR/connect/*.h

    SOURCES += connect/*.cpp \
               $$TOP_SRC_DIR/connect/*.cpp
}

contains(DEFINES, TEST_DBOBJECTS) {
    HEADERS += dbobjects/common/*.h \
               dbobjects/mysql/*.h \
               dbobjects/psql/*.h \
               $$TOP_SRC_DIR/dbobjects/common/*.h \
               $$TOP_SRC_DIR/dbobjects/mysql/*.h \
               $$TOP_SRC_DIR/dbobjects/psql/*.h

    SOURCES += dbobjects/common/*.cpp \
               dbobjects/mysql/*.cpp \
               dbobjects/psql/*.cpp \
               $$TOP_SRC_DIR/dbobjects/common/*.cpp \
               $$TOP_SRC_DIR/dbobjects/mysql/*.cpp \
               $$TOP_SRC_DIR/dbobjects/psql/*.cpp
}


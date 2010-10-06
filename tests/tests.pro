message("We advice you run 'make -j3'")

QT += testlib sql network xml
#CONFIG += object_with_source
#CONFIG += release

# We need to override old TARGET and DESTDIR
TARGET = tests
DESTDIR = .

MOC_DIR = .moc
OBJECTS_DIR = .obj

TOP_SRC_DIR = $$PWD/../src

DEPENDPATH = . dbobjects $$TOP_SRC_DIR $$TOP_SRC_DIR/dbobjects
INCLUDEPATH = $$PWD $$PWD/dbobjects $$TOP_SRC_DIR $$TOP_SRC_DIR/dbobjects

QMAKE_CXXFLAGS = -g -Wextra

LIBS += -L$$OUT_PWD/../src/dbobjects/common \
        -L$$OUT_PWD/../src/dbobjects/factory \
        -L$$OUT_PWD/../src/dbobjects/psql \
        -L$$OUT_PWD/../src/dbobjects/mysql

LIBS += -lcommon -lpsql -lfactory -lcommon -lmysql -lpsql -lfactory -lcommon -lpsql -lfactory -lmysql -lpsql

# Appears to be a solution but decreases the performance too much
#QMAKE_LFLAGS += -Wl,--start-group -lcommon -lpsql -lmysql -lfactory -Wl,--end-group

DEFINES -= QT_NO_DEBUG_OUTPUT QT_NO_WARNING_OUTPUT

#DEFINES += TEST_BEHAVIOUR
#DEFINES += TEST_GUI
DEFINES += TEST_CONNECT
DEFINES += TEST_DBOBJECTS

SOURCES = $$PWD/main.cpp \
          $$TOP_SRC_DIR/consts.cpp

HEADERS = $$TOP_SRC_DIR/consts.h

contains(DEFINES, TEST_BEHAVIOUR) {
    HEADERS += $$files($$PWD/gui/behaviour/*.h) \
               $$files($$TOP_SRC_DIR/gui/behaviour/*.h)

    SOURCES += $$files($$PWD/gui/behaviour/*.cpp) \
               $$files($$TOP_SRC_DIR/gui/behaviour/*.cpp)
}

contains(DEFINES, TEST_GUI) {
    HEADERS += $$files($$PWD/gui/*.h) \
               $$files($$TOP_SRC_DIR/gui/*.h)

    SOURCES += $$files($$PWD/gui/*.cpp) \
               $$files($$TOP_SRC_DIR/gui/*.cpp)
}

contains(DEFINES, TEST_CONNECT) {
    HEADERS += $$files($$PWD/connect/*.h) \
               $$files($$TOP_SRC_DIR/connect/*.h)

    SOURCES += $$files($$PWD/connect/*.cpp) \
               $$files($$TOP_SRC_DIR/connect/*.cpp)
}

contains(DEFINES, TEST_DBOBJECTS) {
    HEADERS += $$files($$PWD/dbobjects/common/*.h) \
               $$files($$PWD/dbobjects/mysql/*.h) \
               $$files($$PWD/dbobjects/psql/*.h) \
               $$files($$PWD/dbobjects/factory/*.h) \
               $$files($$TOP_SRC_DIR/dbobjects/common/*.h) \
               $$files($$TOP_SRC_DIR/dbobjects/mysql/*.h) \
               $$files($$TOP_SRC_DIR/dbobjects/psql/*.h) \
               $$files($$TOP_SRC_DIR/dbobjects/factory/*.h)

    SOURCES += $$files($$PWD/dbobjects/common/*.cpp) \
               $$files($$PWD/dbobjects/mysql/*.cpp) \
               $$files($$PWD/dbobjects/psql/*.cpp) \
               $$files($$PWD/dbobjects/factory/*.cpp)

}

### Expanding clean target
QMAKE_CLEAN += $$OUT_PWD/$$DESTDIR/$$TARGET


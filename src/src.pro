TEMPLATE = app
TARGET = visual-db

QT += sql network xml
CONFIG += debug

DESTDIR = ../bin

DEPENDPATH += . \
              dbobjects \
              dbobject/common \
              dbobject/factory \
              dbobject/psql \
              dbobject/mysql

INCLUDEPATH += $$PWD $$PWD/dbobjects

### Windows specific paths
win32 {
    Debug:LIBS += -L$$OUT_PWD/dbobjects/common/debug \
                  -L$$OUT_PWD/dbobjects/factory/debug \
                  -L$$OUT_PWD/dbobjects/psql/debug \
                  -L$$OUT_PWD/dbobjects/mysql/debug

    Release:LIBS += -L$$OUT_PWD/dbobjects/common/release \
                    -L$$OUT_PWD/dbobjects/factory/release \
                    -L$$OUT_PWD/dbobjects/psql/release \
                    -L$$OUT_PWD/dbobjects/mysql/release
}

### Unix specific paths
unix {
    LIBS += -L$$OUT_PWD/dbobjects/common \
            -L$$OUT_PWD/dbobjects/factory \
            -L$$OUT_PWD/dbobjects/psql \
            -L$$OUT_PWD/dbobjects/mysql
}

LIBS += -lcommon -lpsql -lmysql -lfactory -lpsql

## check dependencies
#CONFIG += link_pkgconfig
#PKGCONFIG += psql

MOC_DIR = .moc
OBJECTS_DIR = .obj

UI_DIR = .uic
UI_HEADERS_DIR = gui/ui

QMAKE_CXXFLAGS += -O2 -g2 -Wextra

FORMS = $$files($$PWD/gui/ui/*.ui)

HEADERS += $$files($$PWD/*.h) \
           $$files($$PWD/connect/*.h) \
           $$files($$PWD/gui/behaviour/*.h) \
           $$files($$PWD/gui/*.h)

### do not use wildcards for main.cpp and consts.cpp !!! it picks the genereated sources
SOURCES += $$PWD/main.cpp \
           $$PWD/consts.cpp \
           $$files($$PWD/connect/*.cpp) \
           $$files($$PWD/gui/behaviour/*.cpp) \
           $$files($$PWD/gui/*.cpp)

RESOURCES += $$PWD/visual_db.qrc

TRANSLATIONS += $$files($$PWD/*.ts)

### Expanding clean target
QMAKE_CLEAN += $$OUT_PWD/$$DESTDIR/$$TARGET


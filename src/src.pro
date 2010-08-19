TEMPLATE = app
TARGET = visual-db

DESTDIR = ../bin

DEPENDPATH += . dbobjects
INCLUDEPATH += . dbobjects

win32 {
    LIBS += -Ldbobjects/debug -Ldbobjects/release
}

LIBS += -Ldbobjects -ldbobjects

QT += sql network xml

CONFIG += debug

## check dependencies
#CONFIG += link_pkgconfig
#PKGCONFIG += psql

MOC_DIR = .moc
OBJECTS_DIR = .obj

UI_DIR = .uic
UI_HEADERS_DIR = gui/ui

QMAKE_CXXFLAGS += -O2 -g2 -Wextra

FORMS = gui/ui/*.ui

# Input
HEADERS += connect/*.h \
           gui/behaviour/*.h \
           gui/*.h

SOURCES += main.cpp \
           connect/*.cpp \
           gui/behaviour/*.cpp \
           gui/*.cpp

RESOURCES += visual_db.qrc

TRANSLATIONS += visual_db_en.ts visual_db_ru.ts

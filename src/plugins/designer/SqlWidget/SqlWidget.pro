CONFIG      += designer plugin
TARGET      = $$qtLibraryTarget($$TARGET)
TEMPLATE    = lib

TOP_SRC_DIR = $$PWD/../../..

QT += sql network gui

UI_DIR = .uic
UI_HEADERS_DIR = $$TOP_SRC_DIR/gui/ui
MOC_DIR = .moc
OBJECTS_DIR = .obj

CONFIG += debug

DEPENDPATH += $$TOP_SRC_DIR/. \
              $$TOP_SRC_DIR/dbobjects \
              $$TOP_SRC_DIR/dbobject/common \
              $$TOP_SRC_DIR/dbobject/factory \
              $$TOP_SRC_DIR/dbobject/psql \
              $$TOP_SRC_DIR/dbobject/mysql

### Windows specific paths
win32 {
    CONFIG(debug, debug|release):LIBS += -L$$TOP_SRC_DIR/dbobjects/common/debug \
                                         -L$$TOP_SRC_DIR/dbobjects/factory/debug \
                                         -L$$TOP_SRC_DIR/dbobjects/psql/debug \
                                         -L$$TOP_SRC_DIR/dbobjects/mysql/debug
    else:LIBS += -L$$TOP_SRC_DIR/dbobjects/common/release \
                 -L$$TOP_SRC_DIR/dbobjects/factory/release \
                 -L$$TOP_SRC_DIR/dbobjects/psql/release \
                 -L$$TOP_SRC_DIR/dbobjects/mysql/release

### Unix specific paths
} else:unix {
    LIBS += -L$$TOP_SRC_DIR/dbobjects/common \
            -L$$TOP_SRC_DIR/dbobjects/factory \
            -L$$TOP_SRC_DIR/dbobjects/psql \
            -L$$TOP_SRC_DIR/dbobjects/mysql
}

LIBS += -lcommon -lfactory -lpsql -lmysql -lcommon -lmysql

INCLUDEPATH += $$TOP_SRC_DIR/ $$TOP_SRC_DIR/dbobjects

FORMS = $$TOP_SRC_DIR/gui/ui/SqlWidget.ui

HEADERS = $$files($$TOP_SRC_DIR/connect/*.h) \
        $$files($$TOP_SRC_DIR/control/*.h) \
        $$files($$TOP_SRC_DIR/gui/behaviour/*.h) \
        $$files($$TOP_SRC_DIR/gui/graphicsitems/*.h) \
        $$files($$TOP_SRC_DIR/gui/strategy/*.h) \
        $$files($$TOP_SRC_DIR/gui/*.h) \
        SqlWidgetPlugin.h

SOURCES = $$files($$TOP_SRC_DIR/connect/*.cpp) \
        $$files($$TOP_SRC_DIR/control/*.cpp) \
        $$files($$TOP_SRC_DIR/gui/behaviour/*.cpp) \
        $$files($$TOP_SRC_DIR/gui/graphicsitems/*.cpp) \
        $$files($$TOP_SRC_DIR/gui/strategy/*.cpp) \
        $$files($$TOP_SRC_DIR/gui/*.cpp) \
        $$files($$TOP_SRC_DIR/*.cpp) \
        SqlWidgetPlugin.cpp

RESOURCES += $$TOP_SRC_DIR/visual_db.qrc

target.path = $$[QT_INSTALL_PLUGINS]/designer
sources.files = $$SOURCES $$HEADERS *.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/designer/SqlWidget
INSTALLS += target sources

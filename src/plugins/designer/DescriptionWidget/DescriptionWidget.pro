CONFIG      += designer plugin
TARGET      = $$qtLibraryTarget($$TARGET)
TEMPLATE    = lib

TOP_SRC_DIR = $$PWD/../../..

UI_DIR = .uic
UI_HEADERS_DIR = $$TOP_SRC_DIR/gui/ui
MOC_DIR = .moc
OBJECTS_DIR = .obj

QT += sql 

CONFIG += debug

DEPENDPATH += . \
              $$TOP_SRC_DIR/dbobjects \
              $$TOP_SRC_DIR/dbobject/common \
              $$TOP_SRC_DIR/dbobject/factory \
              $$TOP_SRC_DIR/dbobject/psql \
              $$TOP_SRC_DIR/dbobject/mysql

INCLUDEPATH += $$TOP_SRC_DIR/ $$TOP_SRC_DIR/dbobjects

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

FORMS = $$TOP_SRC_DIR/gui/ui/DescriptionWidget.ui

HEADERS = $$TOP_SRC_DIR/gui/DescriptionWidget.h \
      DescriptionWidgetPlugin.h

SOURCES = $$TOP_SRC_DIR/gui/DescriptionWidget.cpp \
      DescriptionWidgetPlugin.cpp

RESOURCES += $$TOP_SRC_DIR/visual_db.qrc

target.path = $$[QT_INSTALL_PLUGINS]/designer
sources.files = $$SOURCES $$HEADERS *.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/designer/DescriptionWidget
INSTALLS += target sources

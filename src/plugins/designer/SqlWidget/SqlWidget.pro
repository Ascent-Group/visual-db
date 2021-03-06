CONFIG      += designer plugin
TARGET      = $$qtLibraryTarget($$TARGET)
TEMPLATE    = lib

TOP_SRC_DIR = $$PWD/../../..

QT += sql network gui

UI_DIR = .uic
UI_HEADERS_DIR = $$TOP_SRC_DIR/gui/ui
MOC_DIR = .moc
OBJECTS_DIR = .obj

CONFIG += release

INCLUDEPATH += $$TOP_SRC_DIR/ $$TOP_SRC_DIR/dbobjects

FORMS = $$TOP_SRC_DIR/gui/ui/SqlWidget.ui

HEADERS = $$TOP_SRC_DIR/gui/SqlWidget.h \
      SqlWidgetPlugin.h

SOURCES = $$TOP_SRC_DIR/gui/SqlWidget.cpp \
      SqlWidgetPlugin.cpp

RESOURCES += $$TOP_SRC_DIR/visual_db.qrc

target.path = $$[QT_INSTALL_PLUGINS]/designer
sources.files = $$SOURCES $$HEADERS *.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/designer/SqlWidget
INSTALLS += target sources

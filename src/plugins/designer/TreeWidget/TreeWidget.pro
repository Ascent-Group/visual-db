CONFIG      += designer plugin
TARGET      = $$qtLibraryTarget($$TARGET)
TEMPLATE    = lib

TOP_SRC_DIR = $$PWD/../../..

QT += sql

UI_DIR = .uic
UI_HEADERS_DIR = $$TOP_SRC_DIR/gui/ui
MOC_DIR = .moc
OBJECTS_DIR = .obj

CONFIG += release

INCLUDEPATH += $$TOP_SRC_DIR/ $$TOP_SRC_DIR/dbobjects


HEADERS = $$TOP_SRC_DIR/gui/TreeWidget.h \
      TreeWidgetPlugin.h

SOURCES = $$TOP_SRC_DIR/gui/TreeWidget.cpp \
      TreeWidgetPlugin.cpp

RESOURCES += $$TOP_SRC_DIR/visual_db.qrc

target.path = $$[QT_INSTALL_PLUGINS]/designer
sources.files = $$SOURCES $$HEADERS *.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/designer/TreeWidget
INSTALLS += target sources

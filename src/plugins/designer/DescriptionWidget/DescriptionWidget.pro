CONFIG      += designer plugin
TARGET      = $$qtLibraryTarget($$TARGET)
TEMPLATE    = lib

TOP_SRC_DIR = $$PWD/../../..

UI_DIR = .uic
UI_HEADERS_DIR = $$TOP_SRC_DIR/gui/ui
MOC_DIR = .moc
OBJECTS_DIR = .obj

CONFIG += release

INCLUDEPATH += $$TOP_SRC_DIR/ $$TOP_SRC_DIR/dbobjects

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

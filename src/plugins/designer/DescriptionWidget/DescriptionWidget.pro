CONFIG      += designer plugin
TARGET      = $$qtLibraryTarget($$TARGET)
TEMPLATE    = lib

SRC     = ../../../

UI_DIR = .uic
UI_HEADERS_DIR = $$SRC/gui/ui
MOC_DIR = .moc
OBJECTS_DIR = .obj

CONFIG += release

INCLUDEPATH += $$SRC/ $$SRC/dbobjects

FORMS = $$SRC/gui/ui/DescriptionWidget.ui

HEADERS = $$SRC/gui/DescriptionWidget.h \
      DescriptionWidgetPlugin.h

SOURCES = $$SRC/gui/DescriptionWidget.cpp \
      DescriptionWidgetPlugin.cpp

RESOURCES += $$SRC/visual_db.qrc

target.path = $$[QT_INSTALL_PLUGINS]/designer
sources.files = $$SOURCES $$HEADERS *.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/designer/DescriptionWidget
INSTALLS += target sources

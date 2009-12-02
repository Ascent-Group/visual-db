CONFIG      += designer plugin
TARGET      = $$qtLibraryTarget($$TARGET)
TEMPLATE    = lib

QT += sql network gui

SRC	    = ../../../

UI_DIR = .uic
UI_HEADERS_DIR = $$SRC/gui/ui
MOC_DIR = .moc
OBJECTS_DIR = .obj

CONFIG += release

INCLUDEPATH += $$SRC/ $$SRC/dbobjects

FORMS = $$SRC/gui/ui/SqlWidget.ui

HEADERS = $$SRC/gui/SqlWidget.h \
	  SqlWidgetPlugin.h

SOURCES = $$SRC/gui/SqlWidget.cpp \
	  SqlWidgetPlugin.cpp

RESOURCES += $$SRC/visual_db.qrc

target.path = $$[QT_INSTALL_PLUGINS]/designer
sources.files = $$SOURCES $$HEADERS *.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/designer/SqlWidget
INSTALLS += target sources

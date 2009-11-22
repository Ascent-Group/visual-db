CONFIG      += designer plugin
TARGET      = $$qtLibraryTarget($$TARGET)
TEMPLATE    = lib

QT += sql
SRC	    = ../../../

UI_DIR = .uic
UI_HEADERS_DIR = $$SRC/gui/ui
MOC_DIR = .moc
OBJECTS_DIR = .obj

CONFIG += release

INCLUDEPATH += $$SRC/ $$SRC/dbobjects


HEADERS = $$SRC/gui/TreeWidget.h \
	  TreeWidgetPlugin.h

SOURCES = $$SRC/gui/TreeWidget.cpp \
	  TreeWidgetPlugin.cpp

RESOURCES += $$SRC/visual_db.qrc

target.path = $$[QT_INSTALL_PLUGINS]/designer
sources.files = $$SOURCES $$HEADERS *.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/designer/TreeWidget
INSTALLS += target

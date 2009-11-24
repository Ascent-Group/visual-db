CONFIG      += designer plugin
TARGET      = $$qtLibraryTarget($$TARGET)
TEMPLATE    = lib

SRC	    = ../../../

UI_DIR = .uic
UI_HEADERS_DIR = $$SRC/gui/ui
MOC_DIR = .moc
OBJECTS_DIR = .obj

CONFIG += release

INCLUDEPATH += $$SRC/

FORMS = $$SRC/gui/ui/ControlWidget.ui

HEADERS = $$SRC/gui/ControlWidget.h \
	  ControlWidgetPlugin.h

SOURCES = $$SRC/gui/ControlWidget.cpp \
	  ControlWidgetPlugin.cpp

RESOURCES += $$SRC/visual_db.qrc

target.path = $$[QT_INSTALL_PLUGINS]/designer
sources.files = $$SOURCES $$HEADERS *.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/designer/ControlWidget
INSTALLS += target sources

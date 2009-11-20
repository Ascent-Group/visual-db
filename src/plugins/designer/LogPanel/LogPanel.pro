CONFIG      += designer plugin
TARGET      = $$qtLibraryTarget($$TARGET)
TEMPLATE    = lib

CONFIG += release

INCLUDEPATH += ../../../

HEADERS = ../../../gui/LogPanel.h \
	  LogPanelPlugin.h

SOURCES = ../../../gui/LogPanel.cpp \
	  LogPanelPlugin.cpp

target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS += target

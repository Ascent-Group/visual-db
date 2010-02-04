CONFIG      += designer plugin
TARGET      = $$qtLibraryTarget($$TARGET)
TEMPLATE    = lib

SRC     = ../../../

UI_DIR = .uic
UI_HEADERS_DIR = $$SRC/gui/ui
MOC_DIR = .moc
OBJECTS_DIR = .obj

CONFIG += release

INCLUDEPATH += $$SRC/

FORMS = $$SRC/gui/ui/LogPanel.ui


HEADERS = $$SRC/gui/LogPanel.h \
      LogPanelPlugin.h

SOURCES = $$SRC/gui/LogPanel.cpp \
      LogPanelPlugin.cpp

RESOURCES += $$SRC/visual_db.qrc

target.path = $$[QT_INSTALL_PLUGINS]/designer
sources.files = $$SOURCES $$HEADERS *.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/designer/LogPanelPlugin
INSTALLS += target sources

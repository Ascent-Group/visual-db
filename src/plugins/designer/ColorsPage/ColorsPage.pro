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

FORMS = $$SRC/gui/ui/ColorsPage.ui \
    $$SRC/gui/ui/SelectColorWidget.ui

HEADERS = $$SRC/gui/ColorsPage.h \
      $$SRC/gui/SelectColorWidget.h \
      ColorsPagePlugin.h

SOURCES = $$SRC/gui/ColorsPage.cpp \
      $$SRC/gui/SelectColorWidget.cpp \
      ColorsPagePlugin.cpp

RESOURCES += $$SRC/visual_db.qrc

target.path = $$[QT_INSTALL_PLUGINS]/designer
sources.files = $$SOURCES $$HEADERS *.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/designer/ColorsPage
INSTALLS += target sources

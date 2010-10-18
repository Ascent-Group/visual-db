CONFIG      += designer plugin
TARGET      = $$qtLibraryTarget($$TARGET)
TEMPLATE    = lib

TOP_SRC_DIR = $$PWD/../../..

UI_DIR = .uic
UI_HEADERS_DIR = $$TOP_SRC_DIR/gui/ui
MOC_DIR = .moc
OBJECTS_DIR = .obj

CONFIG += release

INCLUDEPATH += $$TOP_SRC_DIR/

FORMS = $$TOP_SRC_DIR/gui/ui/AppearancePage.ui

HEADERS = $$TOP_SRC_DIR/gui/AppearancePage.h \
      AppearancePagePlugin.h

SOURCES = $$TOP_SRC_DIR/gui/AppearancePage.cpp \
      AppearancePagePlugin.cpp

RESOURCES += $$TOP_SRC_DIR/visual_db.qrc

target.path = $$[QT_INSTALL_PLUGINS]/designer
sources.files = $$SOURCES $$HEADERS *.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/designer/AppearancePage
INSTALLS += target sources

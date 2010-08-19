CONFIG      += designer plugin
TARGET      = $$qtLibraryTarget($$TARGET)
TEMPLATE    = lib

UI_DIR = .uic
UI_HEADERS_DIR = $$TOP_SRC_DIR/gui/ui
MOC_DIR = .moc
OBJECTS_DIR = .obj

CONFIG += release

INCLUDEPATH += $$TOP_SRC_DIR/

FORMS = $$TOP_SRC_DIR/gui/ui/PreferencesPage.ui

HEADERS = $$TOP_SRC_DIR/gui/PreferencesPage.h \
      PreferencesPagePlugin.h

SOURCES = $$TOP_SRC_DIR/gui/PreferencesPage.cpp \
      PreferencesPagePlugin.cpp

RESOURCES += $$TOP_SRC_DIR/visual_db.qrc

target.path = $$[QT_INSTALL_PLUGINS]/designer
sources.files = $$SOURCES $$HEADERS *.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/designer/PreferencesPage
INSTALLS += target sources

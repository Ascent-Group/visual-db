CONFIG      += designer plugin
TARGET      = $$qtLibraryTarget($$TARGET)
TEMPLATE    = lib

QT += xml gui

SRC     = ../../../

UI_DIR = .uic
UI_HEADERS_DIR = $$SRC/gui/ui
MOC_DIR = .moc
OBJECTS_DIR = .obj

CONFIG += release

INCLUDEPATH += $$SRC/ $$SRC/dbobjects

LIBS += -L$$SRC/dbobjects -ldbobjects

FORMS += $$SRC/gui/ui/ControlWidget.ui

HEADERS = $$SRC/gui/SceneWidget.h \
      $$SRC/gui/ArrowItem.h \
      $$SRC/gui/ControlWidget.h \
      $$SRC/gui/GraphicsItem.h \
      $$SRC/gui/GraphicsScene.h \
      $$SRC/gui/GraphicsView.h \
      $$SRC/gui/Legend.h \
      $$SRC/gui/TableItem.h \
      $$SRC/gui/TableItemGroup.h \
      $$SRC/gui/behaviour/AddTableCommand.h \
      $$SRC/gui/behaviour/DeleteTableCommand.h \
      $$SRC/gui/behaviour/MoveTableCommand.h \
      SceneWidgetPlugin.h

SOURCES = $$SRC/gui/SceneWidget.cpp \
      $$SRC/gui/ArrowItem.cpp \
      $$SRC/gui/ControlWidget.cpp \
      $$SRC/gui/GraphicsItem.cpp \
      $$SRC/gui/GraphicsScene.cpp \
      $$SRC/gui/GraphicsView.cpp \
      $$SRC/gui/Legend.cpp \
      $$SRC/gui/TableItem.cpp \
      $$SRC/gui/TableItemGroup.cpp \
      $$SRC/gui/behaviour/AddTableCommand.cpp \
      $$SRC/gui/behaviour/DeleteTableCommand.cpp \
      $$SRC/gui/behaviour/MoveTableCommand.cpp \
      SceneWidgetPlugin.cpp

RESOURCES += $$SRC/visual_db.qrc

target.path = $$[QT_INSTALL_PLUGINS]/designer
sources.files = $$SOURCES $$HEADERS *.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/designer/SceneWidget
INSTALLS += target sources

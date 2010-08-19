CONFIG      += designer plugin
TARGET      = $$qtLibraryTarget($$TARGET)
TEMPLATE    = lib

QT += xml gui

UI_DIR = .uic
UI_HEADERS_DIR = $$TOP_SRC_DIR/gui/ui
MOC_DIR = .moc
OBJECTS_DIR = .obj

CONFIG += release

INCLUDEPATH += $$TOP_SRC_DIR/ $$TOP_SRC_DIR/dbobjects

LIBS += -L$$TOP_SRC_DIR/dbobjects -ldbobjects

FORMS += $$TOP_SRC_DIR/gui/ui/ControlWidget.ui

HEADERS = $$TOP_SRC_DIR/gui/SceneWidget.h \
      $$TOP_SRC_DIR/gui/ArrowItem.h \
      $$TOP_SRC_DIR/gui/ControlWidget.h \
      $$TOP_SRC_DIR/gui/GraphicsItem.h \
      $$TOP_SRC_DIR/gui/GraphicsScene.h \
      $$TOP_SRC_DIR/gui/GraphicsView.h \
      $$TOP_SRC_DIR/gui/Legend.h \
      $$TOP_SRC_DIR/gui/TableItem.h \
      $$TOP_SRC_DIR/gui/TableItemGroup.h \
      $$TOP_SRC_DIR/gui/behaviour/AddTableCommand.h \
      $$TOP_SRC_DIR/gui/behaviour/DeleteTableCommand.h \
      $$TOP_SRC_DIR/gui/behaviour/MoveTableCommand.h \
      SceneWidgetPlugin.h

SOURCES = $$TOP_SRC_DIR/gui/SceneWidget.cpp \
      $$TOP_SRC_DIR/gui/ArrowItem.cpp \
      $$TOP_SRC_DIR/gui/ControlWidget.cpp \
      $$TOP_SRC_DIR/gui/GraphicsItem.cpp \
      $$TOP_SRC_DIR/gui/GraphicsScene.cpp \
      $$TOP_SRC_DIR/gui/GraphicsView.cpp \
      $$TOP_SRC_DIR/gui/Legend.cpp \
      $$TOP_SRC_DIR/gui/TableItem.cpp \
      $$TOP_SRC_DIR/gui/TableItemGroup.cpp \
      $$TOP_SRC_DIR/gui/behaviour/AddTableCommand.cpp \
      $$TOP_SRC_DIR/gui/behaviour/DeleteTableCommand.cpp \
      $$TOP_SRC_DIR/gui/behaviour/MoveTableCommand.cpp \
      SceneWidgetPlugin.cpp

RESOURCES += $$TOP_SRC_DIR/visual_db.qrc

target.path = $$[QT_INSTALL_PLUGINS]/designer
sources.files = $$SOURCES $$HEADERS *.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/designer/SceneWidget
INSTALLS += target sources

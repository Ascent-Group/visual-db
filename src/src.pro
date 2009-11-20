TEMPLATE = app
TARGET = visual-db

DESTDIR = ../bin

DEPENDPATH += . dbobjects
INCLUDEPATH += . dbobjects

LIBS += -Ldbobjects -Ldbobjects/debug -Ldbobjects/release -ldbobjects

QT += sql network xml

CONFIG += debug

MOC_DIR = .moc
OBJECTS_DIR = .obj

QMAKE_CXXFLAGS += -g2

# Input
HEADERS += gui/ArrowItem.h \
    connect/DbParameters.h \
    connect/ProxyParameters.h \
    gui/ControlWidget.h \
    gui/TreeWidget.h \
    gui/DescriptionWidget.h \
    gui/GraphicsScene.h \
    gui/GraphicsView.h \
    gui/MainWindow.h \
    gui/SqlConnectionDialog.h \
    gui/SqlWidget.h \
    gui/OptionsDialog.h \
    gui/AppearancePage.h \
    gui/PreferencesPage.h \
    gui/SelectColorWidget.h \
    gui/ColorsPage.h \
    gui/GraphicsItem.h \
    gui/TableItem.h \
    gui/TableItemGroup.h \
    gui/TabWidget.h \
    gui/Legend.h \
    gui/SceneWidget.h \
    gui/behaviour/MoveTableCommand.h \
    gui/behaviour/AddTableCommand.h

SOURCES += main.cpp \
    connect/DbParameters.cpp \
    connect/ProxyParameters.cpp \
    gui/ArrowItem.cpp \
    gui/ControlWidget.cpp \
    gui/TreeWidget.cpp \
    gui/DescriptionWidget.cpp \
    gui/GraphicsScene.cpp \
    gui/GraphicsView.cpp \
    gui/MainWindow.cpp \
    gui/SqlConnectionDialog.cpp \
    gui/SqlWidget.cpp \
    gui/OptionsDialog.cpp \
    gui/AppearancePage.cpp \
    gui/PreferencesPage.cpp \
    gui/SelectColorWidget.cpp \
    gui/ColorsPage.cpp \
    gui/GraphicsItem.cpp \
    gui/TableItem.cpp \
    gui/TableItemGroup.cpp \
    gui/TabWidget.cpp \
    gui/Legend.cpp \
    gui/SceneWidget.cpp \
    gui/behaviour/MoveTableCommand.cpp \
    gui/behaviour/AddTableCommand.cpp

RESOURCES += visual_db.qrc

TRANSLATIONS += visual_db_en.ts visual_db_ru.ts

TEMPLATE = app
TARGET = visual-db

DESTDIR = ../bin

DEPENDPATH += . dbobjects
INCLUDEPATH += . dbobjects

win32 {
    LIBS += -Ldbobjects/debug -Ldbobjects/release
}

LIBS += -Ldbobjects -ldbobjects

QT += sql network xml

CONFIG += debug

MOC_DIR = .moc
OBJECTS_DIR = .obj

UI_DIR = .uic
UI_HEADERS_DIR = gui/ui

QMAKE_CXXFLAGS += -g2

FORMS = gui/ui/LogPanel.ui

# Input
HEADERS += connect/DbParameters.h \
	   connect/ProxyParameters.h \
	   gui/behaviour/AddTableCommand.h \
	   gui/behaviour/MoveTableCommand.h \
	   gui/AppearancePage.h \
	   gui/ArrowItem.h \
	   gui/ColorsPage.h \
	   gui/ControlWidget.h \
	   gui/DescriptionWidget.h \
	   gui/GraphicsItem.h \
	   gui/GraphicsScene.h \
	   gui/GraphicsView.h \
	   gui/Legend.h \
	   gui/LogPanel.h \
	   gui/MainWindow.h \
	   gui/OptionsDialog.h \
	   gui/PreferencesPage.h \
	   gui/SceneWidget.h \
	   gui/SelectColorWidget.h \
	   gui/SqlConnectionDialog.h \
	   gui/SqlWidget.h \
	   gui/TableItem.h \
	   gui/TableItemGroup.h \
	   gui/TabWidget.h \
	   gui/TreeWidget.h

SOURCES += main.cpp \
	   connect/DbParameters.cpp \
	   connect/ProxyParameters.cpp \
	   gui/behaviour/AddTableCommand.cpp \
	   gui/behaviour/MoveTableCommand.cpp \
	   gui/AppearancePage.cpp \
	   gui/ArrowItem.cpp \
	   gui/ColorsPage.cpp \
	   gui/ControlWidget.cpp \
	   gui/DescriptionWidget.cpp \
	   gui/GraphicsItem.cpp \
	   gui/GraphicsScene.cpp \
	   gui/GraphicsView.cpp \
	   gui/Legend.cpp \
	   gui/LogPanel.cpp \
	   gui/MainWindow.cpp \
	   gui/OptionsDialog.cpp \
	   gui/PreferencesPage.cpp \
	   gui/SceneWidget.cpp \
	   gui/SelectColorWidget.cpp \
	   gui/SqlConnectionDialog.cpp \
	   gui/SqlWidget.cpp \
	   gui/TableItem.cpp \
	   gui/TableItemGroup.cpp \
	   gui/TabWidget.cpp \
	   gui/TreeWidget.cpp

RESOURCES += visual_db.qrc

TRANSLATIONS += visual_db_en.ts visual_db_ru.ts

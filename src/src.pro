TEMPLATE = app
TARGET = visual-db

DESTDIR = $$OUT_PWD/../bin

DEPENDPATH += . dbobjects
INCLUDEPATH += $$PWD $$PWD/dbobjects

win32 {
    LIBS += -L$$OUT_PWD/dbobjects/debug -L$$OUT_PWD/dbobjects/release
}

LIBS += -L$$OUT_PWD/dbobjects -ldbobjects

QT += sql network xml

CONFIG += debug

## check dependencies
#CONFIG += link_pkgconfig
#PKGCONFIG += psql

MOC_DIR = .moc
OBJECTS_DIR = .obj

UI_DIR = .uic
UI_HEADERS_DIR = gui/ui

QMAKE_CXXFLAGS += -O2 -g2 -Wextra

FORMS = $$PWD/gui/ui/AppearancePage.ui \
        $$PWD/gui/ui/ColorsPage.ui \
        $$PWD/gui/ui/ControlWidget.ui \
        $$PWD/gui/ui/DescriptionWidget.ui \
        $$PWD/gui/ui/LogPanel.ui \
        $$PWD/gui/ui/MainWindow.ui \
        $$PWD/gui/ui/OptionsDialog.ui \
        $$PWD/gui/ui/PreferencesPage.ui \
        $$PWD/gui/ui/SelectColorWidget.ui \
        $$PWD/gui/ui/SqlConnectionDialog.ui \
        $$PWD/gui/ui/SqlWidget.ui

HEADERS += $$PWD/gui/behaviour/AddTableCommand.h \
           $$PWD/gui/behaviour/DeleteTableCommand.h \
           $$PWD/gui/behaviour/MoveTableCommand.h \
           $$PWD/connect/DbParameters.h \
           $$PWD/connect/ProxyParameters.h \
           $$PWD/gui/AppearancePage.h \
           $$PWD/gui/ArrowItem.h \
           $$PWD/gui/ColorsPage.h \
           $$PWD/gui/ControlWidget.h \
           $$PWD/gui/DescriptionWidget.h \
           $$PWD/gui/GraphicsItem.h \
           $$PWD/gui/GraphicsScene.h \
           $$PWD/gui/GraphicsView.h \
           $$PWD/gui/Legend.h \
           $$PWD/gui/LogPanel.h \
           $$PWD/gui/MainWindow.h \
           $$PWD/gui/OptionsDialog.h \
           $$PWD/gui/PreferencesPage.h \
           $$PWD/gui/SceneWidget.h \
           $$PWD/gui/SelectColorWidget.h \
           $$PWD/gui/SqlConnectionDialog.h \
           $$PWD/gui/SqlWidget.h \
           $$PWD/gui/TableItem.h \
           $$PWD/gui/TableItemGroup.h \
           $$PWD/gui/TabWidget.h \
           $$PWD/gui/TreeWidget.h

SOURCES += $$PWD/main.cpp \
           $$PWD/consts.cpp \
           $$PWD/gui/behaviour/AddTableCommand.cpp \
           $$PWD/gui/behaviour/DeleteTableCommand.cpp \
           $$PWD/gui/behaviour/MoveTableCommand.cpp \
           $$PWD/connect/DbParameters.cpp \
           $$PWD/connect/ProxyParameters.cpp \
           $$PWD/gui/AppearancePage.cpp \
           $$PWD/gui/ArrowItem.cpp \
           $$PWD/gui/ColorsPage.cpp \
           $$PWD/gui/ControlWidget.cpp \
           $$PWD/gui/DescriptionWidget.cpp \
           $$PWD/gui/GraphicsItem.cpp \
           $$PWD/gui/GraphicsScene.cpp \
           $$PWD/gui/GraphicsView.cpp \
           $$PWD/gui/Legend.cpp \
           $$PWD/gui/LogPanel.cpp \
           $$PWD/gui/MainWindow.cpp \
           $$PWD/gui/OptionsDialog.cpp \
           $$PWD/gui/PreferencesPage.cpp \
           $$PWD/gui/SceneWidget.cpp \
           $$PWD/gui/SelectColorWidget.cpp \
           $$PWD/gui/SqlConnectionDialog.cpp \
           $$PWD/gui/SqlWidget.cpp \
           $$PWD/gui/TableItem.cpp \
           $$PWD/gui/TableItemGroup.cpp \
           $$PWD/gui/TabWidget.cpp \
           $$PWD/gui/TreeWidget.cpp

RESOURCES += $$PWD/visual_db.qrc

TRANSLATIONS += $$PWD/visual_db_en.ts \
                $$PWD/visual_db_ru.ts


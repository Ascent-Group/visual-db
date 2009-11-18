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
HEADERS += ArrowItem.h \
    ControlWidget.h \
    TreeWidget.h \
    DescriptionWidget.h \
    GraphicsScene.h \
    GraphicsView.h \
    MainWindow.h \
    SqlConnectionDialog.h \
    DbParameters.h \
    ProxyParameters.h \
    SqlWidget.h \
    OptionsDialog.h \
    AppearancePage.h \
    PreferencesPage.h \
    SelectColorWidget.h \
    ColorsPage.h \
    GraphicsItem.h \
    TableItem.h \
    TableItemGroup.h \
    TabWidget.h \
    Legend.h \
    SceneWidget.h

SOURCES += ArrowItem.cpp \
    ControlWidget.cpp \
    TreeWidget.cpp \
    DescriptionWidget.cpp \
    GraphicsScene.cpp \
    GraphicsView.cpp \
    main.cpp \
    MainWindow.cpp \
    SqlConnectionDialog.cpp \
    DbParameters.cpp \
    ProxyParameters.cpp \
    SqlWidget.cpp \
    OptionsDialog.cpp \
    AppearancePage.cpp \
    PreferencesPage.cpp \
    SelectColorWidget.cpp \
    ColorsPage.cpp \
    GraphicsItem.cpp \
    TableItem.cpp \
    TableItemGroup.cpp \
    TabWidget.cpp \
    Legend.cpp \
    SceneWidget.cpp

RESOURCES += visual_db.qrc

TRANSLATIONS += visual_db_en.ts visual_db_ru.ts

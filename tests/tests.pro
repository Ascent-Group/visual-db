include (../src/src.pro)

message("We advice you run 'make -j3'")

QT += testlib sql network xml
#CONFIG += release

# We need to override old TARGET and DESTDIR
TARGET = tests
DESTDIR = ./

MOC_DIR = .moc
OBJECTS_DIR = .obj

DEPENDPATH += . dbobjects ../src ../src/dbobjects
INCLUDEPATH += . dbobjects ../src ../src/dbobjects

QMAKE_CXXFLAGS = -g -Wextra

LIBS += -L../src/dbobjects -ldbobjects

#DEFINES += QT_NO_DEBUG_OUTPUT QT_NO_WARNING_OUTPUT

#DEFINES += TEST_BEHAVIOUR
#DEFINES += TEST_GUI
#DEFINES += TEST_CONNECT
DEFINES += TEST_DBOBJECTS

contains(DEFINES, TEST_BEHAVIOUR) {
    HEADERS += gui/behaviour/AddTableCommandTest.h \
               gui/behaviour/MoveTableCommandTest.h

    SOURCES += gui/behaviour/AddTableCommandTest.cpp \
               gui/behaviour/MoveTableCommandTest.cpp
}

contains(DEFINES, TEST_GUI) {
    HEADERS += gui/AppearancePageTest.h \
               gui/ArrowItemTest.h  \
               gui/ColorsPageTest.h \
               gui/ControlWidgetTest.h \
               gui/DescriptionWidgetTest.h \
               gui/GraphicsItemTest.h \
               gui/GraphicsSceneTest.h \
               gui/GraphicsViewTest.h \
               gui/LegendTest.h \
               gui/LogPanelTest.h \
               gui/MainWindowTest.h \
               gui/OptionsDialogTest.h \
               gui/PreferencesPageTest.h \
               gui/SceneWidgetTest.h \
               gui/SelectColorWidgetTest.h \
               gui/SqlConnectionDialogTest.h \
               gui/SqlWidgetTest.h \
               gui/TableItemGroupTest.h \
               gui/TableItemTest.h \
               gui/TabWidgetTest.h \
               gui/TreeWidgetTest.h

    SOURCES += gui/AppearancePageTest.cpp \
               gui/ArrowItemTest.cpp \
               gui/ColorsPageTest.cpp \
               gui/ControlWidgetTest.cpp \
               gui/DescriptionWidgetTest.cpp \
               gui/GraphicsItemTest.cpp \
               gui/GraphicsSceneTest.cpp \
               gui/GraphicsViewTest.cpp \
               gui/LegendTest.cpp \
               gui/LogPanelTest.cpp \
               gui/OptionsDialogTest.cpp \
               gui/MainWindowTest.cpp \
               gui/PreferencesPageTest.cpp \
               gui/SceneWidgetTest.cpp \
               gui/SelectColorWidgetTest.cpp \
               gui/SqlConnectionDialogTest.cpp \
               gui/SqlWidgetTest.cpp \
               gui/TableItemGroupTest.cpp \
               gui/TableItemTest.cpp \
               gui/TabWidgetTest.cpp \
               gui/TreeWidgetTest.cpp
}

contains(DEFINES, TEST_CONNECT) {
    HEADERS += connect/DbParametersTest.h \
               connect/ProxyParametersTest.h

    SOURCES += connect/DbParametersTest.cpp \
               connect/ProxyParametersTest.cpp
}

contains(DEFINES, TEST_DBOBJECTS) {
    HEADERS += dbobjects/common/DatabaseTest.h \
               dbobjects/common/DbIndexTest.h \
               dbobjects/common/DbLanguageTest.h \
               dbobjects/common/DbObjectTest.h \
               dbobjects/common/DbProcedureTest.h \
               dbobjects/common/DbRoleTest.h \
               dbobjects/common/DbSchemaTest.h \
               dbobjects/common/DbTableTest.h \
               dbobjects/common/DbTriggerTest.h \
               dbobjects/common/DbViewTest.h \
               dbobjects/mysql/MysqlTableTest.h \
               dbobjects/psql/PsqlIndexTest.h \
               dbobjects/psql/PsqlLanguageTest.h \
               dbobjects/psql/PsqlProcedureTest.h \
               dbobjects/psql/PsqlRoleTest.h \
               dbobjects/psql/PsqlTableTest.h \
               dbobjects/psql/PsqlTriggerTest.h \
               dbobjects/psql/PsqlViewTest.h

    SOURCES += dbobjects/common/DatabaseTest.cpp \
               dbobjects/common/DbIndexTest.cpp \
               dbobjects/common/DbLanguageTest.cpp \
               dbobjects/common/DbObjectTest.cpp \
               dbobjects/common/DbProcedureTest.cpp \
               dbobjects/common/DbRoleTest.cpp \
               dbobjects/common/DbSchemaTest.cpp \
               dbobjects/common/DbTableTest.cpp \
               dbobjects/common/DbTriggerTest.cpp \
               dbobjects/common/DbViewTest.cpp \
               dbobjects/mysql/MysqlTableTest.cpp \
               dbobjects/psql/PsqlIndexTest.cpp \
               dbobjects/psql/PsqlLanguageTest.cpp \
               dbobjects/psql/PsqlProcedureTest.cpp \
               dbobjects/psql/PsqlRoleTest.cpp \
               dbobjects/psql/PsqlTableTest.cpp \
               dbobjects/psql/PsqlTriggerTest.cpp \
               dbobjects/psql/PsqlViewTest.cpp
}


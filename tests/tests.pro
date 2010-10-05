message("We advice you run 'make -j3'")

QT += testlib sql network xml
#CONFIG += object_with_source
#CONFIG += release

# We need to override old TARGET and DESTDIR
TARGET = tests
DESTDIR = .

compresslibs.commands = echo "======="

target.depends += compresslibs

MOC_DIR = .moc
OBJECTS_DIR = .obj

TOP_SRC_DIR = $$PWD/../src

DEPENDPATH = . dbobjects $$TOP_SRC_DIR $$TOP_SRC_DIR/dbobjects
INCLUDEPATH = $$PWD $$PWD/dbobjects $$TOP_SRC_DIR $$TOP_SRC_DIR/dbobjects

QMAKE_CXXFLAGS = -g -Wextra

LIBS += -L$$OUT_PWD/../src/dbobjects/common \
        -L$$OUT_PWD/../src/dbobjects/factory \
        -L$$OUT_PWD/../src/dbobjects/psql \
        -L$$OUT_PWD/../src/dbobjects/mysql

LIBS += -lcommon -lpsql -lfactory -lcommon -lmysql -lpsql -lfactory -lcommon -lpsql -lfactory -lmysql -lpsql

#QMAKE_LFLAGS += -Wl,--start-group -lcommon -lpsql -lmysql -lfactory -Wl,--end-group

DEFINES -= QT_NO_DEBUG_OUTPUT QT_NO_WARNING_OUTPUT

#DEFINES += TEST_BEHAVIOUR
#DEFINES += TEST_GUI
DEFINES += TEST_CONNECT
DEFINES += TEST_DBOBJECTS

SOURCES = $$PWD/main.cpp\
          $$TOP_SRC_DIR/consts.cpp

HEADERS = $$TOP_SRC_DIR/consts.h

contains(DEFINES, TEST_BEHAVIOUR) {
    HEADERS += $$PWD/gui/behaviour/AddTableCommandTest.h \
               $$PWD/gui/behaviour/DeleteTableCommandTest.h \
               $$PWD/gui/behaviour/MoveTableCommandTest.h \
               $$TOP_SRC_DIR/gui/behaviour/AddTableCommand.h \
               $$TOP_SRC_DIR/gui/behaviour/DeleteTableCommand.h \
               $$TOP_SRC_DIR/gui/behaviour/MoveTableCommand.h

    SOURCES += $$PWD/gui/behaviour/AddTableCommandTest.cpp \
               $$PWD/gui/behaviour/DeleteTableCommandTest.cpp \
               $$PWD/gui/behaviour/MoveTableCommandTest.cpp \
               $$TOP_SRC_DIR/gui/behaviour/AddTableCommand.cpp \
               $$TOP_SRC_DIR/gui/behaviour/DeleteTableCommand.cpp \
               $$TOP_SRC_DIR/gui/behaviour/MoveTableCommand.cpp
}

contains(DEFINES, TEST_GUI) {
    HEADERS += $$PWD/gui/AppearancePageTest.h \
               $$PWD/gui/ArrowItemTest.h \
               $$PWD/gui/ColorsPageTest.h \
               $$PWD/gui/ControlWidgetTest.h \
               $$PWD/gui/DescriptionWidgetTest.h \
               $$PWD/gui/GraphicsItemTest.h \
               $$PWD/gui/GraphicsSceneTest.h \
               $$PWD/gui/GraphicsViewTest.h \
               $$PWD/gui/LegendTest.h \
               $$PWD/gui/LogPanelTest.h \
               $$PWD/gui/MainWindowTest.h \
               $$PWD/gui/OptionsDialogTest.h \
               $$PWD/gui/PreferencesPageTest.h \
               $$PWD/gui/SceneWidgetTest.h \
               $$PWD/gui/SelectColorWidgetTest.h \
               $$PWD/gui/SqlConnectionDialogTest.h \
               $$PWD/gui/SqlWidgetTest.h \
               $$PWD/gui/TableItemTest.h \
               $$PWD/gui/TableItemGroupTest.h \
               $$PWD/gui/TabWidgetTest.h \
               $$PWD/gui/TreeWidgetTest.h \
               $$TOP_SRC_DIR/gui/AppearancePage.h \
               $$TOP_SRC_DIR/gui/ArrowItem.h \
               $$TOP_SRC_DIR/gui/ColorsPage.h \
               $$TOP_SRC_DIR/gui/ControlWidget.h \
               $$TOP_SRC_DIR/gui/DescriptionWidget.h \
               $$TOP_SRC_DIR/gui/GraphicsItem.h \
               $$TOP_SRC_DIR/gui/GraphicsScene.h \
               $$TOP_SRC_DIR/gui/GraphicsView.h \
               $$TOP_SRC_DIR/gui/Legend.h \
               $$TOP_SRC_DIR/gui/LogPanel.h \
               $$TOP_SRC_DIR/gui/MainWindow.h \
               $$TOP_SRC_DIR/gui/OptionsDialog.h \
               $$TOP_SRC_DIR/gui/PreferencesPage.h \
               $$TOP_SRC_DIR/gui/SceneWidget.h \
               $$TOP_SRC_DIR/gui/SelectColorWidget.h \
               $$TOP_SRC_DIR/gui/SqlConnectionDialog.h \
               $$TOP_SRC_DIR/gui/SqlWidget.h \
               $$TOP_SRC_DIR/gui/TableItem.h \
               $$TOP_SRC_DIR/gui/TableItemGroup.h \
               $$TOP_SRC_DIR/gui/TabWidget.h \
               $$TOP_SRC_DIR/gui/TreeWidget.h

    SOURCES += $$PWD/gui/AppearancePageTest.cpp \
               $$PWD/gui/ArrowItemTest.cpp \
               $$PWD/gui/ColorsPageTest.cpp \
               $$PWD/gui/ControlWidgetTest.cpp \
               $$PWD/gui/DescriptionWidgetTest.cpp \
               $$PWD/gui/GraphicsItemTest.cpp \
               $$PWD/gui/GraphicsSceneTest.cpp \
               $$PWD/gui/GraphicsViewTest.cpp \
               $$PWD/gui/LegendTest.cpp \
               $$PWD/gui/LogPanelTest.cpp \
               $$PWD/gui/MainWindowTest.cpp \
               $$PWD/gui/OptionsDialogTest.cpp \
               $$PWD/gui/PreferencesPageTest.cpp \
               $$PWD/gui/SceneWidgetTest.cpp \
               $$PWD/gui/SelectColorWidgetTest.cpp \
               $$PWD/gui/SqlConnectionDialogTest.cpp \
               $$PWD/gui/SqlWidgetTest.cpp \
               $$PWD/gui/TableItemTest.cpp \
               $$PWD/gui/TableItemGroupTest.cpp \
               $$PWD/gui/TabWidgetTest.cpp \
               $$PWD/gui/TreeWidgetTest.cpp \
               $$TOP_SRC_DIR/gui/AppearancePage.cpp \
               $$TOP_SRC_DIR/gui/ArrowItem.cpp \
               $$TOP_SRC_DIR/gui/ColorsPage.cpp \
               $$TOP_SRC_DIR/gui/ControlWidget.cpp \
               $$TOP_SRC_DIR/gui/DescriptionWidget.cpp \
               $$TOP_SRC_DIR/gui/GraphicsItem.cpp \
               $$TOP_SRC_DIR/gui/GraphicsScene.cpp \
               $$TOP_SRC_DIR/gui/GraphicsView.cpp \
               $$TOP_SRC_DIR/gui/Legend.cpp \
               $$TOP_SRC_DIR/gui/LogPanel.cpp \
               $$TOP_SRC_DIR/gui/MainWindow.cpp \
               $$TOP_SRC_DIR/gui/OptionsDialog.cpp \
               $$TOP_SRC_DIR/gui/PreferencesPage.cpp \
               $$TOP_SRC_DIR/gui/SceneWidget.cpp \
               $$TOP_SRC_DIR/gui/SelectColorWidget.cpp \
               $$TOP_SRC_DIR/gui/SqlConnectionDialog.cpp \
               $$TOP_SRC_DIR/gui/SqlWidget.cpp \
               $$TOP_SRC_DIR/gui/TableItem.cpp \
               $$TOP_SRC_DIR/gui/TableItemGroup.cpp \
               $$TOP_SRC_DIR/gui/TabWidget.cpp \
               $$TOP_SRC_DIR/gui/TreeWidget.cpp
}

contains(DEFINES, TEST_CONNECT) {
    HEADERS += $$PWD/connect/DbParametersTest.h \
               $$PWD/connect/ProxyParametersTest.h \
               $$TOP_SRC_DIR/connect/DbParameters.h \
               $$TOP_SRC_DIR/connect/ProxyParameters.h

    SOURCES += $$PWD/connect/DbParametersTest.cpp \
               $$PWD/connect/ProxyParametersTest.cpp \
               $$TOP_SRC_DIR/connect/DbParameters.cpp \
               $$TOP_SRC_DIR/connect/ProxyParameters.cpp
}

contains(DEFINES, TEST_DBOBJECTS) {
    HEADERS += $$PWD/dbobjects/common/DatabaseTest.h \
               $$PWD/dbobjects/common/DatabaseCreator.h \
               $$PWD/dbobjects/common/DbIndexTest.h \
               $$PWD/dbobjects/common/DbLanguageTest.h \
               $$PWD/dbobjects/common/DbObjectTest.h \
               $$PWD/dbobjects/common/DbProcedureTest.h \
               $$PWD/dbobjects/common/DbRoleTest.h \
               $$PWD/dbobjects/common/DbTableTest.h \
               $$PWD/dbobjects/common/DbTriggerTest.h \
               $$PWD/dbobjects/common/DbViewTest.h \
               $$PWD/dbobjects/common/DbSchemaTest.h \
               $$PWD/dbobjects/mysql/MysqlTableTest.h \
               $$PWD/dbobjects/mysql/MysqlToolsTest.h \
               $$PWD/dbobjects/psql/PsqlIndexTest.h \
               $$PWD/dbobjects/psql/PsqlLanguageTest.h \
               $$PWD/dbobjects/psql/PsqlProcedureTest.h \
               $$PWD/dbobjects/psql/PsqlRoleTest.h \
               $$PWD/dbobjects/psql/PsqlTableTest.h \
               $$PWD/dbobjects/psql/PsqlToolsTest.h \
               $$PWD/dbobjects/psql/PsqlTriggerTest.h \
               $$PWD/dbobjects/psql/PsqlViewTest.h \
               $$PWD/dbobjects/factory/IndexFactoryTest.h \
               $$PWD/dbobjects/factory/LanguageFactoryTest.h \
               $$PWD/dbobjects/factory/ProcedureFactoryTest.h \
               $$PWD/dbobjects/factory/RoleFactoryTest.h \
               $$PWD/dbobjects/factory/TableFactoryTest.h \
               $$PWD/dbobjects/factory/TriggerFactoryTest.h \
               $$PWD/dbobjects/factory/ViewFactoryTest.h \
               $$TOP_SRC_DIR/dbobjects/common/Database.h \
               $$TOP_SRC_DIR/dbobjects/common/DbIndex.h \
               $$TOP_SRC_DIR/dbobjects/common/DbLanguage.h \
               $$TOP_SRC_DIR/dbobjects/common/DbObject.h \
               $$TOP_SRC_DIR/dbobjects/common/DbProcedure.h \
               $$TOP_SRC_DIR/dbobjects/common/DbRole.h \
               $$TOP_SRC_DIR/dbobjects/common/DbTable.h \
               $$TOP_SRC_DIR/dbobjects/common/DbTrigger.h \
               $$TOP_SRC_DIR/dbobjects/common/DbView.h \
               $$TOP_SRC_DIR/dbobjects/common/DbSchema.h \
               $$TOP_SRC_DIR/dbobjects/mysql/Table.h \
               $$TOP_SRC_DIR/dbobjects/mysql/Tools.h \
               $$TOP_SRC_DIR/dbobjects/psql/Index.h \
               $$TOP_SRC_DIR/dbobjects/psql/Language.h \
               $$TOP_SRC_DIR/dbobjects/psql/Procedure.h \
               $$TOP_SRC_DIR/dbobjects/psql/Role.h \
               $$TOP_SRC_DIR/dbobjects/psql/Table.h \
               $$TOP_SRC_DIR/dbobjects/psql/Tools.h \
               $$TOP_SRC_DIR/dbobjects/psql/Trigger.h \
               $$TOP_SRC_DIR/dbobjects/psql/View.h \
               $$TOP_SRC_DIR/dbobjects/factory/Index.h \
               $$TOP_SRC_DIR/dbobjects/factory/Language.h \
               $$TOP_SRC_DIR/dbobjects/factory/Procedure.h \
               $$TOP_SRC_DIR/dbobjects/factory/Role.h \
               $$TOP_SRC_DIR/dbobjects/factory/Table.h \
               $$TOP_SRC_DIR/dbobjects/factory/Trigger.h \
               $$TOP_SRC_DIR/dbobjects/factory/View.h

    SOURCES += $$PWD/dbobjects/common/DatabaseTest.cpp \
               $$PWD/dbobjects/common/DatabaseCreator.cpp \
               $$PWD/dbobjects/common/DbIndexTest.cpp \
               $$PWD/dbobjects/common/DbLanguageTest.cpp \
               $$PWD/dbobjects/common/DbObjectTest.cpp \
               $$PWD/dbobjects/common/DbProcedureTest.cpp \
               $$PWD/dbobjects/common/DbRoleTest.cpp \
               $$PWD/dbobjects/common/DbTableTest.cpp \
               $$PWD/dbobjects/common/DbTriggerTest.cpp \
               $$PWD/dbobjects/common/DbViewTest.cpp \
               $$PWD/dbobjects/common/DbSchemaTest.cpp \
               $$PWD/dbobjects/mysql/MysqlTableTest.cpp \
               $$PWD/dbobjects/mysql/MysqlToolsTest.cpp \
               $$PWD/dbobjects/psql/PsqlIndexTest.cpp \
               $$PWD/dbobjects/psql/PsqlLanguageTest.cpp \
               $$PWD/dbobjects/psql/PsqlProcedureTest.cpp \
               $$PWD/dbobjects/psql/PsqlRoleTest.cpp \
               $$PWD/dbobjects/psql/PsqlTableTest.cpp \
               $$PWD/dbobjects/psql/PsqlToolsTest.cpp \
               $$PWD/dbobjects/psql/PsqlTriggerTest.cpp \
               $$PWD/dbobjects/psql/PsqlViewTest.cpp \
               $$PWD/dbobjects/factory/IndexFactoryTest.cpp \
               $$PWD/dbobjects/factory/LanguageFactoryTest.cpp \
               $$PWD/dbobjects/factory/ProcedureFactoryTest.cpp \
               $$PWD/dbobjects/factory/RoleFactoryTest.cpp \
               $$PWD/dbobjects/factory/TableFactoryTest.cpp \
               $$PWD/dbobjects/factory/TriggerFactoryTest.cpp \
               $$PWD/dbobjects/factory/ViewFactoryTest.cpp

}

### Expanding clean target
QMAKE_CLEAN += $$OUT_PWD/$$DESTDIR/$$TARGET


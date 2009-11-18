QT += testlib

MOC_DIR = .moc
OBJECTS_DIR = .obj

DEPENDPATH += . dbobjects ../src ../src/dbobjects
INCLUDEPATH += . dbobjects ../src ../src/dbobjects

HEADERS += AppearancePageTest.h \
	   ArrowItemTest.h  \
	   ColorsPageTest.h \
	   ControlWidgetTest.h \
	   DbParametersTest.h \
	   DescriptionWidgetTest.h \
	   GraphicsItemTest.h \
	   GraphicsSceneTest.h \
	   GraphicsViewTest.h \
	   LegendTest.h \
	   MainWindowTest.h \
	   OptionsDialogTest.h \
	   PreferencesPageTest.h \
	   ProxyParametersTest.h \
	   SceneWidgetTest.h \
	   SelectColorWidgetTest.h \
	   SqlConnectionDialogTest.h \
	   SqlWidgetTest.h \
	   TableItemGroupTest.h \
	   TableItemTest.h \
	   TabWidgetTest.h \
	   TreeWidgetTest.h \
	   \
	   dbobjects/DatabaseTest.h \
	   dbobjects/DbIndexTest.h \
	   dbobjects/DbLanguageTest.h \
	   dbobjects/DbObjectTest.h \
	   dbobjects/DbProcedureTest.h \
	   dbobjects/DbRoleTest.h \
	   dbobjects/DbSchemaTest.h \
	   dbobjects/DbTableTest.h \
	   dbobjects/DbTriggerTest.h \
	   dbobjects/DbViewTest.h \
	   dbobjects/MysqlTableTest.h \
	   dbobjects/PsqlIndexTest.h \
	   dbobjects/PsqlLanguageTest.h \
	   dbobjects/PsqlProcedureTest.h \
	   dbobjects/PsqlRoleTest.h \
	   dbobjects/PsqlTableTest.h \
	   dbobjects/PsqlTriggerTest.h \
	   dbobjects/PsqlViewTest.h


SOURCES += main.cpp \
	   AppearancePageTest.cpp \
	   ArrowItemTest.cpp \
	   ColorsPageTest.cpp \
	   ControlWidgetTest.cpp \
	   DbParametersTest.cpp \
	   DescriptionWidgetTest.cpp \
	   GraphicsItemTest.cpp \
	   GraphicsSceneTest.cpp \
	   GraphicsViewTest.cpp \
	   LegendTest.cpp \
	   OptionsDialogTest.cpp \
	   MainWindowTest.cpp \
	   PreferencesPageTest.cpp \
	   ProxyParametersTest.cpp \
	   SceneWidgetTest.cpp \
	   SelectColorWidgetTest.cpp \
	   SqlConnectionDialogTest.cpp \
	   SqlWidgetTest.cpp \
	   TableItemGroupTest.cpp \
	   TableItemTest.cpp \
	   TabWidgetTest.cpp \
	   TreeWidgetTest.cpp \
	   \
	   dbobjects/DatabaseTest.cpp \
	   dbobjects/DbIndexTest.cpp \
	   dbobjects/DbLanguageTest.cpp \
	   dbobjects/DbObjectTest.cpp \
	   dbobjects/DbProcedureTest.cpp \
	   dbobjects/DbRoleTest.cpp \
	   dbobjects/DbSchemaTest.cpp \
	   dbobjects/DbTableTest.cpp \
	   dbobjects/DbTriggerTest.cpp \
	   dbobjects/DbViewTest.cpp \
	   dbobjects/MysqlTableTest.cpp \
	   dbobjects/PsqlIndexTest.cpp \
	   dbobjects/PsqlLanguageTest.cpp \
	   dbobjects/PsqlProcedureTest.cpp \
	   dbobjects/PsqlRoleTest.cpp \
	   dbobjects/PsqlTableTest.cpp \
	   dbobjects/PsqlTriggerTest.cpp \
	   dbobjects/PsqlViewTest.cpp

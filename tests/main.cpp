#include <QSqlDatabase>
#include <QSqlError>

/* connect */
#include <connect/DbParametersTest.h>
#include <connect/ProxyParametersTest.h>

/* gui/behaviour */
#include <gui/behaviour/AddTableCommandTest.h>
#include <gui/behaviour/MoveTableCommandTest.h>

/* gui */
#include <gui/AppearancePageTest.h>
#include <gui/ArrowItemTest.h>
#include <gui/ColorsPageTest.h>
#include <gui/ControlWidgetTest.h>
#include <gui/DescriptionWidgetTest.h>
#include <gui/GraphicsItemTest.h>
#include <gui/GraphicsSceneTest.h>
#include <gui/GraphicsViewTest.h>
#include <gui/LegendTest.h>
#include <gui/LogPanelTest.h>
#include <gui/MainWindowTest.h>
#include <gui/OptionsDialogTest.h>
#include <gui/PreferencesPageTest.h>
#include <gui/SceneWidgetTest.h>
#include <gui/SelectColorWidgetTest.h>
#include <gui/SqlConnectionDialogTest.h>
#include <gui/SqlWidgetTest.h>
#include <gui/TableItemGroupTest.h>
#include <gui/TableItemTest.h>
#include <gui/TabWidgetTest.h>
#include <gui/TreeWidgetTest.h>

/* dbobjects */
#include <dbobjects/common/DatabaseTest.h>
#include <dbobjects/common/DbIndexTest.h>
#include <dbobjects/common/DbLanguageTest.h>
#include <dbobjects/common/DbObjectTest.h>
#include <dbobjects/common/DbProcedureTest.h>
#include <dbobjects/common/DbRoleTest.h>
#include <dbobjects/common/DbSchemaTest.h>
#include <dbobjects/common/DbTableTest.h>
#include <dbobjects/common/DbTriggerTest.h>
#include <dbobjects/common/DbViewTest.h>
#include <dbobjects/mysql/MysqlTableTest.h>
#include <dbobjects/psql/PsqlIndexTest.h>
#include <dbobjects/psql/PsqlLanguageTest.h>
#include <dbobjects/psql/PsqlProcedureTest.h>
#include <dbobjects/psql/PsqlRoleTest.h>
#include <dbobjects/psql/PsqlTableTest.h>
#include <dbobjects/psql/PsqlTriggerTest.h>
#include <dbobjects/psql/PsqlViewTest.h>

#define DBHOST "localhost"
#define DBNAME "music_db"
#define DBUSER "music_user"

/*
 *
 */
int main(int argc, char **argv)
{

    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL", "mainConnect");
    db.setHostName(DBHOST);
    db.setDatabaseName(DBNAME);
    db.setUserName(DBUSER);

    if (!db.open()) {
	qDebug() << QString("Unable to establish connection with '%1@%2' on behalf of '%3'")
	    .arg(DBNAME)
	    .arg(DBHOST)
	    .arg(DBUSER);

	return QSqlError::ConnectionError;
    }

#if TEST_BEHAVIOUR
    /* gui/behaviour */
    AddTableCommandTest addTableCommandTest;
    QTest::qExec(&addTableCommandTest);

    MoveTableCommandTest moveTableCommandTest;
    QTest::qExec(&moveTableCommandTest);
#endif // TEST_BEHAVIOUR

#if TEST_CONNECT
    /* connect */
    DbParametersTest dbParametersTest ;
    QTest::qExec(&dbParametersTest);
    
    ProxyParametersTest proxyParametersTest;
    QTest::qExec(&proxyParametersTest);
#endif // TEST_CONNECT

#if TEST_GUI
    /* gui */
    AppearancePageTest appearancePageTest;
    QTest::qExec(&appearancePageTest);

    ArrowItemTest arrowItemTest;
    QTest::qExec(&arrowItemTest);

    ColorsPageTest colorsPageTest;
    QTest::qExec(&colorsPageTest);

    ControlWidgetTest controlWidgetTest;
    QTest::qExec(&controlWidgetTest);
    
    DescriptionWidgetTest descriptionWidgetTest;
    QTest::qExec(&descriptionWidgetTest);
    
    GraphicsItemTest graphicsItemTest;
    QTest::qExec(&graphicsItemTest);
    
    GraphicsSceneTest graphicsSceneTest;
    QTest::qExec(&graphicsSceneTest);
    
    GraphicsViewTest graphicsViewTest;
    QTest::qExec(&graphicsViewTest);
    
    LegendTest legendTest;
    QTest::qExec(&legendTest);

    LogPanelTest logPanelTest;
    QTest::qExec(&logPanelTest);

    MainWindowTest mainWindowTest;
    QTest::qExec(&mainWindowTest);
    
    OptionsDialogTest optionsDialogTest;
    QTest::qExec(&optionsDialogTest);
    
    PreferencesPageTest preferencesPageTest;
    QTest::qExec(&preferencesPageTest);
    
    SceneWidgetTest sceneWidgetTest;
    QTest::qExec(&sceneWidgetTest);
    
    SelectColorWidgetTest selectColorWidgetTest;
    QTest::qExec(&selectColorWidgetTest);
    
    SqlConnectionDialogTest sqlConnectionDialogTest;
    QTest::qExec(&sqlConnectionDialogTest);
    
    SqlWidgetTest sqlWidgetTest;
    QTest::qExec(&sqlWidgetTest);
    
    TableItemGroupTest tableItemGroupTest;
    QTest::qExec(&tableItemGroupTest);
    
    TableItemTest tableItemTest;
    QTest::qExec(&tableItemTest);
    
    TabWidgetTest tabWidgetTest;
    QTest::qExec(&tabWidgetTest);
    
    TreeWidgetTest treeWidgetTest;
    QTest::qExec(&treeWidgetTest);
#endif // TEST_GUI

#if TEST_DBOBJECTS
    /* dbobjects */
    DatabaseTest databaseTest;
    QTest::qExec(&databaseTest);

    DbIndexTest dbIndexTest;
    QTest::qExec(&dbIndexTest);
    
    DbLanguageTest dbLanguageTest;
    QTest::qExec(&dbLanguageTest);
    
    DbObjectTest dbObjectTest;
    QTest::qExec(&dbObjectTest);
    
    DbProcedureTest dbProcedureTest;
    QTest::qExec(&dbProcedureTest);
    
    DbRoleTest dbRoleTest;
    QTest::qExec(&dbRoleTest);
    
    DbSchemaTest dbSchemaTest;
    QTest::qExec(&dbSchemaTest);
    
    DbTableTest dbTableTest;
    QTest::qExec(&dbTableTest);
    
    DbTriggerTest dbTriggerTest;
    QTest::qExec(&dbTriggerTest);
    
    DbViewTest dbViewTest;
    QTest::qExec(&dbViewTest);
    
    MysqlTableTest mysqlTableTest;
    QTest::qExec(&mysqlTableTest);
    
    PsqlIndexTest psqlIndexTest;
    QTest::qExec(&psqlIndexTest);
    
    PsqlLanguageTest psqlLanguageTest;
    QTest::qExec(&psqlLanguageTest);
    
    PsqlProcedureTest psqlProcedureTest;
    QTest::qExec(&psqlProcedureTest);
    
    PsqlRoleTest psqlRoleTest;
    QTest::qExec(&psqlRoleTest);
    
    PsqlTableTest psqlTableTest;
    QTest::qExec(&psqlTableTest);
    
    PsqlTriggerTest psqlTriggerTest;
    QTest::qExec(&psqlTriggerTest);
    
    PsqlViewTest psqlViewTest;
    QTest::qExec(&psqlViewTest);
#endif // TEST_DBOBJECTS

    return 0;
}

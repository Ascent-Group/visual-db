//#include <QSqlDatabase>
//#include <QSqlError>

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

#include <connect/DbParameters.h>
#include <common/DatabaseCreator.h>

const QString DBHOST = "localhost";
const QString DBNAME = "music_db";
const QString DBUSER = "music_user";

/*!
 *
 */
int main(int argc, char *argv[])
{
    /*!
     * We cannot pass the db driver to use for testing via argv because QTestLib needs/uses
     * argv (e.g. a name of a test to run) and they will be parsed by QApplication. Adding
     * our custom input argument may influence/harm workflow. So, looks like we will have
     * find another way of specifying db driver. As a solution, we can use environment
     * variable for that.
     */
    const char *drv = getenv("VDB_DB_DRV");

    if (!drv) {
        qCritical("[ERROR] db driver not set!");
        return -1;
    }

    DbParameters dbParams;

    dbParams.setDbHost(DBHOST);
    dbParams.setDbName(DBNAME);
    dbParams.setDbUser(DBUSER);
//    dbParams.setDbPassword();

    if (0 == strncmp(drv, "psql", strlen(drv))) {
        dbParams.setDbDriver("QPSQL");
        DatabaseCreator::setDriver(Database::PostgreSQL);
    } else if (0 == strncmp(drv, "mysql", strlen(drv))) {
        dbParams.setDbDriver("QMYSQL");
        DatabaseCreator::setDriver(Database::MySQL);
    } else if (0 == strncmp(drv, "oracle", strlen(drv))) {
        dbParams.setDbDriver("QODBC");
        DatabaseCreator::setDriver(Database::Oracle);
    } else if (0 == strncmp(drv, "sqlite", strlen(drv))) {
        dbParams.setDbDriver("QSQLITE");
        DatabaseCreator::setDriver(Database::SQLite);
    } else {
        qCritical("[ERROR] Unknown db driver!");

        return -1;
    }

    if (!createConnection(dbParams)) {
        qCritical() << QString("Unable to establish connection with '%1@%2' on behalf of '%3'")
                .arg(DBNAME)
                .arg(DBHOST)
                .arg(DBUSER);

        return -1;
    }

    QApplication app(argc, argv);

//    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL", "mainConnect");
//    db.setHostName(DBHOST);
//    db.setDatabaseName(DBNAME);
//    db.setUserName(DBUSER);
//
//    if (!db.open()) {
//        qDebug() << QString("Unable to establish connection with '%1@%2' on behalf of '%3'")
//            .arg(DBNAME)
//            .arg(DBHOST)
//            .arg(DBUSER);
//
//        return QSqlError::ConnectionError;
//    }

#if TEST_BEHAVIOUR
    /* gui/behaviour */
    AddTableCommandTest addTableCommandTest;
    QTest::qExec(&addTableCommandTest, argc, argv);

    MoveTableCommandTest moveTableCommandTest;
    QTest::qExec(&moveTableCommandTest, argc, argv);
#endif // TEST_BEHAVIOUR

#if TEST_CONNECT
    /* connect */
    DbParametersTest dbParametersTest ;
    QTest::qExec(&dbParametersTest, argc, argv);
    
    ProxyParametersTest proxyParametersTest;
    QTest::qExec(&proxyParametersTest, argc, argv);
#endif // TEST_CONNECT

#if TEST_GUI
    /* gui */
    AppearancePageTest appearancePageTest;
    QTest::qExec(&appearancePageTest, argc, argv);

    ArrowItemTest arrowItemTest;
    QTest::qExec(&arrowItemTest, argc, argv);

    ColorsPageTest colorsPageTest;
    QTest::qExec(&colorsPageTest, argc, argv);

    ControlWidgetTest controlWidgetTest;
    QTest::qExec(&controlWidgetTest, argc, argv);
    
    DescriptionWidgetTest descriptionWidgetTest;
    QTest::qExec(&descriptionWidgetTest, argc, argv);
    
    GraphicsItemTest graphicsItemTest;
    QTest::qExec(&graphicsItemTest, argc, argv);
    
    GraphicsSceneTest graphicsSceneTest;
    QTest::qExec(&graphicsSceneTest, argc, argv);
    
    GraphicsViewTest graphicsViewTest;
    QTest::qExec(&graphicsViewTest, argc, argv);
    
    LegendTest legendTest;
    QTest::qExec(&legendTest, argc, argv);

    LogPanelTest logPanelTest;
    QTest::qExec(&logPanelTest, argc, argv);

    MainWindowTest mainWindowTest;
    QTest::qExec(&mainWindowTest, argc, argv);
    
    OptionsDialogTest optionsDialogTest;
    QTest::qExec(&optionsDialogTest, argc, argv);
    
    PreferencesPageTest preferencesPageTest;
    QTest::qExec(&preferencesPageTest, argc, argv);
    
    SceneWidgetTest sceneWidgetTest;
    QTest::qExec(&sceneWidgetTest, argc, argv);
    
    SelectColorWidgetTest selectColorWidgetTest;
    QTest::qExec(&selectColorWidgetTest, argc, argv);
    
    SqlConnectionDialogTest sqlConnectionDialogTest;
    QTest::qExec(&sqlConnectionDialogTest, argc, argv);
    
    SqlWidgetTest sqlWidgetTest;
    QTest::qExec(&sqlWidgetTest, argc, argv);
    
    TableItemGroupTest tableItemGroupTest;
    QTest::qExec(&tableItemGroupTest, argc, argv);
    
    TableItemTest tableItemTest;
    QTest::qExec(&tableItemTest, argc, argv);
    
    TabWidgetTest tabWidgetTest;
    QTest::qExec(&tabWidgetTest, argc, argv);
    
    TreeWidgetTest treeWidgetTest;
    QTest::qExec(&treeWidgetTest, argc, argv);
#endif // TEST_GUI

#if TEST_DBOBJECTS
    /* dbobjects */
    DatabaseTest databaseTest;
    QTest::qExec(&databaseTest, argc, argv);

    DbIndexTest dbIndexTest;
    QTest::qExec(&dbIndexTest, argc, argv);
    
//    DbLanguageTest dbLanguageTest;
//    QTest::qExec(&dbLanguageTest, argc, argv);
//    
//    DbObjectTest dbObjectTest;
//    QTest::qExec(&dbObjectTest, argc, argv);
//    
//    DbProcedureTest dbProcedureTest;
//    QTest::qExec(&dbProcedureTest, argc, argv);
//    
//    DbRoleTest dbRoleTest;
//    QTest::qExec(&dbRoleTest, argc, argv);
//    
//    DbSchemaTest dbSchemaTest;
//    QTest::qExec(&dbSchemaTest, argc, argv);
//    
//    DbTableTest dbTableTest;
//    QTest::qExec(&dbTableTest, argc, argv);
//    
//    DbTriggerTest dbTriggerTest;
//    QTest::qExec(&dbTriggerTest, argc, argv);
//    
//    DbViewTest dbViewTest;
//    QTest::qExec(&dbViewTest, argc, argv);
//    
//    MysqlTableTest mysqlTableTest;
//    QTest::qExec(&mysqlTableTest, argc, argv);
//    
//    PsqlIndexTest psqlIndexTest;
//    QTest::qExec(&psqlIndexTest, argc, argv);
//    
//    PsqlLanguageTest psqlLanguageTest;
//    QTest::qExec(&psqlLanguageTest, argc, argv);
//    
//    PsqlProcedureTest psqlProcedureTest;
//    QTest::qExec(&psqlProcedureTest, argc, argv);
//    
//    PsqlRoleTest psqlRoleTest;
//    QTest::qExec(&psqlRoleTest, argc, argv);
//    
//    PsqlTableTest psqlTableTest;
//    QTest::qExec(&psqlTableTest, argc, argv);
//    
//    PsqlTriggerTest psqlTriggerTest;
//    QTest::qExec(&psqlTriggerTest, argc, argv);
//    
//    PsqlViewTest psqlViewTest;
//    QTest::qExec(&psqlViewTest, argc, argv);
#endif // TEST_DBOBJECTS

    return 0;
}


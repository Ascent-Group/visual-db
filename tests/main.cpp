/* connect */
#ifdef TEST_CONNECT
#include <connect/HostInfoTest.h>
#include <connect/DbHostInfoTest.h>
#include <connect/ProxyHostInfoTest.h>
#include <connect/ConnectionInfoTest.h>
#endif

#ifdef TEST_CONTROL
/* control */
#include <control/ConfigTest.h>
#include <control/ContextTest.h>
#include <control/DirectorTest.h>
#include <control/SessionTest.h>
#endif

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

#if TEST_DBOBJECTS
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
#include <dbobjects/mysql/MysqlFactoriesTest.h>
#include <dbobjects/mysql/MysqlToolsTest.h>
#include <dbobjects/psql/PsqlFactoriesTest.h>
#include <dbobjects/psql/PsqlToolsTest.h>

#include <common/DatabaseCreator.h>

#endif

const QString DBHOST = "localhost";
const QString DBNAME = "music_db";
const QString DBUSER = "music_user";
const QString DBPASS = "qwe";

/*!
 *
 */
int main(int argc, char *argv[])
{
#if TEST_DBOBJECTS
    /*!
     * We cannot pass the db driver to use for testing via argv because QTestLib needs/uses
     * argv (e.g. a name of a test to run) and they will be parsed by QApplication. Adding
     * our custom input argument may influence/harm workflow. So, looks like we will have
     * find another way of specifying db driver. As a solution, we can use environment
     * variable for that.
     */
    QString drv(getenv("VDB_DB_DRV"));

    if (drv.isEmpty()) {
        qCritical("[ERROR] db driver not set!");
        return -1;
    }

//    DatabaseCreator::setDriver(drv);

    Connect::DbHostInfo dbHostInfo;
    dbHostInfo.setDbDriver(drv);
    dbHostInfo.setAddress(DBHOST);
    dbHostInfo.setDbName(DBNAME);
    dbHostInfo.setUser(DBUSER);
    dbHostInfo.setPassword(DBPASS);


    if (!DatabaseCreator::connect(dbHostInfo)) {
        qDebug() << QString("Unable to establish connection with '%1@%2' on behalf of '%3'")
            .arg(DBNAME)
            .arg(DBHOST)
            .arg(DBUSER);

        return -1;
    }
#endif

    QApplication app(argc, argv);

    // \todo Determine whether this should be set or not for tests
//    app.setOrganizationName("Ascent Group");
//    app.setOrganizationDomain("sourceforge.net");
//    app.setApplicationName("VisualDB");
//    app.setWindowIcon(QIcon(":/img/logo.png"));

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

#if TEST_GUI_BEHAVIOUR
    /* gui/behaviour */
    AddTableCommandTest addTableCommandTest;
    QTest::qExec(&addTableCommandTest, argc, argv);

    MoveTableCommandTest moveTableCommandTest;
    QTest::qExec(&moveTableCommandTest, argc, argv);
#endif // TEST_BEHAVIOUR

#if TEST_CONNECT
    /* connect */
    HostInfoTest hostInfoTest;
    QTest::qExec(&hostInfoTest, argc, argv);

    DbHostInfoTest dbHostInfoTest;
    QTest::qExec(&dbHostInfoTest, argc, argv);

    ProxyHostInfoTest proxyHostInfoTest;
    QTest::qExec(&proxyHostInfoTest, argc, argv);

    ConnectionInfoTest connectionInfoTest;
    QTest::qExec(&connectionInfoTest, argc, argv);
#endif // TEST_CONNECT

#if TEST_CONTROL
    /* control*/
    ConfigTest configTest;
    QTest::qExec(&configTest, argc, argv);

    ContextTest contextTest;
    QTest::qExec(&contextTest, argc, argv);

    DirectorTest directorTest;
    QTest::qExec(&directorTest, argc, argv);

    SessionTest sessionTest;
    QTest::qExec(&sessionTest, argc, argv);
#endif

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

    DbLanguageTest dbLanguageTest;
    QTest::qExec(&dbLanguageTest, argc, argv);

    DbObjectTest dbObjectTest;
    QTest::qExec(&dbObjectTest, argc, argv);

    DbProcedureTest dbProcedureTest;
    QTest::qExec(&dbProcedureTest, argc, argv);

    DbRoleTest dbRoleTest;
    QTest::qExec(&dbRoleTest, argc, argv);

    DbSchemaTest dbSchemaTest;
    QTest::qExec(&dbSchemaTest, argc, argv);

    DbTableTest dbTableTest;
    QTest::qExec(&dbTableTest, argc, argv);

    DbTriggerTest dbTriggerTest;
    QTest::qExec(&dbTriggerTest, argc, argv);

    DbViewTest dbViewTest;
    QTest::qExec(&dbViewTest, argc, argv);

    if (drv.contains("MYSQL")) {
        MysqlFactoriesTest mysqlFactoriesTest;
        QTest::qExec(&mysqlFactoriesTest, argc, argv);

        MysqlToolsTest mysqlToolsTest;
        QTest::qExec(&mysqlToolsTest, argc, argv);
    } else if (drv.contains("PSQL")) {
        PsqlFactoriesTest psqlFactoriesTest;
        QTest::qExec(&psqlFactoriesTest, argc, argv);

        PsqlToolsTest psqlToolsTest;
        QTest::qExec(&psqlToolsTest, argc, argv);
    }
#endif // TEST_DBOBJECTS

    return 0;
}


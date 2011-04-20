#include <QMultiMap>

// connect
#include <connect/HostInfoTest.h>
#include <connect/DbHostInfoTest.h>
#include <connect/ProxyHostInfoTest.h>
#include <connect/ConnectionInfoTest.h>

// control
#include <control/ConfigTest.h>
#include <control/ContextTest.h>
#include <control/DatabaseManagerTest.h>
#include <control/DirectorTest.h>
#include <control/SessionTest.h>

// gui/strategy
#include <gui/strategy/NodeTest.h>
#include <gui/strategy/EdgeTest.h>
#include <gui/strategy/GraphTest.h>

// gui/behaviour
#include <gui/behaviour/AddTableCommandTest.h>
#include <gui/behaviour/MoveTableCommandTest.h>

// gui
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

// dbobjects
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


const QString GUI_BEAHVIOUR = "behaviour";
const QString CONNECT = "connect";
const QString CONTROL = "control";
const QString DBOBJECTS_COMMON = "common";
const QString DBOBJECTS_PSQL = "psql";
const QString DBOBJECTS_MYSQL = "mysql";
const QString GUI = "gui";
const QString GUI_STRATEGY = "strategy";

typedef QPair<QString, QObject*> RegistryRecord;
typedef QMultiMap<QString, RegistryRecord> Registry;

#define REGISTER_TEST_SUITE(Module, Suite) \
    do { \
        QObject *o = new Suite##Test(); \
        testSuites.insert(Module, qMakePair(QString(#Suite), o)); \
    } while (0);

/*!
 *
 */
int main(int argc, char *argv[])
{
    Registry testSuites;

    // connect
    REGISTER_TEST_SUITE(CONNECT, ConnectionInfo);
    REGISTER_TEST_SUITE(CONNECT, DbHostInfo);
    REGISTER_TEST_SUITE(CONNECT, HostInfo);
    REGISTER_TEST_SUITE(CONNECT, ProxyHostInfo);

    // control
    REGISTER_TEST_SUITE(CONTROL, Config);
    REGISTER_TEST_SUITE(CONTROL, Context);
//    REGISTER_TEST_SUITE(CONTROL, Converter);
    REGISTER_TEST_SUITE(CONTROL, DatabaseManager);
    REGISTER_TEST_SUITE(CONTROL, Director);
    REGISTER_TEST_SUITE(CONTROL, Session);

    // dbobjects
    // common
    REGISTER_TEST_SUITE(DBOBJECTS_COMMON, Database);
    REGISTER_TEST_SUITE(DBOBJECTS_COMMON, DbIndex);
    REGISTER_TEST_SUITE(DBOBJECTS_COMMON, DbLanguage);
    REGISTER_TEST_SUITE(DBOBJECTS_COMMON, DbObject);
    REGISTER_TEST_SUITE(DBOBJECTS_COMMON, DbProcedure);
    REGISTER_TEST_SUITE(DBOBJECTS_COMMON, DbRole);
    REGISTER_TEST_SUITE(DBOBJECTS_COMMON, DbSchema);
    REGISTER_TEST_SUITE(DBOBJECTS_COMMON, DbTable);
    REGISTER_TEST_SUITE(DBOBJECTS_COMMON, DbTrigger);
    REGISTER_TEST_SUITE(DBOBJECTS_COMMON, DbView);

    // psql
    REGISTER_TEST_SUITE(DBOBJECTS_PSQL, PsqlFactories);
    REGISTER_TEST_SUITE(DBOBJECTS_PSQL, PsqlTools);

    // mysql
    REGISTER_TEST_SUITE(DBOBJECTS_MYSQL, MysqlFactories);
    REGISTER_TEST_SUITE(DBOBJECTS_MYSQL, MysqlTools);

    // gui
    REGISTER_TEST_SUITE(GUI, AppearancePage);
    REGISTER_TEST_SUITE(GUI, ArrowItem);
    REGISTER_TEST_SUITE(GUI, ColorsPage);
    REGISTER_TEST_SUITE(GUI, ControlWidget);
    REGISTER_TEST_SUITE(GUI, DescriptionWidget);
    REGISTER_TEST_SUITE(GUI, GraphicsItem);
    REGISTER_TEST_SUITE(GUI, GraphicsScene);
    REGISTER_TEST_SUITE(GUI, GraphicsView);
    REGISTER_TEST_SUITE(GUI, Legend);
    REGISTER_TEST_SUITE(GUI, LogPanel);
    REGISTER_TEST_SUITE(GUI, MainWindow);
    REGISTER_TEST_SUITE(GUI, OptionsDialog);
    REGISTER_TEST_SUITE(GUI, PreferencesPage);
    REGISTER_TEST_SUITE(GUI, SceneWidget);
    REGISTER_TEST_SUITE(GUI, SelectColorWidget);
    REGISTER_TEST_SUITE(GUI, SqlConnectionDialog);
    REGISTER_TEST_SUITE(GUI, SqlWidget);
    REGISTER_TEST_SUITE(GUI, TableItemGroup);
    REGISTER_TEST_SUITE(GUI, TableItem);
    REGISTER_TEST_SUITE(GUI, TabWidget);
    REGISTER_TEST_SUITE(GUI, TreeWidget);

    // behaviour
    REGISTER_TEST_SUITE(GUI_BEAHVIOUR, AddTableCommand);
    REGISTER_TEST_SUITE(GUI_BEAHVIOUR, MoveTableCommand);

    // strategy
    REGISTER_TEST_SUITE(GUI_STRATEGY, Edge);
    REGISTER_TEST_SUITE(GUI_STRATEGY, Graph);
    REGISTER_TEST_SUITE(GUI_STRATEGY, Node);

    QSettings settings("./tests.ini", QSettings::IniFormat);
    /*!
     * We cannot pass the db driver to use for testing via argv because QTestLib needs/uses
     * argv (e.g. a name of a test to run) and they will be parsed by QApplication. Adding
     * our custom input argument may influence/harm workflow. So, looks like we will have
     * find another way of specifying db driver. As a solution, we can use environment
     * variable for that.
     */
    QString drv = settings.value("driver", "").toString();
    QString dbHost = settings.value("dbhost", "").toString();
    QString dbName = settings.value("dbname", "").toString();
    QString dbUser = settings.value("dbuser", "").toString();
    QString dbPass = settings.value("dbpass", "").toString();
    

    if (drv.isEmpty()) {
        qCritical("[ERROR] Database driver is not set! In order to run tests you should set the VDB_DB_DRV variable. Available values are: [QPSQL, QMYSQL]");
        return -1;
    }

//    DatabaseCreator::setDriver(drv);

    Connect::DbHostInfo dbHostInfo;
    dbHostInfo.setDbDriver(drv);
    dbHostInfo.setAddress(dbHost);
    dbHostInfo.setDbName(dbName);
    dbHostInfo.setUser(dbUser);
    dbHostInfo.setPassword(dbPass);

    if (!DatabaseCreator::connect(dbHostInfo)) {
        qDebug() << QString("Unable to establish connection with '%1@%2' on behalf of '%3'")
            .arg(dbName)
            .arg(dbHost)
            .arg(dbUser);

        return -1/*QSqlError::ConnectionError*/;
    }

    QApplication app(argc, argv);

    QList<QObject *> tests;
    // if no args were specified, then all tests should be executed
    if (2 > argc) {
        foreach (const QString &key, testSuites.uniqueKeys()) {
            foreach (const RegistryRecord &pair, testSuites.values(key)) {
                tests << pair.second;
            }
        }
    // otherwise, search for those that should be executed
    } else {
        for (int i = 1; i < argc; ++i) {
            bool singleTestFound = false;
            QString testName(argv[i]);

            // try to find such test suite
            foreach (const QString &key, testSuites.uniqueKeys()) {
                foreach (const RegistryRecord &pair, testSuites.values(key)) {
                    if (testName == pair.first) {
                        tests << pair.second;
                        singleTestFound = true;
                    }
                }
            }

            // if not found then treat this name as module name
            if (!singleTestFound) {
                foreach (const RegistryRecord &pair, testSuites.values(testName)) {
                    tests << pair.second;
                }
            }
        }
    }

    foreach (QObject *test, tests) {
        QTest::qExec(test/*, argc, argv*/);
    }

    // cleanup
    foreach (const QString &key, testSuites.uniqueKeys()) {
        foreach (const RegistryRecord &pair, testSuites.values(key)) {
            delete pair.second;
        }
    }

    return 0;
}


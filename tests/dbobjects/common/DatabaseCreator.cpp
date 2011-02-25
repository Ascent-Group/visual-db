#include <common/DatabaseCreator.h>
#include <mysql/Factories.h>
#include <mysql/Tools.h>
#include <psql/Factories.h>
#include <psql/Tools.h>

#include <QFile>

QString DatabaseCreator::mDriverName("");
QSqlDatabase DatabaseCreator::mDbHandle = QSqlDatabase();

bool
DatabaseCreator::connect(const Connect::DbHostInfo &iDbHostInfo)
{
    setDriver(iDbHostInfo.dbDriver());

    bool success = false;
    mDbHandle = QSqlDatabase::addDatabase(iDbHostInfo.dbDriver(), "mainConnect");
    mDbHandle.setDatabaseName(iDbHostInfo.dbName());

    // if SQLite db
    if (QString("QSQLITE") == iDbHostInfo.dbDriver()) {
        // check only if fie exists
        if ((success = QFile::exists(iDbHostInfo.dbName()))) {
            mDbHandle.open();
        }
    // of other DBMS
    } else {
        // check credentials and host
        mDbHandle.setHostName(iDbHostInfo.address());
        mDbHandle.setUserName(iDbHostInfo.user());
        mDbHandle.setPassword(iDbHostInfo.password());
        mDbHandle.setPort(iDbHostInfo.port());

        if ((success = mDbHandle.open())) {
            // initialize Database for further use and get schemas
//            using namespace DbObjects::Common;
//            Database *dbInst = Database::instance();
//            dbInst->setSqlDriver(iDbHostInfo.dbDriver());
        }
    }

    return success;
}

DbObjects::Common::Database*
DatabaseCreator::createDatabase()
{
    using namespace DbObjects::Common;
    Database *dbInst = new(std::nothrow) Database(DatabaseCreator::mDbHandle);

    dbInst->setSqlDriver(mDriverName);

    return dbInst;
}

void
DatabaseCreator::setDriver(const QString &ipDriverName)
{
    mDriverName = ipDriverName;
}

DbObjects::Common::Factories*
DatabaseCreator::factories()
{
    if (mDriverName == "QPSQL") {
        return new(std::nothrow) DbObjects::Psql::Factories();
    } else if (mDriverName == "QMYSQL") {
        return new(std::nothrow) DbObjects::Mysql::Factories();
    }
    return 0;
}

DbObjects::Common::Tools*
DatabaseCreator::tools()
{
    if (mDriverName == "QPSQL") {
        return new(std::nothrow) DbObjects::Psql::Tools();
    } else if (mDriverName == "QMYSQL") {
        return new(std::nothrow) DbObjects::Mysql::Tools();
    }
    return 0;
}


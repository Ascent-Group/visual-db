#include <common/DatabaseCreator.h>

QString DatabaseCreator::mDriverName("");

using namespace DbObjects::Common;

Database*
DatabaseCreator::createDatabase()
{
    Database *dbInst = Database::instance();

    dbInst->setSqlDriver(mDriverName);

    return dbInst;
}

void
DatabaseCreator::setDriver(const QString &ipDriverName)
{
    mDriverName = ipDriverName;
}


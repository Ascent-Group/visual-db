#ifndef DATABASE_CREATOR
#define DATABASE_CREATOR

#include <common/Database.h>
#include <connect/DbHostInfo.h>
#include <QSqlDatabase>

class DatabaseCreator
{
    public:
        static bool connect(const Connect::DbHostInfo &iDbHostInfo);
        static DbObjects::Common::Database* createDatabase();

        static void setDriver(const QString &ipDriverName);

        static DbObjects::Common::Factories* factories();
        static DbObjects::Common::Tools* tools();

    private:
        static QString mDriverName;
        static QSqlDatabase mDbHandle;
};

#endif // DATABASE_CREATOR


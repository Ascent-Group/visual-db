#ifndef DATABASE_CREATOR
#define DATABASE_CREATOR

#include <common/Database.h>

class DatabaseCreator
{
    public:
        static DbObjects::Common::Database* createDatabase();

        static void setDriver(const QString &ipDriverName);

    private:
        static QString mDriverName;
};

#endif // DATABASE_CREATOR


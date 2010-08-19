#ifndef DATABASE_CREATOR
#define DATABASE_CREATOR

#include <common/Database.h>

class DatabaseCreator
{
    public:
        static Database* createDatabase()
        {
            Database *dbInst = Database::instance();

            dbInst->setSqlDriver(mDriver);            

            return dbInst;
        }
        
        static void setDriver(const QString &ipDriver)
        {
            mDriver = ipDriver;
        }

    private:
        static QString mDriver;
};

QString DatabaseCreator::mDriver = QString("");

#endif // DATABASE_CREATOR


/*-
 * Copyright (c) 2009, Ascent Group.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright notice,
 *       this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright notice,
 *       this list of conditions and the following disclaimer in the documentation
 *       and/or other materials provided with the distribution.
 *     * Neither the name of the Ascent Group nor the names of its contributors
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 *
 *     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <control/DatabaseManager.h>
#include <dbobjects/common/Database.h>
#include <dbobjects/common/DatabaseCreator.h>
#include <dbobjects/common/DatabaseTest.h>
#include <dbobjects/common/DbSchema.h>
#include <dbobjects/common/Factories.h>
#include <dbobjects/common/Tools.h>
#include <dbobjects/psql/Index.h>
#include <dbobjects/psql/Language.h>
#include <dbobjects/psql/Role.h>

#include <QSqlDatabase>

using namespace DbObjects;

typedef Common::DbObjectPtr<Common::DbIndex> DbIndexPtr;
typedef Common::DbObjectPtr<Common::DbLanguage> DbLanguagePtr;
typedef Common::DbObjectPtr<Common::DbRole> DbRolePtr;
typedef Common::DbObjectPtr<Common::DbSchema> DbSchemaPtr;
typedef Common::DbObjectPtr<Common::DbTable> DbTablePtr;

// These are the counts fr non-system db objects
const quint8  DatabaseTest::LANGUAGES_COUNT = 4;
const quint64 DatabaseTest::SCHEMAS_COUNT = 6;
const quint64 DatabaseTest::ROLES_COUNT = 2;
const quint64 DatabaseTest::INDICES_COUNT = 5;

#define QDEBUG_PRINT_ALL(type) \
    { \
        QStringList type; \
        mDbInst->type##List(&type); \
         \
        foreach(const QString &name, type) { \
            qDebug() << name; \
        } \
    }


void
DatabaseTest::initTestCase()
{
    mDbInst = 0;
    mFactories = 0;
    mTools = 0;
}

void
DatabaseTest::cleanupTestCase()
{
    delete mDbInst;
    delete mFactories;
    delete mTools;
}

/*!
 * Called before each test function
 */
void
DatabaseTest::init()
{
    mDbInst = DatabaseCreator::createDatabase();
    QVERIFY(0 != mDbInst);

    mFactories = DatabaseCreator::factories();
    QVERIFY(0 != mFactories);

    mTools = DatabaseCreator::tools();
    QVERIFY(0 != mTools);

//    mDbInst->setSqlDriver("QPSQL");
}

/*!
 * Called after each test function
 */
void
DatabaseTest::cleanup()
{
    mDbInst->resetData();

//    Common::DatabaseManager dbMgr;
//    dbMgr.flush();
}

void
DatabaseTest::addIndexTest()
{
    QVERIFY(0 != mDbInst);

    // read all indices
    mDbInst->readIndices(mFactories, mTools);

    quint64 count = mDbInst->indicesCount();

    QVERIFY(0 < count);

    // try to add dummy index
    DbIndexPtr dummyIndex = DbIndexPtr(mDbInst, mFactories, "dummy");

    QVERIFY(0 != dummyIndex.get());

    QVERIFY(mDbInst->addIndex(dummyIndex));

    QCOMPARE(mDbInst->indicesCount(), (quint64)count + 1);

    // try to add a index that already has been there
    QString indexName("ind_artists");
    DbIndexPtr index = mDbInst->findIndex(indexName);

    QVERIFY(index.valid());

    QCOMPARE(mDbInst->addIndex(index), false);
}

void
DatabaseTest::addLanguageTest()
{
    // check db instance
    QVERIFY(0 != mDbInst);

    // read languages
    mDbInst->readLanguages(mFactories, mTools);

    // get language count
    quint8 count = mDbInst->languagesCount();

    QVERIFY(0 < count);

    // add dummy language
    DbLanguagePtr dummyLang(mDbInst, mFactories, "dummy");

    QVERIFY(0 != dummyLang.get());

    QVERIFY(mDbInst->addLanguage(dummyLang));

    // check for language count to increase by 1
    QCOMPARE(mDbInst->languagesCount(), (quint8)(count + 1));

    // add existant language again
    QString langName("plpgsql");
    DbLanguagePtr lang = mDbInst->findLanguage(langName);

    QVERIFY(lang.valid());

    // check for the language count to be the same
    QCOMPARE(mDbInst->addLanguage(lang), false);
}

void
DatabaseTest::addRoleTest()
{
    // check db instance
    QVERIFY(0 != mDbInst);

    // read roles
    mDbInst->readRoles(mFactories, mTools);

    // get roles count
    quint64 count = mDbInst->rolesCount();

    QVERIFY(0 < count);

    // add dummy role
    DbRolePtr dummyRole(mDbInst, mFactories, "dummy");

    QVERIFY(0 != dummyRole.get());

    QVERIFY(mDbInst->addRole(dummyRole));

    // check for role count to increase by 1
    QCOMPARE(mDbInst->rolesCount(), (quint64)(count + 1));

    // add existant role again
    QString roleName("music_user");
    DbRolePtr role = mDbInst->findRole(roleName);

    QVERIFY(role.valid());

    // check for the role count to be the same
    QCOMPARE(mDbInst->addRole(role), false);
}

void
DatabaseTest::addSchemaTest()
{
    // check db instance
    QVERIFY(0 != mDbInst);

    mDbInst->readRoles(mFactories, mTools);
    // read schemas
    mDbInst->readSchemas(mFactories, mTools);

    // get schemas count
    quint64 count = mDbInst->schemasCount();

    QVERIFY(0 < count);

    // add dummy schema
    DbSchemaPtr dummySchema(mDbInst, mFactories, "dummy");

    QVERIFY(0 != dummySchema.get());

//    QDEBUG_PRINT_ALL(schemas);

    QVERIFY(mDbInst->addSchema(dummySchema));

    // check for schemas count to increase by 1
    QCOMPARE(mDbInst->schemasCount(), (quint64)(count + 1));

    // add existant schema again
    QString schemaName("vtunes");
    DbSchemaPtr schema = mDbInst->findSchema(schemaName);

    QVERIFY(schema.valid());

    // check for the schema count to be the same
    QCOMPARE(mDbInst->addSchema(schema), false);

}

void
DatabaseTest::resetDataTest()
{
    QVERIFY(0 != mDbInst);

    mDbInst->loadData(mFactories, mTools);
//    mDbInst->readIndices();
//    mDbInst->readLanguages();
//    mDbInst->readRoles();
//    mDbInst->readSchemas();

    QVERIFY(0 != mDbInst->indicesCount());
    QVERIFY(0 != mDbInst->languagesCount());
    QVERIFY(0 != mDbInst->rolesCount());
    QVERIFY(0 != mDbInst->schemasCount());

    mDbInst->resetData();

    QCOMPARE(mDbInst->indicesCount(), (quint64)0);
    QCOMPARE(mDbInst->languagesCount(), (quint8)0);
    QCOMPARE(mDbInst->rolesCount(), (quint64)0);
    QCOMPARE(mDbInst->schemasCount(), (quint64)0);

}

void
DatabaseTest::findIndexTest()
{
    QVERIFY(0 != mDbInst);

    mDbInst->readIndices(mFactories, mTools);

    QVERIFY(mDbInst->findIndex("ind_artists").valid());
    QVERIFY(mDbInst->findIndex("ind_albums").valid());
    QVERIFY(mDbInst->findIndex("ind_tracks").valid());
    QVERIFY(mDbInst->findIndex("ind_users").valid());
    QVERIFY(mDbInst->findIndex("ind_locations").valid());
}

void
DatabaseTest::findLanguageTest()
{
    QVERIFY(0 != mDbInst);

    mDbInst->readLanguages(mFactories, mTools);

    QVERIFY(mDbInst->findLanguage("plpgsql").valid());
}

void
DatabaseTest::findRoleTest()
{
    QVERIFY(0 != mDbInst);

    mDbInst->readRoles(mFactories, mTools);

    QVERIFY(mDbInst->findRole("music_user").valid());
}

void
DatabaseTest::findSchemaTest()
{
    QVERIFY(0 != mDbInst);

    mDbInst->readRoles(mFactories, mTools);
    mDbInst->readSchemas(mFactories, mTools);

    QVERIFY(mDbInst->findSchema("public").valid());
    QVERIFY(mDbInst->findSchema("vtunes").valid());
    QVERIFY(mDbInst->findSchema("pg_catalog").valid());
}

void
DatabaseTest::findTableIndicesTest()
{
    QVERIFY(0 != mDbInst);

    mDbInst->loadData(mFactories, mTools);

    QVector<DbIndexPtr> indicesList;

    DbSchemaPtr schema = mDbInst->findSchema("vtunes");

    QVERIFY(schema.valid());

    DbTablePtr table = schema->findTable("artists");

    QVERIFY(table.valid());

    mDbInst->findTableIndices(table, indicesList);

    QVERIFY(0 < indicesList.size());
    QVERIFY(indicesList.contains(mDbInst->findIndex("ind_artists")));
}

void
DatabaseTest::flushTest()
{
    QVERIFY(0 != mDbInst);

    mDbInst->loadData(mFactories, mTools);

    QVERIFY(0 != mDbInst->indicesCount());
    QVERIFY(0 != mDbInst->languagesCount());
    QVERIFY(0 != mDbInst->rolesCount());
    QVERIFY(0 != mDbInst->schemasCount());

    // \todo commente out due to removal of flush
//    Common::DatabaseManager dbMgr;
//    dbMgr.flush();

    // this check might not be valid if the object will be reconstructed in the same location
//    QVERIFY(mDbInst != Database::instance());

    mDbInst = Common::Database::instance();

    QVERIFY(0 != mDbInst);

    QCOMPARE(mDbInst->indicesCount(), (quint64)0);
    QCOMPARE(mDbInst->languagesCount(), (quint8)0);
    QCOMPARE(mDbInst->rolesCount(), (quint64)0);
    QCOMPARE(mDbInst->schemasCount(), (quint64)0);

}

void
DatabaseTest::indicesCountTest()
{
    QVERIFY(0 != mDbInst);

    QCOMPARE(mDbInst->indicesCount(), (quint64)0);

    mDbInst->readIndices(mFactories, mTools);

    QVERIFY(mDbInst->indicesCount() >= INDICES_COUNT);

    mDbInst->resetData();

    QCOMPARE(mDbInst->indicesCount(), (quint64)0);
}

void
DatabaseTest::indicesListTest()
{
    QVERIFY(0 != mDbInst);

    mDbInst->readIndices(mFactories, mTools);

    QStringList namesList;

    namesList << "ind_artists"
              << "ind_albums"
              << "ind_tracks"
              << "ind_users"
              << "ind_locations";

    QStringList indicesList;

    mDbInst->indicesList(indicesList);

    QVERIFY((quint64)indicesList.size() >= INDICES_COUNT);

    foreach(const QString &indexName, namesList) {
        QVERIFY(indicesList.contains(indexName));
    }
}

void
DatabaseTest::instanceTest()
{
    // the pointer should be non-zero
    QVERIFY(0 != mDbInst);

    // additionaly it should be valid
    QCOMPARE(mDbInst->sqlDriver(), Common::Database::PostgreSQL);
    Common::Database *inst = Common::Database::instance();

    for (int i = 0; i < 50; ++i)
    {
        QCOMPARE(Common::Database::instance(), inst);
    }
}

void
DatabaseTest::languagesCountTest()
{
    QVERIFY(0 != mDbInst);

    QCOMPARE(mDbInst->languagesCount(), (quint8)0);

    mDbInst->readLanguages(mFactories, mTools);

    QVERIFY(mDbInst->languagesCount() >= LANGUAGES_COUNT);

    mDbInst->resetData();

    QCOMPARE(mDbInst->languagesCount(), (quint8)0);
}

void
DatabaseTest::languagesListTest()
{
    QVERIFY(0 != mDbInst);

    mDbInst->readLanguages(mFactories, mTools);

    QStringList langNames;
    langNames << "plpgsql";

    QStringList langList;

    mDbInst->languagesList(langList);

    QVERIFY((quint8)langList.size() >= LANGUAGES_COUNT);

    foreach(const QString &name, langNames) {
        QVERIFY(langList.contains(name));
    }

}

void
DatabaseTest::readIndicesTest()
{
    QVERIFY(0 != mDbInst);

    QCOMPARE(mDbInst->indicesCount(), (quint64)0);

    mDbInst->readIndices(mFactories, mTools);

    quint64 count = mDbInst->indicesCount();

    QVERIFY(0 < count);

    QVERIFY(mDbInst->indicesCount() >= INDICES_COUNT);

    mDbInst->readIndices(mFactories, mTools);
    QCOMPARE(mDbInst->indicesCount(), count);
}

void
DatabaseTest::readLanguagesTest()
{
    QVERIFY(0 != mDbInst);
    // read langs
    mDbInst->readLanguages(mFactories, mTools);

    // plpgsql MUST be there
    QString name("plpgsql");

    DbLanguagePtr lang = mDbInst->findLanguage(name);

    QVERIFY(lang.valid());

    QCOMPARE(lang->name(), name);
}

void
DatabaseTest::readRolesTest()
{
    QVERIFY(0 != mDbInst);

    // read roles
    mDbInst->readRoles(mFactories, mTools);

    // validate names
    QString name("music_user");

    DbRolePtr role = mDbInst->findRole(name);
    QVERIFY(role.valid());

    QCOMPARE(role->name(), name);
}

void
DatabaseTest::readSchemasTest()
{
    QVERIFY(0 != mDbInst);

    mDbInst->loadData(mFactories, mTools);

    DbSchemaPtr schema;
    // validate their names
    QString name = QString("public");
    schema = mDbInst->findSchema(name);
    QVERIFY(schema.valid());
    QCOMPARE(schema->name(), name);

    name = QString("information_schema");
    schema = mDbInst->findSchema(name);
    QVERIFY(schema.valid());
    QCOMPARE(schema->name(), name);

    name = QString("vtunes");
    schema = mDbInst->findSchema(name);
    QVERIFY(schema.valid());
    QCOMPARE(schema->name(), name);
}

void
DatabaseTest::rolesCountTest()
{
    QVERIFY(0 != mDbInst);

    QCOMPARE(mDbInst->rolesCount(), (quint64)0);

    mDbInst->readRoles(mFactories, mTools);

    QVERIFY(mDbInst->rolesCount() >= ROLES_COUNT);

    mDbInst->resetData();

    QCOMPARE(mDbInst->rolesCount(), (quint64)0);
}

void
DatabaseTest::rolesListTest()
{
    QVERIFY(0 != mDbInst);

    QStringList rolesNames;

    rolesNames << "music_user"
               << "postgres";

    mDbInst->readRoles(mFactories, mTools);

    QStringList rolesList;

    mDbInst->rolesList(rolesList);

    QVERIFY((quint64)rolesList.size() >= ROLES_COUNT);

    foreach(const QString &name, rolesNames) {
        QVERIFY(rolesList.contains(name));
    }
}

void
DatabaseTest::schemasCountTest()
{
    QVERIFY(0 != mDbInst);

    QCOMPARE(mDbInst->schemasCount(), (quint64)0);

    mDbInst->loadData(mFactories, mTools);

//    QDEBUG_PRINT_ALL(schemas);

    QVERIFY(mDbInst->schemasCount() >= SCHEMAS_COUNT);

    mDbInst->resetData();

    QCOMPARE(mDbInst->schemasCount(), (quint64)0);
}

void
DatabaseTest::schemasListTest()
{
    QVERIFY(0 != mDbInst);

    QStringList names;

    names << "public"
          << "information_schema"
          << "vtunes";

    mDbInst->loadData(mFactories, mTools);

    QStringList schemasList;

    mDbInst->schemasList(schemasList);

    QVERIFY((quint64)schemasList.size() >= SCHEMAS_COUNT);

    foreach(const QString &name, names) {
        QVERIFY(schemasList.contains(name));
    }
}

void
DatabaseTest::setSqlDriverTest()
{
    using namespace DbObjects::Common;
    QVERIFY(0 != mDbInst);

    mDbInst->setSqlDriver("QPSQL");
    QCOMPARE(mDbInst->sqlDriver(), Database::PostgreSQL);

    mDbInst->setSqlDriver("QMYSQL");
    QCOMPARE(mDbInst->sqlDriver(), Database::MySQL);

    mDbInst->setSqlDriver("QODBC");
    QCOMPARE(mDbInst->sqlDriver(), Database::Oracle);

    mDbInst->setSqlDriver("QSQLITE");
    QCOMPARE(mDbInst->sqlDriver(), Database::SQLite);

    mDbInst->setSqlDriver("OtherDrv");
    QCOMPARE(mDbInst->sqlDriver(), Database::Unknown);


}

void
DatabaseTest::sqlDriverTest()
{
    QVERIFY(0 != mDbInst);

    // \see DatabaseTest::setSqlDriverTest
}


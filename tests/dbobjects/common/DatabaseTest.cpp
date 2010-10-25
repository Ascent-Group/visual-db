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

#include <dbobjects/common/Database.h>
#include <dbobjects/common/DatabaseCreator.h>
#include <dbobjects/common/DatabaseTest.h>
#include <dbobjects/common/DbSchema.h>
#include <dbobjects/psql/Index.h>
#include <dbobjects/psql/Language.h>
#include <dbobjects/psql/Role.h>

#include <QSqlDatabase>

using namespace DbObjects;

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
}

void
DatabaseTest::cleanupTestCase()
{
}

/*!
 * Called before each test function
 */
void
DatabaseTest::init()
{
    mDbInst = DatabaseCreator::createDatabase();
//    mDbInst->setSqlDriver("QPSQL");
}

/*!
 * Called after each test function
 */
void
DatabaseTest::cleanup()
{
    mDbInst->resetData();

    Common::DatabaseManager dbMgr;
    dbMgr.flush();
}

void
DatabaseTest::addIndexTest()
{
    QVERIFY(0 != mDbInst);

    // read all indices
    mDbInst->readIndices();

    quint64 count = mDbInst->indicesCount();

    QVERIFY(0 < count);

    // try to add dummy index
    Psql::Index *dummyIndex = new(std::nothrow) Psql::Index("dummy");

    QVERIFY(0 != dummyIndex);

    QVERIFY(mDbInst->addIndex(dummyIndex));

    QCOMPARE(mDbInst->indicesCount(), (quint64)count + 1);

    // try to add a index that already has been there
    QString indexName("ind_artists");
    Common::DbIndex *index = mDbInst->findIndex(indexName);

    QVERIFY(0 != index);

    QCOMPARE(mDbInst->addIndex(index), false);
}

void
DatabaseTest::addLanguageTest()
{
    // check db instance
    QVERIFY(0 != mDbInst);

    // read languages
    mDbInst->readLanguages();

    // get language count
    quint8 count = mDbInst->languagesCount();

    QVERIFY(0 < count);

    // add dummy language
    Psql::Language *dummyLang = new(std::nothrow) Psql::Language("dummy");

    QVERIFY(0 != dummyLang);

    QVERIFY(mDbInst->addLanguage(dummyLang));

    // check for language count to increase by 1
    QCOMPARE(mDbInst->languagesCount(), (quint8)(count + 1));

    // add existant language again
    QString langName("plpgsql");
    Common::DbLanguage *lang = mDbInst->findLanguage(langName);

    QVERIFY(0 != lang);

    // check for the language count to be the same
    QCOMPARE(mDbInst->addLanguage(lang), false);
}

void
DatabaseTest::addRoleTest()
{
    // check db instance
    QVERIFY(0 != mDbInst);

    // read roles
    mDbInst->readRoles();

    // get roles count
    quint64 count = mDbInst->rolesCount();

    QVERIFY(0 < count);

    // add dummy role
    Psql::Role *dummyRole = new(std::nothrow) Psql::Role("dummy");

    QVERIFY(0 != dummyRole);

    QVERIFY(mDbInst->addRole(dummyRole));

    // check for role count to increase by 1
    QCOMPARE(mDbInst->rolesCount(), (quint64)(count + 1));

    // add existant role again
    QString roleName("music_user");
    Common::DbRole *role = mDbInst->findRole(roleName);

    QVERIFY(0 != role);

    // check for the role count to be the same
    QCOMPARE(mDbInst->addRole(role), false);
}

void
DatabaseTest::addSchemaTest()
{
    // check db instance
    QVERIFY(0 != mDbInst);

    mDbInst->readRoles();
    // read schemas
    mDbInst->readSchemas();

    // get schemas count
    quint64 count = mDbInst->schemasCount();

    QVERIFY(0 < count);

    // add dummy schema
    Common::DbSchema *dummySchema = new(std::nothrow) Common::DbSchema("dummy");

    QVERIFY(0 != dummySchema);

//    QDEBUG_PRINT_ALL(schemas);

    QVERIFY(mDbInst->addSchema(dummySchema));

    // check for schemas count to increase by 1
    QCOMPARE(mDbInst->schemasCount(), (quint64)(count + 1));

    // add existant schema again
    QString schemaName("vtunes");
    Common::DbSchema *schema = mDbInst->findSchema(schemaName);

    QVERIFY(0 != schema);

    // check for the schema count to be the same
    QCOMPARE(mDbInst->addSchema(schema), false);

}

void
DatabaseTest::resetDataTest()
{
    QVERIFY(0 != mDbInst);

    mDbInst->loadData();
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

    mDbInst->readIndices();

    QVERIFY(0 != mDbInst->findIndex("ind_artists"));
    QVERIFY(0 != mDbInst->findIndex("ind_albums"));
    QVERIFY(0 != mDbInst->findIndex("ind_tracks"));
    QVERIFY(0 != mDbInst->findIndex("ind_users"));
    QVERIFY(0 != mDbInst->findIndex("ind_locations"));
}

void
DatabaseTest::findLanguageTest()
{
    QVERIFY(0 != mDbInst);

    mDbInst->readLanguages();

    QVERIFY(0 != mDbInst->findLanguage("plpgsql"));
}

void
DatabaseTest::findObjectTest()
{
    QVERIFY(0 != mDbInst);
    // keeping in mind the fact that findObject function is an auxilliary,
    // its test case can be considered PASSED if all other find*Test tests
    // pass
    QVERIFY(1);
}

void
DatabaseTest::findRoleTest()
{
    QVERIFY(0 != mDbInst);

    mDbInst->readRoles();

    QVERIFY(0 != mDbInst->findRole("music_user"));
}

void
DatabaseTest::findSchemaTest()
{
    QVERIFY(0 != mDbInst);

    mDbInst->readRoles();
    mDbInst->readSchemas();

    QVERIFY(0 != mDbInst->findSchema("public"));
    QVERIFY(0 != mDbInst->findSchema("vtunes"));
    QVERIFY(0 != mDbInst->findSchema("pg_catalog"));
}

void
DatabaseTest::findTableIndicesTest()
{
    QVERIFY(0 != mDbInst);

    mDbInst->loadData();

    QVector<Common::DbIndex*> indicesList;

    Common::DbSchema *schema = mDbInst->findSchema("vtunes");

    QVERIFY(0 != schema);

    Common::DbTable *table = schema->findTable("artists");

    QVERIFY(0 != table);

    mDbInst->findTableIndices(table, indicesList);

    QVERIFY(0 < indicesList.size());
}

void
DatabaseTest::flushTest()
{
    QVERIFY(0 != mDbInst);

    mDbInst->loadData();

    QVERIFY(0 != mDbInst->indicesCount());
    QVERIFY(0 != mDbInst->languagesCount());
    QVERIFY(0 != mDbInst->rolesCount());
    QVERIFY(0 != mDbInst->schemasCount());

    Common::DatabaseManager dbMgr;
    dbMgr.flush();

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

    mDbInst->readIndices();

    QVERIFY(mDbInst->indicesCount() >= INDICES_COUNT);

    mDbInst->resetData();

    QCOMPARE(mDbInst->indicesCount(), (quint64)0);
}

void
DatabaseTest::indicesListTest()
{
    QVERIFY(0 != mDbInst);

    mDbInst->readIndices();

    QStringList namesList;

    namesList << "ind_artists"
              << "ind_albums"
              << "ind_tracks"
              << "ind_users"
              << "ind_locations";

    QStringList indicesList;

    mDbInst->indicesList(&indicesList);

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

    mDbInst->readLanguages();

    QVERIFY(mDbInst->languagesCount() >= LANGUAGES_COUNT);

    mDbInst->resetData();

    QCOMPARE(mDbInst->languagesCount(), (quint8)0);
}

void
DatabaseTest::languagesListTest()
{
    QVERIFY(0 != mDbInst);

    mDbInst->readLanguages();

    QStringList langNames;
    langNames << "plpgsql";

    QStringList langList;

    mDbInst->languagesList(&langList);

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

    mDbInst->readIndices();

    quint64 count = mDbInst->indicesCount();

    QVERIFY(0 < count);

    QVERIFY(mDbInst->indicesCount() >= INDICES_COUNT);

    mDbInst->readIndices();
    QCOMPARE(mDbInst->indicesCount(), count);
}

void
DatabaseTest::readLanguagesTest()
{
    QVERIFY(0 != mDbInst);
    // read langs
    mDbInst->readLanguages();

    // plpgsql MUST be there
    QString name("plpgsql");

    Common::DbLanguage *lang = mDbInst->findLanguage(name);

    QVERIFY(0 != lang);

    QCOMPARE(lang->name(), name);
}

void
DatabaseTest::readRolesTest()
{
    QVERIFY(0 != mDbInst);

    // read roles
    mDbInst->readRoles();

    // validate names
    QString name("music_user");

    Common::DbRole *role = mDbInst->findRole(name);
    QVERIFY(0 != role);

    QCOMPARE(role->name(), name);
}

void
DatabaseTest::readSchemasTest()
{
    QVERIFY(0 != mDbInst);

    mDbInst->loadData();

    Common::DbSchema *schema;
    // validate their names
    QString name = QString("public");
    schema = mDbInst->findSchema(name);
    QVERIFY(0 != schema);
    QCOMPARE(schema->name(), name);

    name = QString("information_schema");
    schema = mDbInst->findSchema(name);
    QVERIFY(0 != schema);
    QCOMPARE(schema->name(), name);

    name = QString("vtunes");
    schema = mDbInst->findSchema(name);
    QVERIFY(0 != schema);
    QCOMPARE(schema->name(), name);
}

void
DatabaseTest::rolesCountTest()
{
    QVERIFY(0 != mDbInst);

    QCOMPARE(mDbInst->rolesCount(), (quint64)0);

    mDbInst->readRoles();

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

    mDbInst->readRoles();

    QStringList rolesList;

    mDbInst->rolesList(&rolesList);

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

    mDbInst->loadData();

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

    mDbInst->loadData();

    QStringList schemasList;

    mDbInst->schemasList(&schemasList);

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


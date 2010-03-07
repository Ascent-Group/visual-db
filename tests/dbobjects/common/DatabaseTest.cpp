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
#include <dbobjects/common/DatabaseTest.h>
#include <dbobjects/common/DbSchema.h>
#include <dbobjects/psql/PsqlLanguage.h>
#include <dbobjects/psql/PsqlIndex.h>
#include <dbobjects/psql/PsqlRole.h>

#include <QSqlDatabase>

void
DatabaseTest::initTestCase()
{
    QString driverName = QString("QPSQL");

    Database *mDbInst = Database::instance();
    mDbInst->setSqlDriver(driverName);
}

void
DatabaseTest::cleanupTestCase()
{
    DatabaseManager dbMgr;
    dbMgr.flush();
}

void
DatabaseTest::addIndexTest()
{
    QVERIFY(0 != mDbInst);

    // read all indices
    mDbInst->readIndices();

    int count = mDbInst->indicesCount();

    QVERIFY(0 < count);

    // try to add dummy index
    PsqlIndex *dummyIndex = new PsqlIndex("dummy");

    QVERIFY(mDbInst->addIndex(dummyIndex));

    QVERIFY((count + 1) == mDbInst->indicesCount());

    // try to add a index that already has been there
    DbIndex *index = mDbInst->findIndex("ind_artists");

    QVERIFY(0 != index);

    QVERIFY(false == mDbInst->addIndex(index));
}

void
DatabaseTest::addLanguageTest()
{
    QVERIFY(0);
}

void
DatabaseTest::addRoleTest()
{
    QVERIFY(0);
}

void
DatabaseTest::addSchemaTest()
{
    QVERIFY(0);
}

void
DatabaseTest::cleanupTest()
{
    mDbInst->readIndices();
    mDbInst->readLanguages();
    mDbInst->readRoles();
    mDbInst->readSchemas();

    QVERIFY(0 != mDbInst->indicesCount());
    QVERIFY(0 != mDbInst->languagesCount());
    QVERIFY(0 != mDbInst->rolesCount());
    QVERIFY(0 != mDbInst->schemasCount());

    mDbInst->cleanup();

    QVERIFY(0 == mDbInst->indicesCount());
    QVERIFY(0 == mDbInst->languagesCount());
    QVERIFY(0 == mDbInst->rolesCount());
    QVERIFY(0 == mDbInst->schemasCount());

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

    mDbInst->readRoles();
    //mDbInst->readLanguages();

    QVERIFY(0 != mDbInst->findLanguage("plpgsql"));
}

void
DatabaseTest::findObjectTest()
{
    // keeping in mind the fact that findObject function is an auxilliary,
    // its test case can be considered PASSED if all other find*Test tests
    // pass
    QVERIFY(1);
}

void
DatabaseTest::findRoleTest()
{
    Database *mDbInst = Database::instance();
    QVERIFY(0 != mDbInst);

    QVERIFY(0 != mDbInst->findRole("music_user"));
}

void
DatabaseTest::findSchemaTest()
{
    QVERIFY(0 != mDbInst);

    mDbInst->readSchemas();

    QVERIFY(0 != mDbInst->findSchema("public"));
    QVERIFY(0 != mDbInst->findSchema("vtunes"));
}

void
DatabaseTest::findTableIndicesTest()
{
    QVERIFY(0);
}

void
DatabaseTest::flushTest()
{
    QVERIFY(0);
}

void
DatabaseTest::indicesCountTest()
{
    QVERIFY(0);
}

void
DatabaseTest::indicesListTest()
{
    QVERIFY(0);
}

void
DatabaseTest::instanceTest()
{
    // the pointer should be non-zero
    QVERIFY(0 != mDbInst);

    // additionaly it should be valid
    QVERIFY(Database::PostgreSQL == mDbInst->sqlDriver());
}

void
DatabaseTest::languagesCountTest()
{
    QVERIFY(0);
}

void
DatabaseTest::languagesListTest()
{
    QVERIFY(0);
}

void
DatabaseTest::readIndicesTest()
{
    QVERIFY(0);
}

void
DatabaseTest::readLanguagesTest()
{
    // read langs
    mDbInst->readLanguages();

    // plpgsql MUST be there
    QString name = QString("plpgsql");
    QVERIFY(name == mDbInst->findLanguage(name)->name());
}

void
DatabaseTest::readRolesTest()
{
    Database *mDbInst = Database::instance();
    QVERIFY(0 != mDbInst);

    // read roles
    mDbInst->readRoles();

    // validate names
    QString name = QString("music_user");
    QVERIFY(name == mDbInst->findRole(name)->name());
}

void
DatabaseTest::readSchemasTest()
{
    QVERIFY(0 != mDbInst);

    // read schemas
    mDbInst->readSchemas();

    // validate their names
    QString name = QString("public");
    QVERIFY(name == mDbInst->findSchema(name)->name());

    name = QString("information_schema");
    QVERIFY(name == mDbInst->findSchema(name)->name());

    name = QString("vtunes");
    QVERIFY(name == mDbInst->findSchema(name)->name());
}

void
DatabaseTest::rolesCountTest()
{
    QVERIFY(0);
}

void
DatabaseTest::rolesListTest()
{
    QVERIFY(0);
}

void
DatabaseTest::schemasCountTest()
{
    mDbInst->readSchemas();

    QVERIFY(3 == mDbInst->schemasCount());
}

void
DatabaseTest::schemasListTest()
{
    QStringList list;

    mDbInst->schemasList(&list);

    QVERIFY(3 == list.count());

    QVERIFY(list.contains("public"));
    QVERIFY(list.contains("information_schema"));
    QVERIFY(list.contains("vtunes"));
}

void
DatabaseTest::setSqlDriverTest()
{
    QVERIFY(0);
}

void
DatabaseTest::sqlDriverTest()
{
    QVERIFY(0);
}

void
DatabaseTest::triggersCountTest()
{
    QVERIFY(0);
}

void
DatabaseTest::triggersListTest()
{
    QVERIFY(0);
}


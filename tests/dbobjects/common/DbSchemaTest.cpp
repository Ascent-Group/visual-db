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

#include <dbobjects/common/DatabaseCreator.h>
#include <dbobjects/common/DbSchemaTest.h>
#include <dbobjects/common/DbSchema.h>

using namespace DbObjects;

typedef Common::DbObjectPtr<Common::DbProcedure> DbProcedurePtr;
typedef Common::DbObjectPtr<Common::DbSchema> DbSchemaPtr;
typedef Common::DbObjectPtr<Common::DbTable> DbTablePtr;
typedef Common::DbObjectPtr<Common::DbTrigger> DbTriggerPtr;
typedef Common::DbObjectPtr<Common::DbView> DbViewPtr;

void
DbSchemaTest::initTestCase()
{
    mDbInst = DatabaseCreator::createDatabase();
    QVERIFY(0 != mDbInst);
}

void
DbSchemaTest::cleanupTestCase()
{
    Common::DatabaseManager dbMgr;
    dbMgr.flush();
}

/*!
 * Called before each test function
 */
void
DbSchemaTest::init()
{
    mDbInst->readSchemas();
}

/*!
 * Called after each test function
 */
void
DbSchemaTest::cleanup()
{
    mDbInst->resetData();
}

void
DbSchemaTest::addProcedureTest()
{
    DbSchemaPtr schema = mDbInst->findSchema("vtunes");

    QVERIFY(0 != schema.get());

    // find an existing procedure
    DbProcedurePtr procedure = schema->findProcedure("check_release_date");

    QVERIFY(0 != procedure.get());

    // adding proc which is already there should fail
    QVERIFY(false == schema->addProcedure(procedure));

    QString dummyName("dummyProcedure");
    // create a dummy procedure and add it
    DbProcedurePtr dummyProcedure(dummyName, "vtunes");

    // try to add it
    QVERIFY(schema->addProcedure(dummyProcedure));

    // try to find it
    QVERIFY(dummyProcedure == schema->findProcedure(dummyName));
    QVERIFY(0 != schema->findProcedure(dummyName).get());
    QVERIFY(dummyName == schema->findProcedure(dummyName).name());
    QVERIFY(dummyName == schema->findProcedure(dummyName)->name());
}

void
DbSchemaTest::addTableTest()
{
    DbSchemaPtr schema = mDbInst->findSchema("vtunes");

    QVERIFY(0 != schema.get());

    // find an existing table
    DbTablePtr table = schema->findTable("artists");

    QVERIFY(0 != table.get());

    // adding table which is already there should fail
    QVERIFY(false == schema->addTable(table));

    QString dummyName("dummyTable");
    // create a dummy table and add it
    DbTablePtr dummyTable(dummyName, "vtunes");

    // try to add it
    QVERIFY(schema->addTable(dummyTable));

    // try to find it
    QVERIFY(dummyTable == schema->findTable(dummyName));
    QVERIFY(0 != schema->findTable(dummyName).get());
    QVERIFY(dummyName == schema->findTable(dummyName).name());
    QVERIFY(dummyName == schema->findTable(dummyName)->name());
}

void
DbSchemaTest::addTriggerTest()
{
    DbSchemaPtr schema = mDbInst->findSchema("vtunes");

    QVERIFY(0 != schema.get());

    // find an existing trigger
    DbTriggerPtr trigger = schema->findTrigger("locations_biu");

    QVERIFY(0 != trigger.get());

    // adding trigger which is already there should fail
    QVERIFY(false == schema->addTrigger(trigger));

    QString dummyName("dummyTrigger");
    // create a dummy trigger and add it
    DbTriggerPtr dummyTrigger(dummyName, "vtunes");

    // try to add it
    QVERIFY(schema->addTrigger(dummyTrigger));

    // try to find it
    QVERIFY(dummyTrigger == schema->findTrigger(dummyName));
    QVERIFY(0 != schema->findTrigger(dummyName).get());
    QVERIFY(dummyName == schema->findTrigger(dummyName).name());
    QVERIFY(dummyName == schema->findTrigger(dummyName)->name());
}

void
DbSchemaTest::addViewTest()
{
    DbSchemaPtr schema = mDbInst->findSchema("vtunes");

    QVERIFY(0 != schema.get());

    // find an existing view
    DbViewPtr view = schema->findView("users_playlists");

    QVERIFY(0 != view.get());

    // adding view which is already there should fail
    QVERIFY(false == schema->addView(view));

    QString dummyName("dummyView");
    // create a dummy view and add it
    DbViewPtr dummyView(dummyName, "vtunes");

    // try to add it
    QVERIFY(schema->addView(dummyView));

    // try to find it
    QCOMPARE(dummyView, schema->findView(dummyName));
    QVERIFY(0 != schema->findView(dummyName).get());
    QVERIFY(dummyName == schema->findView(dummyName).name());
    QVERIFY(dummyName == schema->findView(dummyName)->name());
}

void
DbSchemaTest::resetDataTest()
{
    DbSchemaPtr schema = mDbInst->findSchema("vtunes");

    QVERIFY(0 != schema.get());
    QCOMPARE(schema.get()->isLoaded(), true);

    QVERIFY(0 < schema->tablesCount());
    QVERIFY(0 < schema->viewsCount());
    QVERIFY(0 < schema->proceduresCount());
    QVERIFY(0 < schema->triggersCount());

    schema->resetData();

    QCOMPARE(schema.get()->isLoaded(), false);

    QCOMPARE(schema->tablesCount(), (quint64)0);
    QCOMPARE(schema->viewsCount(), (quint64)0);
    QCOMPARE(schema->proceduresCount(), (quint64)0);
    QCOMPARE(schema->triggersCount(), (quint16)0);
}

void
DbSchemaTest::findProcedureTest()
{
    DbSchemaPtr schema = mDbInst->findSchema("vtunes");
    QVERIFY(0 != schema.get());

    QStringList proceduresNames;
    proceduresNames
        << "insert_album"
        << "insert_track";

    DbProcedurePtr procedure;
    foreach (const QString &name, proceduresNames) {
        procedure = schema->findProcedure(name);
        QVERIFY(0 != procedure.get());
        QCOMPARE(procedure.name(), name);
        QCOMPARE(procedure->name(), name);
    }
}

void
DbSchemaTest::findTableTest()
{
    DbSchemaPtr schema = mDbInst->findSchema("vtunes");
    QVERIFY(0 != schema.get());

    QStringList tablesNames;
    tablesNames
        << "artists"
        << "genres"
        << "albums"
        << "tracks"
        << "locations"
        << "users"
        << "playlists"
        << "extended_playlists";

    DbTablePtr table;
    foreach (const QString &name, tablesNames) {
        table = schema->findTable(name);
        QVERIFY(0 != table.get());
        QCOMPARE(table.name(), name);
        QCOMPARE(table->name(), name);
    }
}

void
DbSchemaTest::findTriggerTest()
{
    DbSchemaPtr schema = mDbInst->findSchema("vtunes");
    QVERIFY(0 != schema.get());

    QStringList triggersNames;
    triggersNames
        << "albums_biu"
        << "locations_biu";

    DbTriggerPtr trigger;
    foreach (const QString &name, triggersNames) {
        trigger = schema->findTrigger(name);
        QVERIFY(0 != trigger.get());
        QCOMPARE(trigger.name(), name);
        QCOMPARE(trigger->name(), name);
    }
}

void
DbSchemaTest::findViewTest()
{
    DbSchemaPtr schema = mDbInst->findSchema("vtunes");
    QVERIFY(0 != schema.get());

    QStringList viewsNames;
    viewsNames
        << "users_playlists"
        << "artists_tracks";

    DbViewPtr view;
    foreach (const QString &name, viewsNames) {
        view = schema->findView(name);
        QVERIFY(0 != view.get());
        QCOMPARE(view.name(), name);
        QCOMPARE(view->name(), name);
    }
}

void
DbSchemaTest::typeTest()
{
    DbSchemaPtr schema = mDbInst->findSchema("vtunes");
    QVERIFY(0 != schema.get());

    QCOMPARE(schema->type(), DbObjects::Common::DbObject::SchemaObject);
}

void
DbSchemaTest::proceduresCountTest()
{
    QVERIFY(0);
}

void
DbSchemaTest::proceduresListTest()
{
    QVERIFY(0);
}

void
DbSchemaTest::readProceduresTest()
{
    QVERIFY(0);
}

void
DbSchemaTest::readTablesTest()
{
    QVERIFY(0);
}

void
DbSchemaTest::readTriggersTest()
{
    QVERIFY(0);
}

void
DbSchemaTest::readViewsTest()
{
    QVERIFY(0);
}

void
DbSchemaTest::tablesCountTest()
{
    QVERIFY(0);
}

void
DbSchemaTest::tablesListTest()
{
    QVERIFY(0);
}

void
DbSchemaTest::viewsCountTest()
{
    QVERIFY(0);
}

void
DbSchemaTest::viewsListTest()
{
    QVERIFY(0);
}


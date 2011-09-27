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
#include <dbobjects/common/Factories.h>
#include <dbobjects/common/Tools.h>

using namespace DbObjects;

void
DbSchemaTest::initTestCase()
{
    mSchemaName = "vtunes";

    mDbInst = DatabaseCreator::createDatabase();
    QVERIFY(0 != mDbInst);

    mFactories = DatabaseCreator::factories();
    QVERIFY(0 != mFactories);

    mTools = DatabaseCreator::tools();
    QVERIFY(0 != mTools);

    mTablesNamesList << "artists"
                     << "genres"
                     << "albums"
                     << "tracks"
                     << "locations"
                     << "users"
                     << "playlists"
                     << "extended_playlists";

    mTriggersNamesList << "albums_biu"
                       << "locations_biu";

    mProceduresNamesList << "insert_album"
                         << "insert_track";

    mViewsNamesList << "users_playlists"
                    << "artists_tracks";

}

void
DbSchemaTest::cleanupTestCase()
{
    delete mDbInst;
    delete mFactories;
    delete mTools;
}

/*!
 * Called before each test function
 */
void
DbSchemaTest::init()
{
    mDbInst->loadData(mFactories, mTools);
    mSchema = mDbInst->findSchema(mSchemaName);
    QVERIFY(mSchema.valid());
}

/*!
 * Called after each test function
 */
void
DbSchemaTest::cleanup()
{
    mSchema = DbSchemaPtr();
    mDbInst->resetData();
}

void
DbSchemaTest::addProcedureTest()
{
    // find an existing procedure
    DbProcedurePtr procedure = mSchema->findProcedure("check_release_date");

    QVERIFY(procedure.valid());

    // adding proc which is already there should fail
    QVERIFY(false == mSchema->addProcedure(procedure));

    QString dummyName("dummyProcedure");
    // create a dummy procedure and add it
    DbProcedurePtr dummyProcedure(mDbInst, mFactories, dummyName, mSchemaName);

    // try to add it
    QVERIFY(mSchema->addProcedure(dummyProcedure));

    // try to find it
    QVERIFY(dummyProcedure == mSchema->findProcedure(dummyName));
    QVERIFY(mSchema->findProcedure(dummyName).valid());
    QVERIFY(dummyName == mSchema->findProcedure(dummyName).name());
    QVERIFY(dummyName == mSchema->findProcedure(dummyName)->name());
}

void
DbSchemaTest::addTableTest()
{
    // find an existing table
    DbTablePtr table = mSchema->findTable("artists");

    QVERIFY(table.valid());

    // adding table which is already there should fail
    QVERIFY(false == mSchema->addTable(table));

    QString dummyName("dummyTable");
    // create a dummy table and add it
    DbTablePtr dummyTable(mDbInst, mFactories, dummyName, mSchemaName);

    // try to add it
    QVERIFY(mSchema->addTable(dummyTable));

    // try to find it
    QVERIFY(dummyTable == mSchema->findTable(dummyName));
    QVERIFY(mSchema->findTable(dummyName).valid());
    QVERIFY(dummyName == mSchema->findTable(dummyName).name());
    QVERIFY(dummyName == mSchema->findTable(dummyName)->name());
}

void
DbSchemaTest::addTriggerTest()
{
    // find an existing trigger
    DbTriggerPtr trigger = mSchema->findTrigger("locations_biu");

    QVERIFY(trigger.valid());

    // adding trigger which is already there should fail
    QVERIFY(false == mSchema->addTrigger(trigger));

    QString dummyName("dummyTrigger");
    // create a dummy trigger and add it
    DbTriggerPtr dummyTrigger(mDbInst, mFactories, dummyName, mSchemaName);

    // try to add it
    QVERIFY(mSchema->addTrigger(dummyTrigger));

    // try to find it
    QVERIFY(dummyTrigger == mSchema->findTrigger(dummyName));
    QVERIFY(mSchema->findTrigger(dummyName).valid());
    QVERIFY(dummyName == mSchema->findTrigger(dummyName).name());
    QVERIFY(dummyName == mSchema->findTrigger(dummyName)->name());
}

void
DbSchemaTest::addViewTest()
{
    // find an existing view
    DbViewPtr view = mSchema->findView("users_playlists");

    QVERIFY(view.valid());

    // adding view which is already there should fail
    QVERIFY(false == mSchema->addView(view));

    QString dummyName("dummyView");
    // create a dummy view and add it
    DbViewPtr dummyView(mDbInst, mFactories, dummyName, mSchemaName);

    // try to add it
    QVERIFY(mSchema->addView(dummyView));

    // try to find it
    QCOMPARE(dummyView, mSchema->findView(dummyName));
    QVERIFY(mSchema->findView(dummyName).valid());
    QVERIFY(dummyName == mSchema->findView(dummyName).name());
    QVERIFY(dummyName == mSchema->findView(dummyName)->name());
}

void
DbSchemaTest::resetDataTest()
{
    QCOMPARE(mSchema.get()->isLoaded(), true);

    QVERIFY(0 < mSchema->tablesCount());
    QVERIFY(0 < mSchema->viewsCount());
    QVERIFY(0 < mSchema->proceduresCount());
    QVERIFY(0 < mSchema->triggersCount());

    mSchema->resetData();

    QCOMPARE(mSchema.get()->isLoaded(), false);

    QCOMPARE(mSchema->tablesCount(), (quint64)0);
    QCOMPARE(mSchema->viewsCount(), (quint64)0);
    QCOMPARE(mSchema->proceduresCount(), (quint64)0);
    QCOMPARE(mSchema->triggersCount(), (quint16)0);
}

void
DbSchemaTest::findProcedureTest()
{
    mDbInst->readRoles(mFactories, mTools);
    mDbInst->readLanguages(mFactories, mTools);

    DbProcedurePtr procedure;
    foreach (const QString &name, mProceduresNamesList) {
        procedure = mSchema->findProcedure(name);
        QVERIFY(procedure.valid());
        QCOMPARE(procedure.name(), name);
        QCOMPARE(procedure->name(), name);
    }
}

void
DbSchemaTest::findTableTest()
{
    DbTablePtr table;
    foreach (const QString &name, mTablesNamesList) {
        table = mSchema->findTable(name);
        QVERIFY(table.valid());
        QCOMPARE(table.name(), name);
        QCOMPARE(table->name(), name);
    }
}

void
DbSchemaTest::findTriggerTest()
{
//    mDbInst->loadData();
    mDbInst->readRoles(mFactories, mTools);
    mDbInst->readLanguages(mFactories, mTools);

    mSchema->readTriggers(mFactories, mTools);

    DbTriggerPtr trigger;
    foreach (const QString &name, mTriggersNamesList) {
        trigger = mSchema->findTrigger(name);
        QVERIFY(trigger.valid());
        QCOMPARE(trigger.name(), name);
        QCOMPARE(trigger->name(), name);
    }
}

void
DbSchemaTest::findViewTest()
{
    DbViewPtr view;
    foreach (const QString &name, mViewsNamesList) {
        view = mSchema->findView(name);
        QVERIFY(view.valid());
        QCOMPARE(view.name(), name);
        QCOMPARE(view->name(), name);
    }
}

void
DbSchemaTest::typeTest()
{
    QCOMPARE(mSchema->type(), SchemaObject);
}

void
DbSchemaTest::proceduresCountTest()
{
    QVERIFY((quint32)mProceduresNamesList.size() <= mSchema->proceduresCount());
}

void
DbSchemaTest::proceduresListTest()
{
    QStringList actualProceduresNames;
    mSchema->proceduresList(actualProceduresNames);

    foreach (const QString &name, mProceduresNamesList) {
        QVERIFY(actualProceduresNames.contains(name));
    }
}

void
DbSchemaTest::readProceduresTest()
{
    Common::DbSchema *schema = const_cast<Common::DbSchema*>(mSchema.get());
    QVERIFY(0 != schema);
    QVERIFY(0 < schema->proceduresCount());

    schema->resetData();
    QCOMPARE(schema->proceduresCount(), (quint64)0);

    schema->readProcedures(mFactories, mTools);
    QVERIFY(0 < schema->proceduresCount());
    QVERIFY(mProceduresNamesList.count() <= schema->proceduresCount());
}

void
DbSchemaTest::readTablesTest()
{
    Common::DbSchema *schema = const_cast<Common::DbSchema*>(mSchema.get());
    QVERIFY(0 != schema);
    QVERIFY(0 < schema->tablesCount());

    schema->resetData();
    QCOMPARE(schema->tablesCount(), (quint64)0);

    schema->readTables(mFactories, mTools);
    QVERIFY(0 < schema->tablesCount());
    QVERIFY(mTablesNamesList.count() <= schema->tablesCount());
}

void
DbSchemaTest::readTriggersTest()
{
    Common::DbSchema *schema = const_cast<Common::DbSchema*>(mSchema.get());
    QVERIFY(0 != schema);
    QVERIFY(0 < schema->triggersCount());

    schema->resetData();
    QCOMPARE(schema->triggersCount(), (quint16)0);

    schema->readTriggers(mFactories, mTools);
    QVERIFY(0 < schema->triggersCount());
    QVERIFY(mTriggersNamesList.count() <= schema->triggersCount());
}

void
DbSchemaTest::readViewsTest()
{
    Common::DbSchema *schema = const_cast<Common::DbSchema*>(mSchema.get());
    QVERIFY(0 != schema);
    QVERIFY(0 < schema->viewsCount());

    schema->resetData();
    QCOMPARE(schema->viewsCount(), (quint64)0);

    schema->readViews(mFactories, mTools);
    QVERIFY(0 < schema->viewsCount());
    QVERIFY(mViewsNamesList.count() <= schema->viewsCount());
}

void
DbSchemaTest::tablesCountTest()
{
    QVERIFY((quint32)mTablesNamesList.size() <= mSchema->tablesCount());
}

void
DbSchemaTest::tablesListTest()
{
    QStringList actualTablesNames;
    mSchema->tablesList(actualTablesNames);

    foreach (const QString &name, mTablesNamesList) {
        QVERIFY(actualTablesNames.contains(name));
    }
}

void
DbSchemaTest::viewsCountTest()
{
    QVERIFY((quint32)mViewsNamesList.size() <= mSchema->viewsCount());
}

void
DbSchemaTest::viewsListTest()
{
    QStringList actualViewsNames;
    mSchema->viewsList(actualViewsNames);

    foreach (const QString &name, mViewsNamesList) {
        QVERIFY(actualViewsNames.contains(name));
    }
}

void
DbSchemaTest::triggersCountTest()
{
    QVERIFY((quint32)mTriggersNamesList.size() <= mSchema->triggersCount());
}

void
DbSchemaTest::triggersListTest()
{
    QStringList actualTriggersNames;
    mSchema->triggersList(actualTriggersNames);

    foreach (const QString &name, mTriggersNamesList) {
        QVERIFY(actualTriggersNames.contains(name));
    }
}

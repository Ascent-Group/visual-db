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

#include <dbobjects/psql/PsqlFactoriesTest.h>

#include <dbobjects/common/DatabaseCreator.h>
#include <dbobjects/psql/Index.h>
#include <dbobjects/psql/Factories.h>
#include <dbobjects/psql/Language.h>
#include <dbobjects/psql/Procedure.h>
#include <dbobjects/psql/Role.h>
#include <dbobjects/psql/Schema.h>
#include <dbobjects/psql/Table.h>
#include <dbobjects/psql/Trigger.h>
#include <dbobjects/psql/View.h>

typedef DbObjects::Common::DbObjectPtr<DbObjects::Common::DbSchema> DbSchemaPtr;

void
PsqlFactoriesTest::initTestCase()
{
    using namespace DbObjects;

    mDbInst = DatabaseCreator::createDatabase();
    QVERIFY(0 != mDbInst);

    mFactories = new Psql::Factories();
    QVERIFY(0 != mFactories);
}

void
PsqlFactoriesTest::cleanupTestCase()
{
    delete mDbInst;
    delete mFactories;
}

void
PsqlFactoriesTest::createIndexTest()
{
    using namespace DbObjects;

    QString indexName("ind_artists");
    QVERIFY(0 != dynamic_cast<Psql::Index*>(mFactories->indexFactory()->create(indexName)));
}

void
PsqlFactoriesTest::createLanguageTest()
{
    using namespace DbObjects;

    QString languageName("plpgsql");
    QVERIFY(0 != dynamic_cast<Psql::Language*>(mFactories->languageFactory()->create(languageName)));
}

void
PsqlFactoriesTest::createProcedureTest()
{
    using namespace DbObjects;

    QString procedureName("insert_album");
    DbSchemaPtr schema = mDbInst->findSchema("vtunes");
    QVERIFY(0 != dynamic_cast<Psql::Procedure*>(mFactories->procedureFactory()->create(procedureName, schema)));
}

void
PsqlFactoriesTest::createRoleTest()
{
    using namespace DbObjects;

    QString roleName("imusic_user");
    QVERIFY(0 != dynamic_cast<Psql::Role*>(mFactories->roleFactory()->create(roleName)));
}

void
PsqlFactoriesTest::createSchemaTest()
{
    using namespace DbObjects;

    QString schemaName("vtunes");
    QVERIFY(0 != dynamic_cast<Psql::Schema*>(mFactories->schemaFactory()->create(schemaName)));
}

void
PsqlFactoriesTest::createTableTest()
{
    using namespace DbObjects;

    QString tableName("artists");
    DbSchemaPtr schema = mDbInst->findSchema("vtunes");
    QVERIFY(0 != dynamic_cast<Psql::Table*>(mFactories->tableFactory()->create(tableName, schema)));
}

void
PsqlFactoriesTest::createTriggerTest()
{
    using namespace DbObjects;

    QString triggerName("check_location");
    DbSchemaPtr schema = mDbInst->findSchema("vtunes");
    QVERIFY(0 != dynamic_cast<Psql::Trigger*>(mFactories->triggerFactory()->create(triggerName, schema)));
}

void
PsqlFactoriesTest::createViewTest()
{
    using namespace DbObjects;

    QString viewName("ind_artists");
    DbSchemaPtr schema = mDbInst->findSchema("vtunes");
    QVERIFY(0 != dynamic_cast<Psql::View*>(mFactories->viewFactory()->create(viewName, schema)));
}


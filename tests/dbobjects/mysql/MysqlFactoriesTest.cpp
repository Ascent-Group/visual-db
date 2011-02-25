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

#include <dbobjects/mysql/MysqlFactoriesTest.h>

#include <dbobjects/common/DatabaseCreator.h>
#include <dbobjects/mysql/Index.h>
#include <dbobjects/mysql/Factories.h>
#include <dbobjects/mysql/Language.h>
#include <dbobjects/mysql/Procedure.h>
#include <dbobjects/mysql/Role.h>
#include <dbobjects/mysql/Schema.h>
#include <dbobjects/mysql/Table.h>
#include <dbobjects/mysql/Trigger.h>
#include <dbobjects/mysql/View.h>

typedef DbObjects::Common::DbObjectPtr<DbObjects::Common::DbSchema> DbSchemaPtr;

void
MysqlFactoriesTest::initTestCase()
{
    using namespace DbObjects;

    mDbInst = DatabaseCreator::createDatabase();
    QVERIFY(0 != mDbInst);

    mFactories = new Mysql::Factories();
    QVERIFY(0 != mFactories);
}

void
MysqlFactoriesTest::cleanupTestCase()
{
    delete mDbInst;
    delete mFactories;
}

void
MysqlFactoriesTest::createIndexTest()
{
    using namespace DbObjects;

    QString indexName("ind_artists");
    QVERIFY(0 != dynamic_cast<Mysql::Index*>(mFactories->indexFactory()->create(indexName)));
}

void
MysqlFactoriesTest::createLanguageTest()
{
    using namespace DbObjects;

    QString languageName("plpgsql");
    QVERIFY(0 != dynamic_cast<Mysql::Language*>(mFactories->languageFactory()->create(languageName)));
}

void
MysqlFactoriesTest::createProcedureTest()
{
    using namespace DbObjects;

    QString procedureName("insert_album");
    DbSchemaPtr schema = mDbInst->findSchema("vtunes");
    QVERIFY(0 != dynamic_cast<Mysql::Procedure*>(mFactories->procedureFactory()->create(procedureName, schema)));
}

void
MysqlFactoriesTest::createRoleTest()
{
    using namespace DbObjects;

    QString roleName("imusic_user");
    QVERIFY(0 != dynamic_cast<Mysql::Role*>(mFactories->roleFactory()->create(roleName)));
}

void
MysqlFactoriesTest::createSchemaTest()
{
    using namespace DbObjects;

    QString schemaName("vtunes");
    QVERIFY(0 != dynamic_cast<Mysql::Schema*>(mFactories->schemaFactory()->create(schemaName)));
}

void
MysqlFactoriesTest::createTableTest()
{
    using namespace DbObjects;

    QString tableName("artists");
    DbSchemaPtr schema = mDbInst->findSchema("vtunes");
    QVERIFY(0 != dynamic_cast<Mysql::Table*>(mFactories->tableFactory()->create(tableName, schema)));
}

void
MysqlFactoriesTest::createTriggerTest()
{
    using namespace DbObjects;

    QString triggerName("check_location");
    DbSchemaPtr schema = mDbInst->findSchema("vtunes");
    QVERIFY(0 != dynamic_cast<Mysql::Trigger*>(mFactories->triggerFactory()->create(triggerName, schema)));
}

void
MysqlFactoriesTest::createViewTest()
{
    using namespace DbObjects;

    QString viewName("ind_artists");
    DbSchemaPtr schema = mDbInst->findSchema("vtunes");
    QVERIFY(0 != dynamic_cast<Mysql::View*>(mFactories->viewFactory()->create(viewName, schema)));
}


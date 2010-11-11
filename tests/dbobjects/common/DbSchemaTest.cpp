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

#include <dbobjects/common/DbSchemaTest.h>
#include <dbobjects/common/Database.h>
#include <dbobjects/common/DbSchema.h>

using namespace DbObjects;

typedef Common::DbObjectPtr<Common::DbSchema> DbSchemaPtr;

void
DbSchemaTest::initTestCase()
{

}

void
DbSchemaTest::cleanupTestCase()
{

}


void
DbSchemaTest::addProcedureTest()
{
    QVERIFY(0);
}

void
DbSchemaTest::addTableTest()
{
    QVERIFY(0);
}

void
DbSchemaTest::addTriggerTest()
{
    QVERIFY(0);
}

void
DbSchemaTest::addViewTest()
{
    QVERIFY(0);
}

void
DbSchemaTest::cleanupTest()
{
    QVERIFY(0);
}

void
DbSchemaTest::findObjectTest()
{
    QVERIFY(0);
}

void
DbSchemaTest::findProcedureTest()
{
    QVERIFY(0);
}

void
DbSchemaTest::findTableTest()
{
    QVERIFY(0);
}

void
DbSchemaTest::findTriggerTest()
{
    Common::Database *dbInst = Common::Database::instance();
    QVERIFY(0 != dbInst);

    dbInst->readSchemas();

    DbSchemaPtr schema = dbInst->findSchema("vtunes");

    QVERIFY(0 != schema.get());

    schema->readTriggers();

    QVERIFY(0 != schema->findTrigger("check_location").get());
    QVERIFY(0 != schema->findTrigger("check_release_date").get());
}

void
DbSchemaTest::findViewTest()
{
    QVERIFY(0);
}

void
DbSchemaTest::typeTest()
{
    QVERIFY(0);
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


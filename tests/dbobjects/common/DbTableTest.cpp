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
#include <dbobjects/common/DbTableTest.h>

using namespace DbObjects;

typedef Common::DbObjectPtr<Common::DbTable> DbTablePtr;

void
DbTableTest::initTestCase()
{
    mDbInst = DatabaseCreator::createDatabase();
}

void
DbTableTest::cleanupTestCase()
{
    Common::DatabaseManager dbMgr;
    dbMgr.flush();
}

/*!
 * Called before each test function
 */
void
DbTableTest::init()
{
}

/*!
 * Called after each test function
 */
void
DbTableTest::cleanup()
{
    mDbInst->resetData();
}

void
DbTableTest::checkForeignKeyTest()
{
    QVERIFY(0);
}

void
DbTableTest::checkPrimaryKeyTest()
{
    QVERIFY(0);
}

void
DbTableTest::checkUniqueTest()
{
    QVERIFY(0);
}

void
DbTableTest::columnNameTest()
{
    QVERIFY(0);
}

void
DbTableTest::columnTypeTest()
{
    QVERIFY(0);
}

void
DbTableTest::columnsCountTest()
{
    QVERIFY(0);
}

void
DbTableTest::foreignFieldsTest()
{
    QVERIFY(0);
}

void
DbTableTest::foreignSchemaNameTest()
{
    QVERIFY(0);
}

void
DbTableTest::foreignTableNameTest()
{
    QVERIFY(0);
}

void
DbTableTest::fullNameTest()
{
    QVERIFY(0);
}

void
DbTableTest::getIndicesTest()
{
    QVERIFY(0);
}

void
DbTableTest::isColumnForeignKeyTest()
{
    QVERIFY(0);
}

void
DbTableTest::isColumnNullableTest()
{
    QVERIFY(0);
}

void
DbTableTest::isColumnPrimaryKeyTest()
{
    QVERIFY(0);
}

void
DbTableTest::isColumnUniqueTest()
{
    QVERIFY(0);
}

void
DbTableTest::loadDataTest()
{
    QVERIFY(0);
}

void
DbTableTest::typeTest()
{
    QVERIFY(0);
}

void
DbTableTest::schemaTest()
{
    QVERIFY(0);
}

void
DbTableTest::setSchemaTest()
{
    QVERIFY(0);
}

void
DbTableTest::parentTablesTest()
{
    mDbInst->readSchemas();

    DbTablePtr tablePtr = mDbInst->findSchema("vtunes")->findTable("extended_playlists");

    QVERIFY(0 != tablePtr.get());

    QVector<DbTablePtr> parentsList;
    quint32 parentsCount = tablePtr->parentTables(parentsList);

    QCOMPARE(parentsCount, (quint32)1);

    DbTablePtr parent = parentsList.at(0);

    QVERIFY(0 != parent.get());

    QVERIFY("vtunes" == parent.schemaName());
    QVERIFY("vtunes" == parent->schema()->name());
    QVERIFY("playlists" == parent.name());
    QVERIFY("playlists" == parent->name());
}


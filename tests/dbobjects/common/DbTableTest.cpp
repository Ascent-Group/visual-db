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
#include <dbobjects/common/Factories.h>
#include <dbobjects/common/Tools.h>

using namespace DbObjects;


void
DbTableTest::initTestCase()
{
    mSchemaName = "vtunes";
    mTableName = "extended_playlists";

    //           0      1        2       3    4     5      6      7       8
    //          name | type | nullable | PK | FK | Fsch | Ftbl | Ffld | Unique
    mColumns << "id|integer|false|true|false|true||||true"
             << "track|integer|false|false|true|false|vtunes|tracks|id|false"
             << "user|integer|false|false|true|false|vtunes|users|id|false"
             << "expires|date|true|false|false|false||||false";

    mDbInst = DatabaseCreator::createDatabase();
    QVERIFY(0 != mDbInst);

    mFactories = DatabaseCreator::factories();
    QVERIFY(0 != mFactories);

    mTools = DatabaseCreator::tools();
    QVERIFY(0 != mTools);
}

void
DbTableTest::cleanupTestCase()
{
    delete mDbInst;
    delete mFactories;
    delete mTools;
}

/*!
 * Called before each test function
 */
void
DbTableTest::init()
{
    mDbInst->loadData(mFactories, mTools);

    DbSchemaPtr schema = mDbInst->findSchema(mSchemaName);
    QVERIFY(schema.valid());

    mTable = schema->findTable(mTableName);
    QVERIFY(mTable.valid());

    QCOMPARE(mTable.name(), mTableName);
    QCOMPARE(mTable->name(), mTableName);
}

/*!
 * Called after each test function
 */
void
DbTableTest::cleanup()
{
    mTable = DbTablePtr();
    mDbInst->resetData();
}

void
DbTableTest::columnNameTest()
{
    for (int i = 0; i < mColumns.size(); ++i) {
        QCOMPARE(mTable->columnName(i), mColumns.at(i).split("|").at(0));
    }
}

void
DbTableTest::columnTypeTest()
{
    for (int i = 0; i < mColumns.size(); ++i) {
        QCOMPARE(mTable->columnType(i), mColumns.at(i).split("|").at(1));
    }
}

void
DbTableTest::columnsCountTest()
{
    QCOMPARE(mTable->columnsCount(), (quint16)mColumns.count());
}

void
DbTableTest::foreignFieldsTest()
{
    for (int i = 0; i < mColumns.size(); ++i) {
        // Since foreignFields returns a stringlist we will compare them joined with comma
        QCOMPARE(mTable->foreignFields(i).join(","), mColumns.at(i).split("|").at(7));
    }
}

void
DbTableTest::foreignSchemaNameTest()
{
    for (int i = 0; i < mColumns.size(); ++i) {
        QCOMPARE(mTable->foreignSchemaName(i), mColumns.at(i).split("|").at(5));
    }
}

void
DbTableTest::foreignTableNameTest()
{
    for (int i = 0; i < mColumns.size(); ++i) {
        QCOMPARE(mTable->foreignTableName(i), mColumns.at(i).split("|").at(6));
    }
}

void
DbTableTest::fullNameTest()
{
    QCOMPARE(mTable->fullName(), QString("%1.%2").arg(mSchemaName).arg(mTableName));
}

void
DbTableTest::getIndicesTest()
{
    QVERIFY(0);
}

void
DbTableTest::isColumnForeignKeyTest()
{
    for (int i = 0; i < mColumns.size(); ++i) {
        QCOMPARE(mTable->isColumnForeignKey(i), QVariant(mColumns.at(i).split("|").at(4)).toBool());
    }
}

void
DbTableTest::isColumnNullableTest()
{
    for (int i = 0; i < mColumns.size(); ++i) {
        QCOMPARE(mTable->isColumnNullable(i), QVariant(mColumns.at(i).split("|").at(2)).toBool());
    }
}

void
DbTableTest::isColumnPrimaryKeyTest()
{
    for (int i = 0; i < mColumns.size(); ++i) {
        QCOMPARE(mTable->isColumnPrimaryKey(i), QVariant(mColumns.at(i).split("|").at(3)).toBool());
    }
}

void
DbTableTest::isColumnUniqueTest()
{
    for (int i = 0; i < mColumns.size(); ++i) {
        QCOMPARE(mTable->isColumnUnique(i), QVariant(mColumns.at(i).split("|").at(8)).toBool());
    }
}

void
DbTableTest::loadDataTest()
{
    QVERIFY(0);
}

void
DbTableTest::typeTest()
{
    QCOMPARE(mTable->type(), TableObject);
}

void
DbTableTest::schemaTest()
{
    DbSchemaPtr schema = mTable->schema();
    QVERIFY(schema.valid());

    QCOMPARE(schema.name(), mSchemaName);
    QCOMPARE(schema->name(), mSchemaName);
}

void
DbTableTest::setSchemaTest()
{
    DbSchemaPtr schema = mTable->schema();
    QVERIFY(schema.valid());

    QCOMPARE(schema.name(), mSchemaName);
    QCOMPARE(schema->name(), mSchemaName);

    DbSchemaPtr newSchema;

    mTable->setSchema(newSchema);
    QVERIFY(schema.name() != mTable->schema().name());
    QVERIFY(schema->name() != mTable->schema().name());
}

void
DbTableTest::parentTablesTest()
{
    QVERIFY(0 != mTable.get());

    QVector<DbTablePtr> parentsList;
    quint32 parentsCount = mTable->parentTables(parentsList);

    QCOMPARE(parentsCount, (quint32)1);

    DbTablePtr parent = parentsList.at(0);

    QVERIFY(0 != parent.get());

    QVERIFY("vtunes" == parent.schemaName());
    QVERIFY("vtunes" == parent->schema()->name());
    QVERIFY("playlists" == parent.name());
    QVERIFY("playlists" == parent->name());
}


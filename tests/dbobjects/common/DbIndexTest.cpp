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
#include <dbobjects/common/DbIndexTest.h>

#include <dbobjects/common/DatabaseCreator.h>
#include <dbobjects/common/DbIndex.h>

#include <limits>

using namespace DbObjects;

void
DbIndexTest::initTestCase()
{
    mSchemaName = "vtunes";
    mIndexName = "ind_artists";
    mTableName = "artists";
    mDbInst = DatabaseCreator::createDatabase();
    mFactories = DatabaseCreator::factories();
    mTools = DatabaseCreator::tools();
}

void
DbIndexTest::cleanupTestCase()
{
//    Control::DatabaseManager dbMgr;
//    dbMgr.flush();
}

/*!
 * Called before each test function
 */
void
DbIndexTest::init()
{
    mDbInst->loadData(mFactories, mTools);

    mIndex = mDbInst->findIndex(mIndexName);
    QVERIFY(0 != mIndex.get());

    QCOMPARE(mIndex.name(), mIndexName);
    QCOMPARE(mIndex->name(), mIndexName);

    QVERIFY(mDbInst);
    QVERIFY(mFactories);
    QVERIFY(mTools);
}

/*!
 * Called after each test function
 */
void
DbIndexTest::cleanup()
{
    mIndex = DbIndexPtr();
    mDbInst->resetData();
}

void
DbIndexTest::addColumnNumberTest()
{
    QVERIFY(0);

    // here we can operate on index directly
    Common::DbIndex *index = const_cast<Common::DbIndex*>(mIndex.get());

    QVERIFY(0 != index);

    // we need empty index
    index->resetData();

    short min = std::numeric_limits<short>::min();
    short max = std::numeric_limits<short>::max();

    const short limitsCount = 3;
    short upperLimit[limitsCount] = { min, 0, max };

    for (short i = 0; i < limitsCount; ++i) {

        qDebug() << "DbIndexTest::addColumnNumberTest> using upper limit of " << upperLimit[i];

        for (short j = min; j <= upperLimit[i]; ++j) {
            index->addColumnNumber(j);
            // check vector
            QVERIFY(index->columnsNumbers().contains(j));
        }

        // expected size should be centered
        QCOMPARE(index->columnsNumbers().size(), (qint32)(upperLimit[i] - min + 1));

        index->resetData();
    }
}

void
DbIndexTest::checksXMinTest()
{
    QCOMPARE(mIndex->checksXMin(), false);
}

void
DbIndexTest::columnsCountTest()
{
    QCOMPARE(mIndex->columnsCount(), (quint16)1);
}

void
DbIndexTest::columnsNumbersTest()
{
    QCOMPARE(mIndex->columnsNumbers().size(), 1);
    QCOMPARE(mIndex->columnsNumbers().at(0), (qint16)2);
}

void
DbIndexTest::isClusteredTest()
{
    QCOMPARE(mIndex->isClustered(), false);
}

void
DbIndexTest::isPrimaryTest()
{
    QCOMPARE(mIndex->isPrimary(), false);
}

void
DbIndexTest::isReadyTest()
{
    QCOMPARE(mIndex->isReady(), true);
}

void
DbIndexTest::isUniqueTest()
{
    QCOMPARE(mIndex->isUnique(), true);
}

void
DbIndexTest::isValidTest()
{
    QCOMPARE(mIndex->isValid(), true);
}

void
DbIndexTest::loadDataTest()
{
    QVERIFY(0);
}

void
DbIndexTest::typeTest()
{
    QCOMPARE(mIndex->type(), Common::DbObject::IndexObject);
}

void
DbIndexTest::schemaTest()
{
    DbSchemaPtr schema = mIndex->schema();
    QVERIFY(0 != schema.get());

    QCOMPARE(schema.name(), mSchemaName);
    QCOMPARE(schema->name(), mSchemaName);
}

void
DbIndexTest::setChecksXMinTest()
{
    bool oldFlag = mIndex->checksXMin();

    mIndex->setChecksXMin(!oldFlag);

    QCOMPARE(mIndex->checksXMin(), !oldFlag);
}

void
DbIndexTest::setClusteredTest()
{
    bool oldFlag = mIndex->isClustered();

    mIndex->setClustered(!oldFlag);

    QCOMPARE(mIndex->isClustered(), !oldFlag);
}

void
DbIndexTest::setColumnsCountTest()
{
    quint16 oldFlag = mIndex->columnsCount();

    oldFlag *= 2;

    mIndex->setColumnsCount(oldFlag);

    QCOMPARE(mIndex->columnsCount(), oldFlag);
}

void
DbIndexTest::setPrimaryTest()
{
    bool oldFlag = mIndex->isPrimary();

    mIndex->setPrimary(!oldFlag);

    QCOMPARE(mIndex->isPrimary(), !oldFlag);
}

void
DbIndexTest::setReadyTest()
{
    bool oldFlag = mIndex->isReady();

    mIndex->setReady(!oldFlag);

    QCOMPARE(mIndex->isReady(), !oldFlag);
}

void
DbIndexTest::setSchemaTest()
{
    DbSchemaPtr schema = mIndex->schema();
    QVERIFY(0 != schema.get());

    QCOMPARE(schema.name(), mSchemaName);
    QCOMPARE(schema->name(), mSchemaName);

    DbSchemaPtr newSchema;

    mIndex->setSchema(newSchema);
    QVERIFY(schema.name() != mIndex->schema().name());
    QVERIFY(schema->name() != mIndex->schema().name());
}

void
DbIndexTest::setTableTest()
{
    DbTablePtr table = mIndex->table();
    QVERIFY(0 != table.get());

    QCOMPARE(table.name(), mTableName);
    QCOMPARE(table->name(), mTableName);

    DbTablePtr newTable;

    mIndex->setTable(newTable);
    QVERIFY(table.name() != mIndex->table().name());
    QVERIFY(table->name() != mIndex->table().name());
}

void
DbIndexTest::setUniqueTest()
{
    bool oldFlag = mIndex->isUnique();

    mIndex->setUnique(!oldFlag);

    QCOMPARE(mIndex->isUnique(), !oldFlag);
}

void
DbIndexTest::setValidTest()
{
    bool oldFlag = mIndex->isValid();

    mIndex->setValid(!oldFlag);

    QCOMPARE(mIndex->isValid(), !oldFlag);
}

void
DbIndexTest::tableTest()
{
    DbTablePtr table = mIndex->table();

    QVERIFY(table.valid());
    QCOMPARE(table.name(), mTableName);
    QCOMPARE(table->name(), mTableName);

    // \todo How to handle the situation when we use proxy->setName(newName) ??
    // Here we face a little problem, the real object's name is changed, but the proxy
    // name remains old.
}


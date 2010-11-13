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

#include <dbobjects/common/DbIndexTest.h>

#include <dbobjects/common/DatabaseCreator.h>
#include <dbobjects/common/DbIndex.h>

#include <limits>

using namespace DbObjects;

typedef Common::DbObjectPtr<Common::DbIndex> DbIndexPtr;

void
DbIndexTest::initTestCase()
{
    mDbInst = DatabaseCreator::createDatabase();
}

void
DbIndexTest::cleanupTestCase()
{
    Common::DatabaseManager dbMgr;
    dbMgr.flush();
}

/*!
 * Called before each test function
 */
void
DbIndexTest::init()
{
}

/*!
 * Called after each test function
 */
void
DbIndexTest::cleanup()
{
    mDbInst->resetData();
}

void
DbIndexTest::addColumnNumberTest()
{
    QVERIFY(0);
    mDbInst->readIndices();

    DbIndexPtr indexPtr = mDbInst->findIndex("ind_artists");

    // here we can operate on index directly
    Common::DbIndex *index = const_cast<Common::DbIndex*>(indexPtr.get());

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
    QVERIFY(0);
}

void
DbIndexTest::columnsCountTest()
{
    QVERIFY(0);
}

void
DbIndexTest::columnsNumbersTest()
{
    QVERIFY(0);
}

void
DbIndexTest::isClusteredTest()
{
    QVERIFY(0);
}

void
DbIndexTest::isPrimaryTest()
{
    QVERIFY(0);
}

void
DbIndexTest::isReadyTest()
{
    QVERIFY(0);
}

void
DbIndexTest::isUniqueTest()
{
    QVERIFY(0);
}

void
DbIndexTest::isValidTest()
{
    QVERIFY(0);
}

void
DbIndexTest::loadDataTest()
{
    QVERIFY(0);
}

void
DbIndexTest::typeTest()
{
    QVERIFY(0);
}

void
DbIndexTest::schemaNameTest()
{
    QVERIFY(0);
}

void
DbIndexTest::schemaTest()
{
    QVERIFY(0);
}

void
DbIndexTest::setChecksXMinTest()
{
    QVERIFY(0);
}

void
DbIndexTest::setClusteredTest()
{
    QVERIFY(0);
}

void
DbIndexTest::setColumnsCountTest()
{
    QVERIFY(0);
}

void
DbIndexTest::setPrimaryTest()
{
    QVERIFY(0);
}

void
DbIndexTest::setReadyTest()
{
    QVERIFY(0);
}

void
DbIndexTest::setSchemaNameTest()
{
    QVERIFY(0);
}

void
DbIndexTest::setSchemaTest()
{
    QVERIFY(0);
}

void
DbIndexTest::setTableNameTest()
{
    QVERIFY(0);
}

void
DbIndexTest::setTableTest()
{
    QVERIFY(0);
}

void
DbIndexTest::setUniqueTest()
{
    QVERIFY(0);
}

void
DbIndexTest::setValidTest()
{
    QVERIFY(0);
}

void
DbIndexTest::tableNameTest()
{
    mDbInst->readIndices();

    DbIndexPtr indexPtr = mDbInst->findIndex("ind_artists");

    QVERIFY(indexPtr.name() == indexPtr->name());

    QString newName("dummy");
    indexPtr->setName(newName);
    QVERIFY(newName == indexPtr->name());
}

void
DbIndexTest::tableTest()
{
    QVERIFY(0);
}


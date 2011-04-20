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

#include <control/Context.h>
#include <control/DatabaseManager.h>
#include <control/DatabaseManagerTest.h>
#include <dbobjects/common/Database.h>

using namespace Control;
using namespace DbObjects::Common;

void
DatabaseManagerTest::initTestCase()
{

}

void
DatabaseManagerTest::cleanupTestCase()
{

}

void
DatabaseManagerTest::init()
{
    mDbMgr = new Control::DatabaseManager();

    Q_CHECK_PTR(mDbMgr);
}

void
DatabaseManagerTest::cleanup()
{
    delete mDbMgr;
    mDbMgr = 0;
}


void
DatabaseManagerTest::indicesTest()
{
QVERIFY(0);
}

void
DatabaseManagerTest::establishConnectionTest()
{
QVERIFY(0);
}

void
DatabaseManagerTest::reloadDataTest()
{
QVERIFY(0);
}

void
DatabaseManagerTest::addTest()
{
    int count = 0;
    QCOMPARE(mDbMgr->mRegistry.size(), count);

    Context *ctx1 = new Context();
    Context *ctx2 = new Context();
    Database *db1 = new Database(QSqlDatabase());
    db1->setSqlDriver("QPSQL");
    Database *db2 = new Database(QSqlDatabase());
    db2->setSqlDriver("QPSQL");

//    QCOMPARE(mDbMgr->add(ctx1, 0), false);
//    QCOMPARE(mDbMgr->mRegistry.size(), 0);

//    QCOMPARE(mDbMgr->add(0, db1), false);
//    QCOMPARE(mDbMgr->mRegistry.size(), 0);

    QCOMPARE(mDbMgr->add(ctx1, db1), true);
    QCOMPARE(mDbMgr->mRegistry.size(), ++count);

    QCOMPARE(mDbMgr->add(ctx2, db2), true);
    QCOMPARE(mDbMgr->mRegistry.size(), ++count);

//    QCOMPARE(mDbMgr->add(ctx1, db2), false);
//    QCOMPARE(mDbMgr->mRegistry.size(), count);
}

void
DatabaseManagerTest::removeContextTest()
{
    int count = 0;
    QCOMPARE(mDbMgr->mRegistry.size(), count);

    Context *ctx1 = new Context();
    Context *ctx2 = new Context();
    Database *db1 = new Database(QSqlDatabase());
    db1->setSqlDriver("QPSQL");
    Database *db2 = new Database(QSqlDatabase());
    db2->setSqlDriver("QPSQL");

    QCOMPARE(mDbMgr->add(ctx1, db1), true);
    QCOMPARE(mDbMgr->mRegistry.size(), ++count);

    QCOMPARE(mDbMgr->add(ctx2, db2), true);
    QCOMPARE(mDbMgr->mRegistry.size(), ++count);

    QCOMPARE(mDbMgr->remove(ctx1), true);
    QCOMPARE(mDbMgr->mRegistry.size(), --count);

    QCOMPARE(mDbMgr->remove(ctx1), false);
    QCOMPARE(mDbMgr->mRegistry.size(), count);

    QCOMPARE(mDbMgr->remove(ctx2), true);
    QCOMPARE(mDbMgr->mRegistry.size(), --count);
}

void
DatabaseManagerTest::removeDatabaseTest()
{
QVERIFY(0);
}

void
DatabaseManagerTest::findContextTest()
{
QVERIFY(0);
}

void
DatabaseManagerTest::findDatabaseTest()
{
QVERIFY(0);
}

void
DatabaseManagerTest::lastErrorTest()
{
QVERIFY(0);
}

void
DatabaseManagerTest::newObjectsTest()
{
QVERIFY(0);
}


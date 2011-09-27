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
#include <dbobjects/common/DbTriggerTest.h>
#include <dbobjects/common/Factories.h>
#include <dbobjects/common/Tools.h>

using namespace DbObjects;

void
DbTriggerTest::initTestCase()
{
    mSchemaName = "vtunes";
    mTriggerName = "albums_biu";
    mProcedureName = "check_release_date";
    mTableName = "albums";
    // \note albums_biu is not a constraint trigger => constr name is empty and ther is no
    // referenced table.
    mConstraintName = "";
    mRefTableName = "";

    mDbInst = DatabaseCreator::createDatabase();
    QVERIFY(0 != mDbInst);

    mFactories = DatabaseCreator::factories();
    QVERIFY(0 != mFactories);

    mTools = DatabaseCreator::tools();
    QVERIFY(0 != mTools);
}

void
DbTriggerTest::cleanupTestCase()
{
    delete mDbInst;
    delete mFactories;
    delete mTools;
}

void
DbTriggerTest::init()
{
    mDbInst->loadData(mFactories, mTools);

    DbSchemaPtr schema = mDbInst->findSchema(mSchemaName);
    QVERIFY(schema.valid());

    mTrigger = schema->findTrigger(mTriggerName);
    QVERIFY(mTrigger.valid());

    QCOMPARE(mTrigger.name(), mTriggerName);
    QCOMPARE(mTrigger->name(), mTriggerName);
}

void
DbTriggerTest::cleanup()
{
    mTrigger = DbTriggerPtr();
    mDbInst->resetData();
}

void
DbTriggerTest::enabledTest()
{
    QCOMPARE(mTrigger->enabled(), QChar('O'));
}

void
DbTriggerTest::isConstraintTest()
{
    QCOMPARE(mTrigger->isConstraint(), false);
}

void
DbTriggerTest::isDeferrableTest()
{
    QCOMPARE(mTrigger->isDeferrable(), false);
}

void
DbTriggerTest::isInitiallyDeferredTest()
{
    QCOMPARE(mTrigger->isInitiallyDeferred(), false);
}

void
DbTriggerTest::loadDataTest()
{
    QVERIFY(0);
}

void
DbTriggerTest::numArgsTest()
{
    QCOMPARE(mTrigger->numArgs(), (quint16)0);
}

void
DbTriggerTest::typeTest()
{
    QCOMPARE(mTrigger->type(), TriggerObject);
}

void
DbTriggerTest::procedureTest()
{
    DbProcedurePtr procedure = mTrigger->procedure();
    QVERIFY(procedure.valid());

    QCOMPARE(procedure.name(), mProcedureName);
    QCOMPARE(procedure->name(), mProcedureName);
}

void
DbTriggerTest::constraintNameTest()
{
    QCOMPARE(mTrigger->constraintName(), mConstraintName);
}

void
DbTriggerTest::referencedTableTest()
{
    DbTablePtr table = mTrigger->referencedTable();
    QVERIFY(!table.valid()); // \note this trigger doesn't reference any table

//    QCOMPARE(table.name(), mRefTableName);
//    QCOMPARE(table->name(), mRefTableName);
}

void
DbTriggerTest::schemaTest()
{
    DbSchemaPtr schema = mTrigger->schema();
    QVERIFY(schema.valid());

    QCOMPARE(schema.name(), mSchemaName);
    QCOMPARE(schema->name(), mSchemaName);
}

void
DbTriggerTest::setConstraintNameTest()
{
    QString name = mTrigger->constraintName();
    QString newName = name + "_" + name;
    mTrigger->setConstraintName(newName);
    QCOMPARE(mTrigger->constraintName(), newName);
}

void
DbTriggerTest::setConstraintTest()
{
    bool oldFlag = mTrigger->isConstraint();
    mTrigger->setConstraint(!oldFlag);
    QCOMPARE(mTrigger->isConstraint(), !oldFlag);
}

void
DbTriggerTest::setDeferrableTest()
{
    bool oldFlag = mTrigger->isDeferrable();
    mTrigger->setDeferrable(!oldFlag);
    QCOMPARE(mTrigger->isDeferrable(), !oldFlag);
}

void
DbTriggerTest::setEnabledTest()
{
    QChar newEnabled(0xa0);
    mTrigger->setEnabled(newEnabled);
    QCOMPARE(mTrigger->enabled(), newEnabled);
}

void
DbTriggerTest::setInitiallyDeferredTest()
{
    bool oldFlag = mTrigger->isInitiallyDeferred();
    mTrigger->setInitiallyDeferred(!oldFlag);
    QCOMPARE(mTrigger->isInitiallyDeferred(), !oldFlag);
}

void
DbTriggerTest::setNumArgsTest()
{
    quint16 numArgs = mTrigger->numArgs();
    quint16 newNumArgs = 2 * (numArgs + 1);
    mTrigger->setNumArgs(newNumArgs);
    QCOMPARE(mTrigger->numArgs(), newNumArgs);
}

void
DbTriggerTest::setProcedureTest()
{
    DbProcedurePtr procedure = mTrigger->procedure();
    QVERIFY(procedure.valid());

    QCOMPARE(procedure.name(), mProcedureName);
    QCOMPARE(procedure->name(), mProcedureName);

    DbProcedurePtr newProcedure;

    mTrigger->setProcedure(newProcedure);
    QVERIFY(procedure.name() != mTrigger->procedure().name());
    QVERIFY(procedure->name() != mTrigger->procedure().name());
}

void
DbTriggerTest::setReferencedTableTest()
{
    DbTablePtr table = mTrigger->referencedTable();
    QVERIFY(!table.valid());

    QCOMPARE(table.name(), mRefTableName);
    //\ note skip this check, because the trigger doesn't reference any table, so the
    // proxy is not valid to 
//    QCOMPARE(table->name(), mRefTableName);

    // just for test set table as referrenced table
    DbSchemaPtr schema = mDbInst->findSchema(mSchemaName);
    QVERIFY(schema.valid());

    DbTablePtr newTable = schema->findTable(mTableName);
    QVERIFY(newTable.valid());

    mTrigger->setReferencedTable(newTable);
    QVERIFY(table.name() != mTrigger->table().name());
//    QVERIFY(table->name() != mTrigger->table().name());
}

void
DbTriggerTest::setSchemaTest()
{
    DbSchemaPtr schema = mTrigger->schema();
    QVERIFY(schema.valid());

    QCOMPARE(schema.name(), mSchemaName);
    QCOMPARE(schema->name(), mSchemaName);

    DbSchemaPtr newSchema;

    mTrigger->setSchema(newSchema);
    QVERIFY(schema.name() != mTrigger->schema().name());
    QVERIFY(schema->name() != mTrigger->schema().name());
}

void
DbTriggerTest::setTableTest()
{
    DbTablePtr table = mTrigger->table();
    QVERIFY(table.valid());

    QCOMPARE(table.name(), mTableName);
    QCOMPARE(table->name(), mTableName);

    DbTablePtr newTable;

    mTrigger->setTable(newTable);
    QVERIFY(table.name() != mTrigger->table().name());
    QVERIFY(table->name() != mTrigger->table().name());
}

void
DbTriggerTest::tableTest()
{
    DbTablePtr table = mTrigger->table();
    QVERIFY(table.valid());

    QCOMPARE(table.name(), mTableName);
    QCOMPARE(table->name(), mTableName);
}


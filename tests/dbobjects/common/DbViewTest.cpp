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
#include <dbobjects/common/DbViewTest.h>

using namespace DbObjects;

void
DbViewTest::initTestCase()
{
    mSchemaName = "vtunes";
    mViewName = "users_playlists";
    mRoleName = "music_user";
    mDbInst = DatabaseCreator::createDatabase();
    QVERIFY(0 != mDbInst);
}

void
DbViewTest::cleanupTestCase()
{
    Common::DatabaseManager dbMgr;
    dbMgr.flush();
}

/*!
 * Called before each test function
 */
void
DbViewTest::init()
{
    mDbInst->loadData();

    DbSchemaPtr schema = mDbInst->findSchema(mSchemaName);
    QVERIFY(schema.valid());

    mView = schema->findView(mViewName);
    QVERIFY(mView.valid());

    QCOMPARE(mView.name(), mViewName);
    QCOMPARE(mView->name(), mViewName);
}

/*!
 * Called after each test function
 */
void
DbViewTest::cleanup()
{
    mView = DbViewPtr();
    mDbInst->resetData();
}

void
DbViewTest::definitionTest()
{
    QString def = mView->definition();
    QVERIFY(0 < def.length());

    int selectPos = def.indexOf("SELECT");
    int fromPos = def.indexOf("FROM");
    int wherePos = def.indexOf("WHERE");

    QVERIFY(0 <= selectPos);
    QVERIFY(selectPos <= fromPos);
    QVERIFY(fromPos <= wherePos);
}

void
DbViewTest::fullNameTest()
{
    QCOMPARE(mView->fullName(), QString("%1.%2").arg(mSchemaName).arg(mViewName));
}

void
DbViewTest::loadDataTest()
{
    QVERIFY(0);
}

void
DbViewTest::typeTest()
{
    QCOMPARE(mView->type(), Common::DbObject::ViewObject);
}

void
DbViewTest::ownerTest()
{
    DbRolePtr role = mView->owner();
    QVERIFY(role.valid());

    QCOMPARE(role.name(), mRoleName);
    QCOMPARE(role->name(), mRoleName);
}

void
DbViewTest::schemaTest()
{
    DbSchemaPtr schema = mView->schema();
    QVERIFY(schema.valid());

    QCOMPARE(schema.name(), mSchemaName);
    QCOMPARE(schema->name(), mSchemaName);
}

void
DbViewTest::setDefinitionTest()
{
    QString def = mView->definition();

    QString newDef = def;
    newDef.replace(" ", "_");

    mView->setDefinition(newDef);

    QCOMPARE(mView->definition(), newDef);
}

void
DbViewTest::setOwnerTest()
{
    DbRolePtr role = mView->owner();
    QVERIFY(role.valid());

    QCOMPARE(role.name(), mRoleName);
    QCOMPARE(role->name(), mRoleName);

    DbRolePtr newOwner;

    mView->setOwner(newOwner);
    QVERIFY(role.name() != mView->owner().name());
    QVERIFY(role->name() != mView->owner().name());
}

void
DbViewTest::setSchemaTest()
{
    DbSchemaPtr schema = mView->schema();
    QVERIFY(schema.valid());

    QCOMPARE(schema.name(), mSchemaName);
    QCOMPARE(schema->name(), mSchemaName);

    DbSchemaPtr newSchema;

    mView->setSchema(newSchema);
    QVERIFY(schema.name() != mView->schema().name());
    QVERIFY(schema->name() != mView->schema().name());
}

void
DbViewTest::columnsNamesTest()
{
    QStringList list;
    list = mView->columnsNames();

    QCOMPARE(list.size(), 2);
    QVERIFY(list.contains("name"));
    QVERIFY(list.contains("track"));
}


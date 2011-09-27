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
#include <dbobjects/common/DbIndex.h>
#include <dbobjects/common/DbLanguage.h>
#include <dbobjects/common/DbObject.h>
#include <dbobjects/common/DbObjectTest.h>

using namespace DbObjects::Common;

//typedef DbObjectPtr<DbIndex> DbIndexPtr;
//typedef DbObjectPtr<DbLanguage> DbLanguagePtr;
//typedef DbObjectPtr<DbRole> DbRolePtr;
//typedef DbObjectPtr<DbProcedure> DbProcedurePtr;
//typedef DbObjectPtr<DbSchema> DbSchemaPtr;
//typedef DbObjectPtr<DbTable> DbTablePtr;
//typedef DbObjectPtr<DbTrigger> DbTriggerPtr;
//typedef DbObjectPtr<DbView> DbViewPtr;

void
DbObjectTest::initTestCase()
{
    mDbInst = DatabaseCreator::createDatabase();
    QVERIFY(0 != mDbInst);

    mFactories = DatabaseCreator::factories();
    QVERIFY(0 != mFactories);

    mTools = DatabaseCreator::tools();
    QVERIFY(0 != mTools);
}

void
DbObjectTest::cleanupTestCase()
{
//    DatabaseManager dbMgr;
//    dbMgr.flush();
}


void
DbObjectTest::nameTest()
{
    DbObject *obj;
    QString objName("dummy");
    QString nsName("ns");

    DbIndexPtr index(mDbInst, mFactories, objName);
    QVERIFY(0 != (obj = const_cast<DbIndex*>(index.get())));
    QCOMPARE(obj->name(), objName);

    DbLanguagePtr language(mDbInst, mFactories, objName);
    QVERIFY(0 != (obj = const_cast<DbLanguage*>(language.get())));
    QCOMPARE(obj->name(), objName);

    DbProcedurePtr procedure(mDbInst, mFactories, objName, nsName);
    QVERIFY(0 != (obj = const_cast<DbProcedure*>(procedure.get())));
    QCOMPARE(obj->name(), objName);

    DbRolePtr role(mDbInst, mFactories, objName);
    QVERIFY(0 != (obj = const_cast<DbRole*>(role.get())));
    QCOMPARE(obj->name(), objName);

    DbSchemaPtr schema(mDbInst, mFactories, objName);
    QVERIFY(0 != (obj = const_cast<DbSchema*>(schema.get())));
    QCOMPARE(obj->name(), objName);

    DbTablePtr table(mDbInst, mFactories, objName, nsName);
    QVERIFY(0 != (obj = const_cast<DbTable*>(table.get())));
    QCOMPARE(obj->name(), objName);

    DbTriggerPtr trigger(mDbInst, mFactories, objName, nsName);
    QVERIFY(0 != (obj = const_cast<DbTrigger*>(trigger.get())));
    QCOMPARE(obj->name(), objName);

    DbViewPtr view(mDbInst, mFactories, objName, nsName);
    QVERIFY(0 != (obj = const_cast<DbView*>(view.get())));
    QCOMPARE(obj->name(), objName);
}

void
DbObjectTest::typeTest()
{
    DbObject *obj;

    DbIndexPtr index(mDbInst, mFactories, "index");
    QVERIFY(0 != (obj = const_cast<DbIndex*>(index.get())));
    QCOMPARE(obj->type(), IndexObject);

    DbLanguagePtr language(mDbInst, mFactories, "language");
    QVERIFY(0 != (obj = const_cast<DbLanguage*>(language.get())));
    QCOMPARE(obj->type(), LanguageObject);

    DbProcedurePtr procedure(mDbInst, mFactories, "procedure", "ns");
    QVERIFY(0 != (obj = const_cast<DbProcedure*>(procedure.get())));
    QCOMPARE(obj->type(), ProcedureObject);

    DbRolePtr role(mDbInst, mFactories, "role");
    QVERIFY(0 != (obj = const_cast<DbRole*>(role.get())));
    QCOMPARE(obj->type(), RoleObject);

    DbSchemaPtr schema(mDbInst, mFactories, "schema");
    QVERIFY(0 != (obj = const_cast<DbSchema*>(schema.get())));
    QCOMPARE(obj->type(), SchemaObject);

    DbTablePtr table(mDbInst, mFactories, "table", "ns");
    QVERIFY(0 != (obj = const_cast<DbTable*>(table.get())));
    QCOMPARE(obj->type(), TableObject);

    DbTriggerPtr trigger(mDbInst, mFactories, "trigger", "ns");
    QVERIFY(0 != (obj = const_cast<DbTrigger*>(trigger.get())));
    QCOMPARE(obj->type(), TriggerObject);

    DbViewPtr view(mDbInst, mFactories, "view", "ns");
    QVERIFY(0 != (obj = const_cast<DbView*>(view.get())));
    QCOMPARE(obj->type(), ViewObject);
}

void
DbObjectTest::setNameTest()
{
    DbObject *obj;
    QString objName("dummy");
    QString newObjName("dummy~");
    QString nsName("ns");

    DbIndexPtr index(mDbInst, mFactories, objName);
    QVERIFY(0 != (obj = const_cast<DbIndex*>(index.get())));
    QCOMPARE(obj->name(), objName);
    // change name
    obj->setName(newObjName);
    QCOMPARE(obj->name(), newObjName);

    DbLanguagePtr language(mDbInst, mFactories, objName);
    QVERIFY(0 != (obj = const_cast<DbLanguage*>(language.get())));
    QCOMPARE(obj->name(), objName);
    obj->setName(newObjName);
    QCOMPARE(obj->name(), newObjName);

    DbProcedurePtr procedure(mDbInst, mFactories, objName, nsName);
    QVERIFY(0 != (obj = const_cast<DbProcedure*>(procedure.get())));
    QCOMPARE(obj->name(), objName);
    obj->setName(newObjName);
    QCOMPARE(obj->name(), newObjName);

    DbRolePtr role(mDbInst, mFactories, objName);
    QVERIFY(0 != (obj = const_cast<DbRole*>(role.get())));
    QCOMPARE(obj->name(), objName);
    obj->setName(newObjName);
    QCOMPARE(obj->name(), newObjName);

    DbSchemaPtr schema(mDbInst, mFactories, objName);
    QVERIFY(0 != (obj = const_cast<DbSchema*>(schema.get())));
    QCOMPARE(obj->name(), objName);
    obj->setName(newObjName);
    QCOMPARE(obj->name(), newObjName);

    DbTablePtr table(mDbInst, mFactories, objName, nsName);
    QVERIFY(0 != (obj = const_cast<DbTable*>(table.get())));
    QCOMPARE(obj->name(), objName);
    obj->setName(newObjName);
    QCOMPARE(obj->name(), newObjName);

    DbTriggerPtr trigger(mDbInst, mFactories, objName, nsName);
    QVERIFY(0 != (obj = const_cast<DbTrigger*>(trigger.get())));
    QCOMPARE(obj->name(), objName);
    obj->setName(newObjName);
    QCOMPARE(obj->name(), newObjName);

    DbViewPtr view(mDbInst, mFactories, objName, nsName);
    QVERIFY(0 != (obj = const_cast<DbView*>(view.get())));
    QCOMPARE(obj->name(), objName);
    obj->setName(newObjName);
    QCOMPARE(obj->name(), newObjName);
}


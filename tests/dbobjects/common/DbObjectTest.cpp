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
    Database *dbInst = DatabaseCreator::createDatabase();
}

void
DbObjectTest::cleanupTestCase()
{

}


void
DbObjectTest::nameTest()
{
    DbObject *obj;
    QString objName("dummy");
    QString nsName("ns");

    DbIndexPtr index(objName);
    QVERIFY(0 != (obj = const_cast<DbIndex*>(index.get())));
    QCOMPARE(obj->name(), objName);

    DbLanguagePtr language(objName);
    QVERIFY(0 != (obj = const_cast<DbLanguage*>(language.get())));
    QCOMPARE(obj->name(), objName);

    DbProcedurePtr procedure(objName, nsName);
    QVERIFY(0 != (obj = const_cast<DbProcedure*>(procedure.get())));
    QCOMPARE(obj->name(), objName);

    DbRolePtr role(objName);
    QVERIFY(0 != (obj = const_cast<DbRole*>(role.get())));
    QCOMPARE(obj->name(), objName);

    DbSchemaPtr schema(objName);
    QVERIFY(0 != (obj = const_cast<DbSchema*>(schema.get())));
    QCOMPARE(obj->name(), objName);

    DbTablePtr table(objName, nsName);
    QVERIFY(0 != (obj = const_cast<DbTable*>(table.get())));
    QCOMPARE(obj->name(), objName);

    DbTriggerPtr trigger(objName, nsName);
    QVERIFY(0 != (obj = const_cast<DbTrigger*>(trigger.get())));
    QCOMPARE(obj->name(), objName);

    DbViewPtr view(objName, nsName);
    QVERIFY(0 != (obj = const_cast<DbView*>(view.get())));
    QCOMPARE(obj->name(), objName);
}

void
DbObjectTest::typeTest()
{
    DbObject *obj;

    DbIndexPtr index("index");
    QVERIFY(0 != (obj = const_cast<DbIndex*>(index.get())));
    QCOMPARE(obj->type(), DbObjects::Common::DbObject::IndexObject);

    DbLanguagePtr language("language");
    QVERIFY(0 != (obj = const_cast<DbLanguage*>(language.get())));
    QCOMPARE(obj->type(), DbObjects::Common::DbObject::LanguageObject);

    DbProcedurePtr procedure("procedure", "ns");
    QVERIFY(0 != (obj = const_cast<DbProcedure*>(procedure.get())));
    QCOMPARE(obj->type(), DbObjects::Common::DbObject::ProcedureObject);

    DbRolePtr role("role");
    QVERIFY(0 != (obj = const_cast<DbRole*>(role.get())));
    QCOMPARE(obj->type(), DbObjects::Common::DbObject::RoleObject);

    DbSchemaPtr schema("schema");
    QVERIFY(0 != (obj = const_cast<DbSchema*>(schema.get())));
    QCOMPARE(obj->type(), DbObjects::Common::DbObject::SchemaObject);

    DbTablePtr table("table", "ns");
    QVERIFY(0 != (obj = const_cast<DbTable*>(table.get())));
    QCOMPARE(obj->type(), DbObjects::Common::DbObject::TableObject);

    DbTriggerPtr trigger("trigger", "ns");
    QVERIFY(0 != (obj = const_cast<DbTrigger*>(trigger.get())));
    QCOMPARE(obj->type(), DbObjects::Common::DbObject::TriggerObject);

    DbViewPtr view("view", "ns");
    QVERIFY(0 != (obj = const_cast<DbView*>(view.get())));
    QCOMPARE(obj->type(), DbObjects::Common::DbObject::ViewObject);
}

void
DbObjectTest::setNameTest()
{
    DbObject *obj;
    QString objName("dummy");
    QString newObjName("dummy~");
    QString nsName("ns");

    DbIndexPtr index(objName);
    QVERIFY(0 != (obj = const_cast<DbIndex*>(index.get())));
    QCOMPARE(obj->name(), objName);
    // change name
    obj->setName(newObjName);
    QCOMPARE(obj->name(), newObjName);

    DbLanguagePtr language(objName);
    QVERIFY(0 != (obj = const_cast<DbLanguage*>(language.get())));
    QCOMPARE(obj->name(), objName);
    obj->setName(newObjName);
    QCOMPARE(obj->name(), newObjName);

    DbProcedurePtr procedure(objName, nsName);
    QVERIFY(0 != (obj = const_cast<DbProcedure*>(procedure.get())));
    QCOMPARE(obj->name(), objName);
    obj->setName(newObjName);
    QCOMPARE(obj->name(), newObjName);

    DbRolePtr role(objName);
    QVERIFY(0 != (obj = const_cast<DbRole*>(role.get())));
    QCOMPARE(obj->name(), objName);
    obj->setName(newObjName);
    QCOMPARE(obj->name(), newObjName);

    DbSchemaPtr schema(objName);
    QVERIFY(0 != (obj = const_cast<DbSchema*>(schema.get())));
    QCOMPARE(obj->name(), objName);
    obj->setName(newObjName);
    QCOMPARE(obj->name(), newObjName);

    DbTablePtr table(objName, nsName);
    QVERIFY(0 != (obj = const_cast<DbTable*>(table.get())));
    QCOMPARE(obj->name(), objName);
    obj->setName(newObjName);
    QCOMPARE(obj->name(), newObjName);

    DbTriggerPtr trigger(objName, nsName);
    QVERIFY(0 != (obj = const_cast<DbTrigger*>(trigger.get())));
    QCOMPARE(obj->name(), objName);
    obj->setName(newObjName);
    QCOMPARE(obj->name(), newObjName);

    DbViewPtr view(objName, nsName);
    QVERIFY(0 != (obj = const_cast<DbView*>(view.get())));
    QCOMPARE(obj->name(), objName);
    obj->setName(newObjName);
    QCOMPARE(obj->name(), newObjName);
}


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
#include <dbobjects/common/DbProcedureTest.h>
#include <dbobjects/common/Factories.h>
#include <dbobjects/common/Tools.h>

using namespace DbObjects;

void
DbProcedureTest::initTestCase()
{
    mSchemaName = "vtunes";
    mProcedureName = "insert_album";
    mRoleName = "music_user";
    mLanguageName = "plpgsql";

    mDbInst = DatabaseCreator::createDatabase();
    QVERIFY(0 != mDbInst);

    mFactories = DatabaseCreator::factories();
    QVERIFY(0 != mFactories);

    mTools = DatabaseCreator::tools();
    QVERIFY(0 != mTools);
}

void
DbProcedureTest::cleanupTestCase()
{
    delete mDbInst;
    delete mFactories;
    delete mTools;
}

void
DbProcedureTest::init()
{
    mDbInst->loadData(mFactories, mTools);

    mProcedure = mDbInst->findSchema(mSchemaName)->findProcedure(mProcedureName);
    QVERIFY(0 != mProcedure.get());

    QCOMPARE(mProcedure.name(), mProcedureName);
    QCOMPARE(mProcedure->name(), mProcedureName);
}

void
DbProcedureTest::cleanup()
{
    mProcedure = DbProcedurePtr();
    mDbInst->resetData();
}

void
DbProcedureTest::fullNameTest()
{
    QCOMPARE(mProcedure->fullName(), QString("%1.%2").arg(mSchemaName).arg(mProcedureName));
}

void
DbProcedureTest::languageTest()
{
    DbLanguagePtr language = mProcedure->language();
    QVERIFY(language.valid());

    QCOMPARE(language.name(), mLanguageName);
    QCOMPARE(language->name(), mLanguageName);
}

void
DbProcedureTest::loadDataTest()
{
    QVERIFY(0);
}

void
DbProcedureTest::typeTest()
{
    QCOMPARE(mProcedure->type(), ProcedureObject);
}

void
DbProcedureTest::ownerTest()
{
    mDbInst->readRoles(mFactories, mTools);

    DbRolePtr role = mProcedure->owner();
    QVERIFY(role.valid());

    QCOMPARE(role.name(), mRoleName);
    QCOMPARE(role->name(), mRoleName);
}

void
DbProcedureTest::schemaTest()
{
    DbSchemaPtr schema = mProcedure->schema();
    QVERIFY(schema.valid());

    QCOMPARE(schema.name(), mSchemaName);
    QCOMPARE(schema->name(), mSchemaName);
}

void
DbProcedureTest::setLanguageTest()
{
    DbLanguagePtr language = mProcedure->language();
    QVERIFY(language.valid());

    QCOMPARE(language.name(), mLanguageName);
    QCOMPARE(language->name(), mLanguageName);

    DbLanguagePtr newLanguage;

    mProcedure->setLanguage(newLanguage);
    QVERIFY(language.name() != mProcedure->language().name());
//    QVERIFY(language->name() != mProcedure->language()->name());
}

void
DbProcedureTest::setOwnerTest()
{
    DbRolePtr role = mProcedure->owner();
    QVERIFY(role.valid());

    QCOMPARE(role.name(), mRoleName);
    QCOMPARE(role->name(), mRoleName);

    DbRolePtr newRole;

    mProcedure->setOwner(newRole);
    QVERIFY(role.name() != mProcedure->owner().name());
//    QVERIFY(role->name() != mProcedure->owner()->name());
}

void
DbProcedureTest::setSchemaTest()
{
    DbSchemaPtr schema = mProcedure->schema();
    QVERIFY(schema.valid());

    QCOMPARE(schema.name(), mSchemaName);
    QCOMPARE(schema->name(), mSchemaName);

    DbSchemaPtr newSchema;

    mProcedure->setSchema(newSchema);
    QVERIFY(schema.name() != mProcedure->schema().name());
//    QVERIFY(schema->name() != mProcedure->schema().name());
}

void
DbProcedureTest::setSourceCodeTest()
{
    QString sourceCode = mProcedure->sourceCode();

    QString newSourceCode = sourceCode;
    newSourceCode.replace(" ", "_");

    mProcedure->setSourceCode(newSourceCode);

    QCOMPARE(mProcedure->sourceCode(), newSourceCode);
}

void
DbProcedureTest::sourceCodeTest()
{
    QString sourceCode = mProcedure->sourceCode();
    QVERIFY(0 < sourceCode.length());
    QVERIFY(0 < sourceCode.indexOf("CREATE"));
    QVERIFY(0 < sourceCode.indexOf("IF"));
    QVERIFY(0 < sourceCode.indexOf("END"));
    QVERIFY(0 < sourceCode.indexOf("SELECT"));
    QVERIFY(0 < sourceCode.indexOf("ALIAS"));
    QVERIFY(0 < sourceCode.indexOf("DECLARE"));
    QVERIFY(0 < sourceCode.indexOf("BEGIN"));
    QVERIFY(0 < sourceCode.indexOf("RETURNS"));
}


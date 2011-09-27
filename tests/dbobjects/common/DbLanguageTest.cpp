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
#include <dbobjects/common/DbLanguageTest.h>

using namespace DbObjects;

void
DbLanguageTest::initTestCase()
{
    mLanguageName = "plpgsql";
    mDbInst = DatabaseCreator::createDatabase();
    QVERIFY(0 != mDbInst);

    mFactories = DatabaseCreator::factories();
    QVERIFY(0 != mFactories);

    mTools = DatabaseCreator::tools();
    QVERIFY(0 != mTools);

}

void
DbLanguageTest::cleanupTestCase()
{
//    Common::DatabaseManager dbMgr;
//    dbMgr.flush();
}

void
DbLanguageTest::init()
{
    mDbInst->readLanguages(mFactories, mTools);

    mLanguage = mDbInst->findLanguage(mLanguageName);
    QVERIFY(0 != mLanguage.get());

    QCOMPARE(mLanguage.name(), mLanguageName);
    QCOMPARE(mLanguage->name(), mLanguageName);
}

void
DbLanguageTest::cleanup()
{
    mLanguage = DbLanguagePtr();
    mDbInst->resetData();
}

void
DbLanguageTest::isTrustedTest()
{
    QCOMPARE(mLanguage->isTrusted(), true);
}

void
DbLanguageTest::loadDataTest()
{
    QVERIFY(0);
}

void
DbLanguageTest::typeTest()
{
    QCOMPARE(mLanguage->type(), LanguageObject);
}

void
DbLanguageTest::setTrustedTest()
{
    bool oldFlag = mLanguage->isTrusted();

    mLanguage->setTrusted(!oldFlag);

    QCOMPARE(mLanguage->isTrusted(), !oldFlag);
}


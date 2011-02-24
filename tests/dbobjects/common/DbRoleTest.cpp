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
#include <dbobjects/common/DbRoleTest.h>
#include <dbobjects/common/Factories.h>
#include <dbobjects/common/Tools.h>

using namespace DbObjects;

void
DbRoleTest::initTestCase()
{
    mRoleName = "music_user";

    mDbInst = DatabaseCreator::createDatabase();
    QVERIFY(0 != mDbInst);

    mFactories = DatabaseCreator::factories();
    QVERIFY(0 != mFactories);

    mTools = DatabaseCreator::tools();
    QVERIFY(0 != mTools);
}

void
DbRoleTest::cleanupTestCase()
{
    delete mDbInst;
    delete mFactories;
    delete mTools;
}

void
DbRoleTest::init()
{
    mDbInst->readRoles(mFactories, mTools);

    mRole = mDbInst->findRole(mRoleName);
    QVERIFY(0 != mRole.get());

    QCOMPARE(mRole.name(), mRoleName);
    QCOMPARE(mRole->name(), mRoleName);
}

void
DbRoleTest::cleanup()
{
    mRole = DbRolePtr();
    mDbInst->resetData();
}

void
DbRoleTest::canCreateDbTest()
{
    QCOMPARE(mRole->canCreateDb(), true);
}

void
DbRoleTest::canCreateRoleTest()
{
    QCOMPARE(mRole->canCreateRole(), false);
}

void
DbRoleTest::canLoginTest()
{
    QCOMPARE(mRole->canLogin(), true);
}

void
DbRoleTest::canUpdateSysCatTest()
{
    QCOMPARE(mRole->canUpdateSysCat(), false);
}

void
DbRoleTest::connectionLimitTest()
{
    QCOMPARE(mRole->connectionLimit(), (quint32)-1);
}

void
DbRoleTest::expiryDateTest()
{
    QCOMPARE(mRole->expiryDate(), QDate());
}

void
DbRoleTest::idTest()
{
    QVERIFY(0 < mRole->id());
}

void
DbRoleTest::inheritsPrivilegesTest()
{
    QCOMPARE(mRole->inheritsPrivileges(), true);
}

void
DbRoleTest::isSuperUserTest()
{
    QCOMPARE(mRole->isSuperUser(), false);
}

void
DbRoleTest::loadDataTest()
{
    QVERIFY(0);
}

void
DbRoleTest::typeTest()
{
    QCOMPARE(mRole->type(), Common::DbObject::RoleObject);
}

void
DbRoleTest::setCanCreateDbTest()
{
    bool oldFlag = mRole->canCreateDb();

    QCOMPARE(oldFlag, true);

    mRole->setCanCreateDb(!oldFlag);

    QCOMPARE(mRole->canCreateDb(), !oldFlag);
}

void
DbRoleTest::setCanCreateRoleTest()
{
    bool oldFlag = mRole->canCreateRole();

    QCOMPARE(oldFlag, false);

    mRole->setCanCreateRole(!oldFlag);

    QCOMPARE(mRole->canCreateRole(), !oldFlag);
}

void
DbRoleTest::setCanLoginTest()
{
    bool oldFlag = mRole->canLogin();

    QCOMPARE(oldFlag, true);

    mRole->setCanLogin(!oldFlag);

    QCOMPARE(mRole->canLogin(), !oldFlag);
}

void
DbRoleTest::setCanUpdateSysCatTest()
{
    bool oldFlag = mRole->canUpdateSysCat();

    QCOMPARE(oldFlag, false);

    mRole->setCanUpdateSysCat(!oldFlag);

    QCOMPARE(mRole->canUpdateSysCat(), !oldFlag);
}

void
DbRoleTest::setConnectionLimitTest()
{
    quint32 oldFlag = mRole->connectionLimit();

    QCOMPARE(oldFlag, (quint32)-1);

    mRole->setConnectionLimit(oldFlag / 2);

    QCOMPARE(mRole->connectionLimit(), oldFlag / 2);
}

void
DbRoleTest::setExpiryDateTest()
{
    QDate oldFlag = mRole->expiryDate();

    QCOMPARE(oldFlag, QDate());

    mRole->setExpiryDate(oldFlag.addDays(7));

    QCOMPARE(mRole->expiryDate(), QDate().addDays(7));
}

void
DbRoleTest::setIdTest()
{
    quint64 oldFlag = mRole->id();

    QVERIFY(0 < oldFlag);

    mRole->setId(oldFlag / 2);

    QCOMPARE(mRole->id(), oldFlag / 2);
}

void
DbRoleTest::setInheritsPrivilegesTest()
{
    bool oldFlag = mRole->inheritsPrivileges();

    QCOMPARE(oldFlag, true);

    mRole->setInheritsPrivileges(!oldFlag);

    QCOMPARE(mRole->inheritsPrivileges(), !oldFlag);
}

void
DbRoleTest::setSuperUserTest()
{
    bool oldFlag = mRole->isSuperUser();

    QCOMPARE(oldFlag, false);

    mRole->setSuperUser(!oldFlag);

    QCOMPARE(mRole->isSuperUser(), !oldFlag);
}


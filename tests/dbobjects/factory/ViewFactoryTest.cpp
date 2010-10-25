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

#include <dbobjects/factory/ViewFactoryTest.h>

#include <dbobjects/common/Database.h>
#include <dbobjects/factory/View.h>
#include <dbobjects/psql/View.h>

void
ViewFactoryTest::initTestCase()
{

}

void
ViewFactoryTest::cleanupTestCase()
{

}

void
ViewFactoryTest::createViewTest()
{
    using namespace DbObjects;

    QString viewName("users_playlists");
    QString schemaName("vtunes");

    Common::Database *dbInst = Common::Database::instance();

    QVERIFY(0 != dbInst);

    dbInst->setSqlDriver("QPSQL");
    QVERIFY(0 != dynamic_cast<Psql::View*>(Factory::View::createView(schemaName, viewName)));

    dbInst->setSqlDriver("QMYSQL");
    QVERIFY(0 == Factory::View::createView(schemaName, viewName));

    dbInst->setSqlDriver("QIODBC");
    QVERIFY(0 == Factory::View::createView(schemaName, viewName));

    dbInst->setSqlDriver("QSQLITE");
    QVERIFY(0 == Factory::View::createView(schemaName, viewName));

    dbInst->setSqlDriver("NODRV");
    QVERIFY(0 == Factory::View::createView(schemaName, viewName));
}


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

#include <dbobjects/common/DbSchema.h>
#include <dbobjects/psql/PsqlView.h>
#include <dbobjects/psql/PsqlViewTest.h>

void
PsqlViewTest::initTestCase()
{
}

void
PsqlViewTest::cleanupTestCase()
{

}


void
PsqlViewTest::loadDataTest()
{
    QString schemaName = QString("vtunes");
    QString viewName = QString("users_playlists");

    Database *dbInst = Database::instance();

    // we need this for schema parent
    dbInst->readSchemas();
    dbInst->readRoles();

    DbSchema *schema = dbInst->findSchema(schemaName);

    QVERIFY(0 != schema);

    DbView *view = schema->findView(viewName);

    QVERIFY(0 != view);

    QVERIFY(QString(schemaName) == view->schema()->name());
    QVERIFY(QString(viewName) == view->name());
    QVERIFY(QString("%1.%2").arg(schemaName).arg(viewName) == view->fullName());

    QVERIFY(DbObject::ViewObject == view->type());

    QVERIFY(0 < view->definition().length());
}


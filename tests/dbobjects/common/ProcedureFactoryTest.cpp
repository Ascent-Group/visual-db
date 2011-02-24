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

#include <dbobjects/common/ProcedureFactoryTest.h>

#include <dbobjects/common/Database.h>
#include <dbobjects/common/ProcedureFactory.h>
#include <dbobjects/psql/Procedure.h>


void
ProcedureFactoryTest::initTestCase()
{

}

void
ProcedureFactoryTest::cleanupTestCase()
{

}

void
ProcedureFactoryTest::createProcedureTest()
{
    QVERIFY(0);
//    using namespace DbObjects;
//
//    QString procName("insert_album");
//    QString schemaName("vtunes");
//
//    Common::Database *dbInst = Common::Database::instance();
//    QVERIFY(0 != dbInst);
//
////    DbSchemaPtr schema = dbInst->findSchema(schemaName);
////    QVERIFY(0 != schema.get());
//
//    dbInst->setSqlDriver("QPSQL");
//    QVERIFY(0 != dynamic_cast<Psql::Procedure*>(Factory::Procedure::createProcedure(procName, schemaName)));
//
//    // \todo update this check as for psql when more RDBMS are supported
//    dbInst->setSqlDriver("QMYSQL");
//    QVERIFY(0 == Factory::Procedure::createProcedure(procName, schemaName));
//
//    dbInst->setSqlDriver("QIODBC");
//    QVERIFY(0 == Factory::Procedure::createProcedure(procName, schemaName));
//
//    dbInst->setSqlDriver("QSQLITE");
//    QVERIFY(0 == Factory::Procedure::createProcedure(procName, schemaName));
//
//    dbInst->setSqlDriver("NODRV");
//    QVERIFY(0 == Factory::Procedure::createProcedure(procName, schemaName));
}


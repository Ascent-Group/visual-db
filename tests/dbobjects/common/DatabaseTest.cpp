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

#include <dbobjects/common/Database.h>
#include <dbobjects/common/DatabaseTest.h>
#include <dbobjects/common/DbLanguage.h>
#include <dbobjects/common/DbSchema.h>

#include <QSqlDatabase>

void
DatabaseTest::initTestCase()
{
    QString driverName = QString("QPSQL");

    Database *dbInst = Database::instance();
    dbInst->setSqlDriver(driverName);

}

void
DatabaseTest::cleanupTestCase()
{

}


void
DatabaseTest::addIndexTest()
{
    QVERIFY(0);
}

void
DatabaseTest::addLanguageTest()
{
    QVERIFY(0);
}

void
DatabaseTest::addRoleTest()
{
    QVERIFY(0);
}

void
DatabaseTest::addSchemaTest()
{
    QVERIFY(0);
}

void
DatabaseTest::addTriggerTest()
{
    QVERIFY(0);
}

void
DatabaseTest::cleanupTest()
{
    QVERIFY(0);
}

void
DatabaseTest::findIndexTest()
{
    QVERIFY(0);
}

void
DatabaseTest::findLanguageTest()
{
    QVERIFY(0);
}

void
DatabaseTest::findObjectTest()
{
    QVERIFY(0);
}

void
DatabaseTest::findRoleTest()
{
    QVERIFY(0);
}

void
DatabaseTest::findSchemaTest()
{
    QVERIFY(0);
}

void
DatabaseTest::findTableIndicesTest()
{
    QVERIFY(0);
}

void
DatabaseTest::findTriggerTest()
{
    QVERIFY(0);
}

void
DatabaseTest::flushTest()
{
    QVERIFY(0);
}

void
DatabaseTest::indicesCountTest()
{
    QVERIFY(0);
}

void
DatabaseTest::indicesListTest()
{
    QVERIFY(0);
}

void
DatabaseTest::instanceTest()
{
    Database *dbInst = Database::instance();

    // the pointer should be non-zero
    QVERIFY(0 != dbInst);

    // additionaly it should be valid
    QVERIFY(Database::PostgreSQL == dbInst->sqlDriver());
}

void
DatabaseTest::languagesCountTest()
{
    QVERIFY(0);
}

void
DatabaseTest::languagesListTest()
{
    QVERIFY(0);
}

void
DatabaseTest::readIndicesTest()
{
    QVERIFY(0);
}

void
DatabaseTest::readLanguagesTest()
{
    Database *dbInst = Database::instance();

    // read langs
    dbInst->readLanguages();

    // plpgsql MUST be there
    QString name = QString("plpgsql");
    QVERIFY(name == dbInst->findLanguage(name)->name());
}

void
DatabaseTest::readRolesTest()
{
    QVERIFY(0);
}

void
DatabaseTest::readSchemasTest()
{
    Database *dbInst = Database::instance();

    // read schemas
    dbInst->readSchemas();

    // validate their names
    QString name = QString("public");
    QVERIFY(name == dbInst->findSchema(name)->name());

    name = QString("information_schema");
    QVERIFY(name == dbInst->findSchema(name)->name());

    name = QString("vtunes");
    QVERIFY(name == dbInst->findSchema(name)->name());
}

void
DatabaseTest::readTriggersTest()
{
    QVERIFY(0);
}

void
DatabaseTest::rolesCountTest()
{
    QVERIFY(0);
}

void
DatabaseTest::rolesListTest()
{
    QVERIFY(0);
}

void
DatabaseTest::schemasCountTest()
{
    Database *dbInst = Database::instance();

    dbInst->readSchemas();

    QVERIFY(3 == dbInst->schemasCount());
}

void
DatabaseTest::schemasListTest()
{
    QStringList list;

    Database::instance()->schemasList(&list);

    QVERIFY(3 == list.count());

    QVERIFY(list.contains("public"));
    QVERIFY(list.contains("information_schema"));
    QVERIFY(list.contains("vtunes"));
}

void
DatabaseTest::setSqlDriverTest()
{
    QVERIFY(0);
}

void
DatabaseTest::sqlDriverTest()
{
    QVERIFY(0);
}

void
DatabaseTest::triggersCountTest()
{
    QVERIFY(0);
}

void
DatabaseTest::triggersListTest()
{
    QVERIFY(0);
}


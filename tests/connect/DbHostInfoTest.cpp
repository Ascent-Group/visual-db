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

#include <DbHostInfoTest.h>
#include <connect/DbHostInfo.h>

void
DbHostInfoTest::initTestCase()
{
}

void
DbHostInfoTest::cleanupTestCase()
{
}

void
DbHostInfoTest::createConnectionTest()
{
	using namespace Connect;

    QVERIFY(0);
}

void
DbHostInfoTest::dbDriverTest()
{
	using namespace Connect;

    DbHostInfo dbHostInfo("dbhost", 1234, "dbuser", "dbpassword", "dbname", "dbdriver");
    QVERIFY("dbdriver" == dbHostInfo.dbDriver());
}

void
DbHostInfoTest::dbNameTest()
{
	using namespace Connect;

    DbHostInfo dbHostInfo("dbhost", 1234, "dbuser", "dbpassword", "dbname", "dbdriver");
    QVERIFY("dbname" == dbHostInfo.dbName());
}

void
DbHostInfoTest::fromXmlTest()
{
	using namespace Connect;

    QDomDocument xmlDocument;
    QDomElement xmlElement = xmlDocument.createElement("DbHostInfo");
    xmlElement.setAttribute("host", "dbhost");
    xmlElement.setAttribute("port", "1234");
    xmlElement.setAttribute("user", "dbuser");
    xmlElement.setAttribute("name", "dbname");
    xmlElement.setAttribute("driver", "dbdriver");

    DbHostInfo dbHostInfo;
    dbHostInfo.fromXml(xmlElement);
    
    QVERIFY("dbhost" == dbHostInfo.address() && 1234 == dbHostInfo.port()
            && "dbuser" == dbHostInfo.user() && "dbname" == dbHostInfo.dbName()
            && "dbdriver" == dbHostInfo.dbDriver());
}

void
DbHostInfoTest::notEqualsTest()
{
	using namespace Connect;

    DbHostInfo dbHostInfo1("dbhost", 1234, "dbuser", "dbpassword", "dbname", "dbdriver");
    DbHostInfo dbHostInfo2;

    QVERIFY(dbHostInfo1 != dbHostInfo2);

    DbHostInfo dbHostInfo3("dbhost", 1234, "dbuser", "dbpassword", "dbname", "dbdriver");
    DbHostInfo dbHostInfo4("dbhost", 1234, "dbuser", "dbpassword", "dbname", "dbdriver");

    QVERIFY(!(dbHostInfo3 != dbHostInfo4));
}

void
DbHostInfoTest::equalsTest()
{
	using namespace Connect;

    DbHostInfo dbHostInfo1("dbhost", 1234, "dbuser", "dbpassword", "dbname", "dbdriver");
    DbHostInfo dbHostInfo2;

    QVERIFY(!(dbHostInfo1 == dbHostInfo2));

    DbHostInfo dbHostInfo3("dbhost", 1234, "dbuser", "dbpassword", "dbname", "dbdriver");
    DbHostInfo dbHostInfo4("dbhost", 1234, "dbuser", "dbpassword", "dbname", "dbdriver");

    QVERIFY(dbHostInfo3 == dbHostInfo4);
}

void
DbHostInfoTest::assignmentTest()
{
	using namespace Connect;

    DbHostInfo dbHostInfo1("dbhost", 1234, "dbuser", "dbpassword", "dbname", "dbdriver");
    DbHostInfo dbHostInfo2;

    dbHostInfo2 = dbHostInfo1;

    QVERIFY(dbHostInfo2.address() == dbHostInfo1.address() && dbHostInfo2.port() == dbHostInfo1.port() &&
            dbHostInfo2.user() == dbHostInfo1.user() && dbHostInfo2.password() == dbHostInfo2.password() &&
            dbHostInfo2.dbName() == dbHostInfo1.dbName() && dbHostInfo2.dbDriver() == dbHostInfo2.dbDriver());
}

void
DbHostInfoTest::setDbDriverTest()
{
	using namespace Connect;

    DbHostInfo dbHostInfo;
    dbHostInfo.setDbDriver("dbdriver");
    QVERIFY("dbdriver" == dbHostInfo.dbDriver());
}

void
DbHostInfoTest::setDbNameTest()
{
	using namespace Connect;

    DbHostInfo dbHostInfo;
    dbHostInfo.setDbName("dbname");
    QVERIFY("dbname" == dbHostInfo.dbName());
}

void
DbHostInfoTest::toXmlTest()
{
	using namespace Connect;

    QDomDocument xmlDocument;
    QDomElement xmlElement = xmlDocument.createElement("DbHostInfo");
    
    DbHostInfo dbHostInfo("dbhost", 1234, "dbuser", "", "dbname", "dbdriver");
    dbHostInfo.toXml(xmlElement);
    
    QVERIFY("dbhost" == xmlElement.attribute("host") && "1234" == xmlElement.attribute("port")
            && "dbuser" == xmlElement.attribute("user") && "dbname" == xmlElement.attribute("name")
            && "dbdriver" == xmlElement.attribute("driver"));
}


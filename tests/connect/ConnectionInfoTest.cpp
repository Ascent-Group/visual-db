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

#include <ConnectionInfoTest.h>
#include <connect/ConnectionInfo.h>
#include <connect/DbHostInfo.h>
#include <connect/ProxyHostInfo.h>

void
ConnectionInfoTest::initTestCase()
{
}

void
ConnectionInfoTest::cleanupTestCase()
{
}

void
ConnectionInfoTest::dbHostInfoTest()
{
	using namespace Connect;

    DbHostInfo dbHostInfo("dbhost", 1234, "dbuser", "dbpassword", "dbname", "dbdriver");
    ProxyHostInfo proxyHostInfo;
    ConnectionInfo connectionInfo(dbHostInfo, true, proxyHostInfo);
    
    QVERIFY("dbhost" == connectionInfo.dbHostInfo().address() && 1234 == connectionInfo.dbHostInfo().port()
            && "dbuser" == connectionInfo.dbHostInfo().user() && "dbpassword" == connectionInfo.dbHostInfo().password()
            && "dbname" == connectionInfo.dbHostInfo().dbName() && "dbdriver" == connectionInfo.dbHostInfo().dbDriver());

}

void
ConnectionInfoTest::fromXmlTest()
{
	using namespace Connect;

    QDomDocument xmlDocument;
    QDomElement xmlDoc = xmlDocument.createElement("connection");

    QDomElement xmlDb = xmlDocument.createElement("database");
    xmlDb.setAttribute("host", "dbhost");
    xmlDb.setAttribute("port", "1234");
    xmlDb.setAttribute("user", "dbuser");
    xmlDb.setAttribute("name", "dbname");
    xmlDb.setAttribute("driver", "dbdriver");

    DbHostInfo dbHostInfo;
    dbHostInfo.fromXml(xmlDb);
    
    QDomElement xmlProxy = xmlDocument.createElement("proxy");
    xmlProxy.setAttribute("use", "1");
    xmlProxy.setAttribute("host", "proxyhost");
    xmlProxy.setAttribute("port", "1234");
    xmlProxy.setAttribute("user", "proxyuser");
    xmlProxy.setAttribute("type", "1");

    xmlDoc.appendChild(xmlDb);
    xmlDoc.appendChild(xmlProxy);

    ConnectionInfo connectionInfo;
    connectionInfo.fromXml(xmlDoc);

    QVERIFY("dbhost" == connectionInfo.dbHostInfo().address() && 1234 == connectionInfo.dbHostInfo().port()
            && "dbuser" == connectionInfo.dbHostInfo().user() && "dbname" == connectionInfo.dbHostInfo().dbName()
            && "dbdriver" == connectionInfo.dbHostInfo().dbDriver() && connectionInfo.proxyHostInfo().address() == "proxyhost" 
            && connectionInfo.proxyHostInfo().port() == 1234 && connectionInfo.proxyHostInfo().user() == "proxyuser" 
            && connectionInfo.proxyHostInfo().type() == (QNetworkProxy::ProxyType)1 && connectionInfo.useProxy() == true);
}

void
ConnectionInfoTest::notEqualsTest()
{
	using namespace Connect;

    DbHostInfo dbHostInfo1("dbhost", 1234, "dbuser", "dbpassword", "dbname", "dbdriver");
    DbHostInfo dbHostInfo2;
    ProxyHostInfo proxyHostInfo1("proxyhost", 1234, "proxyuser", "proxypassword", (QNetworkProxy::ProxyType)1);
    ProxyHostInfo proxyHostInfo2;
    ConnectionInfo connectionInfo1(dbHostInfo1, true, proxyHostInfo1);
    ConnectionInfo connectionInfo2(dbHostInfo2, false, proxyHostInfo2);

    QVERIFY(connectionInfo1 != connectionInfo2);

    DbHostInfo dbHostInfo3("dbhost", 1234, "dbuser", "dbpassword", "dbname", "dbdriver");
    DbHostInfo dbHostInfo4("dbhost", 1234, "dbuser", "dbpassword", "dbname", "dbdriver");
    ProxyHostInfo proxyHostInfo3("proxyhost", 1234, "proxyuser", "proxypassword", (QNetworkProxy::ProxyType)1);
    ProxyHostInfo proxyHostInfo4("proxyhost", 1234, "proxyuser", "proxypassword", (QNetworkProxy::ProxyType)1);
    ConnectionInfo connectionInfo3(dbHostInfo3, true, proxyHostInfo3);
    ConnectionInfo connectionInfo4(dbHostInfo4, true, proxyHostInfo4);

    QVERIFY(!(proxyHostInfo3 != proxyHostInfo4));
}

void
ConnectionInfoTest::equalsTest()
{
	using namespace Connect;

    DbHostInfo dbHostInfo1("dbhost", 1234, "dbuser", "dbpassword", "dbname", "dbdriver");
    DbHostInfo dbHostInfo2;
    ProxyHostInfo proxyHostInfo1("proxyhost", 1234, "proxyuser", "proxypassword", (QNetworkProxy::ProxyType)1);
    ProxyHostInfo proxyHostInfo2;
    ConnectionInfo connectionInfo1(dbHostInfo1, true, proxyHostInfo1);
    ConnectionInfo connectionInfo2(dbHostInfo2, false, proxyHostInfo2);

    QVERIFY(!(connectionInfo1 == connectionInfo2));

    DbHostInfo dbHostInfo3("dbhost", 1234, "dbuser", "dbpassword", "dbname", "dbdriver");
    DbHostInfo dbHostInfo4("dbhost", 1234, "dbuser", "dbpassword", "dbname", "dbdriver");
    ProxyHostInfo proxyHostInfo3("proxyhost", 1234, "proxyuser", "proxypassword", (QNetworkProxy::ProxyType)1);
    ProxyHostInfo proxyHostInfo4("proxyhost", 1234, "proxyuser", "proxypassword", (QNetworkProxy::ProxyType)1);
    ConnectionInfo connectionInfo3(dbHostInfo3, true, proxyHostInfo3);
    ConnectionInfo connectionInfo4(dbHostInfo4, true, proxyHostInfo4);

    QVERIFY(proxyHostInfo3 == proxyHostInfo4);
}

void
ConnectionInfoTest::assignmentTest()
{
	using namespace Connect;

    DbHostInfo dbHostInfo("dbhost", 1234, "dbuser", "dbpassword", "dbname", "dbdriver");
    ProxyHostInfo proxyHostInfo("proxyhost", 1234, "proxyuser", "proxypassword", (QNetworkProxy::ProxyType)1);
    ConnectionInfo connectionInfo1(dbHostInfo, true, proxyHostInfo);
    ConnectionInfo connectionInfo2 = connectionInfo1;

    QVERIFY(connectionInfo2.dbHostInfo() == connectionInfo1.dbHostInfo() && 
            connectionInfo2.proxyHostInfo() == connectionInfo1.proxyHostInfo() &&
            connectionInfo2.useProxy() == connectionInfo1.useProxy());
//    QVERIFY(connectionInfo2.dbHostInfo().address() == connectionInfo1.dbHostInfo().address() && 
//            connectionInfo2.dbHostInfo().port() == connectionInfo1.dbHostInfo().port() &&
//            connectionInfo2.dbHostInfo().user() == connectionInfo1.dbHostInfo().user() && 
//            connectionInfo2.dbHostInfo().password() == connectionInfo2.dbHostInfo().password() &&
//            connectionInfo2.dbHostInfo().dbName() == connectionInfo1.dbHostInfo().dbName() && 
//            connectionInfo2.dbHostInfo().dbDriver() == connectionInfo2.dbHostInfo().dbDriver() &&
//            connectionInfo2.proxyHostInfo().address() == connectionInfo1.proxyHostInfo().address() && 
//            connectionInfo2.proxyHostInfo().port() == connectionInfo1.proxyHostInfo().port() &&
//            connectionInfo2.proxyHostInfo().user() == connectionInfo1.proxyHostInfo().user() && 
//            connectionInfo2.proxyHostInfo().password() == connectionInfo2.proxyHostInfo().password() &&
//            connectionInfo2.proxyHostInfo().type() == connectionInfo1.proxyHostInfo().type());
}

void
ConnectionInfoTest::proxyHostInfoTest()
{
	using namespace Connect;

    DbHostInfo dbHostInfo;
    ProxyHostInfo proxyHostInfo("proxyhost", 1234, "proxyuser", "proxypassword", (QNetworkProxy::ProxyType)1);
    ConnectionInfo connectionInfo(dbHostInfo, true, proxyHostInfo);

    QVERIFY(connectionInfo.proxyHostInfo().address() == "proxyhost" 
            && connectionInfo.proxyHostInfo().port() == 1234 && connectionInfo.proxyHostInfo().user() == "proxyuser" 
            && connectionInfo.proxyHostInfo().type() == (QNetworkProxy::ProxyType)1 && connectionInfo.useProxy() == true);   
}

void
ConnectionInfoTest::setDbHostInfoTest()
{
	using namespace Connect;

    DbHostInfo dbHostInfo("dbhost", 1234, "dbuser", "dbpassword", "dbname", "dbdriver");
    ProxyHostInfo proxyHostInfo;
    ConnectionInfo connectionInfo;
    connectionInfo.setProxyHostInfo(proxyHostInfo);
    connectionInfo.setDbHostInfo(dbHostInfo);

    QVERIFY("dbhost" == connectionInfo.dbHostInfo().address() && 1234 == connectionInfo.dbHostInfo().port()
            && "dbuser" == connectionInfo.dbHostInfo().user() && "dbname" == connectionInfo.dbHostInfo().dbName()
            && "dbdriver" == connectionInfo.dbHostInfo().dbDriver());
}

void
ConnectionInfoTest::setProxyHostInfoTest()
{
	using namespace Connect;

    DbHostInfo dbHostInfo;
    ProxyHostInfo proxyHostInfo("proxyhost", 1234, "proxyuser", "proxypassword", (QNetworkProxy::ProxyType)1);
    ConnectionInfo connectionInfo;
    connectionInfo.setProxyHostInfo(proxyHostInfo);

    QVERIFY(connectionInfo.proxyHostInfo().address() == "proxyhost" 
            && connectionInfo.proxyHostInfo().port() == 1234 && connectionInfo.proxyHostInfo().user() == "proxyuser" 
            && connectionInfo.proxyHostInfo().type() == (QNetworkProxy::ProxyType)1);   
}

void
ConnectionInfoTest::setUseProxyTest()
{
	using namespace Connect;

    DbHostInfo dbHostInfo;
    ProxyHostInfo proxyHostInfo;
    ConnectionInfo connectionInfo(dbHostInfo, true, proxyHostInfo);
    connectionInfo.setUseProxy(false);
    
    QVERIFY(false == connectionInfo.useProxy());
}

void
ConnectionInfoTest::toXmlTest()
{
	using namespace Connect;
}

void
ConnectionInfoTest::useProxyTest()
{
	using namespace Connect;

    DbHostInfo dbHostInfo;
    ProxyHostInfo proxyHostInfo;
    ConnectionInfo connectionInfo(dbHostInfo, true, proxyHostInfo);
    
    QVERIFY(true == connectionInfo.useProxy());
}


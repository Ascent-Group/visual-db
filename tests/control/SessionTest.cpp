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

#include <control/Session.h>
#include <control/SessionTest.h>
#include <connect/ConnectionInfo.h>

void 
SessionTest::initTestCase()
{

}

void 
SessionTest::cleanupTestCase()
{

}

void
SessionTest::loadTest()
{
    using namespace Control;
    using namespace Connect;

    saveTest();

    Session session;
    QVERIFY(session.load("test.vdb"));
    
    // test loading from file with wrong order
    ConnectionInfo connectionInfo0;
    QVERIFY(session.loadConnectionInfo(connectionInfo0, 0));
    QVERIFY("dbhost0" == connectionInfo0.dbHostInfo().address());
    QVERIFY(4321 == connectionInfo0.dbHostInfo().port());
    QVERIFY("dbuser0" == connectionInfo0.dbHostInfo().user());
    QVERIFY("dbname0" == connectionInfo0.dbHostInfo().dbName());
    QVERIFY("dbdriver0" == connectionInfo0.dbHostInfo().dbDriver());
    QVERIFY("proxyhost0" == connectionInfo0.proxyHostInfo().address());
    QVERIFY(4321 == connectionInfo0.proxyHostInfo().port());
    QVERIFY("proxyuser0" == connectionInfo0.proxyHostInfo().user());
    QVERIFY((QNetworkProxy::ProxyType)0 == connectionInfo0.proxyHostInfo().type());
    QVERIFY(false == connectionInfo0.useProxy());

    ConnectionInfo connectionInfo;
    QVERIFY(session.load("test.vdb"));
    QVERIFY(session.loadConnectionInfo(connectionInfo, 1));
    QVERIFY("dbhost" == connectionInfo.dbHostInfo().address());
    QVERIFY(1234 == connectionInfo.dbHostInfo().port());
    QVERIFY("dbuser" == connectionInfo.dbHostInfo().user());
    QVERIFY("dbname" == connectionInfo.dbHostInfo().dbName());
    QVERIFY("dbdriver" == connectionInfo.dbHostInfo().dbDriver());
    QVERIFY("proxyhost" == connectionInfo.proxyHostInfo().address());
    QVERIFY(1234 == connectionInfo.proxyHostInfo().port());
    QVERIFY("proxyuser" == connectionInfo.proxyHostInfo().user());
    QVERIFY((QNetworkProxy::ProxyType)1 == connectionInfo.proxyHostInfo().type());
    QVERIFY(true == connectionInfo.useProxy());

    // test loading connection info with non-existant index
    QVERIFY(!session.loadConnectionInfo(connectionInfo, 100500));

    // test loading connection info without calling load function before
    Session sessionWrongLoad;
    ConnectionInfo connectionInfoEmpty;
    QVERIFY(!sessionWrongLoad.loadConnectionInfo(connectionInfoEmpty, 0));
    
    QVERIFY("localhost" == connectionInfoEmpty.dbHostInfo().address());
    QVERIFY(5432 == connectionInfoEmpty.dbHostInfo().port());
    QVERIFY("" == connectionInfoEmpty.dbHostInfo().user());
    QVERIFY("" == connectionInfoEmpty.dbHostInfo().dbName());
    QVERIFY("QPSQL" == connectionInfoEmpty.dbHostInfo().dbDriver());
    QVERIFY("" == connectionInfoEmpty.proxyHostInfo().address());
    QVERIFY(0 == connectionInfoEmpty.proxyHostInfo().port());
    QVERIFY("" == connectionInfoEmpty.proxyHostInfo().user());
    QVERIFY(QNetworkProxy::NoProxy == connectionInfoEmpty.proxyHostInfo().type());
    QVERIFY(false == connectionInfoEmpty.useProxy());

    // test loading from the non-existant file
    QVERIFY(!session.load("test_not_found.vdb"));
}

void 
SessionTest::loadConnectionInfoTest()
{

}

void 
SessionTest::loadSceneTest()
{

}

void
SessionTest::saveTest()
{
    using namespace Control;
    using namespace Connect;
    using namespace Gui;
    
    Session session;
    GraphicsScene scene;
    
    DbHostInfo dbHostInfo("dbhost", 1234, "dbuser", "dbpassword", "dbname", "dbdriver");
    ProxyHostInfo proxyInfo("proxyhost", 1234, "proxyuser", "proxypassword", (QNetworkProxy::ProxyType)1);
    ConnectionInfo connectionInfo(dbHostInfo, true, proxyInfo); 
    session.saveConnectionInfo(connectionInfo, 1);

    DbHostInfo dbHostInfo0("dbhost0", 4321, "dbuser0", "dbpassword0", "dbname0", "dbdriver0");
    ProxyHostInfo proxyInfo0("proxyhost0", 4321, "proxyuser0", "proxypassword0", (QNetworkProxy::ProxyType)0);
    ConnectionInfo connectionInfo0(dbHostInfo0, false, proxyInfo0); 
    session.saveConnectionInfo(connectionInfo0, 0);
    
    session.save("test.vdb");
}

void 
SessionTest::saveConnectionInfoTest()
{
}

void 
SessionTest::saveSceneTest()
{

}


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
    ConnectionInfo connectionInfo;

    QVERIFY(session.load("test.vdb"));
    QVERIFY(session.loadConnectionInfo(connectionInfo));

    QVERIFY("dbhost" == connectionInfo.dbHostInfo().address() && 1234 == connectionInfo.dbHostInfo().port()
            && "dbuser" == connectionInfo.dbHostInfo().user() && "dbname" == connectionInfo.dbHostInfo().dbName()
            && "dbdriver" == connectionInfo.dbHostInfo().dbDriver() && connectionInfo.proxyHostInfo().address() == "proxyhost" 
            && connectionInfo.proxyHostInfo().port() == 1234 && connectionInfo.proxyHostInfo().user() == "proxyuser" 
            && connectionInfo.proxyHostInfo().type() == (QNetworkProxy::ProxyType)1 && connectionInfo.useProxy() == true);

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
   
    session.saveConnectionInfo(connectionInfo);
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


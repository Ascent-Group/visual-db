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

#include <control/Context.h>
#include <control/ContextTest.h>

void
ContextTest::initTestCase()
{

}

void
ContextTest::cleanupTestCase()
{

}

/*!
 * This test case tests connection info setting/getting by passing it to the context's
 * constructor.
 */
void
ContextTest::connectionInfoTest1()
{
    using namespace Control;
    using namespace Connect;

    QString dbHost("123.456.789.0");
    quint16 dbPort = 5432;
    QString dbUser("Alice");
    QString dbPassword("qwerty");
    QString dbName("TopSecretDB");
    QString dbDriver("QPSQL");

    // create db host info
    DbHostInfo dbHostInfo(dbHost, dbPort, dbUser, dbPassword, dbName, dbDriver);

    QString proxyHost("1.2.3.4");
    quint16 proxyPort = 3128;
    QString proxyUser("Bob");
    QString proxyPassword("ytrewq");
    QNetworkProxy::ProxyType proxyType = QNetworkProxy::NoProxy;

    // create proxy host info
    ProxyHostInfo proxyHostInfo(true, proxyHost, proxyPort, proxyUser, proxyPassword, proxyType);
    // create connection info
    ConnectionInfo connInfo(dbHostInfo, proxyHostInfo);

    // create context
    Context ctx(connInfo, QSqlDatabase());

    // verify the getter returns the same connection info as we have set
    QCOMPARE(ctx.connectionInfo().dbHostInfo().address(), dbHost);
    QCOMPARE(ctx.connectionInfo().dbHostInfo().port(), dbPort);
    QCOMPARE(ctx.connectionInfo().dbHostInfo().user(), dbUser);
    QCOMPARE(ctx.connectionInfo().dbHostInfo().password(), dbPassword);
    QCOMPARE(ctx.connectionInfo().dbHostInfo().dbName(), dbName);
    QCOMPARE(ctx.connectionInfo().dbHostInfo().dbDriver(), dbDriver);

    QCOMPARE(ctx.connectionInfo().proxyHostInfo().useProxy(), true);
    QCOMPARE(ctx.connectionInfo().proxyHostInfo().address(), proxyHost);
    QCOMPARE(ctx.connectionInfo().proxyHostInfo().port(), proxyPort);
    QCOMPARE(ctx.connectionInfo().proxyHostInfo().user(), proxyUser);
    QCOMPARE(ctx.connectionInfo().proxyHostInfo().password(), proxyPassword);
    QCOMPARE(ctx.connectionInfo().proxyHostInfo().type(), proxyType);
}

/*!
 * This test case tests connection info setting/getting by using the provided getter and
 * setter.
 */
void
ContextTest::connectionInfoTest2()
{
    using namespace Control;
    using namespace Connect;

    QString dbHost("123.456.789.0");
    quint16 dbPort = 5432;
    QString dbUser("Alice");
    QString dbPassword("qwerty");
    QString dbName("TopSecretDB");
    QString dbDriver("QPSQL");

    // create db host info
    DbHostInfo dbHostInfo(dbHost, dbPort, dbUser, dbPassword, dbName, dbDriver);

    QString proxyHost("1.2.3.4");
    quint16 proxyPort = 3128;
    QString proxyUser("Bob");
    QString proxyPassword("ytrewq");
    QNetworkProxy::ProxyType proxyType = QNetworkProxy::NoProxy;

    // create proxy host info
    ProxyHostInfo proxyHostInfo(true, proxyHost, proxyPort, proxyUser, proxyPassword, proxyType);
    // create connection info
    ConnectionInfo connInfo(dbHostInfo, proxyHostInfo);

    // create context
    Context ctx;
    ctx.setConnectionInfo(connInfo);

    // verify the getter returns the same connection info as we have set
    QCOMPARE(ctx.connectionInfo().dbHostInfo().address(), dbHost);
    QCOMPARE(ctx.connectionInfo().dbHostInfo().port(), dbPort);
    QCOMPARE(ctx.connectionInfo().dbHostInfo().user(), dbUser);
    QCOMPARE(ctx.connectionInfo().dbHostInfo().password(), dbPassword);
    QCOMPARE(ctx.connectionInfo().dbHostInfo().dbName(), dbName);
    QCOMPARE(ctx.connectionInfo().dbHostInfo().dbDriver(), dbDriver);

    QCOMPARE(ctx.connectionInfo().proxyHostInfo().useProxy(), true);
    QCOMPARE(ctx.connectionInfo().proxyHostInfo().address(), proxyHost);
    QCOMPARE(ctx.connectionInfo().proxyHostInfo().port(), proxyPort);
    QCOMPARE(ctx.connectionInfo().proxyHostInfo().user(), proxyUser);
    QCOMPARE(ctx.connectionInfo().proxyHostInfo().password(), proxyPassword);
    QCOMPARE(ctx.connectionInfo().proxyHostInfo().type(), proxyType);
}

/*!
 * This test case tests connection info setting/getting by passing it to the context's
 * constructor.
 */
void
ContextTest::dbHandleTest1()
{
    using namespace Control;

    QString dbName("db");
    QString driverName("driver");
    QString hostName("host");
    int port = 123;
    QString userName("user");

    // create database connection
    QSqlDatabase dbHandle = QSqlDatabase::addDatabase(driverName);
    dbHandle.setDatabaseName(dbName);
    dbHandle.setHostName(hostName);
    dbHandle.setPort(port);
    dbHandle.setUserName(userName);

    // create context
    Context ctx(Connect::ConnectionInfo(), dbHandle);
    // verify the getter returns the same database connection
    QCOMPARE(ctx.dbHandle().databaseName(), dbName);
    QCOMPARE(ctx.dbHandle().driverName(), driverName);
    QCOMPARE(ctx.dbHandle().hostName(), hostName);
    QCOMPARE(ctx.dbHandle().port(), port);
    QCOMPARE(ctx.dbHandle().userName(), userName);
}

/*!
 * This test case tests connection info setting/getting by using the provided getter and
 * setter.
 */
void
ContextTest::dbHandleTest2()
{
    using namespace Control;

    QString dbName("db");
    QString driverName("driver");
    QString hostName("host");
    int port = 123;
    QString userName("user");

    // create database connection
    QSqlDatabase dbHandle = QSqlDatabase::addDatabase(driverName);
    dbHandle.setDatabaseName(dbName);
    dbHandle.setHostName(hostName);
    dbHandle.setPort(port);
    dbHandle.setUserName(userName);

    // create context
    Context ctx;
    // set database connection for the context
    ctx.setDbHandle(dbHandle);
    // verify the getter returns the same database connection
    QCOMPARE(ctx.dbHandle().databaseName(), dbName);
    QCOMPARE(ctx.dbHandle().driverName(), driverName);
    QCOMPARE(ctx.dbHandle().hostName(), hostName);
    QCOMPARE(ctx.dbHandle().port(), port);
    QCOMPARE(ctx.dbHandle().userName(), userName);
}

void
ContextTest::comparisonTest()
{
    using namespace Connect;
    using namespace Control;

    // create db host info
    DbHostInfo dbInfo1("host1", 111, "user1", "pass1", "db1", "driver1");
    // create proxy host info
    ProxyHostInfo proxyInfo1(true, "host1", 111, "user1", "pass1", QNetworkProxy::HttpProxy);
    // create connection info
    ConnectionInfo connInfo1(dbInfo1, proxyInfo1);
    // create ctx1
    Context ctx1(connInfo1, QSqlDatabase::addDatabase("a", "b"));

    // create db host info
    DbHostInfo dbInfo2("host2", 222, "user2", "pass2", "db2", "driver1");
    // create proxy host info
    ProxyHostInfo proxyInfo2(proxyInfo1);
    // create connection info
    ConnectionInfo connInfo2(dbInfo2, proxyInfo2);
    // create ctx2 (!= ctx1)
    Context ctx2(connInfo2, QSqlDatabase::addDatabase("c", "d"));

    // create ctx3 (== ctx1)
    Context ctx3(connInfo1, ctx1.dbHandle());

    QVERIFY(ctx1 != ctx2);
    QVERIFY(ctx1 == ctx3);
}


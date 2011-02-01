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

#include <QDomElement>
#include <HostInfoTest.h>
#include <connect/HostInfo.h>

void
HostInfoTest::initTestCase()
{
}

void
HostInfoTest::cleanupTestCase()
{
}

void
HostInfoTest::assignmentTest()
{
	using namespace Connect;

    HostInfo hostInfo1("testhost", 1234, "testuser", "testpassword");
    HostInfo hostInfo2;

    hostInfo2 = hostInfo1;

    QVERIFY(hostInfo2.address() == hostInfo1.address() && hostInfo2.port() == hostInfo1.port() &&
            hostInfo2.user() == hostInfo1.user() && hostInfo2.password() == hostInfo2.password());
}

void
HostInfoTest::addressTest()
{
	using namespace Connect;

    HostInfo hostInfo("testhost", 1234, "testuser", "testpassword");
    QVERIFY("testhost" == hostInfo.address());
}

void
HostInfoTest::fromXmlTest()
{
	using namespace Connect;

    QDomDocument xmlDocument;
    QDomElement xmlElement = xmlDocument.createElement("HostInfo");
    xmlElement.setAttribute("host", "testhost");
    xmlElement.setAttribute("port", "1234");
    xmlElement.setAttribute("testuser", "testuser");

    HostInfo hostInfo;
    hostInfo.fromXml(xmlElement);
    
    QVERIFY(hostInfo.address() == xmlElement.attribute("host") && hostInfo.port() == xmlElement.attribute("port").toInt()
            && hostInfo.user() == xmlElement.attribute("user"));
}

void
HostInfoTest::notEqualsTest()
{
	using namespace Connect;

    HostInfo hostInfo1("testhost", 1234, "testuser", "testpassword");
    HostInfo hostInfo2;

    QVERIFY(hostInfo1 != hostInfo2);

    HostInfo hostInfo3("testhost", 1234, "testuser", "testpassword");
    HostInfo hostInfo4("testhost", 1234, "testuser", "testpassword");

    QVERIFY(!(hostInfo3 != hostInfo4));
}

void
HostInfoTest::equalsTest()
{
	using namespace Connect;

    HostInfo hostInfo1("testhost", 1234, "testuser", "testpassword");
    HostInfo hostInfo2;

    QVERIFY(!(hostInfo1 == hostInfo2));

    HostInfo hostInfo3("testhost", 1234, "testuser", "testpassword");
    HostInfo hostInfo4("testhost", 1234, "testuser", "testpassword");

    QVERIFY(hostInfo3 == hostInfo4);
}

void
HostInfoTest::passwordTest()
{
	using namespace Connect;

    HostInfo hostInfo("testhost", 1234, "testuser", "testpassword");
    QVERIFY("testpassword" == hostInfo.password());
}

void
HostInfoTest::portTest()
{
	using namespace Connect;

    HostInfo hostInfo("testhost", 1234, "testuser", "testpassword");
    QVERIFY(1234 == hostInfo.port());;
}

void
HostInfoTest::setAddressTest()
{
	using namespace Connect;

    HostInfo hostInfo;
    hostInfo.setAddress("testhost");
    QVERIFY("testhost" == hostInfo.address());
}

void
HostInfoTest::setPasswordTest()
{
	using namespace Connect;

    HostInfo hostInfo;
    hostInfo.setPassword("testpassword");
    QVERIFY("testpassword" == hostInfo.password());
}

void
HostInfoTest::setPortTest()
{
	using namespace Connect;

    HostInfo hostInfo;
    hostInfo.setPort(1234);
    QVERIFY(1234 == hostInfo.port());
}

void
HostInfoTest::setUserTest()
{
	using namespace Connect;

    HostInfo hostInfo;
    hostInfo.setUser("testuser");
    QVERIFY("testuser" == hostInfo.user());
}

void
HostInfoTest::toXmlTest()
{
	using namespace Connect;

    QDomDocument xmlDocument;
    QDomElement xmlElement = xmlDocument.createElement("HostInfo");
    
    HostInfo hostInfo("testhost", 1234, "testuser", "");
    hostInfo.toXml(xmlElement);
    
    QVERIFY("testhost" == xmlElement.attribute("host") && "1234" == xmlElement.attribute("port")
            && "testuser" == xmlElement.attribute("user"));
}

void
HostInfoTest::userTest()
{
    using namespace Connect;

    HostInfo hostInfo("testuser", 1234, "testuser", "testpassword");
    QVERIFY("testuser" == hostInfo.user());
}


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

#include <ProxyHostInfoTest.h>
#include <connect/ProxyHostInfo.h>

void
ProxyHostInfoTest::initTestCase()
{
}

void
ProxyHostInfoTest::cleanupTestCase()
{
}

void
ProxyHostInfoTest::fromXmlTest()
{
 	using namespace Connect;

    QDomDocument xmlDocument;
    QDomElement xmlElement = xmlDocument.createElement("ProxyHostInfo");
    xmlElement.setAttribute("host", "proxyhost");
    xmlElement.setAttribute("port", "1234");
    xmlElement.setAttribute("user", "proxyuser");
    xmlElement.setAttribute("type", "1");

    ProxyHostInfo proxyHostInfo;
    proxyHostInfo.fromXml(xmlElement);
    
    QVERIFY(proxyHostInfo.address() == "proxyhost" && proxyHostInfo.port() == 1234
            && proxyHostInfo.user() == "proxyuser" && proxyHostInfo.type() == (QNetworkProxy::ProxyType)1);
}

void
ProxyHostInfoTest::notEqualsTest()
{
	using namespace Connect;

    ProxyHostInfo proxyHostInfo1("proxyhost", 1234, "proxyuser", "proxypassword", (QNetworkProxy::ProxyType)1);
    ProxyHostInfo proxyHostInfo2;

    QVERIFY(proxyHostInfo1 != proxyHostInfo2);

    ProxyHostInfo proxyHostInfo3("proxyhost", 1234, "proxyuser", "proxypassword", (QNetworkProxy::ProxyType)1);
    ProxyHostInfo proxyHostInfo4("proxyhost", 1234, "proxyuser", "proxypassword", (QNetworkProxy::ProxyType)1);

    QVERIFY(!(proxyHostInfo3 != proxyHostInfo4));
}

void
ProxyHostInfoTest::equalsTest()
{
	using namespace Connect;

    ProxyHostInfo proxyHostInfo1("proxyhost", 1234, "proxyuser", "proxypassword", (QNetworkProxy::ProxyType)1);
    ProxyHostInfo proxyHostInfo2;

    QVERIFY(!(proxyHostInfo1 == proxyHostInfo2));

    ProxyHostInfo proxyHostInfo3("proxyhost", 1234, "proxyuser", "proxypassword", (QNetworkProxy::ProxyType)1);
    ProxyHostInfo proxyHostInfo4("proxyhost", 1234, "proxyuser", "proxypassword", (QNetworkProxy::ProxyType)1);

    QVERIFY(proxyHostInfo3 == proxyHostInfo4);
}

void
ProxyHostInfoTest::assignmentTest()
{
	using namespace Connect;

    ProxyHostInfo proxyHostInfo1("proxyhost", 1234, "proxyuser", "proxypassword", (QNetworkProxy::ProxyType)1);
    ProxyHostInfo proxyHostInfo2;

    proxyHostInfo2 = proxyHostInfo1;

    QVERIFY(proxyHostInfo2.address() == proxyHostInfo1.address() && proxyHostInfo2.port() == proxyHostInfo1.port() &&
            proxyHostInfo2.user() == proxyHostInfo1.user() && proxyHostInfo2.password() == proxyHostInfo2.password() &&
            proxyHostInfo2.type() == proxyHostInfo1.type());
}

void
ProxyHostInfoTest::setTypeTest()
{
	using namespace Connect;

    ProxyHostInfo proxyHostInfo;
    proxyHostInfo.setType((QNetworkProxy::ProxyType)1);
    QVERIFY((QNetworkProxy::ProxyType)1 == proxyHostInfo.type());
}

void
ProxyHostInfoTest::toXmlTest()
{
	using namespace Connect;

    QDomDocument xmlDocument;
    QDomElement xmlElement = xmlDocument.createElement("ProxyHostInfo");
    
    ProxyHostInfo proxyHostInfo("proxyhost", 1234, "proxyuser", "", (QNetworkProxy::ProxyType)1);
    proxyHostInfo.toXml(xmlElement);
    
    QVERIFY("proxyhost" == xmlElement.attribute("host") && "1234" == xmlElement.attribute("port")
            && "proxyuser" == xmlElement.attribute("user") && 1 == xmlElement.attribute("type").toInt());
}

void
ProxyHostInfoTest::typeTest()
{
	using namespace Connect;

    ProxyHostInfo proxyHostInfo("proxyhost", 1234, "proxyuser", "proxypassword", (QNetworkProxy::ProxyType)1);
    QVERIFY((QNetworkProxy::ProxyType)1 == proxyHostInfo.type());
}


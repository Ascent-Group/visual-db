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

#include <connect/ProxyParameters.h>
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QNetworkProxy>
#include <QVariant>

/*
 * Constructor
 */
ProxyParameters::ProxyParameters()
{
    mUseProxy = mSettings.value("LastSession/useProxy", "false").toBool();
    mProxyType = (QNetworkProxy::ProxyType)mSettings.value("LastSession/proxyType", QNetworkProxy::HttpProxy).toInt();
    mProxyHost = mSettings.value("LastSession/proxyAddress", QString("")).toString();
    mProxyPort = mSettings.value("LastSession/proxyPort", 8080).toInt();
    mProxyUser = mSettings.value("LastSession/proxyUser", QString("")).toString();
}

/*
 * Destructor
 */
ProxyParameters::~ProxyParameters()
{

}

bool
ProxyParameters::useProxy() const
{
    return mUseProxy;
}

QNetworkProxy::ProxyType
ProxyParameters::proxyType() const
{
    return mProxyType;
}

QString
ProxyParameters::proxyHost() const
{
    return mProxyHost;
}

int
ProxyParameters::proxyPort() const
{
    return mProxyPort;
}

QString
ProxyParameters::proxyUser() const
{
    return mProxyUser;
}

void
ProxyParameters::setUseProxy(bool ipUseProxy)
{
    mUseProxy = ipUseProxy;
    mSettings.setValue("LastSession/useProxy", mUseProxy);
}

void
ProxyParameters::setProxyType(QNetworkProxy::ProxyType ipProxyType)
{
    mProxyType = ipProxyType;
    mSettings.setValue("LastSession/proxyType", mProxyType);
}

void
ProxyParameters::setProxyHost(QString ipProxyHost)
{
    mProxyHost = ipProxyHost;
    mSettings.setValue("LastSession/proxyHost", mProxyHost);
}

void
ProxyParameters::setProxyPort(int ipProxyPort)
{
    mProxyPort = ipProxyPort;
    mSettings.setValue("LastSession/proxyPort", mProxyPort);
}

void
ProxyParameters::setProxyUser(QString ipProxyUser)
{
    mProxyUser = ipProxyUser;
    mSettings.setValue("LastSession/proxyUser", mProxyUser);
}

/*
 * Set proxy for application
 */
void
setProxy(ProxyParameters &ipConnection)
{
    // create proxy and set settings
    QNetworkProxy proxy;
    proxy.setType(ipConnection.proxyType());
    proxy.setHostName(ipConnection.proxyHost());
    proxy.setPort(ipConnection.proxyPort());
    proxy.setUser(ipConnection.proxyUser());
    QNetworkProxy::setApplicationProxy(proxy);
}

/*
 * Load proxy settings from the xml file
 */
void
ProxyParameters::fromXml(QDomElement &ipElement)
{
    setUseProxy(ipElement.attribute("use").toInt());
    setProxyType((QNetworkProxy::ProxyType)ipElement.attribute("type").toInt());
    setProxyHost(ipElement.attribute("host"));
    setProxyPort(ipElement.attribute("port").toInt());
    setProxyUser(ipElement.attribute("user"));
}

/*
 * Get xml structure of proxy settings
 */
QDomElement
ProxyParameters::toXml(QDomDocument &ipDoc) const
{
    QDomElement proxyElement = ipDoc.createElement("proxy");
//    proxyElement.setAttribute("type", mSettings.value("LastSession/proxyType", QString("Http")).toString());
    proxyElement.setAttribute("use", useProxy());
    proxyElement.setAttribute("type", proxyType());
    proxyElement.setAttribute("host", proxyHost());
    proxyElement.setAttribute("port", proxyPort());
    proxyElement.setAttribute("user", proxyUser());

    return proxyElement;
}


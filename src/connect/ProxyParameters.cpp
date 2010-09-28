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
#include <consts.h>
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
    using namespace Consts;
    mUseProxy = mSettings.value(LAST_SESSION_GRP + "/" + USE_PROXY_SETTING, "false").toBool();
    mProxyType = (QNetworkProxy::ProxyType)mSettings.value(LAST_SESSION_GRP + "/" + PROXY_TYPE_SETTING, QNetworkProxy::HttpProxy).toInt();
    mProxyHost = mSettings.value(LAST_SESSION_GRP + "/" + PROXY_HOST_SETTING, QString("")).toString();
    mProxyPort = mSettings.value(LAST_SESSION_GRP + "/" + PROXY_PORT_SETTING, 8080).toInt();
    mProxyUser = mSettings.value(LAST_SESSION_GRP + "/" + PROXY_USER_SETTING, QString("")).toString();
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

quint16
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
    mSettings.setValue(Consts::LAST_SESSION_GRP + "/" + Consts::USE_PROXY_SETTING, mUseProxy);
}

void
ProxyParameters::setProxyType(QNetworkProxy::ProxyType ipProxyType)
{
    mProxyType = ipProxyType;
    mSettings.setValue(Consts::LAST_SESSION_GRP + "/" + Consts::PROXY_TYPE_SETTING, mProxyType);
}

void
ProxyParameters::setProxyHost(QString ipProxyHost)
{
    mProxyHost = ipProxyHost;
    mSettings.setValue(Consts::LAST_SESSION_GRP + "/" + Consts::PROXY_HOST_SETTING, mProxyHost);
}

void
ProxyParameters::setProxyPort(quint16 ipProxyPort)
{
    mProxyPort = ipProxyPort;
    mSettings.setValue(Consts::LAST_SESSION_GRP + "/" + Consts::PROXY_PORT_SETTING, mProxyPort);
}

void
ProxyParameters::setProxyUser(QString ipProxyUser)
{
    mProxyUser = ipProxyUser;
    mSettings.setValue(Consts::LAST_SESSION_GRP + "/" + Consts::PROXY_USER_SETTING, mProxyUser);
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
//    proxyElement.setAttribute("type", mSettings.value(Consts::LAST_SESSION_GRP + "/" + Consts::PROXY_TYPE_SETTING, QString("Http")).toString());
    proxyElement.setAttribute("use", useProxy());
    proxyElement.setAttribute("type", proxyType());
    proxyElement.setAttribute("host", proxyHost());
    proxyElement.setAttribute("port", proxyPort());
    proxyElement.setAttribute("user", proxyUser());

    return proxyElement;
}


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

#include <connect/ConnectionInfo.h>

namespace Connect {

ConnectionInfo::ConnectionInfo()
    : mDbHostInfo(), mUseProxy(false), mProxyHostInfo()
{
}

ConnectionInfo::ConnectionInfo(const DbHostInfo &iDbHostInfo, bool iUseProxy, const ProxyHostInfo &iProxyHostInfo)
    : mDbHostInfo(iDbHostInfo), mUseProxy(iUseProxy), mProxyHostInfo(iProxyHostInfo)
{
}

ConnectionInfo::~ConnectionInfo()
{
}

ConnectionInfo::ConnectionInfo(const ConnectionInfo &iConnectionInfo)
    : mDbHostInfo(iConnectionInfo.mDbHostInfo), mUseProxy(iConnectionInfo.mUseProxy)
    , mProxyHostInfo(iConnectionInfo.mProxyHostInfo)
{
}

ConnectionInfo &ConnectionInfo::operator=(const ConnectionInfo &iConnectionInfo)
{
    swap(iConnectionInfo);
    return *this;
}

const DbHostInfo &
ConnectionInfo::dbHostInfo() const
{
    return mDbHostInfo;
}

void 
ConnectionInfo::setDbHostInfo(const DbHostInfo &iDbHostInfo)
{
    mDbHostInfo = iDbHostInfo;
}

bool 
ConnectionInfo::useProxy() const
{
    return mUseProxy;
}

void 
ConnectionInfo::setUseProxy(bool iUseProxy)
{
    mUseProxy = iUseProxy;
}

QNetworkProxy::ProxyType 
ProxyHostInfo::type() const
{
    return mType;
}

const ProxyHostInfo &
ConnectionInfo::proxyHostInfo() const
{
    return mProxyHostInfo;
}

void 
ConnectionInfo::setProxyHostInfo(const ProxyHostInfo &iProxyHostInfo)
{
    mProxyHostInfo = iProxyHostInfo;
}

/*
 * Load proxy settings from the xml file
 */
void
ConnectionInfo::fromXml(QDomElement &iElement)
{
    QDomNode child = iElement.firstChild();
    while (!child.isNull()) {
        QDomElement element = child.toElement(); // try to convert the node to an element.
        if (!element.isNull()) {
            if (element.tagName() == "database") {
                mDbHostInfo.fromXml(element);
            } else if (element.tagName() == "proxy") {
                mProxyHostInfo.fromXml(element);
                setUseProxy((int)element.attribute("use").toInt());
            }
        }
        child = child.nextSibling();
    }
}

/*
 * Get xml structure of proxy settings
 */
void
ConnectionInfo::toXml(QDomDocument &iDoc, QDomElement &iElement) const
{
    QDomElement connElement = iDoc.createElement("connectionInfo");
    iElement.appendChild(connElement);

    QDomElement dbElement = iDoc.createElement("database");
    connElement.appendChild(mDbHostInfo.toXml(dbElement));

    QDomElement proxyElement = iDoc.createElement("proxy");
    connElement.appendChild(mProxyHostInfo.toXml(proxyElement));
    proxyElement.setAttribute("use", useProxy());
}

bool 
ConnectionInfo::operator==(const ConnectionInfo &iConnectionInfo) const
{
    return mDbHostInfo == iConnectionInfo.mDbHostInfo && mProxyHostInfo == iConnectionInfo.mProxyHostInfo;
}

bool 
ConnectionInfo::operator!=(const ConnectionInfo &iConnectionInfo) const
{
    return !(operator==(iConnectionInfo));
}

void 
ConnectionInfo::swap(const ConnectionInfo &iConnectionInfo)
{
    mDbHostInfo = iConnectionInfo.mDbHostInfo;
    mUseProxy = iConnectionInfo.mUseProxy;
    mProxyHostInfo = iConnectionInfo.mProxyHostInfo;
}

}

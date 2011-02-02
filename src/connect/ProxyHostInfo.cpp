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

#include <connect/ProxyHostInfo.h>

namespace Connect {

ProxyHostInfo::ProxyHostInfo(const QString &iAddress, quint16 iPort,
        const QString &iUser, const QString &iPassword, 
        const QNetworkProxy::ProxyType &iType)
    : HostInfo(iAddress, iPort, iUser, iPassword), mType(iType)
{
}

ProxyHostInfo::~ProxyHostInfo()
{
}

ProxyHostInfo::ProxyHostInfo(const ProxyHostInfo &iProxyHostInfo)
    : HostInfo(iProxyHostInfo), mType(iProxyHostInfo.mType)
{
}

ProxyHostInfo &ProxyHostInfo::operator=(const ProxyHostInfo &iProxyHostInfo)
{
    swap(iProxyHostInfo);
    return *this;
}

void 
ProxyHostInfo::setType(const QNetworkProxy::ProxyType &iType)
{
    mType = iType;
}

/*
 * Load proxy settings from the xml file
 */
void
ProxyHostInfo::fromXml(QDomElement &iElement)
{
    HostInfo::fromXml(iElement);
    setType((QNetworkProxy::ProxyType)iElement.attribute("type").toInt());
}

/*
 * Get xml structure of proxy settings
 */
QDomElement
ProxyHostInfo::toXml(QDomElement &iElement) const
{
    HostInfo::toXml(iElement);
    iElement.setAttribute("type", type());

    return iElement;
}

bool 
ProxyHostInfo::operator==(const ProxyHostInfo &iProxyHostInfo) const
{
    return HostInfo::operator==(iProxyHostInfo) && mType == iProxyHostInfo.mType; 
}

bool 
ProxyHostInfo::operator!=(const ProxyHostInfo &iProxyHostInfo) const
{
    return !(operator==(iProxyHostInfo));
}

void 
ProxyHostInfo::swap(const ProxyHostInfo &iProxyHostInfo)
{
    HostInfo::swap(iProxyHostInfo);
    mType = iProxyHostInfo.mType;
}

} // namespace Connect

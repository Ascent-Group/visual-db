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

#include <connect/HostInfo.h>

namespace Connect {

HostInfo::HostInfo(const QString &iAddress, quint16 iPort, 
        const QString &iUser, const QString &iPassword)
    : mAddress(iAddress), mPort(iPort)
    , mUser(iUser), mPassword(iPassword)
{
}

HostInfo::~HostInfo()
{
}

HostInfo::HostInfo(const HostInfo &iHostInfo)
    : mAddress(iHostInfo.mAddress), mPort(iHostInfo.mPort)
    , mUser(iHostInfo.mUser), mPassword(iHostInfo.mPassword)
{
}

HostInfo &HostInfo::operator=(const HostInfo &iHostInfo)
{
    swap(iHostInfo);
    return *this;
}

QString 
HostInfo::address() const
{
    return mAddress;
}

void 
HostInfo::setAddress(const QString &iAddress)
{
    mAddress = iAddress;
}


quint16 
HostInfo::port() const
{
    return mPort;
}

void 
HostInfo::setPort(quint16 iPort)
{
    mPort = iPort;
}

QString 
HostInfo::user() const
{
    return mUser;
}

void 
HostInfo::setUser(const QString &iUser)
{
    mUser = iUser;
}

QString 
HostInfo::password() const
{
    return mPassword;
}

void 
HostInfo::setPassword(const QString &iPassword)
{
    mPassword = iPassword;
}

/*
 * Load host parameters from the xml file
 */
void
HostInfo::fromXml(QDomElement &iElement)
{
    setAddress(iElement.attribute("host"));
    setPort(iElement.attribute("port").toInt());
    setUser(iElement.attribute("user"));
}

/*
 * Get xml structure of host settings
 */
QDomElement &
HostInfo::toXml(QDomElement &iElement) const
{
    iElement.setAttribute("host", address());
    iElement.setAttribute("port", port());
    iElement.setAttribute("user", user());

    return iElement;
}

bool 
HostInfo::operator==(const HostInfo &iHostInfo) const
{
    return mAddress == iHostInfo.mAddress && mPort == iHostInfo.mPort && 
        mUser == iHostInfo.mUser && mPassword == iHostInfo.mPassword;
}

bool 
HostInfo::operator!=(const HostInfo &iHostInfo) const
{
    return !(operator==(iHostInfo));
}

void 
HostInfo::swap(const HostInfo &iHostInfo)
{
    mAddress = iHostInfo.mAddress;
    mPort = iHostInfo.mPort;
    mUser = iHostInfo.mUser;
    mPassword = iHostInfo.mPassword;
}

} // namespace Connect

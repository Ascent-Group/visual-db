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

#ifndef PROXYPARAMETERS_H
#define PROXYPARAMETERS_H

#include <QNetworkProxy>
#include <QSettings>
#include <QString>

class QDomDocument;
class QDomElement;

/*
 * Incapsulation of proxy connection parameters
 */
class ProxyParameters
{
    public:
        ProxyParameters();
        ~ProxyParameters();

        void setUseProxy(bool);
        bool useProxy() const;

        void setProxyType(QNetworkProxy::ProxyType);
        QNetworkProxy::ProxyType proxyType() const;

        void setProxyHost(QString);
        QString proxyHost() const;

        void setProxyPort(quint16);
        quint16 proxyPort() const;

        void setProxyUser(QString);
        QString proxyUser() const;

        void fromXml(QDomElement &);
        QDomElement toXml(QDomDocument &) const;

    private:
        QSettings mSettings;

        bool mUseProxy;
        QNetworkProxy::ProxyType mProxyType;
        QString mProxyHost;
        quint16 mProxyPort;
        QString mProxyUser;
};

void setProxy(ProxyParameters &);

#endif // PROXYPARAMETERS_H


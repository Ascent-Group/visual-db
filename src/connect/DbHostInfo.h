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

#ifndef CONNECT_DBHOSTINFO_H
#define CONNECT_DBHOSTINFO_H

#include <connect/HostInfo.h>
#include <QDomDocument>
#include <QDomElement>

namespace Connect {

/*!
 * \class DbHostInfo
 * \headerfile connect/DbHostInfo.h
 * \brief Incapsulation of database connection parameters
 */
class DbHostInfo : public HostInfo
{
    public:
        explicit DbHostInfo(const QString &iAddress = "localhost", quint16 iPort = 5432, 
                const QString &iUser = "", const QString &iPassword = "", 
                const QString &iDbName = "", const QString &iDbDriver = "QPSQL");
        virtual ~DbHostInfo();

        DbHostInfo(const DbHostInfo &);
        DbHostInfo &operator=(const DbHostInfo &iDbHostInfo);

        QString dbName() const;
        void setDbName(const QString &iDbName);

        QString dbDriver() const;
        void setDbDriver(const QString &iDbDriver);

        QDomElement toXml(QDomElement &) const;
        void fromXml(QDomElement &);

        bool operator==(const DbHostInfo &iDbHostInfo) const;
        bool operator!=(const DbHostInfo &iDbHostInfo) const;

    protected:
        virtual void swap(const DbHostInfo &iDbHostInfo);

    private:
        QString mDbName;
        QString mDbDriver;
};

// \todo remove this func
//bool createConnection(const DbHostInfo &iDbHostInfo);

}

#endif // CONNECT_DBHOSTINFO_H


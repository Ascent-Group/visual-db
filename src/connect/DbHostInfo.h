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

#include <HostInfo.h>

namespace Connect {

/*!
 * \class DbHostInfo
 * \headfile connect/DbHostInfo.h
 * \brief Incapsulation of database connection parameters
 */
class DbHostInfo : public HostInfo
{
    public:
        explicit DbHostInfo(const QString &iAddress = "", quint16 iPort = 0, const QString &iUser = "", const QString &iPassword = "", const QString &iDbName = "", const QString &iDbDriver = "");
        ~DbHostInfo();

        QString dbName() const;
        void setDbName(const QString &iDbName);

        QString dbDriver() const;
        void setDbDriver(const QString &iDbDriver);

        bool operator==(const DbHostInfo &iDbHostInfo);
        bool operator!=(const DbHostInfo &iDbHostInfo);

    private:
        DbHostInfo(const DbHostInfo &) {};
        DbHostInfo &operator=(const DbHostInfo &/*iDbHostInfo*/) {};

    private:
        QString mDbName;
        QString mDbDriver;
};

}

#endif // CONNECT_DBHOSTINFO_H


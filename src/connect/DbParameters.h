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

#ifndef DBPARAMETERS_H
#define DBPARAMETERS_H

#include <QSettings>
#include <QString>

class QDomDocument;
class QDomElement;

/*
 * Incapsulation database connection parameters
 */
class DbParameters
{
    public:
        DbParameters();
        ~DbParameters();

        void setDbDriver(QString);
        QString dbDriver() const;

        void setDbHost(QString);
        QString dbHost() const;

        void setDbPort(quint16);
        quint16 dbPort() const;

        void setDbName(QString);
        QString dbName() const;

        void setDbUser(QString);
        QString dbUser() const;

        void setDbPassword(QString);
        QString dbPassword() const;

        void fromXml(QDomElement &);
        QDomElement toXml(QDomDocument &) const;

    private:
        QSettings mSettings;

        QString mDbDriver;
        QString mDbHost;
        quint16 mDbPort;
        QString mDbName;
        QString mDbUser;
        QString mDbPassword;
};

bool createConnection(DbParameters &);

#endif // DBPARAMETERS_H


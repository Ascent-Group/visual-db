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

#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QSqlDatabase>
#include <QVariant>
#include <common/Database.h>
#include <connect/DbParameters.h>
#include <consts.h>

/*
 * Constructor
 */
DbParameters::DbParameters()
{
    using namespace Consts;
    mDbDriver = mSettings.value(LAST_SESSION_GRP + "/" + DB_DRV_SETTING, QString("QSQLITE")).toString();
    mDbHost = mSettings.value(LAST_SESSION_GRP + "/" + DB_HOST_SETTING, QString("localhost")).toString();
    mDbPort = mSettings.value(LAST_SESSION_GRP + "/" + DB_PORT_SETTING, 5432).toInt();
    mDbName = mSettings.value(LAST_SESSION_GRP + "/" + DB_NAME_SETTING, QString("")).toString();
    mDbUser = mSettings.value(LAST_SESSION_GRP + "/" + DB_USER_SETTING, QString("postgres")).toString();
}

/*
 * Destructor
 */
DbParameters::~DbParameters()
{

}

QString
DbParameters::dbDriver() const
{
    return mDbDriver;
}

QString
DbParameters::dbHost() const
{
    return mDbHost;
}

quint16
DbParameters::dbPort() const
{
    return mDbPort;
}

QString
DbParameters::dbName() const
{
    return mDbName;
}

QString
DbParameters::dbUser() const
{
    return mDbUser;
}

QString
DbParameters::dbPassword() const
{
    return mDbPassword;
}

void
DbParameters::setDbDriver(QString ipDbDriver)
{
    mDbDriver = ipDbDriver;
    mSettings.setValue(Consts::LAST_SESSION_GRP + "/" + Consts::DB_DRV_SETTING, mDbDriver);
}

void
DbParameters::setDbHost(QString ipDbHost)
{
    mDbHost = ipDbHost;
    mSettings.setValue(Consts::LAST_SESSION_GRP + "/" + Consts::DB_HOST_SETTING, mDbHost);
}

void
DbParameters::setDbPort(quint16 ipDbPort)
{
    mDbPort = ipDbPort;
    mSettings.setValue(Consts::LAST_SESSION_GRP + "/" + Consts::DB_PORT_SETTING, mDbPort);
}

void
DbParameters::setDbName(QString ipDbName)
{
    mDbName = ipDbName;
    mSettings.setValue(Consts::LAST_SESSION_GRP + "/" + Consts::DB_NAME_SETTING, mDbName);
}

void
DbParameters::setDbPassword(QString ipDbPassword)
{
    mDbPassword = ipDbPassword;
//    mSettings.setValue(Consts::LAST_SESSION_GRP + "/" + Consts::DB_PASS_SETTING, mDbPassword);
}

void
DbParameters::setDbUser(QString ipDbUser)
{
    mDbUser = ipDbUser;
    mSettings.setValue(Consts::LAST_SESSION_GRP + "/" + Consts::DB_USER_SETTING, mDbUser);
}

/*
 * Create connection to the database
 */
bool
createConnection(DbParameters &ipConnection)
{
    bool success = false;
    QSqlDatabase db = QSqlDatabase::addDatabase(ipConnection.dbDriver(), "mainConnect");
    db.setDatabaseName(ipConnection.dbName());

    // if SQLite db
    if (QString("QSQLITE") == ipConnection.dbDriver()) {
        // check only if fie exists
        if ((success = QFile::exists(ipConnection.dbName()))) {
            db.open();
        }
    // of other DBMS
    } else {
        // check credentials and host
        db.setHostName(ipConnection.dbHost());
        db.setUserName(ipConnection.dbUser());
        db.setPassword(ipConnection.dbPassword());
        db.setPort(ipConnection.dbPort());

        if ((success = db.open())) {
            // initialize Database for further use and get schemas
            using namespace DbObjects::Common;
            Database *dbInst = Database::instance();
            dbInst->setSqlDriver(ipConnection.dbDriver());
        }
    }

    return success;
}

/*
 * Load database parameters from the xml file
 */
void
DbParameters::fromXml(QDomElement &ipElement)
{
    setDbDriver(ipElement.attribute("driver"));
    setDbHost(ipElement.attribute("host"));
    setDbPort(ipElement.attribute("port").toInt());
    setDbName(ipElement.attribute("name"));
    setDbUser(ipElement.attribute("user"));
}

/*
 * Get xml structure of database settings
 */
QDomElement
DbParameters::toXml(QDomDocument &ipDoc) const
{
    QDomElement dbElement = ipDoc.createElement("database");
    dbElement.setAttribute("driver", dbDriver());
    dbElement.setAttribute("host", dbHost());
    dbElement.setAttribute("port", dbPort());
    dbElement.setAttribute("name", dbName());
    dbElement.setAttribute("user", dbUser());

    return dbElement;
}


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

#include <psql/Role.h>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>

#include <QtDebug>

namespace DbObjects
{

namespace Psql
{

/*!
 * Constructor
 *
 * \param[in] iName - Name of the given table
 */
Role::Role(QString iName)
    : DbRole(iName)
{

}

/*
 * Dtor
 */
Role::~Role()
{

}

/*
 * Loads role definition data
 */
bool
Role::loadData()
{
    if (mIsLoaded) {
        return true;
    }

    // lyuts: maybe it should be done in Database in one sweep
    QSqlDatabase db = QSqlDatabase::database("mainConnect");
    QSqlQuery query(db);
    QString qstr;

    // create a query
    qstr = QString("SELECT "
                        "r.rolname as name, "
                        "r.rolsuper as super, "
                        "r.rolinherit as inherit, "
                        "r.rolcreaterole as createrole, "
                        "r.rolcreatedb as createdb, "
                        "r.rolcatupdate as catupdate, "
                        "r.rolcanlogin as canlogin, "
                        "r.rolconnlimit as connlimit, "
                        "r.rolvaliduntil as validuntil, "
                        "r.oid as id "
                   "FROM "
                        "pg_catalog.pg_roles r "
                   "WHERE "
                        "r.rolname = '%1';")
           .arg(mName);

#ifdef DEBUG_QUERY
    qDebug() << "Psql::Role::loadData> " << qstr;
#endif

    // if query failed
    if (!query.exec(qstr)) {
        qDebug() << query.lastError().text();
        return false;
    }

    // if query didn't retrieve a row
    if (!query.first()) {
        return false;
    }

    qint32 colId;

    // name is set at object construction
    /*colId= query.record().indexOf("name");
    Q_ASSERT(colId > 0);
    mName = query.value(colId).toString();*/

    colId = query.record().indexOf("super");
    Q_ASSERT(colId > 0);
    mIsSuperUser = query.value(colId).toBool();

    colId = query.record().indexOf("inherit");
    Q_ASSERT(colId > 0);
    mInheritsPrivileges = query.value(colId).toBool();

    colId = query.record().indexOf("createrole");
    Q_ASSERT(colId > 0);
    mCanCreateRole = query.value(colId).toBool();

    colId = query.record().indexOf("createdb");
    Q_ASSERT(colId > 0);
    mCanCreateDb = query.value(colId).toBool();

    colId = query.record().indexOf("catupdate");
    Q_ASSERT(colId > 0);
    mCanUpdateSysCat = query.value(colId).toBool();

    colId = query.record().indexOf("canlogin");
    Q_ASSERT(colId > 0);
    mCanLogin = query.value(colId).toBool();

    colId = query.record().indexOf("connlimit");
    Q_ASSERT(colId > 0);
    mConnectionLimit = query.value(colId).toInt();

    colId = query.record().indexOf("validuntil");
    Q_ASSERT(colId > 0);
    mExpiryDate = query.value(colId).toDate();

    colId = query.record().indexOf("id");
    Q_ASSERT(colId > 0);
    mId = query.value(colId).toInt();

    /* temporary debug output */
#if DEBUG_TRACE
    qDebug() << "Psql::Role::mName: " << mName;
    qDebug() << "Psql::Role::mIsSuperUser: " << mIsSuperUser;
    qDebug() << "Psql::Role::mInheritsPrivileges: " << mInheritsPrivileges;
    qDebug() << "Psql::Role::mCanCreateRole: " << mCanCreateRole;
    qDebug() << "Psql::Role::mCanCreateDb: " << mCanCreateDb;
    qDebug() << "Psql::Role::mCanUpdateSysCat: " << mCanUpdateSysCat;
    qDebug() << "Psql::Role::mCanLogin: " << mCanLogin;
    qDebug() << "Psql::Role::mConnectionLimit: " << mConnectionLimit;
    qDebug() << "Psql::Role::mExpiryDate: " << mExpiryDate;
    qDebug() << "Psql::Role::mId: " << mId;
#endif

    return DbRole::loadData();
}

} // namespace Psql

} // namespace DbObjects


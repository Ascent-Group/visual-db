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

#include <mysql/Schema.h>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>

#include <QtDebug>

namespace DbObjects
{
namespace Mysql
{

/*!
 * Constructor
 */
Schema::Schema(QString iName)
    : Common::DbSchema(iName)
{

}

/*!
 * Destructor
 */
Schema::~Schema()
{

}

/*!
 * \brief Load the definition of the given schema
 * \return true - If the loading succeeded
 * \return false - Otherwise
 */
bool
Schema::loadData()
{
    if (mIsLoaded) {
        return true;
    }

    QSqlQuery query(mDatabase->dbHandle());
    QString qstr;

    qstr = QString("MYSQL specific query");

#ifdef DEBUG_QUERY
    qDebug() << "Mysql::Schema::loadData> " << qstr;
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

    // get data from query
    int colId = 0;
    colId = query.record().indexOf("ownername");
    QString ownerName = query.value(colId).toString();

    colId = query.record().indexOf("description");
    QString description = query.value(colId).toString();

    using namespace DbObjects::Common;
    // find owner for scheme
    DbRolePtr dbRole = Database::instance()->findRole(ownerName);
//    Q_CHECK_PTR(dbRole);

    setOwner(dbRole);

    // set scheme description
    setDescription(description);

    return DbSchema::loadData();
}

} // namespace Mysql
} // namespace DbObjects


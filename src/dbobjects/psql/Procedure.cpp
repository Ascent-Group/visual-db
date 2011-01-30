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

#include <psql/Language.h>
#include <psql/Procedure.h>
#include <psql/Schema.h>
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
 * \param[in] iName - Name of the given procedure
 * \param[in] iSchema - Handle to schema containing the procedure
 */
Procedure::Procedure(QString iName, const DbSchemaPtr &iSchema)
    : DbProcedure(iName, iSchema)
{

}

/*
 * Dtor
 */
Procedure::~Procedure()
{

}

/*
 * Loads database proc's definition
 */
bool
Procedure::loadData()
{
    if (mIsLoaded) {
        return true;
    }

    QSqlDatabase db = QSqlDatabase::database("mainConnect");
    QSqlQuery query(db);
    QString qstr;

    // prepare a query
    qstr = QString("SELECT "
                        "p.proname AS name, "
                        "n.nspname AS schema, "
                        "o.rolname AS owner, "
                        "l.lanname AS lang, "
                        "p.prosrc AS src "
                    "FROM "
                        "pg_catalog.pg_proc p, "
                        "pg_catalog.pg_namespace n, "
                        "pg_catalog.pg_roles o, "
                        "pg_catalog.pg_language l "
                    "WHERE "
                        "p.pronamespace = n.oid "
                        "AND p.proowner = o.oid "
                        "AND p.prolang = l.oid "
                        "AND p.proname = '%1' "
                        "AND n.nspname = '%2';")
            .arg(mName)
            .arg(mSchema->name());

#ifdef DEBUG_QUERY
    qDebug() << "Procedure::loadData> " << qstr;
#endif

    // if query execution failed
    if (!query.exec(qstr)) {
        qDebug() << query.lastError().text();

        return false;;
    }

    // if record was not found
    if (!query.first()) {
        return false;
    }

    qint32 colId = query.record().indexOf("owner");
    Q_ASSERT(colId > 0);
    QString ownerName = query.value(colId).toString();
    mOwner = Common::Database::instance()->findRole(ownerName);

    colId = query.record().indexOf("lang");
    Q_ASSERT(colId > 0);
    QString languageName = query.value(colId).toString();
    mLanguage = Common::Database::instance()->findLanguage(languageName);

    colId = query.record().indexOf("src");
    Q_ASSERT(colId > 0);
    mSourceCode = query.value(colId).toString();

#if DEBUG_TRACE
    qDebug() << "Psql::Procedure::loadData> name = " << mName;
    qDebug() << "Psql::Procedure::loadData> schema = " << mSchema->name();
    qDebug() << "Psql::Procedure::loadData> owner = " << mOwner->name();
    qDebug() << "Psql::Procedure::loadData> lang = " << mLanguage->name();
    qDebug() << "Psql::Procedure::loadData> src = " << mSourceCode;
#endif

    return DbProcedure::loadData();
}

} // namespace Psql

} // namespace DbObjects


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

#include <DbSchema.h>
#include <PsqlLanguage.h>
#include <PsqlProcedure.h>
#include <PsqlRole.h>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>

#include <QtDebug>

/*
 * Ctor
 */
PsqlProcedure::PsqlProcedure(QString ipSchemaName, QString ipName)
    : DbProcedure(ipSchemaName, ipName)
{

}

/*
 * Dtor
 */
PsqlProcedure::~PsqlProcedure()
{

}

/*
 * Loads database proc's definition
 */
void
PsqlProcedure::loadData()
{
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
            .arg(mSchemaName);

#ifdef DEBUG_QUERY
    qDebug() << "PsqlProcedure::loadData> " << qstr;
#endif

    // if query execution failed
    if (!query.exec(qstr)) {
        qDebug() << query.lastError().text();

        return;
    }

    // if record was found
    if (query.first()) {

        qint32 colId = query.record().indexOf("owner");
        Q_ASSERT(colId > 0);
        QString ownerName = query.value(colId).toString();
        mOwner = Database::instance()->findRole(ownerName);

        colId = query.record().indexOf("lang");
        Q_ASSERT(colId > 0);
        QString languageName = query.value(colId).toString();
        mLanguage = Database::instance()->findLanguage(languageName);

        colId = query.record().indexOf("src");
        Q_ASSERT(colId > 0);
        mSourceCode = query.value(colId).toString();

#if DEBUG_TRACE
      qDebug() << "PsqlProcedure::loadData> name = " << mName;
      qDebug() << "PsqlProcedure::loadData> schema = " << mSchema->name();
      qDebug() << "PsqlProcedure::loadData> owner = " << mOwner->name();
      qDebug() << "PsqlProcedure::loadData> lang = " << mLanguage->name();
      qDebug() << "PsqlProcedure::loadData> src = " << mSourceCode;
#endif
    }

}


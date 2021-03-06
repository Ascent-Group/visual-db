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
#include <psql/Schema.h>
#include <psql/View.h>
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
 * \param[in] ipName - Name of the given view
 * \param[in] ipSchema - Handle to schema containing this view
 */
View::View(QString ipName, const DbSchemaPtr &ipSchema)
    : DbView(ipName, ipSchema)
{

}

/*!
 * Destructor
 */
View::~View()
{

}

/*!
 * \brief Load view's data
 *
 * \return true - If the view information has been successfully loaded
 * \return false - Otherwise
 */
bool
View::loadData()
{
    if (mIsLoaded) {
        return true;
    }

    QSqlDatabase db = QSqlDatabase::database("mainConnect");
    QSqlQuery query(db);
    QString qstr;

    // prepare a query
    qstr = QString("SELECT "
                        "v.schemaname as schema, "
                        "v.viewname as name, "
                        "v.viewowner as owner, "
                        "v.definition as def "
                    "FROM "
                        "pg_catalog.pg_views v "
                    "WHERE "
                        "schemaname = '%1' "
                        "AND viewname = '%2';")
            .arg(mSchema->name())
            .arg(mName);

#ifdef DEBUG_QUERY
    qDebug() << "View::loadData> " << qstr;
#endif

    // if query execution failed
    if (!query.exec(qstr)) {
        qDebug() << query.lastError().text();
        return false;
    }

    // if data was not found
    if (!query.first()) {
        return false;
    }

    qint32 colId = query.record().indexOf("schema");
    Q_ASSERT(colId >= 0);
//    mSchemaName = query.value(colId).toString();
//    mSchema->setName(mSchemaName);

//    colId = query.record().indexOf("name");
//    Q_ASSERT(colId > 0);
//    mName = query.value(colId).toString();

    colId = query.record().indexOf("owner");
    Q_ASSERT(colId > 0);
    QString ownerName = query.value(colId).toString();
    mOwner = Common::Database::instance()->findRole(ownerName);

    colId = query.record().indexOf("def");
    Q_ASSERT(colId > 0);
    mDefinition = query.value(colId).toString();

#if DEBUG_TRACE
    qDebug() << "Psql::View::loadData> name = " << mName;
    qDebug() << "Psql::View::loadData> owner = " << mOwner->name();
    qDebug() << "Psql::View::loadData> def = " << mDefinition;
#endif

    query.clear();

    // load column names
    // create query
    qstr = QString("SELECT "
                        "pga.attname "
                    "FROM "
                        "pg_catalog.pg_attribute pga "
                    "WHERE "
                        "pga.attnum > 0 "
                        "AND NOT pga.attisdropped "
                        "AND pga.attrelid = ("
                        "SELECT pgc.oid FROM pg_catalog.pg_class pgc "
                        "LEFT JOIN pg_catalog.pg_namespace pgn ON pgn.oid = pgc.relnamespace "
                        "WHERE pgc.relname = '%1' and pgn.nspname = '%2'); ")
            .arg(mName)
            .arg(mSchema->name());

    // if query execution failed
    if (!query.exec(qstr)) {
        qDebug() << query.lastError().text();
        return false;
    }

    // if columns wer found
    if (query.first()) {
        do {
            mColumnsNames.push_back(query.value(0).toString());
            qDebug() << "Psql::View: column name = " << query.value(0).toString();
        } while (query.next());
    }

    return DbView::loadData();
}

} // namespace Psql

} // namespace DbObjects


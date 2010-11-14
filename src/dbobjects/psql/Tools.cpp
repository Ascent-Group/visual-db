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

#include <psql/Tools.h>

#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QString>
#include <QVariant>

#include <QtDebug>

namespace DbObjects
{

namespace Psql
{

namespace Tools
{

/*!
 * \todo Implement
 * \brief Detects the version of PostgreSQL
 * \return The version of PostgreSQL that is currently used
 */
Tools::Version
version()
{
    QSqlDatabase db = QSqlDatabase::database("mainConnect");
    QSqlQuery query(db);
    QString qstr("SELECT version();");

    if (!query.exec(qstr) || !query.first()) {
        return Tools::PostgreSQL_Unknown;
    }

    QString versionString = query.value(0).toString();
    /* \todo detect the version */

    return Tools::PostgreSQL_Unknown;
}

/*!
 * \brief Read schemas that are available in the database
 *
 * \param[out] opList - The list that will containt schemas' names
 *
 * \return The number of schemas read.
 */
quint32
schemasList(QStringList &opList)
{
    QString qstr = QString("SELECT "
                               "nspname AS name "
                           "FROM "
                               "pg_catalog.pg_namespace pgn;");

    return objectNamesList(qstr, opList);
}

/*!
 * \brief Read indices that are in available in the database
 *
 * \param[out] opList - The list that will contain indices' names
 *
 * \return The number of indices read by the query
 */
quint32
indicesList(QStringList &opList)
{
    QString qstr = QString("SELECT "
                                "index.relname as name "
                            "FROM "
                                "pg_catalog.pg_index pgi, "
                                "pg_catalog.pg_class index, "
                                "pg_catalog.pg_namespace pgn, "
                                "pg_catalog.pg_class rel "
                            "WHERE "
                                "pgi.indrelid = rel.oid "
                                "AND pgn.oid = rel.relnamespace "
                                "AND pgi.indexrelid = index.oid;");

    return objectNamesList(qstr, opList);
}

/*!
 * Read names of all languages in the given schema
 *
 * \param[out] opList - List that will contain names of languages
 *
 * \return The number of languages' names read from db
 */
quint32
languagesList(QStringList &opList)
{
    QString qstr("SELECT "
                    "l.lanname AS name "
                "FROM "
                    "pg_catalog.pg_language l;");

    return objectNamesList(qstr, opList);
}

/*!
 * Read names of all procedures in the given schema
 *
 * \param[in] ipSchemaName - The name of schema
 * \param[out] opList - List that will contain names of procedures
 *
 * \return The number of procedures' names read from db
 */
quint32
proceduresList(const QString &ipSchemaName, QStringList &opList)
{
    QString qstr = QString("SELECT "
                                "p.proname AS name "
                           "FROM "
                                "pg_catalog.pg_proc p, "
                                "pg_catalog.pg_namespace n "
                           "WHERE "
                                "p.pronamespace = n.oid "
                                "AND n.nspname = '%1';")
        .arg(ipSchemaName);

    return objectNamesList(qstr, opList);
}

/*!
 * \brief Read roles that are in available in the database
 *
 * \param[out] opList - The list that will contain roles' names
 *
 * \return The number of roles read by the query
 */
quint32
rolesList(QStringList &opList)
{
    QString qstr = QString("SELECT "
                               "r.rolname as name "
                           "FROM "
                               "pg_catalog.pg_roles r;");

    return objectNamesList(qstr, opList);
}

/*!
 * \brief Reads thelist of objects' names
 *
 * \note The functions looks for a field by name 'name' !!!
 *
 * \param[in] ipQstr - The query used to select names of objects
 * \param[out] opList - List that will hold objects' names
 *
 * \return The number of names in the list
 */
quint32
objectNamesList(const QString &ipQstr, QStringList &opList)
{
#if DEBUG_QUERY
    qDebug() << "DbObjects::Psql::Tools::objectNamesList> " << ipQstr;
#endif

    QSqlDatabase db = QSqlDatabase::database("mainConnect");
    QSqlQuery query(db);

    // if query failed
    if (!query.exec(ipQstr)) {
        qDebug() << query.lastError().text();
        return 0;
    }

    // if query didn't retrieve a row
    if (!query.first()) {
        return 0;
    }

    quint32 count = 0;
    qint32 colId = query.record().indexOf("name");

    do {
        opList.append(query.value(colId).toString());
        ++count;
    } while (query.next());

    // we return count instead of opList.count() because opList might come not empty to
    // this function (since it is passed by reference
    return count;
}

/*!
 * Read names of all tables in the given schema
 *
 * \param[in] ipSchemaName - The name of schema
 * \param[out] opList - List that will contain names of tables
 *
 * \return The number of tables' names read from db
 */
quint32
tablesList(const QString &ipSchemaName, QStringList &opList)
{
    QString qstr = QString("SELECT tablename AS name "
                           "FROM pg_catalog.pg_tables pgt "
                           "WHERE schemaname='%1';")
        .arg(ipSchemaName);

    return objectNamesList(qstr, opList);
}

/*!
 * Read names of all triggers in the given schema
 *
 * \param[in] ipSchemaName - The name of schema
 * \param[out] opList - List that will contain names of triggers
 *
 * \return The number of triggers' names read from db
 */
quint32
triggersList(const QString &ipSchemaName, QStringList &opList)
{
    QString qstr = QString("SELECT "
                    "tbl_nsp.nspname AS schema, "
                    "tbl.relname AS table, "
                    "t.tgname AS name, "
                    "proc_nsp.nspname AS proc_schema, "
                    "proc.proname AS proc, "
                    "t.tgenabled AS enabled, "
                    "t.tgisconstraint AS isconstraint, "
                    "t.tgconstrname AS constrname, "
                    "ref_tbl_nsp.nspname AS ref_schema, "
                    "ref_tbl.relname AS ref_table, "
                    "t.tgdeferrable AS deferrable, "
                    "t.tginitdeferred AS initdeferred, "
                    "t.tgnargs AS nargs "
                "FROM "
                    "pg_catalog.pg_trigger t, "
                    "pg_catalog.pg_class tbl, "
                    "pg_catalog.pg_class ref_tbl, "
                    "pg_catalog.pg_namespace tbl_nsp, "
                    "pg_catalog.pg_namespace ref_tbl_nsp, "
                    "pg_catalog.pg_proc proc, "
                    "pg_catalog.pg_namespace proc_nsp "
                "WHERE "
                    "tbl.oid = t.tgrelid "
                    "AND tbl.relnamespace = tbl_nsp.oid "
                    "AND t.tgfoid = proc.oid "
                    "AND proc.pronamespace = proc_nsp.oid "
                    "AND ref_tbl.oid = t.tgconstrrelid "
                    "AND tbl_nsp.nspname = '%1' "
                    //"AND ref_tbl_nsp.nspname NOT LIKE 'pg_%' "
                    //"AND proc_nsp.nspname NOT LIKE 'pg_%' "
                    "AND ref_tbl.relnamespace = ref_tbl_nsp.oid;")
                .arg(ipSchemaName);

    return objectNamesList(qstr, opList);
}

/*!
 * Read names of all views in the given schema
 *
 * \param[in] ipSchemaName - The name of schema
 * \param[out] opList - List that will contain names of views
 *
 * \return The number of views' names read from db
 */
quint32
viewsList(const QString &ipSchemaName, QStringList &opList)
{
    QString qstr = QString("SELECT "
                                "v.viewname as name "
                           "FROM "
                                "pg_catalog.pg_views v "
                           "WHERE "
                                "schemaname = '%1';")
        .arg(ipSchemaName);

    return objectNamesList(qstr, opList);
}

} // namespace Tools

} // namespace Psql

} // namespace DbObjects


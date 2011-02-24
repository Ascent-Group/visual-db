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

//#include <QString>
#include <QSqlQuery>
#include <QVariant>

#include <QtDebug>

namespace DbObjects
{
namespace Psql
{

/*!
 * Constructor
 */
Tools::Tools()
    : Common::Tools()
{
}

/*!
 *
 */
Tools::~Tools()
{
}

/*!
 * \todo Implement
 * \brief Detects the version of PostgreSQL
 * \return The version of PostgreSQL that is currently used
 */
Common::Database::DBMSVersion
Tools::version(const QSqlDatabase &iDbHandle)
{
    QSqlQuery query(iDbHandle);
    QString qstr("SELECT version();");

    using namespace DbObjects::Common;

    if (!query.exec(qstr) || !query.first()) {
        return Database::PostgreSQL_Unknown;
    }

    QString versionString = query.value(0).toString();
    query.clear();

    versionString.remove(0, versionString.indexOf(" ") + 1);
    versionString.remove(versionString.indexOf("."), versionString.length() - versionString.indexOf("."));

    if ("9" == versionString) {
        return Database::PostgreSQL_9;
    } else if ("8" == versionString) {
        return Database::PostgreSQL_8;
    }

    return Database::PostgreSQL_Unknown;
}

/*!
 * \brief Read schemas that are available in the database
 *
 * \param[out] oList - The list that will containt schemas' names
 *
 * \return The number of schemas read.
 */
quint32
Tools::schemasList(const QSqlDatabase &iDbHandle, QStringList &oList)
{
    QString qstr = QString("SELECT "
                               "nspname AS name "
                           "FROM "
                               "pg_catalog.pg_namespace pgn;");

    return objectNamesList(iDbHandle, qstr, oList);
}

/*!
 * \brief Read indices that are in available in the database
 *
 * \param[out] oList - The list that will contain indices' names
 *
 * \return The number of indices read by the query
 */
quint32
Tools::indicesList(const QSqlDatabase &iDbHandle, QStringList &oList)
{
    QString qstr = QString("SELECT "
                                "pgi.indexname as name "
                            "FROM "
                                "pg_catalog.pg_indexes pgi;");
//                            "WHERE "
//                                "pgi.schemaname = '%1' "

    return objectNamesList(iDbHandle, qstr, oList);
}

/*!
 * Read names of all languages in the given schema
 *
 * \param[out] oList - List that will contain names of languages
 *
 * \return The number of languages' names read from db
 */
quint32
Tools::languagesList(const QSqlDatabase &iDbHandle, QStringList &oList)
{
    QString qstr("SELECT "
                    "l.lanname AS name "
                "FROM "
                    "pg_catalog.pg_language l;");

    return objectNamesList(iDbHandle, qstr, oList);
}

/*!
 * Read names of all procedures in the given schema
 *
 * \param[in] iSchemaName - The name of schema
 * \param[out] oList - List that will contain names of procedures
 *
 * \return The number of procedures' names read from db
 */
quint32
Tools::proceduresList(const QSqlDatabase &iDbHandle, const QString &iSchemaName, QStringList &oList)
{
    QString qstr = QString("SELECT "
                                "p.proname AS name "
                           "FROM "
                                "pg_catalog.pg_proc p, "
                                "pg_catalog.pg_namespace n "
                           "WHERE "
                                "p.pronamespace = n.oid "
                                "AND n.nspname = '%1';")
        .arg(iSchemaName);

    return objectNamesList(iDbHandle, qstr, oList);
}

/*!
 * \brief Read roles that are in available in the database
 *
 * \param[out] oList - The list that will contain roles' names
 *
 * \return The number of roles read by the query
 */
quint32
Tools::rolesList(const QSqlDatabase &iDbHandle, QStringList &oList)
{
    QString qstr = QString("SELECT "
                               "r.rolname as name "
                           "FROM "
                               "pg_catalog.pg_roles r;");

    return objectNamesList(iDbHandle, qstr, oList);
}

/*!
 * Read names of all tables in the given schema
 *
 * \param[in] iSchemaName - The name of schema
 * \param[out] oList - List that will contain names of tables
 *
 * \return The number of tables' names read from db
 */
quint32
Tools::tablesList(const QSqlDatabase &iDbHandle, const QString &iSchemaName, QStringList &oList)
{
    QString qstr = QString("SELECT tablename AS name "
                           "FROM pg_catalog.pg_tables pgt "
                           "WHERE schemaname='%1';")
        .arg(iSchemaName);

    return objectNamesList(iDbHandle, qstr, oList);
}

/*!
 * Read names of all triggers in the given schema
 *
 * \param[in] iSchemaName - The name of schema
 * \param[out] oList - List that will contain names of triggers
 *
 * \return The number of triggers' names read from db
 */
quint32
Tools::triggersList(const QSqlDatabase &iDbHandle, const QString &iSchemaName, QStringList &oList)
{
    QString qstr = QString("SELECT "
                               "t.tgname AS name "
                           "FROM "
                               "pg_catalog.pg_trigger t, "
                               "pg_catalog.pg_class tbl, "
                               "pg_catalog.pg_namespace tbl_nsp "
                           "WHERE "
                               "tbl.oid = t.tgrelid "
                               "AND tbl.relnamespace = tbl_nsp.oid "
                               "AND tbl_nsp.nspname = '%1';")
                .arg(iSchemaName);

    return objectNamesList(iDbHandle, qstr, oList);
}

/*!
 * Read names of all views in the given schema
 *
 * \param[in] iSchemaName - The name of schema
 * \param[out] oList - List that will contain names of views
 *
 * \return The number of views' names read from db
 */
quint32
Tools::viewsList(const QSqlDatabase &iDbHandle, const QString &iSchemaName, QStringList &oList)
{
    QString qstr = QString("SELECT "
                                "v.viewname as name "
                           "FROM "
                                "pg_catalog.pg_views v "
                           "WHERE "
                                "schemaname = '%1';")
        .arg(iSchemaName);

    return objectNamesList(iDbHandle, qstr, oList);
}

} // namespace Psql
} // namespace DbObjects


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

#include <common/Database.h>
#include <psql/Table.h>
#include <psql/Tools.h>
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

/*
 * Constructor
 */
Table::Table(QString ipSchemaName, QString ipTableName)
    :DbTable(ipSchemaName, ipTableName)
{
    // load column definitions
    loadData();
}

/*
 * Destructor
 */
Table::~Table()
{

}

/*
 * Load column definitions data
 */
bool
Table::loadData()
{
    QSqlDatabase db = QSqlDatabase::database("mainConnect");
    QSqlQuery query(db);
    QString qstr;

    /*! \todo Before creating a query we need to find out postgres version;
     *        Bersion check can help support different Postgres versions,
     *        especcialy when the system catalogs change in newer versions
     */
    Tools::Version version = Tools::version();

    /*! \todo version check */
    /*! \todo do version specific actions */

    // create query
    qstr = QString("SELECT "
                        "pga.attname, "
                        "pg_catalog.format_type(pga.atttypid, pga.atttypmod), "
                        "pga.attnotnull "
                    "FROM "
                        "pg_catalog.pg_attribute pga "
                    "WHERE "
                        "pga.attnum > 0 "
                        "AND NOT pga.attisdropped "
                        "AND pga.attrelid = ("
                        "SELECT pgc.oid FROM pg_catalog.pg_class pgc "
                        "LEFT JOIN pg_catalog.pg_namespace pgn ON pgn.oid = pgc.relnamespace "
                        "WHERE pgc.relname = '%1' and pgn.nspname = '%2'); ")
//          "AND pg_catalog.pg_table_is_visible(pgc.oid));")
        .arg(mName)
        .arg(mSchemaName);

#ifdef DEBUG_QUERY
    qDebug() << "Psql::Table::loadData> " << qstr;
#endif

    // if query exectuion failed
    if (!query.exec(qstr)) {
        qDebug() << query.lastError().text();
        return false;
    }

    // if query result is empty
    if (!query.first()) {
        return false;
    }

    // for each retrieved row
    do {
        /* temporary debug output */
#if DEBUG_TRACE
        qDebug() << query.value(0).toString();
        qDebug() << query.value(1).toString();
        qDebug() << query.value(2).toString();
#endif

        // create column definition
        struct ColumnDefinition cDef;

        // populate column definition
        cDef.name = query.value(0).toString();
        cDef.type = query.value(1).toString();
        cDef.isNullable = !query.value(2).toBool();
        cDef.isPrimaryKey = checkPrimaryKey(cDef.name);
        cDef.isForeignKey = checkForeignKey(cDef.name, &cDef.foreignSchemaName, &cDef.foreignTableName, &cDef.foreignFieldNames);
        cDef.isUnique = checkUnique(cDef.name);

        /* temporary debug output */
#if DEBUG_TRACE
        qDebug() << "NAME: " << cDef.name;
        qDebug() << "isPrimaryKey: " << cDef.isPrimaryKey;
        qDebug() << "isForeignKey: " << cDef.isForeignKey;
        qDebug() << "isUnique: " << cDef.isUnique;
        qDebug() << "FKEY_SCHEMA: " << cDef.foreignSchemaName;
        qDebug() << "FKEY_TABLE: " << cDef.foreignTableName;
#endif

        // add column definition
        mColumnDefs.push_back(cDef);
    } while (query.next());

    return true;
}

/*
 * Checks if column is a primary key for the given table
 */
bool
Table::checkPrimaryKey(const QString &ipColumnName) const
{
    QSqlDatabase db = QSqlDatabase::database("mainConnect");
    QSqlQuery query(db);

    // create query
    QString qstr = QString("SELECT "
                        "pgc.relname, "
                        "pga.attname "
                    "FROM "
                        "pg_class pgc, "
                        "pg_attribute pga, "
                        "pg_index pgi, "
                        "pg_namespace pgn "
                    "WHERE "
                        "pgc.oid = pga.attrelid "
                        "AND pgc.oid = pgi.indrelid "
                        "AND pgi.indkey[0] = pga.attnum "
                        "AND pgi.indisprimary = 't' "
                        "AND pgn.oid = pgc.relnamespace "
                        "AND pgn.nspname = '%1' "
                        "AND pgc.relname = '%2' "
                        "AND pga.attname = '%3';")
                .arg(mSchemaName)
                .arg(mName)
                .arg(ipColumnName);

#ifdef DEBUG_QUERY
    qDebug() << "Psql::Table::checkPrimaryKey> " << qstr;
#endif

    // if query execution failed
    if (!query.exec(qstr)) {
        qDebug() << query.lastError().text();

        return false;
    }

    // primary constraint definition found
    return query.first();
}

/*
 * Checks if ipColumnName is a foreign key for ipTableName
 */
bool
Table::checkForeignKey(const QString &ipColumnName,
               QString *opForeignSchemaName,
               QString *opForeignTableName,
               QStringList *opForeignFieldsNames) const
{
    QSqlDatabase db = QSqlDatabase::database("mainConnect");
    QSqlQuery query(db);

    // create query
    QString qstr = QString("SELECT "
                                "p1.relname, "
                                "a1.attname, "
                                "pgn2.nspname AS foreign_schema, "
                                "p2.relname AS reltable, "
                                "a2.attname AS ref_field "
                            "FROM "
                                "pg_constraint c, "
                                "pg_class p1, "
                                "pg_class p2, "
                                "pg_attribute a1, "
                                "pg_attribute a2, "
                                "pg_namespace pgn1, "
                                "pg_namespace pgn2 "
                            "WHERE "
                                "c.contype = 'f' "
                                "AND c.confrelid > 0 "
                                "AND c.conrelid = p1.oid "
                                "AND c.confrelid = p2.oid "
                                "AND c.conrelid = a1.attrelid "
                                "AND a1.attnum = ANY (c.conkey) "
                                "AND c.confrelid = a2.attrelid "
                                "AND a2.attnum = ANY (c.confkey) "
                                "AND p2.relnamespace = pgn2.oid "
                                "AND pgn1.oid = p1.relnamespace "
                                "AND pgn1.nspname = '%1' "
                                "AND p1.relname = '%2' "
                                "AND a1.attname = '%3';")
        .arg(mSchemaName)
        .arg(mName)
        .arg(ipColumnName);

#ifdef DEBUG_QUERY
    qDebug() << "Psql::Table::checkForeignKey> " << qstr;
#endif

    // if query execution failed
    if (!query.exec(qstr)) {
        qDebug() << query.lastError().text();

        return false;
    }

    bool flag = false;
    qint32 colId;

    // if query result is empty
    if (!query.first()) {
    return flag;
    }

    // if we are here then the result is not empty
    flag = true;

    // this needs to be don only once for all fields
    colId = query.record().indexOf("reltable");
    *opForeignTableName = query.value(colId).toString();

    colId = query.record().indexOf("foreign_schema");
    *opForeignSchemaName = query.value(colId).toString();

    // fields themselves
    do {
        colId = query.record().indexOf("ref_field");
        opForeignFieldsNames->push_back(query.value(colId).toString());
    } while (query.next());

    // foreign key constraint definition found
    return flag;
}

/*
 * Checks if ipColumnName is unique for ipTableName
 */
bool
Table::checkUnique(const QString &ipColumnName) const
{
    QSqlDatabase db = QSqlDatabase::database("mainConnect");
    QSqlQuery query(db);

    // create query
    QString qstr = QString("SELECT "
                                "pgc.relname, "
                                "pga.attname, "
                "pgc.relnamespace "
                            "FROM "
                                "pg_constraint constr, "
                                "pg_class pgc, "
                                "pg_attribute pga, "
                                "pg_namespace pgn "
                            "WHERE "
                                "pga.attnum = ANY (constr.conkey) "
                                "AND constr.contype = 'u' "
                                "AND constr.conrelid = pgc.oid "
                                "AND constr.conrelid = pga.attrelid "
                                "AND pgn.oid = pgc.relnamespace "
                                "AND pgn.nspname = '%1' "
                                "AND pgc.relname = '%2' "
                                "AND pga.attname = '%3';")
        .arg(mSchemaName)
        .arg(mName)
        .arg(ipColumnName);

#if DEBUG_QUERY
    qDebug() << "Psql::Table::checkUnique> " << qstr;
#endif

    // if query execution failed
    if (!query.exec(qstr)) {
        qDebug() << query.lastError().text();

        return false;
    }

    // unique constraint definition found
    return query.first();
}

/*!
 * \todo Implement
 */
void
Table::resetData()
{

}

} // namespace Psql

} // namespace DbObjects

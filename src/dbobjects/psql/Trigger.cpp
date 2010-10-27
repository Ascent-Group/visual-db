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

#include <psql/Schema.h>
#include <psql/Trigger.h>
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
Trigger::Trigger(QString ipName, Common::DbSchema *ipSchema)
    : DbTrigger(ipName, ipSchema)
{

}

/*
 * Dtor
 */
Trigger::~Trigger()
{

}

/*
 * Loads trigger definition
 */
bool
Trigger::loadData()
{
    QSqlDatabase db = QSqlDatabase::database("mainConnect");
    QSqlQuery query(db);
    QString qstr;

    // prepare query
    qstr = QString("SELECT "
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
                        "AND ref_tbl.relnamespace = ref_tbl_nsp.oid "
                        "AND tbl_nsp.nspname = '%2' "
                        //"AND tbl_nsp.nspname NOT LIKE 'pg_%' "
                        //"AND ref_tbl_nsp.nspname NOT LIKE 'pg_%' "
                        //"AND proc_nsp.nspname NOT LIKE 'pg_%' "
                        "AND t.tgname = '%1';")
            .arg(mName)
            .arg(mSchema->name());

#ifdef DEBUG_QUERY
    qDebug() << "Trigger::loadData> " << qstr;
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

    // table
    qint32 colId = query.record().indexOf("schema");
    Q_ASSERT(colId > 0);
    QString schemaName = query.value(colId).toString();

    colId = query.record().indexOf("table");
    Q_ASSERT(colId > 0);
    QString tableName = query.value(colId).toString();

    Common::DbTable *table = Common::Database::instance()->findSchema(schemaName)->findTable(tableName);

    setTable(table);

    // proc
    colId = query.record().indexOf("proc_schema");
    Q_ASSERT(colId > 0);
    QString procSchemaName = query.value(colId).toString();

    colId = query.record().indexOf("proc");
    Q_ASSERT(colId > 0);
    QString procName = query.value(colId).toString();

    Common::DbProcedure *proc = Common::Database::instance()->findSchema(procSchemaName)->findProcedure(procName);

    setProcedure(proc);

    // enabled
    colId = query.record().indexOf("enabled");
    Q_ASSERT(colId > 0);
    mEnabled = query.value(colId).toChar();

    // isconstraint
    colId = query.record().indexOf("isconstraint");
    Q_ASSERT(colId > 0);
    mIsConstraint = query.value(colId).toBool();

    // constrname
    colId = query.record().indexOf("constrname");
    Q_ASSERT(colId > 0);
    mConstraintName = query.value(colId).toString();

    // ref table
    colId = query.record().indexOf("ref_schema");
    Q_ASSERT(colId > 0);
    QString refSchemaName = query.value(colId).toString();

    colId = query.record().indexOf("ref_table");
    Q_ASSERT(colId > 0);
    QString refTableName = query.value(colId).toString();

    Common::DbTable *refTable = Common::Database::instance()->findSchema(refSchemaName)->findTable(refTableName);

    setReferencedTable(refTable);

    // deferrable
    colId = query.record().indexOf("deferrable");
    Q_ASSERT(colId > 0);
    mIsDeferrable = query.value(colId).toBool();

    // initdeferred
    colId = query.record().indexOf("initdeferred");
    Q_ASSERT(colId > 0);
    mIsInitiallyDeferred = query.value(colId).toBool();

    // nargs
    colId = query.record().indexOf("nargs");
    Q_ASSERT(colId > 0);
    mNumArgs = query.value(colId).toInt();

#if DEBUG_TRACE
    qDebug() << "Psql::Trigger::loadData> name = " << mName;
    qDebug() << "Psql::Trigger::loadData> table = " << mTable->name();
    qDebug() << "Psql::Trigger::loadData> proc = " << mProcedure->name();
    qDebug() << "Psql::Trigger::loadData> enabled = " << mEnabled;
    qDebug() << "Psql::Trigger::loadData> isConstraint = " << mIsConstraint;
    qDebug() << "Psql::Trigger::loadData> constraintName = " << mConstraintName;
    qDebug() << "Psql::Trigger::loadData> ref_table = " << mReferencedTable->name();
    qDebug() << "Psql::Trigger::loadData> isDeferrable = " << mIsDeferrable;
    qDebug() << "Psql::Trigger::loadData> isInitiallyDeferred = " << mIsInitiallyDeferred;
    qDebug() << "Psql::Trigger::loadData> nargs = " << mNumArgs;
#endif

    return DbTrigger::loadData();
}

} // namespace Psql

} // namespace DbObjects


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
#include <mysql/Tools.h>
#include <mysql/Trigger.h>
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
 *
 * \param[in] iName - Name of the given view
 * \param[in] iSchema - Handle to schema containing this view

 */
Trigger::Trigger(QString iName, const DbSchemaPtr &iSchema)
    : DbTrigger(iName, iSchema)
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
//    qDebug() << "Mysql::Trigger::loadData> " << this;
    if (mIsLoaded) {
        return DbTrigger::loadData();
    }

    QSqlDatabase db = mDatabase->dbHandle();
    QSqlQuery query(db);
    QString qstr;

    using namespace DbObjects::Common;
    // \todo how are we going to use tools here?
    Database::DBMSVersion version = mDatabase->version();

    switch (version) {
        case Database::MySQL_5:
                // prepare query
                qstr = QString("MySQL specific query 1");
                break;

        case Database::MySQL_Unknown:
                qstr = QString("MySQL specific query 1");
                break;

        default:
                return false;
    }

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

    using namespace DbObjects::Common;
    // schema proxy for intermediate storing of parent schemas
    DbSchemaPtr schema;

    // table
    qint32 colId = query.record().indexOf("schema");
    Q_ASSERT(colId >= 0);
    QString schemaName = query.value(colId).toString();

    schema = mDatabase->findSchema(schemaName);

    if (schema.valid()) {
        colId = query.record().indexOf("table");
        Q_ASSERT(colId > 0);
        QString tableName = query.value(colId).toString();

//        qDebug() << "Mysql::Trigger::loadData> detecting table... " << tableName;

        DbTablePtr table = schema->findTable(tableName);

//        qDebug() << "Mysql::Trigger::loadData> table = " << table.get();
        setTable(table);
    }

    // proc
    colId = query.record().indexOf("proc_schema");
    Q_ASSERT(colId > 0);
    QString procSchemaName = query.value(colId).toString();

    schema = mDatabase->findSchema(procSchemaName);

    if (schema.valid()) {
        colId = query.record().indexOf("proc");
        Q_ASSERT(colId > 0);
        QString procName = query.value(colId).toString();

        DbProcedurePtr proc = schema->findProcedure(procName);

        setProcedure(proc);
    }

    // enabled
    colId = query.record().indexOf("enabled");
    Q_ASSERT(colId > 0);
    // \note toChar doesn't get 'O' properly. Junk is stored in mEnabled. =>
    // toString().at(0) workaround is used.
//    mEnabled = query.value(colId).toChar();
    mEnabled = query.value(colId).toString().at(0);

    // isconstraint
    colId = query.record().indexOf("isconstraint");
    Q_ASSERT(colId > 0);
    mIsConstraint = query.value(colId).toBool();

    if (mIsConstraint) {
        // constrname
        colId = query.record().indexOf("constrname");
        Q_ASSERT(colId > 0);
        mConstraintName = query.value(colId).toString();
    }

    // ref table
    colId = query.record().indexOf("ref_schema");
    Q_ASSERT(colId > 0);
    QString refSchemaName = query.value(colId).toString();

    schema = mDatabase->findSchema(refSchemaName);

    if (schema.valid()) {
        colId = query.record().indexOf("ref_table");
        Q_ASSERT(colId > 0);
        QString refTableName = query.value(colId).toString();

        Q_ASSERT(!refTableName.isEmpty());

        DbTablePtr refTable = schema->findTable(refTableName);
        setReferencedTable(refTable);
    }

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
    qDebug() << "Mysql::Trigger::loadData> name = " << mName;
    qDebug() << "Mysql::Trigger::loadData> table = " << mTable->name();
    qDebug() << "Mysql::Trigger::loadData> proc = " << mProcedure->name();
    qDebug() << "Mysql::Trigger::loadData> enabled = " << mEnabled.digitValue();
    qDebug() << "Mysql::Trigger::loadData> isConstraint = " << mIsConstraint;
    qDebug() << "Mysql::Trigger::loadData> constraintName = " << mConstraintName;
    qDebug() << "Mysql::Trigger::loadData> ref_table = " << mReferencedTable->name();
    qDebug() << "Mysql::Trigger::loadData> isDeferrable = " << mIsDeferrable;
    qDebug() << "Mysql::Trigger::loadData> isInitiallyDeferred = " << mIsInitiallyDeferred;
    qDebug() << "Mysql::Trigger::loadData> nargs = " << mNumArgs;
#endif

    return DbTrigger::loadData();
}

} // namespace Mysql
} // namespace DbObjects


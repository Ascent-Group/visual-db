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

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>
#include <common/DbSchema.h>
#include <factory/Procedure.h>
#include <factory/Trigger.h>
#include <factory/View.h>
#include <mysql/Table.h>
#include <psql/Language.h>
#include <psql/Procedure.h>
#include <psql/Role.h>
#include <psql/Table.h>
#include <psql/Tools.h>
#include <psql/Trigger.h>
#include <psql/View.h>

#include <QtDebug>

namespace DbObjects
{

namespace Common
{

/*!
 * Constructor
 *
 * On creation automatically adds newly created schema object to
 * a Database's vector of schemas
 */
DbSchema::DbSchema(QString ipName, DbRole *ipOwner)
    :DbObject(ipName),
     mOwner(ipOwner)
{
}

/*!
 * Destructor
 */
DbSchema::~DbSchema()
{
    resetData();
}

/*!
 * \brief Add table to a schema
 *
 * \param[in] ipTable
 *
 * \return true - If the table object has been added
 * \return false - If the table object has alread existed in the vector
 */
bool
DbSchema::addTable(DbTable *ipTable)
{
    if (mTables.contains(ipTable)) {
        return false;
    }

    mTables.push_back(ipTable);
    return true;
}

/*!
 * \brief Add view to a schema
 *
 * \param[in] ipView
 *
 * \return true - If the view object has been added
 * \return false - If the view object has already existed in the vector
 */
bool
DbSchema::addView(DbView *ipView)
{
    if (mViews.contains(ipView)) {
        return false;
    }

    mViews.push_back(ipView);
    return true;
}

/*!
 * \brief Add proc to a schema
 *
 * \param[in] ipProc
 *
 * \return true - If the proc object has been added
 * \return false - If the proc object has already existed in the vector
 */
bool
DbSchema::addProcedure(DbProcedure *ipProc)
{
    if (mProcedures.contains(ipProc)) {
        return false;
    }

    mProcedures.push_back(ipProc);
    return true;
}

/*!
 * \brief Add trig to a schema
 *
 * \param[in] ipTrig
 *
 * \return true - If the trigger object has been added
 * \return false - If the trigger object has laready existed in the vector
 */
bool
DbSchema::addTrigger(DbTrigger *ipTrig)
{
    if (mTriggers.contains(ipTrig)) {
        return false;
    }

    mTriggers.push_back(ipTrig);
    return true;
}

/*!
 * Get a list of tables' names
 *
 * \param[out] opList - List of tables' names
 */
void
DbSchema::tablesList(QStringList *opList) const
{
    if (0 == opList) {
        return;
    }

    opList->clear();

    QVector<DbTable*>::const_iterator iter;

    for (iter = mTables.constBegin(); iter != mTables.constEnd(); ++iter) {
        opList->append((*iter)->name());
    }

    //opList->sort();
}

/*!
 * Calculate the number of tables in the given schema
 *
 * \return Number of tables currently stored in the schema
 */
quint64
DbSchema::tablesCount() const
{
    return mTables.count();
}

/*!
 * Get a list of views' names
 *
 * \param[out] opList - List of views' names
 */
void
DbSchema::viewsList(QStringList *opList) const
{
    if (0 == opList) {
        return;
    }

    opList->clear();

    QVector<DbView*>::const_iterator iter;

    for (iter = mViews.constBegin(); iter != mViews.constEnd(); ++iter) {
        opList->append((*iter)->name());
    }

    //opList->sort();
}

/*!
 * Calculate the number of views in the given schema
 *
 * \return Number of views currently stored in the schema
 */
quint64
DbSchema::viewsCount() const
{
    return mViews.count();
}

/*!
 * Get a list of procs' names
 *
 * \param[out] opList - List of procs' names
 */
void
DbSchema::proceduresList(QStringList *opList) const
{
    if (0 == opList) {
        return;
    }

    opList->clear();

    QVector<DbProcedure*>::const_iterator iter;

    for (iter = mProcedures.constBegin(); iter != mProcedures.constEnd(); ++iter) {
        opList->append((*iter)->name());
    }

    //opList->sort();
}

/*!
 * Get a list of triggers' names
 *
 * \param[out] opList - List of triggers' names
 */
void
DbSchema::triggersList(QStringList *opList) const
{
    if (0 == opList) {
        return;
    }

    opList->clear();

    QVector<DbTrigger*>::const_iterator iter;

    for (iter = mTriggers.constBegin(); iter != mTriggers.constEnd(); ++iter) {
        opList->append((*iter)->name());
    }

    //opList->sort();
}

/*!
 * Calculate the number of procs in the given schema
 *
 * \return Number of procs currently stored in the schema
 */
quint64
DbSchema::proceduresCount() const
{
    return mProcedures.count();
}

/*!
 * Calculate the number of triggers in the given schema
 *
 * \return Number of triggers currently stored in the schema
 */
quint16
DbSchema::triggersCount() const
{
    return mTriggers.count();
}

/*!
 * Find table by its name
 *
 * \param[in] ipTableName - Name of the table we are looking for
 *
 * \return Handle to the found table or NULL is not found.
 */
DbTable*
DbSchema::findTable(const QString &ipTableName) const
{
    return dynamic_cast<DbTable*>(findObject(ipTableName, DbObject::TableObject));
}

/*!
 * Find view by its name
 *
 * \param[in] ipViewName - Name of the view we are looking for
 *
 * \return Handle to the found view or NULL is not found.
 */
DbView*
DbSchema::findView(const QString &ipViewName) const
{
    return dynamic_cast<DbView*>(findObject(ipViewName, DbObject::ViewObject));
}

/*!
 * Find procedure by its name
 *
 * \param[in] ipProcName - Name of the procedure we are looking for
 *
 * \return Handle to the found procedure or NULL is not found.
 */
DbProcedure*
DbSchema::findProcedure(const QString &ipProcName) const
{
    return dynamic_cast<DbProcedure*>(findObject(ipProcName, DbObject::ProcedureObject));
}

/*!
 * Find trigger by its name
 *
 * \param[in] ipTrigName - Name of the trigger we are looking for
 *
 * \return Handle to the found trigger or NULL is not found.
 */
DbTrigger*
DbSchema::findTrigger(const QString &ipTrigName) const
{
    return dynamic_cast<DbTrigger*>(findObject(ipTrigName, DbObject::TriggerObject));
}

/*!
 * Read tables' names list from DB and populate
 * tables vector with *Table objects
 */
void
DbSchema::readTables()
{
    QSqlDatabase db = QSqlDatabase::database("mainConnect");
    QSqlQuery query(db);
    QString qstr;

    // clear tables list
    mTables.clear();

    // get sql driver
    Database::SqlDriverType sqlDriverType = Database::instance()->sqlDriver();

    // choose a query depending on sql driver
    switch (sqlDriverType) {
        case Database::Unknown:
                        qDebug() << "DbSchema::readTables> Sql driver was not set";
                        return;
            break;
        case Database::PostgreSQL:
                        qstr = QString("SELECT tablename "
                    "FROM pg_catalog.pg_tables pgt "
                    "WHERE schemaname='%1';")
                .arg(mName);
            break;

        case Database::MySQL:
        case Database::Oracle:
        case Database::SQLite:
            /* temporarily no support */
            return;
            break;

    }

    // if query execution failed
    if (!query.exec(qstr)) {
    qDebug() << "DbSchema::readTables> Unable to read tables.";
        qDebug() << query.lastError().text();

    return;
    }

    // if query returned nothing
    if (!query.first()) {
    qDebug() << "DbSchema::readTables> No tables were found.";

    return;
    }

    // for every retrieved row
    do {
        QString tableName = query.value(0).toString();

        DbTable *table = 0;
        // create a *table object by its name
        switch (sqlDriverType) {
            case Database::Unknown:
                break;
            case Database::PostgreSQL:
                table = new Psql::Table(mName, tableName);

                break;
            case Database::MySQL:
            case Database::Oracle:
            case Database::SQLite:
            default:
                /* no temporary support */
                return;
                break;
    }

        // add table
        addTable(table);

    } while (query.next());
}

/*!
 * Reads views within the schema
 */
void
DbSchema::readViews()
{
    QSqlDatabase db = QSqlDatabase::database("mainConnect");
    QSqlQuery query(db);
    QString qstr;

    // clear views list
    mViews.clear();

    QStringList viewsList;

    // get sql driver
    Database::SqlDriverType sqlDriverType = Database::instance()->sqlDriver();

    // choose a query depending on sql driver
    switch (sqlDriverType) {
        case Database::Unknown:
                        qDebug() << "Database::readViews> SqlDriver was not set";
        case Database::PostgreSQL:
                        Psql::Tools::viewsList(mName, viewsList);
                        break;
        case Database::MySQL:
        case Database::Oracle:
        case Database::SQLite:
        default:
                        /* temporarily no support for these DBMS */
                        return;
                        break;
    }

    // for every retrieved row
    foreach (const QString &name, viewsList) {

        // declare new view object
        DbView *view = Factory::View::createView(mName, name);

        Q_ASSERT(0 != view);

        // add view
        addView(view);
    }
}

/*!
 * Read procs within the given schema
 */
void
DbSchema::readProcedures()
{
    // clear procs list
    mProcedures.clear();

    QStringList proceduresList;

    // get sql driver
    Database::SqlDriverType sqlDriverType = Database::instance()->sqlDriver();

        // choose a query depending on sql driver
    switch (sqlDriverType) {
        case Database::Unknown:
                        qDebug() << "Database::readProcedures> SqlDriver was not set";
        case Database::PostgreSQL:
                        Psql::Tools::proceduresList(mName, proceduresList);
                        break;
        case Database::MySQL:
        case Database::Oracle:
        case Database::SQLite:
        default:
                        /* temporarily no support for these DBMS */
                        return;
                        break;
    }

    // for every retrieved row
    foreach (const QString &name, proceduresList) {

        // declare new proc object
        DbProcedure *proc = 0;
        proc = Factory::Procedure::createProcedure(mName, name);

        Q_ASSERT(0 != proc);

        // add proc
        addProcedure(proc);
    }
}

/*!
 * Reads triggers
 */
void
DbSchema::readTriggers()
{
    // clear triggers list
    mTriggers.clear();

    QStringList triggersList;

    // get sql driver
    Database::SqlDriverType sqlDriverType = Database::instance()->sqlDriver();

    // choose a query depending on sql driver
    switch (sqlDriverType) {
        case Database::Unknown:
                        qDebug() << "DbSchema::readTriggers> SqlDriver was not set";
                        return;
        case Database::PostgreSQL:
                        Psql::Tools::triggersList(mName, triggersList);
                        break;
        case Database::MySQL:
//                        Mysql::Tools::triggersList(mName, triggersList);
                        break;
        case Database::Oracle:
        case Database::SQLite:
        default:
                        /* temporarily no support for these DBMS */
                        return;
                        break;
    }

    // for every retrieved row
    foreach (const QString &name, triggersList) {
        // declare new trigger object
        DbTrigger *trigger;

        trigger = Factory::Trigger::createTrigger(mName, name);

        Q_ASSERT(0 != trigger);

        // add trigger
        addTrigger(trigger);
    }
}

/*!
 * \todo Implement
 *
 * \return true - Even if nothing has been read
 */
bool
DbSchema::loadData()
{
    readTables();
    readViews();
    readProcedures();
    readTriggers();

    return true;
}

/*!
 * Cleanup
 */
void
DbSchema::resetData()
{
#ifdef DEBUG_QUERY
    qDebug() << "DbSchema::resetData> cleaning for " << mName;
#endif

    // clear lists
    qDeleteAll(mTables);
    mTables.clear();

    qDeleteAll(mViews);
    mViews.clear();

    qDeleteAll(mProcedures);
    mProcedures.clear();

    qDeleteAll(mTriggers);
    mTriggers.clear();
}

/*!
 * \return DbObject::SchemaObject
 */
DbObject::Type
DbSchema::type() const
{
    return DbObject::SchemaObject;
}

/*!
 * Searches for an object by its name
 *
 * \param[in] ipObjectName - Object's name
 * \param[in] ipObjectType - Object's type \see DbObject::Type
 *
 * \return Handle to the found object or NULL if not found.
 */
DbObject*
DbSchema::findObject(const QString &ipObjectName, DbObject::Type ipObjectType) const
{
    quint64 count;
    QStringList list;

    //
    switch (ipObjectType) {
        case DbObject::TableObject:
                count = mTables.count();
                tablesList(&list);
                break;

        case DbObject::ViewObject:
                count = mViews.count();
                viewsList(&list);
                break;

        case DbObject::ProcedureObject:
                count = mProcedures.count();
                proceduresList(&list);
                break;

        case DbObject::TriggerObject:
                count = mTriggers.count();
                triggersList(&list);
                break;

        case DbObject::UnkObject:
        default:
                qDebug() << "DbSchema::findObject> Unknown object type";
                return 0;
    }

    // declare empty ptrs
    DbObject *object = 0;

    // if we don't have any objects of the given type
    if (0 == count) {
        // return nothing
        return 0;
    }

    quint64 i = 0;
    // look through objects' names
    while ( i < count && ipObjectName != list.at(i) ) {
        i++;
    }

    // if lang was found
    if ( !(i == count - 1 && ipObjectName != list.at(i)) ) {
        switch (ipObjectType) {
            case DbObject::TableObject:
                    object = mTables.at(i);
                    break;
            case DbObject::ViewObject:
                    object = mViews.at(i);
                    break;
            case DbObject::ProcedureObject:
                    object = mProcedures.at(i);
                    break;
            case DbObject::TriggerObject:
                    object = mTriggers.at(i);
                    break;

            // useless - done just to remove warning
            case DbObject::UnkObject:
            default:
                    break;
        }
    }

    return object;
}

/*!
 * \return Name of the given schema's owner
 */
QString
DbSchema::ownerName() const
{
    return mOwner->name();
}

/*!
 * \return Db schema description
 */
QString
DbSchema::description() const
{
    return mDescription;
}

/*!
 * Set description for the schema
 *
 * \param[in] ipDescription - Description text
 */
void
DbSchema::setDescription(const QString & ipDescription)
{
    mDescription = ipDescription;
}

} // namespace Common

} // namespace DbObjects


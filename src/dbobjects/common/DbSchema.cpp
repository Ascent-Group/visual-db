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

    // get sql driver
    Database::SqlDriverType sqlDriverType = Database::instance()->sqlDriver();

    // choose a query depending on sql driver
    switch (sqlDriverType) {
        case Database::Unknown:
                        qDebug() << "Database::readViews> SqlDriver was not set";
        case Database::PostgreSQL:
                        qstr = QString("SELECT "
                                            "v.schemaname as schema, "
                                            "v.viewname as name, "
                                            "v.viewowner as owner, "
                                            "v.definition as def "
                                        "FROM "
                                            "pg_catalog.pg_views v "
                                        "WHERE "
                                            "schemaname = '%1';")
                                .arg(mName);
                        break;
        case Database::MySQL:
        case Database::Oracle:
        case Database::SQLite:
        default:
                        /* temporarily no support for these DBMS */
                        return;
                        break;
    }

#ifdef DEBUG_QUERY
    qDebug() << "DbSchema::readViews> " << qstr;
#endif

    // if query failed
    if (!query.exec(qstr)) {
        qDebug() << query.lastError().text();
    }

    // if query returned nothing
    if (!query.first()) {
        qDebug() << "DbSchema::readViews> No views were found for " << mName;

        return;
    }

    // for every retrieved row
    do {

    qint32 colId;

        // declare new view object
        DbView *view;

        // choose a query depending on sql driver
        switch (sqlDriverType) {
            // lyuts: looks like this case is useless. if driver is not set then
            // previous switch will handle this and return from function.
            /*case Database::Unknown:
                            qDebug() << "DbSchema::readViews> SqlDriver was not set";
                            return;
                            */
            case Database::PostgreSQL:
                            colId = query.record().indexOf("name");


                            view = new Psql::View(mName, query.value(colId).toString());


                            break;
            case Database::MySQL:
            case Database::Oracle:
            case Database::SQLite:
            default:
                            qDebug() << "DbSchema::readViews> SqlDriver is not supported currently!";
                            /* temporarily no support for these DBMS */
                            return;
                            break;

        }

        // set views's attributes
        colId = query.record().indexOf("schema");
        Q_ASSERT(colId > 0);

        colId = query.record().indexOf("name");
        Q_ASSERT(colId > 0);
        view->setName(query.value(colId).toString());

        colId = query.record().indexOf("owner");
        Q_ASSERT(colId > 0);
        QString ownerName = query.value(colId).toString();
        view->setOwner(Database::instance()->findRole(ownerName));

        colId = query.record().indexOf("def");
        Q_ASSERT(colId > 0);
        view->setDefinition(query.value(colId).toString());

        /* temporary debug output */
#if DEBUG_TRACE
        qDebug() << "view->mName: " << view->name();
        qDebug() << "view->mSchema: " << view->schema()->name();
        qDebug() << "view->mOwner: " << view->owner()->name();
        qDebug() << "view->mDefinition: " << view->definition();
#endif

        // add view
        addView(view);

    } while (query.next());
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

        qint32 colId;

        // declare new proc object
        DbProcedure *proc = 0;
        proc = Factory::Procedure::createProcedure(mName, name);

        Q_ASSERT(proc != 0);

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
    QSqlDatabase db = QSqlDatabase::database("mainConnect");
    QSqlQuery query(db);
    QString qstr;

    // clear triggers list
    mTriggers.clear();

    // get sql driver
    Database::SqlDriverType sqlDriverType = Database::instance()->sqlDriver();

    // choose a query depending on sql driver
    switch (sqlDriverType) {
        case Database::Unknown:
                        qDebug() << "DbSchema::readTriggers> SqlDriver was not set";
                        return;
        case Database::PostgreSQL:
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
                                            "AND tbl_nsp.nspname = '%1' "
                                            //"AND ref_tbl_nsp.nspname NOT LIKE 'pg_%' "
                                            //"AND proc_nsp.nspname NOT LIKE 'pg_%' "
                                            "AND ref_tbl.relnamespace = ref_tbl_nsp.oid;")
                                            .arg(mName);
                        qDebug() << qstr;
                        break;
        case Database::MySQL:
                        qstr = QString(";");
                        break;
        case Database::Oracle:
        case Database::SQLite:
        default:
                        /* temporarily no support for these DBMS */
                        return;
                        break;
    }

#ifdef DEBUG_QUERY
    qDebug() << "Database::readTriggers> "<< qstr;
#endif

    // if query failed
    if (!query.exec(qstr)) {
        qDebug() << query.lastError().text();

        return;
    }

    // if query returned nothing
    if (!query.first()) {
        qDebug() << "Database::readTriggers> No triggers were found.";

        return;
    }

    // for every retrieved row
    do {
        // declare new trigger object
        DbTrigger *trig;

        qint32 colId;

        // choose a query depending on sql driver
        switch (sqlDriverType) {
            // lyuts: looks like this case is useless. if driver is not set then
            // previous switch will handle this and return from function.
            /*case Database::Unknown:
                            qDebug() << "Database::readTriggers> SqlDriver was not set";
                            return;
                            */
            case Database::PostgreSQL:
                            colId = query.record().indexOf(/*"name"*/"constrname");
                            Q_ASSERT(colId > 0);

                            trig = new Psql::Trigger(mName, query.value(colId).toString());


                            break;
            case Database::MySQL:
            case Database::Oracle:
            case Database::SQLite:
            default:
                            qDebug() << "Database::readTriggers> SqlDriver is not supported currently!";
                            /* temporarily no support for these DBMS */
                            return;
                            break;

        }

        // set trig's attributes


        // table
        colId = query.record().indexOf("schema");
        Q_ASSERT(colId > 0);
        QString schemaName = query.value(colId).toString();
        trig->setSchema(this);

        colId = query.record().indexOf("table");
        Q_ASSERT(colId > 0);
        QString tableName = query.value(colId).toString();

        DbTable *table = 0;
        table = findTable(tableName);
        trig->setTable(table);

        // proc
        colId = query.record().indexOf("proc_schema");
        Q_ASSERT(colId > 0);
        QString procSchemaName = query.value(colId).toString();

        colId = query.record().indexOf("proc");
        Q_ASSERT(colId > 0);
        QString procName = query.value(colId).toString();

        qDebug() << "Looking for proc_schema: " << procSchemaName;
        DbSchema *schema = Database::instance()->findSchema(procSchemaName);
        qDebug() << "proc_schema = " << schema;
        DbProcedure *proc = 0;

        if (schema) {
            proc = schema->findProcedure(procName);
        }

        qDebug() << proc;

        trig->setProcedure(proc);

        // enabled
        colId = query.record().indexOf("enabled");
        Q_ASSERT(colId > 0);
        trig->setEnabled(query.value(colId).toChar());

        // isconstraint
        colId = query.record().indexOf("isconstraint");
        Q_ASSERT(colId > 0);
        trig->setConstraint(query.value(colId).toBool());

        // constrname
        colId = query.record().indexOf("constrname");
        Q_ASSERT(colId > 0);
        trig->setConstraintName(query.value(colId).toString());

        // ref table
        colId = query.record().indexOf("ref_schema");
        Q_ASSERT(colId > 0);
        QString refSchemaName = query.value(colId).toString();

        colId = query.record().indexOf("ref_table");
        Q_ASSERT(colId > 0);
        QString refTableName = query.value(colId).toString();

        schema = Database::instance()->findSchema(refSchemaName);

        if (schema) {
            table = schema->findTable(refTableName);
        }

        qDebug() << table;

        trig->setReferencedTable(table);

        // deferrable
        colId = query.record().indexOf("deferrable");
        Q_ASSERT(colId > 0);
        trig->setDeferrable(query.value(colId).toBool());

        // initdeferred
        colId = query.record().indexOf("initdeferred");
        Q_ASSERT(colId > 0);
        trig->setInitiallyDeferred(query.value(colId).toBool());

        // nargs
        colId = query.record().indexOf("nargs");
        Q_ASSERT(colId > 0);
        trig->setNumArgs(query.value(colId).toInt());


        /* temporary debug output */
#if DEBUG_TRACE
       qDebug() << "Database::readTriggers> name = " << trig->name();
       qDebug() << "Database::readTriggers> table = " << trig->table()->name();
       qDebug() << "Database::readTriggers> proc = " << trig->procedure()->name();
       qDebug() << "Database::readTriggers> enabled = " << trig->enabled();
       qDebug() << "Database::readTriggers> isConstraint = " << trig->isConstraint();
       qDebug() << "Database::readTriggers> constraintName = " << trig->constraintName();
       qDebug() << "Database::readTriggers> ref_table = " << trig->referencedTable()->name();
       qDebug() << "Database::readTriggers> isDeferrable = " << trig->isDeferrable();
       qDebug() << "Database::readTriggers> isInitiallyDeferred = " << trig->isInitiallyDeferred();
       qDebug() << "Database::readTriggers> nargs = " << trig->numArgs();
#endif

        // add trigger
        addTrigger(trig);

    } while (query.next());
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


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
#include <mysql/MysqlTable.h>
#include <psql/PsqlLanguage.h>
#include <psql/PsqlProcedure.h>
#include <psql/PsqlRole.h>
#include <psql/PsqlTable.h>
#include <psql/PsqlTrigger.h>
#include <psql/PsqlView.h>

#include <QtDebug>

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
    Database::instance()->addSchema(this);
}

/*!
 * Destructor
 */
DbSchema::~DbSchema()
{
    cleanup();
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
 * \param[in] ipSchema
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
/*
 * Return a list of tables' names
 */
void
DbSchema::tablesList(QStringList *ipList) const
{
    if (0 == ipList) {
        return;
    }

    ipList->clear();

    QVector<DbTable*>::const_iterator iter;

    for (iter = mTables.constBegin(); iter != mTables.constEnd(); ++iter) {
        ipList->append((*iter)->name());
    }

    //ipList->sort();
}

/*
 * Calculate the number of tables in the given schema
 */
quint64
DbSchema::tablesCount() const
{
    return mTables.count();
}

/*
 * Returns a list of views' names
 */
void
DbSchema::viewsList(QStringList *ipList) const
{
    if (0 == ipList) {
        return;
    }

    ipList->clear();

    QVector<DbView*>::const_iterator iter;

    for (iter = mViews.constBegin(); iter != mViews.constEnd(); ++iter) {
        ipList->append((*iter)->name());
    }

    //ipList->sort();
}

/*
 * Returns the number of views in the given schema
 */
quint64
DbSchema::viewsCount() const
{
    return mViews.count();
}

/*
 * Returns a list of procs' names
 */
void
DbSchema::proceduresList(QStringList *ipList) const
{
    if (0 == ipList) {
        return;
    }

    ipList->clear();

    QVector<DbProcedure*>::const_iterator iter;

    for (iter = mProcedures.constBegin(); iter != mProcedures.constEnd(); ++iter) {
        ipList->append((*iter)->name());
    }

    //ipList->sort();
}

/*
 * Returns a list of triggers' names
 */
void
DbSchema::triggersList(QStringList *ipList) const
{
    if (0 == ipList) {
        return;
    }

    ipList->clear();

    QVector<DbTrigger*>::const_iterator iter;

    for (iter = mTriggers.constBegin(); iter != mTriggers.constEnd(); ++iter) {
        ipList->append((*iter)->name());
    }

    //ipList->sort();
}

/*
 * Returns the number of procs in the given schema
 */
quint64
DbSchema::proceduresCount() const
{
    return mProcedures.count();
}

/*
 * Calculates the number of triggers
 */
quint16
DbSchema::triggersCount() const
{
    return mTriggers.count();
}

/*
 * Find table by its name
 */
DbTable*
DbSchema::findTable(const QString &ipTableName) const
{
    return dynamic_cast<DbTable*>(findObject(ipTableName, DbObject::TableObject));
}

/*
 * Find view by its name
 */
DbView*
DbSchema::findView(const QString &ipViewName) const
{
    return dynamic_cast<DbView*>(findObject(ipViewName, DbObject::ViewObject));
}

/*
 * Find proc by its name
 */
DbProcedure*
DbSchema::findProcedure(const QString &ipProcName) const
{
    return dynamic_cast<DbProcedure*>(findObject(ipProcName, DbObject::ProcedureObject));
}

/*
 * Returns a trigger by its name
 */
DbTrigger*
DbSchema::findTrigger(const QString &ipTrigName) const
{
    return dynamic_cast<DbTrigger*>(findObject(ipTrigName, DbObject::TriggerObject));
}

/*
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
                table = new PsqlTable(mName, tableName);

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

/*
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


                            view = new PsqlView(mName, query.value(colId).toString());


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

/*
 * Read procs within the given schema
 */
void
DbSchema::readProcedures()
{
    QSqlDatabase db = QSqlDatabase::database("mainConnect");
    QSqlQuery query(db);
    QString qstr;

    // clear procs list
    mProcedures.clear();

    // get sql driver
    Database::SqlDriverType sqlDriverType = Database::instance()->sqlDriver();

        // choose a query depending on sql driver
    switch (sqlDriverType) {
        case Database::Unknown:
                        qDebug() << "Database::readProcedures> SqlDriver was not set";
        case Database::PostgreSQL:
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
                                            "AND n.nspname = '%1';")
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
    qDebug() << "DbSchema::readProcedures> " << qstr;
#endif

    // if query failed
    if (!query.exec(qstr)) {
        qDebug() << query.lastError().text();
    }

    // if query returned nothing
    if (!query.first()) {
        qDebug() << "DbSchema::readProcedures> No procs were found.";

        return;
    }

    // for every retrieved row
    do {

        qint32 colId;

        // declare new proc object
        DbProcedure *proc;

        // choose a query depending on sql driver
        switch (sqlDriverType) {
            // lyuts: looks like this case is useless. if driver is not set then
            // previous switch will handle this and return from function.
            /*case Database::Unknown:
                            qDebug() << "DbSchema::readProcedures> SqlDriver was not set";
                            return;
                            */
            case Database::PostgreSQL:
                            colId = query.record().indexOf("name");

                            proc = new PsqlProcedure(mName, query.value(colId).toString());

                            break;
            case Database::MySQL:
            case Database::Oracle:
            case Database::SQLite:
            default:
                            qDebug() << "DbSchema::readProcedures> SqlDriver is not supported currently!";
                            /* temporarily no support for these DBMS */
                            return;
                            break;

        }

        // set proc's attributes
        colId = query.record().indexOf("owner");
        Q_ASSERT(colId > 0);
        QString ownerName = query.value(colId).toString();
        proc->setOwner(Database::instance()->findRole(ownerName));

        colId = query.record().indexOf("lang");
        Q_ASSERT(colId > 0);
        QString langName = query.value(colId).toString();
        proc->setLanguage(Database::instance()->findLanguage(langName));

        colId = query.record().indexOf("src");
        Q_ASSERT(colId > 0);
        proc->setSourceCode(query.value(colId).toString());

        /* temporary debug output */
#if DEBUG_TRACE
        qDebug() << "proc->mName: " << proc->name();
        qDebug() << "proc->mSchema: " << proc->schema()->name();
        qDebug() << "proc->mOwner: " << proc->owner()->name();
        qDebug() << "proc->mLanguage: " << proc->language()->name();
        qDebug() << "proc->mSourceCode: " << proc->sourceCode();
#endif

        // add proc
        addProcedure(proc);

    } while (query.next());
}

/*
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

                            trig = new PsqlTrigger(mName, query.value(colId).toString());


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

/*
 * Cleanup
 */
void
DbSchema::cleanup()
{
#ifdef DEBUG_QUERY
    qDebug() << "DbSchema::cleanup> cleaning for " << mName;
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

/*
 * Returns the id of database object type
 */
DbObject::Type
DbSchema::type() const
{
    return DbObject::SchemaObject;
}

/*
 * Searches for an object by its name
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

QString DbSchema::ownerName() const
{
    return mOwner->name();
}

QString DbSchema::description() const
{
    return mDescription;
}

void DbSchema::setDescription(const QString & ipDescription)
{
    mDescription = ipDescription;
}


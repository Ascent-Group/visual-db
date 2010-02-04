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

#include <Database.h>
#include <DbIndex.h>
#include <DbLanguage.h>
#include <DbSchema.h>
#include <PsqlIndex.h>
#include <PsqlLanguage.h>
#include <PsqlRole.h>
#include <PsqlTrigger.h>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>

#include <QtDebug>

// null initialization of instance
Database* Database::mInstance = 0;

/*
 * Constructor
 */
Database::Database()
{

}

/*
 * Destructor
 */
Database::~Database()
{
    cleanup();
}

/*
 * Get/create an instance of Database
 */
Database*
Database::instance()
{
    // if not yet created => create
    if (0 == mInstance) {
        mInstance = new Database();
    }

    return mInstance;
}

/*
 * Add schema to DB schema list
 */
void
Database::addSchema(DbSchema *ipSchema)
{
    if (!mSchemas.contains(ipSchema)) {
        mSchemas.push_back(ipSchema);
    }
}

/*
 * Add role to DB roles list
 */
    void
Database::addRole(DbRole *ipRole)
{
    if (!mRoles.contains(ipRole)) {
        mRoles.push_back(ipRole);
    }
}

/*
 * Adds index to indices list
 */
    void
Database::addIndex(DbIndex *ipIndex)
{
    if (!mIndices.contains(ipIndex)) {
        mIndices.push_back(ipIndex);
    }
}

/*
 * Adds language to list
 */
    void
Database::addLanguage(DbLanguage *ipLanguage)
{
    if (!mLanguages.contains(ipLanguage)) {
        mLanguages.push_back(ipLanguage);
    }
}

/*
 * Returns a list of schemas' names
 */
void
Database::schemasList(QStringList *ipList) const
{
    if (0 == ipList) {
        return;
    }

    ipList->clear();

    QVector<DbSchema*>::const_iterator iter;

    for (iter = mSchemas.constBegin(); iter != mSchemas.constEnd(); ++iter) {
        ipList->append((*iter)->name());
    }

    /* lyuts - if uncommented, then something gets wrong with the list, the list
     * has "public" and "public" 0_0
     *
     */
    //ipList->sort();
}

/*
 * Returns a list of roles' names
 */
void
Database::rolesList(QStringList *ipList) const
{
    if (0 == ipList) {
        return;
    }

    ipList->clear();

    QVector<DbRole*>::const_iterator iter;

    for (iter = mRoles.constBegin(); iter != mRoles.constEnd(); ++iter) {
        ipList->append((*iter)->name());
    }

    //ipList->sort();
}

/*
 * Returns a list of indices' names
 */
void
Database::indicesList(QStringList *ipList) const
{
    if (0 == ipList) {
        return;
    }

    ipList->clear();

    QVector<DbIndex*>::const_iterator iter;

    for (iter = mIndices.constBegin(); iter != mIndices.constEnd(); ++iter) {
        ipList->append((*iter)->name());
    }

    //ipList->sort();
}

/*
 * Returns a list of languages' names
 */
void
Database::languagesList(QStringList *ipList) const
{
    if (0 == ipList) {
        return;
    }

    ipList->clear();

    QVector<DbLanguage*>::const_iterator iter;

    for (iter = mLanguages.constBegin(); iter != mLanguages.constEnd(); ++iter) {
        ipList->append((*iter)->name());
    }

    //ipList->sort();
}

/*
 * Calculate the number of schemas in a given database
 */
quint64
Database::schemasCount() const
{
    return mSchemas.count();
}

/*
 * Calculate the number of database roles
 */
quint64
Database::rolesCount() const
{
    return mRoles.count();
}

/*
 * Calculates the number of database indices
 */
quint64
Database::indicesCount() const
{
    return mIndices.count();
}

/*
 * Calculates the number of pl languages
 */
quint8
Database::languagesCount() const
{
    return mLanguages.count();
}

/*
 * Find and return schema by its name
 */
DbSchema*
Database::findSchema(const QString &ipSchemaName) const
{
    return dynamic_cast<DbSchema*>(findObject(ipSchemaName, DbObject::SchemaObject));
}

/*
 * Returns a database role by its name
 */
DbRole*
Database::findRole(const QString &ipRoleName) const
{
    return dynamic_cast<DbRole*>(findObject(ipRoleName, DbObject::RoleObject));
}

/*
 * Returns a database index by its name
 */
DbIndex*
Database::findIndex(const QString &ipIndexName) const
{
    return dynamic_cast<DbIndex*>(findObject(ipIndexName, DbObject::IndexObject));
}

/*
 * Returns a language by its name
 */
DbLanguage*
Database::findLanguage(const QString &ipLangName) const
{
    return dynamic_cast<DbLanguage*>(findObject(ipLangName, DbObject::LanguageObject));
}

/*
 * Returns the number of indices found and populates the input list
 */
quint64
Database::findTableIndices(const DbTable *ipTable, QVector<DbIndex*> &ipList) const
{
    ipList.clear();

    quint64 count = mIndices.count();

    // if we don't have any indices
    if (0 == count || 0 == ipTable) {
        return 0;
    }

    QString schemaName = ipTable->schemaName();
    QString tableName = ipTable->name();

    // look through index's table names
    for (quint64 i = 0; i < count; ++i) {
        // if index is for our table -- add it
        if (tableName == mIndices.at(i)->tableName()
            && schemaName == mIndices.at(i)->schemaName()) {
            ipList.push_back(mIndices.at(i));
        }
    }

    return ipList.count();
}
/*
 * Obtain a type of DBMS
 */
void
Database::setSqlDriver(const QString &ipSqlDriverName)
{
    Database* db = instance();

    if (ipSqlDriverName.contains("PSQL")) {
        db->mSqlDriver = PostgreSQL;
    } else if (ipSqlDriverName.contains("MYSQL")) {
        db->mSqlDriver = MySQL;
    } else if (ipSqlDriverName.contains("ODBC")) {
        db->mSqlDriver = Oracle;
    } else if (ipSqlDriverName.contains("SQLITE")) {
        db->mSqlDriver = Database::SQLite;
    }

}

/*
 * Get the type of sql driver (see enum in header)
 */
Database::SqlDriverType
Database::sqlDriver() const
{
    return mSqlDriver;
}
/*
 * Retrieve available schemas
 */
void
Database::readSchemas()
{
    QSqlDatabase db = QSqlDatabase::database("mainConnect");
    QSqlQuery query(db);
    QString qstr;

    // clear schemas list
    mSchemas.clear();

    // choose a query depending on sql driver
    switch (mSqlDriver) {
        case Database::Unknown:
            qDebug() << "Database::readSchemas> SqlDriver was not set";
        case Database::PostgreSQL:
            qstr = QString("SELECT nspname as name, roles.rolname as ownername, description "
                    "FROM pg_catalog.pg_namespace pgn "
                    "left join pg_roles roles on roles.oid = pgn.nspowner "
                    "left join pg_description descr on descr.objoid = pgn.oid "
                    ";");
                    //"WHERE nspname NOT LIKE 'pg_%';");
            break;
        case Database::MySQL:
            qstr = QString("SELECT schema_name "
                    "FROM information_schema.schemata;");
            break;
        case Database::Oracle:
        case Database::SQLite:
        default:
            /* temporarily no support for these DBMS */
            return;
            break;
    }

#ifdef DEBUG_QUERY
    qDebug() << qstr;
#endif

    // if query execution failed
    if (!query.exec(qstr)) {
        qDebug() << "Database::readSchemas> Unable to retrieve schemas.";
        qDebug() << query.lastError().text();

        return;
    }

    // if query returned nothing
    if (!query.first()) {
        qDebug() << "Database::readSchemas> No schemas were found.";

        return;
    }

    // for every retrieved row
    do {
        // get data from query
        qint32 colId = query.record().indexOf("name");
        QString name = query.value(colId).toString();

        colId = query.record().indexOf("ownername");
        QString ownerName = query.value(colId).toString();

        colId = query.record().indexOf("description");
        QString description = query.value(colId).toString();

        // find owner for scheme
        DbRole *dbRole = findRole(ownerName);
        Q_CHECK_PTR(dbRole);

        // create new schema object
        DbSchema *schema = new DbSchema(name, dbRole);

        // set scheme description
        schema->setDescription(description);

        // read tables
        schema->readTables();
        // read views
        schema->readViews();
        // read procs
        schema->readProcedures();
        // read trigs
        schema->readTriggers();

        // add schema - not needed anymore, done in schema's ctor
        //addSchema(schema);
    } while (query.next());
}

/*
 * Retrieve database roles
 */
void
Database::readRoles()
{
    QSqlDatabase db = QSqlDatabase::database("mainConnect");
    QSqlQuery query(db);
    QString qstr;

    // clear roles list
    mRoles.clear();

    // choose a query depending on sql driver
    switch (mSqlDriver) {
        case Database::Unknown:
                        qDebug() << "Database::readRoles> SqlDriver was not set";
                        return;
        case Database::PostgreSQL:
                        qstr = QString("SELECT "
                                            "r.rolname as name, "
                                            "r.rolsuper as super, "
                                            "r.rolinherit as inherit, "
                                            "r.rolcreaterole as createrole, "
                                            "r.rolcreatedb as createdb, "
                                            "r.rolcatupdate as catupdate, "
                                            "r.rolcanlogin as canlogin, "
                                            "r.rolconnlimit as connlimit, "
                                            "r.rolvaliduntil as validuntil, "
                                            "r.oid as id "
                                        "FROM "
                                            "pg_catalog.pg_roles r;");
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
    qDebug() << qstr;
#endif

    // if query failed
    if (!query.exec(qstr)) {
        qDebug() << query.lastError().text();
    }

    // if query returned nothing
    if (!query.first()) {
        qDebug() << "Database::readRoles> No roles were found.";

        return;
    }

    // for every retrieved row
    do {
        // declare new role object
        DbRole *role;

        // choose a query depending on sql driver
        switch (mSqlDriver) {
            // lyuts: looks like this case is useless. if driver is not set then
            // previous switch will handle this and return from function.
            /*case Database::Unknown:
                            qDebug() << "Database::readRoles> SqlDriver was not set";
                            return;
                            */
            case Database::PostgreSQL:
                            role = new PsqlRole();


                            break;
            case Database::MySQL:
            case Database::Oracle:
            case Database::SQLite:
            default:
                            qDebug() << "Database::readRoles> SqlDriver is not supported currently!";
                            /* temporarily no support for these DBMS */
                            return;
                            break;

        }

        // set role's attributes
        qint32 colId = query.record().indexOf("name");
        Q_ASSERT(colId > 0);
        role->setName(query.value(colId).toString());

        colId = query.record().indexOf("super");
        Q_ASSERT(colId > 0);
        role->setSuperUser(query.value(colId).toBool());

        colId = query.record().indexOf("inherit");
        Q_ASSERT(colId > 0);
        role->setInheritsPriviligese(query.value(colId).toBool());

        colId = query.record().indexOf("createrole");
        Q_ASSERT(colId > 0);
        role->setCanCreateRole(query.value(colId).toBool());

        colId = query.record().indexOf("createdb");
        Q_ASSERT(colId > 0);
        role->setCanCreateDb(query.value(colId).toBool());

        colId = query.record().indexOf("catupdate");
        Q_ASSERT(colId > 0);
        role->setCanUpdateSysCat(query.value(colId).toBool());

        colId = query.record().indexOf("canlogin");
        Q_ASSERT(colId > 0);
        role->setCanLogin(query.value(colId).toBool());

        colId = query.record().indexOf("connlimit");
        Q_ASSERT(colId > 0);
        role->setConnectionLimit(query.value(colId).toInt());

        colId = query.record().indexOf("validuntil");
        Q_ASSERT(colId > 0);
        role->setExpiryDate(query.value(colId).toDate());

        colId = query.record().indexOf("id");
        Q_ASSERT(colId > 0);
        role->setId(query.value(colId).toInt());

        /* temporary debug output */
#if DEBUG_TRACE
        qDebug() << "role->mName: " << role->name();
        qDebug() << "role->mIsSuperUser: " << role->isSuperUser();
        qDebug() << "role->mInheritsPrivileges: " << role->inheritsPrivileges();
        qDebug() << "role->mCanCreateRole: " << role->canCreateRole();
        qDebug() << "role->mCanCreateDb: " << role->canCreateDb();
        qDebug() << "role->mCanUpdateSysCat: " << role->canUpdateSysCat();
        qDebug() << "role->mCanLogin: " << role->canLogin();
        qDebug() << "role->mConnectionLimit: " << role->connectionLimit();
        qDebug() << "role->mExpiryDate: " << role->expiryDate();
        qDebug() << "role->mId: " << role->id();
#endif

        // add role
        addRole(role);

    } while (query.next());
}

/*
 *
 */
void
Database::readIndices()
{
    QSqlDatabase db = QSqlDatabase::database("mainConnect");
    QSqlQuery query(db);
    QString qstr;

    // clear indices list
    mIndices.clear();

    // choose a query depending on sql driver
    switch (mSqlDriver) {
        case Database::Unknown:
                        qDebug() << "Database::readIndices> SqlDriver was not set";
                        return;
        case Database::PostgreSQL:
                        qstr = QString("SELECT "
                                            "index.relname as name, "
                                            "pgi.indisunique as unique, "
                                            "pgi.indisprimary as primary, "
                                            "pgi.indisclustered as clustered, "
                                            "pgi.indisvalid as valid, "
                                            "pgi.indcheckxmin as xmin, "
                                            "pgi.indisready as ready, "
                                            "pgi.indnatts as colcount, "
                                            "pgi.indkey as fields, "
                                            "pgn.nspname as schema, "
                                            "rel.relname as table "
                                        "FROM "
                                            "pg_catalog.pg_index pgi, "
                                            "pg_catalog.pg_class index, "
                                            "pg_catalog.pg_namespace pgn, "
                                            "pg_catalog.pg_class rel "
                                        "WHERE "
                                            "pgi.indrelid = rel.oid "
                                            "AND pgn.oid = rel.relnamespace "
                                            //"AND index.relname NOT LIKE 'pg_%' "
                                            "AND pgi.indexrelid = index.oid;");
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
    qDebug() << qstr;
#endif

    // if query failed
    if (!query.exec(qstr)) {
        qDebug() << query.lastError().text();

        return;
    }

    // if query returned nothing
    if (!query.first()) {
        qDebug() << "Database::readIndices> No indices were found.";

        return;
    }

    // for every retrieved row
    do {
        // declare new index object
        DbIndex *index;

        qint32 colId;

        // choose a query depending on sql driver
        switch (mSqlDriver) {
            // lyuts: looks like this case is useless. if driver is not set then
            // previous switch will handle this and return from function.
            /*case Database::Unknown:
                            qDebug() << "Database::readIndices> SqlDriver was not set";
                            return;
                            */
            case Database::PostgreSQL:
                            colId = query.record().indexOf("name");
                            Q_ASSERT(colId > 0);

                            index = new PsqlIndex(query.value(colId).toString());

                            break;
            case Database::MySQL:
            case Database::Oracle:
            case Database::SQLite:
            default:
                            qDebug() << "Database::readIndices> SqlDriver is not supported currently!";
                            /* temporarily no support for these DBMS */
                            return;
                            break;

        }

        // set index's attributes

        //int colId;

        /*colId= query.record().indexOf("name");
        Q_ASSERT(colId > 0);
        index->setName(query.value(colId).toString());*/


        colId = query.record().indexOf("unique");
        Q_ASSERT(colId > 0);
        index->setUnique(query.value(colId).toBool());

        colId = query.record().indexOf("primary");
        Q_ASSERT(colId > 0);
        index->setPrimary(query.value(colId).toBool());

        colId = query.record().indexOf("clustered");
        Q_ASSERT(colId > 0);
        index->setClustered(query.value(colId).toBool());

        colId = query.record().indexOf("valid");
        Q_ASSERT(colId > 0);
        index->setValid(query.value(colId).toBool());

        colId = query.record().indexOf("xmin");
        Q_ASSERT(colId > 0);
        index->setChecksXMin(query.value(colId).toBool());

        colId = query.record().indexOf("ready");
        Q_ASSERT(colId > 0);
        index->setReady(query.value(colId).toBool());

        colId = query.record().indexOf("table");
        Q_ASSERT(colId > 0);
        QString tableName = query.value(colId).toString();

        colId = query.record().indexOf("schema");
        Q_ASSERT(colId > 0);
        QString schemaName = query.value(colId).toString();

        DbSchema *schema = Database::instance()->findSchema(schemaName);
        DbTable *table = 0;

        if (schema) {
            table = schema->findTable(tableName);
        }

        index->setSchema(schema);
        index->setTable(table);

        index->setTableName(tableName);
        index->setSchemaName(schemaName);

        colId = query.record().indexOf("colcount");
        Q_ASSERT(colId > 0);
        index->setColumnsCount(query.value(colId).toInt());

        // get the list of column numbers
        colId = query.record().indexOf("fields");
        Q_ASSERT(colId > 0);
        QString str = query.value(colId).toString();

        // populate the vector
        foreach (QString section, str.split(" ")) {
            index->addColumnNumber(section.toInt());
        }


        /* temporary debug output */
#if DEBUG_TRACE
        qDebug() << "index->name(): " << index->name();
        qDebug() << "index->isUnique: " << index->isUnique();
        qDebug() << "index->isPrimary: " << index->isPrimary();
        qDebug() << "index->isClustered: " << index->isClustered();
        qDebug() << "index->isValid: " << index->isValid();
        qDebug() << "index->checksXMin: " << index->checksXMin();
        qDebug() << "index->isReady: " << index->isReady();
        qDebug() << "index->tableName: " << index->tableName();
        qDebug() << "index->schemaName: " << index->schemaName();
        qDebug() << "index->columnsCount: " << index->columnsCount();
        qDebug() << "index->columnsNumbers: " << index->columnsNumbers();
#endif

        // add index
        addIndex(index);

    } while (query.next());

}

/*
 * Reads pl languages
 */
void
Database::readLanguages()
{
    QSqlDatabase db = QSqlDatabase::database("mainConnect");
    QSqlQuery query(db);
    QString qstr;

    // clear languages list
    mLanguages.clear();

    // choose a query depending on sql driver
    switch (mSqlDriver) {
        case Database::Unknown:
                        qDebug() << "Database::readLanguages> SqlDriver was not set";
                        return;
        case Database::PostgreSQL:
                        qstr = QString("SELECT "
                                            "l.lanname AS name, "
                                            "l.lanowner AS owner, "
                                            "l.lanispl AS ispl, "
                                            "l.lanpltrusted AS trusted, "
                                            "l.lanplcallfoid AS executor, "
                                            "l.lanvalidator AS validator, "
                                            "l.lanacl AS acl "
                                        "FROM "
                                            "pg_catalog.pg_language l;");
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
    qDebug() << qstr;
#endif

    // if query failed
    if (!query.exec(qstr)) {
        qDebug() << query.lastError().text();

        return;
    }

    // if query returned nothing
    if (!query.first()) {
        qDebug() << "Database::readLanguages> No languages were found.";

        return;
    }

    // for every retrieved row
    do {
        // declare new language object
        DbLanguage *lang;

        qint32 colId;

        // choose a query depending on sql driver
        switch (mSqlDriver) {
            // lyuts: looks like this case is useless. if driver is not set then
            // previous switch will handle this and return from function.
            /*case Database::Unknown:
                            qDebug() << "Database::readLanguages> SqlDriver was not set";
                            return;
                            */
            case Database::PostgreSQL:
                            colId= query.record().indexOf("name");
                            Q_ASSERT(colId > 0);

                            lang = new PsqlLanguage(query.value(colId).toString());


                            break;
            case Database::MySQL:
            case Database::Oracle:
            case Database::SQLite:
            default:
                            qDebug() << "Database::readLanguages> SqlDriver is not supported currently!";
                            /* temporarily no support for these DBMS */
                            return;
                            break;

        }

        // set lang's attributes

        //int colId;

        /*colId= query.record().indexOf("name");
        Q_ASSERT(colId > 0);
        index->setName(query.value(colId).toString());*/


        colId = query.record().indexOf("trusted");
        Q_ASSERT(colId > 0);
        lang->setTrusted(query.value(colId).toBool());

        /* temporary debug output */
#if DEBUG_TRACE
        qDebug() << "lang->name() = " << lang->name();
        qDebug() << "lang->isTrusted() =  " << lang->isTrusted();
#endif

        // add language
        addLanguage(lang);

    } while (query.next());
}

/*
 * Cleanup data
 */
void
Database::cleanup()
{
#ifdef DEBUG_TRACE
    qDebug() << "Database::cleanup> cleaning...";
#endif

    quint64 schemasCount = mSchemas.count();

    for (quint64 i = 0; i < schemasCount; ++i) {
        mSchemas.at(i)->cleanup();
    }

    // clear vectors
    qDeleteAll(mSchemas);
    mSchemas.clear();

    qDeleteAll(mRoles);
    mRoles.clear();

    qDeleteAll(mIndices);
    mIndices.clear();
}

/*
 * Searches for an object by its name
 */
DbObject*
Database::findObject(const QString &ipObjectName, DbObject::Type ipObjectType) const
{
    quint64 count;
    QStringList list;

    //
    switch (ipObjectType) {
        case DbObject::SchemaObject:
                count = mSchemas.count();
                schemasList(&list);
                break;

        case DbObject::RoleObject:
                count = mRoles.count();
                rolesList(&list);
                break;

        case DbObject::IndexObject:
                count = mIndices.count();
                indicesList(&list);
                break;

        case DbObject::LanguageObject:
                count = mLanguages.count();
                languagesList(&list);
                break;

        case DbObject::UnkObject:
        default:
                qDebug() << "Database::findObject> Unknown object type";
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

    // if object was found
    if ( !(i == count - 1 && ipObjectName != list.at(i)) ) {
        switch (ipObjectType) {
            case DbObject::SchemaObject:
                    object = mSchemas.at(i);
                    break;
            case DbObject::RoleObject:
                    object = mRoles.at(i);
                    break;
            case DbObject::IndexObject:
                    object = mIndices.at(i);
                    break;
            case DbObject::LanguageObject:
                    object = mLanguages.at(i);
                    break;
            // actually useless - done just to remove warning
            case DbObject::UnkObject:
            default:
                    break;
        }
    }

    return object;
}

/*
 *
 */
void
DatabaseManager::flush()
{
     delete Database::instance();
}


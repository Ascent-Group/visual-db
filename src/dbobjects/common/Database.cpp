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
#include <common/DbIndex.h>
#include <common/DbLanguage.h>
#include <common/DbSchema.h>
#include <factory/Index.h>
#include <factory/Language.h>
#include <factory/Role.h>
#include <psql/Role.h>
// \ todo remove next include
#include <psql/Trigger.h>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>

#include <QtDebug>

// \todo Database should not have any SQL queries in it. Instead, it should
//       delegate SQL routines to DbTable's subclasses

namespace DbObjects
{

namespace Common
{

// null initialization of instance
Database *Database::mInstance = 0;

/*!
 * Constructor
 */
Database::Database()
{

}

/*!
 * Constructor. FORBIDDEN
 */
Database::Database(const Database &iInst)
    : //mInstance(iInst.mInstance),
      mSchemas(iInst.mSchemas),
      mRoles(iInst.mRoles),
      mIndices(iInst.mIndices),
      mLanguages(iInst.mLanguages),
      mSqlDriver(iInst.mSqlDriver)
{

}

/*!
 * Destructor
 */
Database::~Database()
{
    resetData();
    Database::mInstance = 0;
}

/*!
 * Assignment operator. FORBIDDEN
 */
const Database&
Database::operator=(const Database &iRhs)
{
    if (this != &iRhs)
    {
        mSchemas = iRhs.mSchemas;
        mRoles = iRhs.mRoles;
        mIndices = iRhs.mIndices;
        mLanguages = iRhs.mLanguages;
        mSqlDriver = iRhs.mSqlDriver;
    }

    return *this;
}

/*!
 * \brief Get/create an instance of Database
 *
 * \return Instance of Database
 */
Database*
Database::instance()
{
    // if not yet created => create
    if (0 == mInstance) {
        mInstance = new(std::nothrow) Database();
    }

    return mInstance;
}

/*!
 * \brief Add schema to DB schema list
 *
 * \param[in] ipSchema Schema object to add to schemas vector
 *
 * \return true - If the schema object has been added
 * \return false - If the schema object has already existed in the vector
 */
bool
Database::addSchema(DbSchema *ipSchema)
{
    if (mSchemas.contains(ipSchema)) {
        return false;
    }

    mSchemas.push_back(ipSchema);
    return true;
}

/*!
 * \brief Add role to DB roles list
 *
 * \param[in] ipRole Role object to add to roles vector
 *
 * \return true - If the role object has been added
 * \return false - If the role object has already existed in the vector
 */
bool
Database::addRole(DbRole *ipRole)
{
    if (mRoles.contains(ipRole)) {
        return false;
    }

    mRoles.push_back(ipRole);
    return true;
}

/*!
 * \brief Adds index to indices list
 *
 * \param[in] ipIndex Index object to add to indices vector
 *
 * \return true - If the index object has been added
 * \return false - If the index object has already existed in the vector
 */
bool
Database::addIndex(DbIndex *ipIndex)
{
    if (mIndices.contains(ipIndex)) {
        return false;
    }

    mIndices.push_back(ipIndex);
    return true;
}

/*!
 * \brief Adds language to list
 *
 * \param[in] ipLanguage Language object to add to indices vector
 *
 * \return true - If the language object has been added
 * \return false - If the language object has already existed in the vector
 */
bool
Database::addLanguage(DbLanguage *ipLanguage)
{
    if (mLanguages.contains(ipLanguage)) {
        return false;
    }

    mLanguages.push_back(ipLanguage);
    return true;
}

/*!
 * \brief Returns a list of schemas' names
 *
 * \param[out] opList List with names of stored schemas
 */
void
Database::schemasList(QStringList *opList) const
{
    if (0 == opList) {
        return;
    }

    opList->clear();

    QVector<DbSchema*>::const_iterator iter;

    for (iter = mSchemas.constBegin(); iter != mSchemas.constEnd(); ++iter) {
        opList->append((*iter)->name());
    }

    /* lyuts - if uncommented, then something gets wrong with the list, the list
     * has "public" and "public" 0_0
     *
     */
    //opList->sort();
}

/*!
 * \brief Returns a list of roles' names
 *
 * \param[out] opList List with names of stored roles
 */
void
Database::rolesList(QStringList *opList) const
{
    if (0 == opList) {
        return;
    }

    opList->clear();

    QVector<DbRole*>::const_iterator iter;

    for (iter = mRoles.constBegin(); iter != mRoles.constEnd(); ++iter) {
        opList->append((*iter)->name());
    }

    //opList->sort();
}

/*!
 * \brief Returns a list of indices' names
 *
 * \param[out] opList List with names of stored indices
 */
void
Database::indicesList(QStringList *opList) const
{
    if (0 == opList) {
        return;
    }

    opList->clear();

    QVector<DbIndex*>::const_iterator iter;

    for (iter = mIndices.constBegin(); iter != mIndices.constEnd(); ++iter) {
        opList->append((*iter)->name());
    }

    //opList->sort();
}

/*!
 * \brief Returns a list of languages' names
 *
 * \param[out] opList List with names of stored languages
 */
void
Database::languagesList(QStringList *opList) const
{
    if (0 == opList) {
        return;
    }

    opList->clear();

    QVector<DbLanguage*>::const_iterator iter;

    for (iter = mLanguages.constBegin(); iter != mLanguages.constEnd(); ++iter) {
        opList->append((*iter)->name());
    }

    //opList->sort();
}

/*!
 * \return Number of schemas currently store in Database
 */
quint64
Database::schemasCount() const
{
    return mSchemas.count();
}

/*!
 * \return Number of roles currently store in Database
 */
quint64
Database::rolesCount() const
{
    return mRoles.count();
}

/*!
 * \return Number of indices currently store in Database
 */
quint64
Database::indicesCount() const
{
    return mIndices.count();
}

/*!
 * \return Number of languages currently store in Database
 */
quint8
Database::languagesCount() const
{
    return mLanguages.count();
}

/*!
 * \brief Finds schema by its name
 *
 * \param[in] ipSchemaName - Name of a schema we are looking for
 *
 * \return Found schema object or NULL if not found
 */
DbSchema*
Database::findSchema(const QString &ipSchemaName) const
{
    return dynamic_cast<DbSchema*>(findObject(ipSchemaName, DbObject::SchemaObject));
}

/*!
 * \brief Finds role by its name
 *
 * \param[in] ipRoleName - Name of a role we are looking for
 *
 * \return Found role object or NULL if not found
 */
DbRole*
Database::findRole(const QString &ipRoleName) const
{
    return dynamic_cast<DbRole*>(findObject(ipRoleName, DbObject::RoleObject));
}

/*!
 * \brief Finds index by its name
 *
 * \param[in] ipIndexName - Name of an index we are looking for
 *
 * \return Found index object or NULL if not found
 */
DbIndex*
Database::findIndex(const QString &ipIndexName) const
{
    return dynamic_cast<DbIndex*>(findObject(ipIndexName, DbObject::IndexObject));
}

/*!
 * \brief Finds language by its name
 *
 * \param[in] ipLangName - Name of a language we are looking for
 *
 * \return Found language object or NULL if not found
 */
DbLanguage*
Database::findLanguage(const QString &ipLangName) const
{
    return dynamic_cast<DbLanguage*>(findObject(ipLangName, DbObject::LanguageObject));
}

/*!
 * \brief Finds all indices for a table
 *
 * \param[in] ipTable - Table object whose indices we are looking for
 * \param[out] opList - Resulting list of indices objects that are assigned
 *                      to the given table
 *
 * \return Number of found indices
 */
quint64
Database::findTableIndices(const DbTable *ipTable, QVector<DbIndex*> &opList) const
{
    opList.clear();

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
            opList.push_back(mIndices.at(i));
        }
    }

    return opList.count();
}
/*!
 * \brief Sets sql driver by its name
 *
 * \param[in] ipSqlDriverName - SQL driver name
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
    } else {
        db->mSqlDriver = Database::Unknown;

        qDebug() << __PRETTY_FUNCTION__ << "> Unknown SQL driver '" << ipSqlDriverName << "'";
    }

}

/*!
 * \brief Get SQL driver type
 *
 * \return SQL Driver type
 */
Database::SqlDriverType
Database::sqlDriver() const
{
    return mSqlDriver;
}
/*!
 * \brief Retrieve all available schemas from the DB
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
            qDebug() << __PRETTY_FUNCTION__ << "> SqlDriver was not set";
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
    qDebug() << __PRETTY_FUNCTION__ << qstr;
#endif

    // if query execution failed
    if (!query.exec(qstr)) {
        qDebug() << __PRETTY_FUNCTION__ << "> Unable to retrieve schemas.";
        qDebug() << __PRETTY_FUNCTION__ << query.lastError().text();

        return;
    }

    // if query returned nothing
    if (!query.first()) {
        qDebug() << __PRETTY_FUNCTION__ << "> No schemas were found.";

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
        DbSchema *schema = new(std::nothrow) DbSchema(name, dbRole);

        Q_CHECK_PTR(schema);

        // set scheme description
        schema->setDescription(description);

        /*!
         * We need to add schema to database's vector BEFORE we start
         * caling schema->read*() methods !!!
         */
        addSchema(schema);

        // read tables
        schema->readTables();
        // read views
        schema->readViews();
        // read procs
        schema->readProcedures();
        // read trigs
        schema->readTriggers();

    } while (query.next());
}

/*!
 * \brief Retrieve all available roles from the DB
 */
void
Database::readRoles()
{
    QSqlDatabase db = QSqlDatabase::database("mainConnect");
    QSqlQuery query(db);
    QString qstr;

    // clear roles list
    mRoles.clear();

    // \todo QStringList rolesList;
    //       Tools::rolesList(&rolesList);
    //       // and remove SELECTs
    //       // there should be no sql here

    // choose a query depending on sql driver
    switch (mSqlDriver) {
        case Database::Unknown:
                        qDebug() << __PRETTY_FUNCTION__ << "> SqlDriver was not set";
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
    qDebug() << __PRETTY_FUNCTION__ << qstr;
#endif

    // if query failed
    if (!query.exec(qstr)) {
        qDebug() << __PRETTY_FUNCTION__ << query.lastError().text();

        return;
    }

    // if query returned nothing
    if (!query.first()) {
        qDebug() << __PRETTY_FUNCTION__ << "> No roles were found.";

        return;
    }

    qint32 colId;
    // for every retrieved row
    do {
        colId = query.record().indexOf("name");
        Q_ASSERT(colId > 0);
        // declare new role object
        DbRole *role = Factory::Role::createRole(query.value(colId).toString());

        Q_ASSERT(role != 0);

        // add role
        addRole(role);

    } while (query.next());
}

/*!
 * \brief Retrieve all available indices from the DB
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
                        qDebug() << __PRETTY_FUNCTION__ << "> SqlDriver was not set";
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
    qDebug() << __PRETTY_FUNCTION__ << qstr;
#endif

    // if query failed
    if (!query.exec(qstr)) {
        qDebug() << __PRETTY_FUNCTION__ << query.lastError().text();

        return;
    }

    // if query returned nothing
    if (!query.first()) {
        qDebug() << __PRETTY_FUNCTION__ << "> No indices were found.";

        return;
    }

    // for every retrieved row
    do {
        QString name = query.value(query.record().indexOf("name")).toString();
        // declare new index object
        DbIndex *index = Factory::Index::createIndex(name);

        qint32 colId;

        // set index's attributes

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
        foreach (const QString &section, str.split(" ")) {
            index->addColumnNumber(section.toInt());
        }


        /* temporary debug output */
#if DEBUG_TRACE
        qDebug() << __PRETTY_FUNCTION__ << "index->name(): " << index->name();
        qDebug() << __PRETTY_FUNCTION__ << "index->isUnique: " << index->isUnique();
        qDebug() << __PRETTY_FUNCTION__ << "index->isPrimary: " << index->isPrimary();
        qDebug() << __PRETTY_FUNCTION__ << "index->isClustered: " << index->isClustered();
        qDebug() << __PRETTY_FUNCTION__ << "index->isValid: " << index->isValid();
        qDebug() << __PRETTY_FUNCTION__ << "index->checksXMin: " << index->checksXMin();
        qDebug() << __PRETTY_FUNCTION__ << "index->isReady: " << index->isReady();
        qDebug() << __PRETTY_FUNCTION__ << "index->tableName: " << index->tableName();
        qDebug() << __PRETTY_FUNCTION__ << "index->schemaName: " << index->schemaName();
        qDebug() << __PRETTY_FUNCTION__ << "index->columnsCount: " << index->columnsCount();
        qDebug() << __PRETTY_FUNCTION__ << "index->columnsNumbers: " << index->columnsNumbers();
#endif

        // add index
        addIndex(index);

    } while (query.next());

}

/*!
 * \brief Retrieve all available languages from the DB
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
                        qDebug() << __PRETTY_FUNCTION__ << "> SqlDriver was not set";
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
    qDebug() << __PRETTY_FUNCTION__ << qstr;
#endif

    // if query failed
    if (!query.exec(qstr)) {
        qDebug() << __PRETTY_FUNCTION__ << query.lastError().text();

        return;
    }

    // if query returned nothing
    if (!query.first()) {
        qDebug() << __PRETTY_FUNCTION__ << "> No languages were found.";

        return;
    }

    // for every retrieved row
    do {
        QString name = query.value(query.record().indexOf("name")).toString();
        // declare new language object
        DbLanguage *lang = Factory::Language::createLanguage(name);

        qint32 colId;

        // set lang's attributes

        /*colId= query.record().indexOf("name");
        Q_ASSERT(colId > 0);
        index->setName(query.value(colId).toString());*/


        colId = query.record().indexOf("trusted");
        Q_ASSERT(colId > 0);
        lang->setTrusted(query.value(colId).toBool());

        /* temporary debug output */
#if DEBUG_TRACE
        qDebug() << __PRETTY_FUNCTION__ << "lang->name() = " << lang->name();
        qDebug() << __PRETTY_FUNCTION__ << "lang->isTrusted() =  " << lang->isTrusted();
#endif

        // add language
        addLanguage(lang);

    } while (query.next());
}

/*!
 * Read everything from database
 *
 * \return true - Even if nothing has been read
 */
bool
Database::loadData()
{
    readRoles();
    readLanguages();
    readSchemas();
    readIndices();

    return true;
}

/*!
 * \brief Cleanup data (all objects' vectors)
 */
void
Database::resetData()
{
#ifdef DEBUG_TRACE
    qDebug() << __PRETTY_FUNCTION__ << "> cleaning...";
#endif

    quint64 schemasCount = mSchemas.count();

    for (quint64 i = 0; i < schemasCount; ++i) {
        mSchemas.at(i)->resetData();
    }

    // clear vectors
    qDeleteAll(mSchemas);
    mSchemas.clear();

    qDeleteAll(mRoles);
    mRoles.clear();

    qDeleteAll(mIndices);
    mIndices.clear();

    qDeleteAll(mLanguages);
    mLanguages.clear();
}

/*!
 * \brief Searches for an object by its name
 *
 * An auxilliary function used by findSchema, findRole, findIndex and
 * findLanguage functions
 *
 * \param[in] ipObjectName - Object name to look for
 * \param[in] ipObjectType - Type of object we are looking for
 *
 * \return A pointer to a found object or NULL if search failed
 *
 * \todo Try to make the search algorithm more "cute", that will not
 *       have a look of huge switches and ifs
 */
DbObject*
Database::findObject(const QString &ipObjectName, DbObject::Type ipObjectType) const
{
    quint64 count;
    QStringList list;

    // detect object type and read all objects of that type
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
                qDebug() << __PRETTY_FUNCTION__ << "> Unknown object type";
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
    while (i < count && ipObjectName != list.at(i)) {
        i++;
    }

    // if object was found
    if (!(i == count - 1 && ipObjectName != list.at(i))) {
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

/*!
 * \brief Deletes an instance of the Database class
 */
void
DatabaseManager::flush()
{
    delete Database::mInstance;
}

} // namespace Common

} // namespace DbObjects


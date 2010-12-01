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
#include <common/DbProcedure.h>
#include <common/DbRole.h>
#include <common/DbSchema.h>
#include <common/DbTable.h>
#include <common/DbTrigger.h>
#include <common/DbView.h>
#include <mysql/Tools.h>
#include <psql/Tools.h>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>

#include <QtDebug>

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
    : mSchemas(),
      mRoles(),
      mIndices(),
      mLanguages(),
      mSqlDriver(Unknown)
//      mNewObjects(),
//      mModifiedObjects(),
//      mDeletedObjects()
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
 * \brief Register an object that has been created
 *
 * \param[in] ipObject - Pointer to the object that we want to register
 *
 * \return true If the object has been successfully registered
 * \return false If the object has been registered earlier
 */
//bool
//Database::registerNew(const DbObject *ipObject)
//{
//    if (mNewObjects.contains(ipObject)) {
//        return false;
//    }
//
//    mNewObjects.push_back(ipObject);
//    return true;
//}

/*!
 * \brief Register an object that has been modified
 *
 * \param[in] ipObject - Pointer to the object that we want to register
 *
 * \return true If the object has been successfully registered
 * \return false If the object has been registered earlier
 */
//bool
//Database::registerModified(const DbObject *ipObject)
//{
//    if (mModifiedObjects.contains(ipObject)) {
//        return false;
//    }
//
//    mModifiedObjects.push_back(ipObject);
//    return true;
//}

/*!
 * \brief Register an object that has been deleted
 *
 * \param[in] ipObject - Pointer to the object that we want to register
 *
 * \return true If the object has been successfully registered
 * \return false If the object has been registered earlier
 */
//bool
//Database::registerDeleted(const DbObject *ipObject)
//{
//    if (mDeletedObjects.contains(ipObject)) {
//        return false;
//    }
//
//    mDeletedObjects.push_back(ipObject);
//    return true;
//}

/*!
 * \brief Add schema to DB schema list
 *
 * \param[in] ipSchema - Schema object to add to schemas vector
 *
 * \return true - If the schema object has been added
 * \return false - If the schema object has already existed in the vector
 */
bool
Database::addSchema(const DbSchemaPtr &ipSchema)
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
Database::addRole(const DbRolePtr &ipRole)
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
Database::addIndex(const DbIndexPtr &ipIndex)
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
Database::addLanguage(const DbLanguagePtr &ipLanguage)
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
Database::schemasList(QStringList &opList) const
{
    opList.clear();

    QVector<DbSchemaPtr>::const_iterator iter;

    for (iter = mSchemas.constBegin(); iter != mSchemas.constEnd(); ++iter) {
        opList.append(iter->name());
    }
}

/*!
 * \brief Returns a list of roles' names
 *
 * \param[out] opList List with names of stored roles
 */
void
Database::rolesList(QStringList &opList) const
{
    opList.clear();

    QVector<DbRolePtr>::const_iterator iter;

    for (iter = mRoles.constBegin(); iter != mRoles.constEnd(); ++iter) {
        opList.append(iter->name());
    }
}

/*!
 * \brief Returns a list of indices' names
 *
 * \param[out] opList List with names of stored indices
 */
void
Database::indicesList(QStringList &opList) const
{
    opList.clear();

    QVector<DbIndexPtr>::const_iterator iter;

    for (iter = mIndices.constBegin(); iter != mIndices.constEnd(); ++iter) {
        opList.append(iter->name());
    }
}

/*!
 * \brief Returns a list of languages' names
 *
 * \param[out] opList List with names of stored languages
 */
void
Database::languagesList(QStringList &opList) const
{
    opList.clear();

    QVector<DbLanguagePtr>::const_iterator iter;

    for (iter = mLanguages.constBegin(); iter != mLanguages.constEnd(); ++iter) {
        opList.append(iter->name());
    }
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
DbSchemaPtr
Database::findSchema(const QString &ipSchemaName) const
{
    foreach (const DbSchemaPtr &schema, mSchemas) {
        if (ipSchemaName == schema.name()) {
            return schema;
        }
    }
    return DbSchemaPtr();
}

/*!
 * \brief Finds role by its name
 *
 * \param[in] ipRoleName - Name of a role we are looking for
 *
 * \return Found role object or NULL if not found
 */
DbRolePtr
Database::findRole(const QString &ipRoleName) const
{
    foreach (const DbRolePtr &role, mRoles) {
        if (ipRoleName == role.name()) {
            return role;
        }
    }
    return DbRolePtr();
}

/*!
 * \brief Finds index by its name
 *
 * \param[in] ipIndexName - Name of an index we are looking for
 *
 * \return Found index object or NULL if not found
 */
DbIndexPtr
Database::findIndex(const QString &ipIndexName) const
{
    foreach (const DbIndexPtr &index, mIndices) {
        if (ipIndexName == index.name()) {
            return index;
        }
    }

    return DbIndexPtr();
}

/*!
 * \brief Finds language by its name
 *
 * \param[in] ipLanguageName - Name of a language we are looking for
 *
 * \return Found language object or NULL if not found
 */
DbLanguagePtr
Database::findLanguage(const QString &ipLanguageName) const
{
    foreach (const DbLanguagePtr &language, mLanguages) {
        if (ipLanguageName == language.name()) {
            return language;
        }
    }
    return DbLanguagePtr();
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
Database::findTableIndices(DbTablePtr &ipTable, QVector<DbIndexPtr> &opList) const
{
    opList.clear();

    quint64 count = mIndices.count();

    // if we don't have any indices
    if (0 == count || 0 == ipTable.valid()) {
        return 0;
    }

    QString schemaName = ipTable.schemaName();
    QString tableName = ipTable.name();

    // look through index's table names
    for (quint64 i = 0; i < count; ++i) {
        // if index is for our table -- add it
        qDebug() << "Database::findTableIndices> table.name() = [" << mIndices.at(i)->table().name() << "]";
        if (tableName == mIndices.at(i)->table().name()
            && schemaName == mIndices.at(i)->schema().name()) {
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
    QStringList schemasNamesList;

    // clear schemas list
    mSchemas.clear();

    // read schemas names depending on sql driver
    switch (mSqlDriver) {
        case Database::Unknown:
            qDebug() << __PRETTY_FUNCTION__ << "> SqlDriver was not set";
        case Database::PostgreSQL:
            Psql::Tools::schemasList(schemasNamesList);
            break;
        case Database::MySQL:
            Mysql::Tools::schemasList(schemasNamesList);
            break;
        case Database::Oracle:
        case Database::SQLite:
        default:
            /* temporarily no support for these DBMS */
            return;
            break;
    }

    // for every retrieved row
    foreach (const QString &name, schemasNamesList) {
        // create new schema object
        DbSchemaPtr schema(name);

        Q_CHECK_PTR(schema.get());

        // we should add schema to database vector BEFORE we start calling read* functions
        addSchema(schema);

        schema->loadChildren();
    }
}

/*!
 * \brief Retrieve all available roles from the DB
 */
void
Database::readRoles()
{
    // clear roles list
    mRoles.clear();

    QStringList rolesList;
    // read names depending on sql driver
    switch (mSqlDriver) {
        case Database::Unknown:
                        qDebug() << __PRETTY_FUNCTION__ << "> SqlDriver was not set";
                        return;
        case Database::PostgreSQL:
                        Psql::Tools::rolesList(rolesList);
                        break;
        case Database::MySQL:
                        Mysql::Tools::rolesList(rolesList);
                        break;
        case Database::Oracle:
        case Database::SQLite:
        default:
                        /* temporarily no support for these DBMS */
                        return;
                        break;
    }

    DbRolePtr role;
    // for every retrieved row
    foreach (const QString &name, rolesList) {
        // declare new role object
        role = DbRolePtr(name);

        Q_ASSERT(0 != role.get());

        // add role
        addRole(role);
    }
}

/*!
 * \brief Retrieve all available indices from the DB
 */
void
Database::readIndices()
{
    // clear indices list
    mIndices.clear();

    QStringList indicesList;
    // read names depending on sql driver
    switch (mSqlDriver) {
        case Database::Unknown:
                        qDebug() << __PRETTY_FUNCTION__ << "> SqlDriver was not set";
                        return;
        case Database::PostgreSQL:
                        Psql::Tools::indicesList(indicesList);
                        break;
        case Database::MySQL:
                        Mysql::Tools::indicesList(indicesList);
                        break;
        case Database::Oracle:
        case Database::SQLite:
        default:
                        /* temporarily no support for these DBMS */
                        return;
                        break;
    }

    // for every retrieved row
    foreach (const QString &name, indicesList) {
        // declare new index object
        DbIndexPtr index(name);

        // add index
        addIndex(index);

    }

}

/*!
 * \brief Retrieve all available languages from the DB
 */
void
Database::readLanguages()
{
    // clear languages list
    mLanguages.clear();

    QStringList languagesList;
    // read names depending on sql driver
    switch (mSqlDriver) {
        case Database::Unknown:
                        qDebug() << __PRETTY_FUNCTION__ << "> SqlDriver was not set";
                        return;
        case Database::PostgreSQL:
                        Psql::Tools::languagesList(languagesList);
                        break;
        case Database::MySQL:
                        Mysql::Tools::languagesList(languagesList);
                        break;
        case Database::Oracle:
        case Database::SQLite:
        default:
                        /* temporarily no support for these DBMS */
                        return;
                        break;
    }

    // for every retrieved row
    foreach (const QString &name, languagesList) {
        // declare new language object
        DbLanguagePtr lang(name);

        // add language
        addLanguage(lang);
    }
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


    foreach (DbSchemaPtr schema, mSchemas) {
        schema->resetData();
    }

    // clear vectors
    mSchemas.clear();

    mRoles.clear();
    mIndices.clear();
    mLanguages.clear();

    /*! We don't need to call qDeleteAll for UnitOfWork pattern's vectors, we just need to
     * flush them.
     * Reason: We don't need to destroy the objects, they should stay alive
     */
//    mNewObjects.clear();
//    mModifiedObjects.clear();
//    mDeletedObjects.clear();
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


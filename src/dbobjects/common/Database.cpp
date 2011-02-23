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
//Database *Database::mInstance = 0;

/*!
 * Constructor
 */
Database::Database(const QSqlDatabase &iDbHandle)
    : mDbHandle(iDbHandle),
      mSchemas(),
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
//    Database::mInstance = 0;
}

/*!
 * \brief Get/create an instance of Database
 *
 * \return Instance of Database
 */
Database*
Database::instance()
{
    /*intended hack*/return 0;
//    // if not yet created => create
//    if (0 == mInstance) {
//        mInstance = new(std::nothrow) Database();
//    }
//
//    return mInstance;
}

/*!
 * \brief Register an object that has been created
 *
 * \param[in] iObject - Pointer to the object that we want to register
 *
 * \return true If the object has been successfully registered
 * \return false If the object has been registered earlier
 */
//bool
//Database::registerNew(const DbObject *iObject)
//{
//    if (mNewObjects.contains(iObject)) {
//        return false;
//    }
//
//    mNewObjects.push_back(iObject);
//    return true;
//}

/*!
 * \brief Register an object that has been modified
 *
 * \param[in] iObject - Pointer to the object that we want to register
 *
 * \return true If the object has been successfully registered
 * \return false If the object has been registered earlier
 */
//bool
//Database::registerModified(const DbObject *iObject)
//{
//    if (mModifiedObjects.contains(iObject)) {
//        return false;
//    }
//
//    mModifiedObjects.push_back(iObject);
//    return true;
//}

/*!
 * \brief Register an object that has been deleted
 *
 * \param[in] iObject - Pointer to the object that we want to register
 *
 * \return true If the object has been successfully registered
 * \return false If the object has been registered earlier
 */
//bool
//Database::registerDeleted(const DbObject *iObject)
//{
//    if (mDeletedObjects.contains(iObject)) {
//        return false;
//    }
//
//    mDeletedObjects.push_back(iObject);
//    return true;
//}

/*!
 * \return Database connection handle
 */
QSqlDatabase
Database::dbHandle() const
{
    return mDbHandle;
}

/*!
 * \brief Add schema to DB schema list
 *
 * \param[in] iSchema - Schema object to add to schemas vector
 *
 * \return true - If the schema object has been added
 * \return false - If the schema object has already existed in the vector
 */
bool
Database::addSchema(const DbSchemaPtr &iSchema)
{
    if (mSchemas.contains(iSchema)) {
        return false;
    }

    mSchemas.push_back(iSchema);
    return true;
}

/*!
 * \brief Add role to DB roles list
 *
 * \param[in] iRole Role object to add to roles vector
 *
 * \return true - If the role object has been added
 * \return false - If the role object has already existed in the vector
 */
bool
Database::addRole(const DbRolePtr &iRole)
{
    if (mRoles.contains(iRole)) {
        return false;
    }

    mRoles.push_back(iRole);
    return true;
}

/*!
 * \brief Adds index to indices list
 *
 * \param[in] iIndex Index object to add to indices vector
 *
 * \return true - If the index object has been added
 * \return false - If the index object has already existed in the vector
 */
bool
Database::addIndex(const DbIndexPtr &iIndex)
{
    if (mIndices.contains(iIndex)) {
        return false;
    }

    mIndices.push_back(iIndex);
    return true;
}

/*!
 * \brief Adds language to list
 *
 * \param[in] iLanguage Language object to add to indices vector
 *
 * \return true - If the language object has been added
 * \return false - If the language object has already existed in the vector
 */
bool
Database::addLanguage(const DbLanguagePtr &iLanguage)
{
    if (mLanguages.contains(iLanguage)) {
        return false;
    }

    mLanguages.push_back(iLanguage);
    return true;
}

/*!
 * \brief Returns a list of schemas' names
 *
 * \param[out] oList List with names of stored schemas
 */
void
Database::schemasList(QStringList &oList) const
{
    oList.clear();

    QVector<DbSchemaPtr>::const_iterator iter;

    for (iter = mSchemas.constBegin(); iter != mSchemas.constEnd(); ++iter) {
        oList.append(iter->name());
    }
}

/*!
 * \brief Returns a list of roles' names
 *
 * \param[out] oList List with names of stored roles
 */
void
Database::rolesList(QStringList &oList) const
{
    oList.clear();

    QVector<DbRolePtr>::const_iterator iter;

    for (iter = mRoles.constBegin(); iter != mRoles.constEnd(); ++iter) {
        oList.append(iter->name());
    }
}

/*!
 * \brief Returns a list of indices' names
 *
 * \param[out] oList List with names of stored indices
 */
void
Database::indicesList(QStringList &oList) const
{
    oList.clear();

    QVector<DbIndexPtr>::const_iterator iter;

    for (iter = mIndices.constBegin(); iter != mIndices.constEnd(); ++iter) {
        oList.append(iter->name());
    }
}

/*!
 * \brief Returns a list of languages' names
 *
 * \param[out] oList List with names of stored languages
 */
void
Database::languagesList(QStringList &oList) const
{
    oList.clear();

    QVector<DbLanguagePtr>::const_iterator iter;

    for (iter = mLanguages.constBegin(); iter != mLanguages.constEnd(); ++iter) {
        oList.append(iter->name());
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
 * \param[in] iSchemaName - Name of a schema we are looking for
 *
 * \return Found schema object or NULL if not found
 */
DbSchemaPtr
Database::findSchema(const QString &iSchemaName) const
{
    foreach (const DbSchemaPtr &schema, mSchemas) {
        if (iSchemaName == schema.name()) {
            return schema;
        }
    }
    return DbSchemaPtr();
}

/*!
 * \brief Finds role by its name
 *
 * \param[in] iRoleName - Name of a role we are looking for
 *
 * \return Found role object or NULL if not found
 */
DbRolePtr
Database::findRole(const QString &iRoleName) const
{
    foreach (const DbRolePtr &role, mRoles) {
        if (iRoleName == role.name()) {
            return role;
        }
    }
    return DbRolePtr();
}

/*!
 * \brief Finds index by its name
 *
 * \param[in] iIndexName - Name of an index we are looking for
 *
 * \return Found index object or NULL if not found
 */
DbIndexPtr
Database::findIndex(const QString &iIndexName) const
{
    foreach (const DbIndexPtr &index, mIndices) {
        if (iIndexName == index.name()) {
            return index;
        }
    }

    return DbIndexPtr();
}

/*!
 * \brief Finds language by its name
 *
 * \param[in] iLanguageName - Name of a language we are looking for
 *
 * \return Found language object or NULL if not found
 */
DbLanguagePtr
Database::findLanguage(const QString &iLanguageName) const
{
    foreach (const DbLanguagePtr &language, mLanguages) {
        if (iLanguageName == language.name()) {
            return language;
        }
    }
    return DbLanguagePtr();
}

/*!
 * \brief Finds all indices for a table
 *
 * \param[in] iTable - Table object whose indices we are looking for
 * \param[out] oList - Resulting list of indices objects that are assigned
 *                      to the given table
 *
 * \return Number of found indices
 */
quint64
Database::findTableIndices(const DbTablePtr &iTable, QVector<DbIndexPtr> &oList) const
{
    oList.clear();

    quint64 count = mIndices.count();

    // if we don't have any indices
    if (0 == count || !iTable.valid()) {
        return 0;
    }

    QString schemaName = iTable.schemaName();
    QString tableName = iTable.name();

    // look through index's table names
    for (quint64 i = 0; i < count; ++i) {
        // if index is for our table -- add it
        qDebug() << "Database::findTableIndices> table.name() = [" << mIndices.at(i)->table().name() << "]";
        if (tableName == mIndices.at(i)->table().name()
            && schemaName == mIndices.at(i)->schema().name()) {
            oList.push_back(mIndices.at(i));
        }
    }

    return oList.count();
}
/*!
 * \brief Sets sql driver by its name
 *
 * \param[in] iSqlDriverName - SQL driver name
 */
void
Database::setSqlDriver(const QString &iSqlDriverName)
{
//    Database* db = instance();

    if (iSqlDriverName.contains("PSQL")) {
        mSqlDriver = PostgreSQL;
    } else if (iSqlDriverName.contains("MYSQL")) {
        mSqlDriver = MySQL;
    } else if (iSqlDriverName.contains("ODBC")) {
        mSqlDriver = Oracle;
    } else if (iSqlDriverName.contains("SQLITE")) {
        mSqlDriver = Database::SQLite;
    } else {
        mSqlDriver = Database::Unknown;

        qDebug() << __PRETTY_FUNCTION__ << "> Unknown SQL driver '" << iSqlDriverName << "'";
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
            Psql::Tools::schemasList(mDbHandle, schemasNamesList);
            break;
        case Database::MySQL:
            Mysql::Tools::schemasList(mDbHandle, schemasNamesList);
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
                        Psql::Tools::rolesList(mDbHandle, rolesList);
                        break;
        case Database::MySQL:
                        Mysql::Tools::rolesList(mDbHandle, rolesList);
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
                        Psql::Tools::indicesList(mDbHandle, indicesList);
                        break;
        case Database::MySQL:
                        Mysql::Tools::indicesList(mDbHandle, indicesList);
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
                        Psql::Tools::languagesList(mDbHandle, languagesList);
                        break;
        case Database::MySQL:
                        Mysql::Tools::languagesList(mDbHandle, languagesList);
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

} // namespace Common

} // namespace DbObjects


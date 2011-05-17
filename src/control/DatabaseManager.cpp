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

#include <control/Context.h>
#include <control/DatabaseManager.h>
#include <dbobjects/common/Database.h>
#include <dbobjects/common/DbObjectPtr.h>
#include <dbobjects/mysql/Factories.h>
#include <dbobjects/mysql/Tools.h>
#include <dbobjects/psql/Factories.h>
#include <dbobjects/psql/Tools.h>
#include <QFile>
#include <QSqlDatabase>
#include <QSqlError>

namespace Control
{

/*!
 * Constructor
 */
DatabaseManager::DatabaseManager()
{

}

/*!
 * Destructor
 */
DatabaseManager::~DatabaseManager()
{
    foreach (DbObjects::Common::Database *db, mRegistry.values()) {
        remove(db);
    }
    mRegistry.clear();

    qDeleteAll(mFactories);
    mFactories.clear();

    qDeleteAll(mTools);
    mTools.clear();
}

/*!
 * Receives vector of indices for given context and db table.
 *
 * \param[in] iContext - context we are dealing with
 * \param[in] iDbTablePtr - we will find indices for this table.
 * \param[out] oDbIndexPtrVector - here we will store all found indices.
 *
 * \return true if everething is ok, false otherwise.
 */
bool
DatabaseManager::indices(const Context &iContext,
        const DbObjects::Common::DbTablePtr &iDbTablePtr,
        QVector<DbObjects::Common::DbIndexPtr> oDbIndexPtrVector)
{
    using namespace DbObjects::Common;

    Database *db = findDatabase(&iContext);

    Q_ASSERT(db);

    db->findTableIndices(iDbTablePtr, oDbIndexPtrVector);

    return true;
}

/*!
 * Establishes a connection and creates a context for it.
 *
 * \param[in] iInfo - Connection info
 * \param[out] oErrorMsg - Text of occured error, if connection to db failed. This text is
 * used for displaying to a user via message box.
 *
 * \note The caller of this function doesn't need to user lastError(Control::Context *)
 * function and has to use oErrorMsg parameter, because if some error occurs during
 * execution of this function it means we don't have a context at all, so we can't get the
 * error for the context that has not been created.
 *
 * \return Context for the specified connection info if the connection was successfully
 * established. Otherwise, it return 0;
 */
Control::Context*
DatabaseManager::establishConnection(const Connect::ConnectionInfo &iInfo, QString &oErrorMsg)
{
    // set proxy connection
    iInfo.proxyHostInfo().activate();

    // set db
    bool success = false;
    QString connName = QString("%1@%2 (%3)")
        .arg(iInfo.dbHostInfo().dbName())
        .arg(iInfo.dbHostInfo().address())
        .arg(iInfo.dbHostInfo().user());

    QSqlDatabase db = QSqlDatabase::addDatabase(iInfo.dbHostInfo().dbDriver(), connName);
    db.setDatabaseName(iInfo.dbHostInfo().dbName());

    // if SQLite db
    if (QString("QSQLITE") == iInfo.dbHostInfo().dbDriver()) {
        // check only if fie exists
        if ((success = QFile::exists(iInfo.dbHostInfo().dbName()))) {
            db.open();
        }
    // of other DBMS
    } else {
        // check credentials and host
        db.setHostName(iInfo.dbHostInfo().address());
        db.setUserName(iInfo.dbHostInfo().user());
        db.setPassword(iInfo.dbHostInfo().password());
        db.setPort(iInfo.dbHostInfo().port());

        success = db.open();
    }

    Control::Context *ctx = 0;
    if (success) {
        using namespace DbObjects::Common;
        ctx = new Context(iInfo, db);

        Database *database = new Database(db);
        database->setSqlDriver(iInfo.dbHostInfo().dbDriver());

        add(ctx, database);
        oErrorMsg.clear();
    } else {
        oErrorMsg = db.lastError().text();
    }

    return ctx;
}

/*!
 * Reloads database for for the specified context.
 *
 * \param[in] iCtx - Context whose data is going to be reloaded
 *
 * \return true - If data for the given context has been successfully reloaded
 * \return false - Otherwise
 */
bool
DatabaseManager::reloadData(Control::Context *iCtx) const
{
    using namespace DbObjects::Common;

    Database *db = findDatabase(iCtx);
    db->resetData();

    // find factories for this driver
    Factories *factories = mFactories.value(db->sqlDriver());
    // find tools for this driver
    Tools *tools = mTools.value(db->sqlDriver());

    Q_ASSERT(0 != factories);
    Q_ASSERT(0 != tools);

    return db->loadData(factories, tools);
}

/*!
 * Adds contex-database pair to the registry.
 *
 * \param[in] iContext - Context
 * \param[in] iDatabase - Database that corresponds to the given context
 *
 * \return true - If the pair has been successfully registered.
 * \return false - Otherwise, i.e. the pair has been registered before.
 */
bool
DatabaseManager::add(const Control::Context *iContext, DbObjects::Common::Database *iDatabase)
{
    Q_ASSERT(0 != iContext);
    Q_ASSERT(0 != iDatabase);

    if (!mRegistry.contains(iContext)) {
        using namespace DbObjects;
        using namespace DbObjects::Common;

        mRegistry.insert(iContext, iDatabase);
        Factories *factories = 0;
        Tools *tools = 0;

        // if no factories/tools were created before - then create and put into registry
        switch (iDatabase->sqlDriver()) {
            case Database::PostgreSQL:
                    factories = new(std::nothrow) Psql::Factories();
                    tools = new(std::nothrow) Psql::Tools();
                    break;

            case Database::MySQL:
                    factories = new(std::nothrow) Mysql::Factories();
                    tools = new(std::nothrow) Mysql::Tools();
                    break;

            case Database::Oracle:
            case Database::SQLite:
            case Database::Unknown:
                    break;
        }

        Q_ASSERT(0 != factories);
        Q_ASSERT(0 != tools);

        mFactories.insert(iDatabase->sqlDriver(), factories);
        mTools.insert(iDatabase->sqlDriver(), tools);

        return true;
    }

    // unlike director, whose map is many-to-one, we have one-to-one registry and if we
    // got here, then it means we are trying to register several contexts for the same
    // database (or several databases for the same context), which is wrong.
    Q_ASSERT(false);
    return false;
}

/*!
 * Removes given context and its database from the registry.
 *
 * \param[in] iContext - Context we are trying to unregsiter.
 *
 * \return true - If the pair has been successfully unregsitered.
 * \return false - Otherwise.
 */
bool
DatabaseManager::remove(const Control::Context *iContext)
{
    if (mRegistry.contains(iContext)) {
        DbObjects::Common::Database *database = mRegistry.value(iContext);
        database->resetData();
        delete database;

        iContext->dbHandle().close();

        mRegistry.remove(iContext);

        // \note we won't drop factories/tools here. They will be dropped in destructor
        return true;
    }

    // We should not get here (in theory). If we got here, then it means that we are
    // trying to unregister a context that has never been registered, i.e. we missed its
    // registration and need to check the code.
    Q_ASSERT(false);

    return false;
}

/*!
 * Removes a record for the specified database from the registry
 *
 * \param[in] iDatabase - Database that we are trying to unregister.
 *
 * \return true - If the database has been successfully unregistered.
 * \return false - Otherwise.
 */
bool
DatabaseManager::remove(DbObjects::Common::Database *iDatabase)
{
    QList<const Control::Context *> contexts = mRegistry.keys(iDatabase);

    // we have 1-to-1 registry, that's why the next condition should always be kept.
    // note, that this assert may fail if:
    // 1) we have several contexts for the same database.
    // 2) we don't have any context for the given database.
    // Both cases are invalid, and if they occur, we need to check code.
    Q_ASSERT(contexts.size() == 1);

    return remove(contexts.first());
}

/*!
 * Finds the context for the specified database.
 *
 * \param[in] iDatabase - Database
 *
 * \return Context for the specified database.
 */
const Control::Context*
DatabaseManager::findContext(DbObjects::Common::Database *iDatabase) const
{
    return mRegistry.key(iDatabase, 0);
}

/*!
 * Finds the database of the specified context.
 *
 * \param[in] iContext - Context whose database we are looking for
 *
 * \return Database of the specified context
 */
DbObjects::Common::Database*
DatabaseManager::findDatabase(const Control::Context *iContext) const
{
    return mRegistry.value(iContext);
}

/*!
 * \param[in] iCtx - Context whose last error we want to get.
 *
 * \return Text of the very last error that occured for the given database.
 */
QString
DatabaseManager::lastError(const Control::Context *iCtx) const
{
    return findDatabase(iCtx)->lastError();
}

/*!
 * Reads new objects for the given context
 *
 * \param[in] iCtx - Context
 * \param[out] oList - List with indices names.
 */
void
DatabaseManager::newObjects(const Control::Context *iCtx, Objects &oList) const
{
    using namespace DbObjects::Common;

    Database *db = findDatabase(iCtx);

    if (db) {
        QString parentName("");

        // roles
        QStringList rolesNames;
        db->rolesList(rolesNames);
        foreach (const QString &name, rolesNames) {
            oList.insert(name, qMakePair(parentName, (int)DbObject::RoleObject));
        }

        // languages
        QStringList languagesNames;
        db->languagesList(languagesNames);
        foreach (const QString &name, languagesNames) {
            oList.insert(name, qMakePair(parentName, (int)DbObject::LanguageObject));
        }

        // indices
        QStringList indicesNames;
        db->indicesList(indicesNames);
        foreach (const QString &name, indicesNames) {
            oList.insert(name, qMakePair(parentName, (int)DbObject::IndexObject));
        }

        // schemas
        QStringList schemasNames;
        db->schemasList(schemasNames);
        foreach (const QString &name, schemasNames) {
            oList.insert(name, qMakePair(parentName, (int)DbObject::SchemaObject));
        }

        // read schemas children
        QStringList childrenNames;
        foreach (const QString &schemaName, schemasNames) {
            DbSchemaPtr schema = db->findSchema(schemaName);
            Q_ASSERT(schema.valid());

            // tables
            schema->tablesList(childrenNames);
            foreach (const QString &name, childrenNames) {
                oList.insert(name, qMakePair(schemaName, (int)DbObject::TableObject));
            }

            childrenNames.clear();

            // triggers
            schema->triggersList(childrenNames);
            foreach (const QString &name, childrenNames) {
                oList.insert(name, qMakePair(schemaName, (int)DbObject::TriggerObject));
            }

            childrenNames.clear();

            // views
            schema->viewsList(childrenNames);
            foreach (const QString &name, childrenNames) {
                oList.insert(name, qMakePair(schemaName, (int)DbObject::ViewObject));
            }

            childrenNames.clear();

            // procs
            schema->proceduresList(childrenNames);
            foreach (const QString &name, childrenNames) {
                oList.insert(name, qMakePair(schemaName, (int)DbObject::ProcedureObject));
            }
        }
    }
}

} // namespace Control


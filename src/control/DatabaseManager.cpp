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
#include <QFile>
#include <QSqlDatabase>
#include <QSqlError>

namespace Control
{

/*!
 *
 */
DatabaseManager::DatabaseManager()
{

}

/*!
 *
 */
DatabaseManager::~DatabaseManager()
{

}

/*!
 * \brief Deletes an instance of the Database class
 * \todo replace this function
 */
void
DatabaseManager::flush()
{
//    delete Database::mInstance;
}

/*!
 *
 * \todo Implement
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
        ctx = new Context(iInfo, db);
        // \todo create Database
//        DbObjects::Common::Database *database = new Database(db, iInfo.dbHostInfo().dbDriver());
        // \todo register ctx<->db pair
//        add(ctx, database);
        oErrorMsg.clear();
    } else {
        oErrorMsg = db.lastError().text();
    }

    return ctx;
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
    if (!mRegistry.contains(iContext)) {
        mRegistry.insert(iContext, iDatabase);
        return true;
    }

    // unlike director, whose map is many-to-one, we have one-one registry and if we got
    // here, then it means we are trying to register several contexts for the same
    // database, which is wrong.
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
    if (!mRegistry.contains(iContext)) {
        mRegistry.remove(iContext);
        // \todo destroy database
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
 *
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

} // namespace Control


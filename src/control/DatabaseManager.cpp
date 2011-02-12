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
        // \todo set driver for it
        // \todo set db connection descriptor
        // \todo register ctx<->db pair
    } else {
        oErrorMsg = db.lastError().text();
    }

    return ctx;
}

} // namespace Control


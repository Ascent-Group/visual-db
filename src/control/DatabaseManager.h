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

#ifndef CONTROL_DATABASEMANAGER_H
#define CONTROL_DATABASEMANAGER_H

#include <dbobjects/common/Database.h>
#include <connect/ConnectionInfo.h>

namespace DbObjects
{
namespace Common
{
//class Database;
class Factories;
class Tools;
}
}

namespace Control
{

class Context;

/*!
 * \class DatabaseManager
 * \headerfile control/Database.h
 * \brief This class manages databases and controls them
 */
class DatabaseManager
{
    public:
        ~DatabaseManager();

    private:
        DatabaseManager();
        /*!
         * One-to-one registry of contexts and database instances. Context is under
         * control of director, that's why we handle it as const. As for the database,
         * then we don't make it const, since Database manager is taking care of all of
         * them.
         */
        QMap<const Control::Context *, DbObjects::Common::Database *> mRegistry;
        // \todo Maybe we should combine next 2 maps ???
        QMap<DbObjects::Common::Database::SqlDriverType, DbObjects::Common::Factories *> mFactories;
        QMap<DbObjects::Common::Database::SqlDriverType, DbObjects::Common::Tools *> mTools;

        Control::Context* establishConnection(const Connect::ConnectionInfo &, QString &);
        bool reloadData(Control::Context *) const;

        bool add(const Control::Context *, DbObjects::Common::Database *);
        bool remove(const Control::Context *);
        bool remove(DbObjects::Common::Database *);

        const Control::Context* findContext(DbObjects::Common::Database *) const;
        DbObjects::Common::Database* findDatabase(const Control::Context *) const;

        QString lastError(const Control::Context *) const;

        // actual database functions
        void indicesList(const Control::Context *, QStringList &oList) const;
        void languagesList(const Control::Context *, QStringList &oList) const;
        void rolesList(const Control::Context *, QStringList &oList) const;
        void schemasList(const Control::Context *, QStringList &oList) const;

        friend class Director;

};

} // namespace Control

#endif // CONNECT_DATABASEMANAGER_H


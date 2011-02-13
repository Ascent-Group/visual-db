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

#include <connect/ConnectionInfo.h>

namespace DbObjects
{
namespace Common
{
class Database;
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
        DatabaseManager();
        ~DatabaseManager();

        void flush();
        Control::Context* establishConnection(const Connect::ConnectionInfo &, QString &);

        bool add(const Control::Context *, DbObjects::Common::Database *);
        bool remove(const Control::Context *);
        bool remove(DbObjects::Common::Database *);

    private:
        /*!
         * One-to-one registry of contexts and database instances. Context is under
         * control of director, that's why we handle it as const. As for the database,
         * then we don't make it const, since Database manager is taking care of all of
         * them.
         */
        QMap<const Control::Context *, DbObjects::Common::Database *> mRegistry;

//        QMap<QString, QMap<DbObjects::Common::DbOject::Type, DbObjects::Factory>>

        const Control::Context* findContext(DbObjects::Common::Database *) const;
        DbObjects::Common::Database* findDatabase(const Control::Context *) const;

};

} // namespace Control

#endif // CONNECT_DATABASEMANAGER_H

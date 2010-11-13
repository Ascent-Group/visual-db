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
#include <factory/Trigger.h>
#include <psql/Trigger.h>

using namespace DbObjects::Common;

namespace DbObjects
{

namespace Factory
{

/*!
 * \param[in] ipName - Name of the trigger to construct
 * \param[in] ipSchemaName - Name of schema containing the procedure
 *
 * \return Database trigger object
 */
DbObjects::Common::DbTrigger*
Trigger::createTrigger(const QString &ipName, const QString &ipSchemaName)
{
    using namespace DbObjects::Common;

    DbTrigger *trigger = 0;

    switch (Database::instance()->sqlDriver()) {
        case Database::PostgreSQL:
                trigger = createPsqlTrigger(ipName, ipSchemaName);
                break;
        case Database::MySQL:
//                trigger = createMysqlTrigger(ipName, ipSchemaName);
                break;
        case Database::Oracle:
        case Database::SQLite:
        case Database::Unknown:
        default:
                break;
    }

    // if the trigger creation failed or trigger inforamtion could not be read from database.
//    if (!trigger || !trigger->loadData()) {
//        delete trigger;
//        trigger = 0;
//    }

    return trigger;
}

/*!
 * \param[in] ipName - Name of trigger to construct
 * \param[in] ipSchemaName - Name of schema containing the procedure
 *
 * \return Pointer to PostgreSQL trigger object
 */
Psql::Trigger*
Trigger::createPsqlTrigger(const QString &ipName, const QString &ipSchemaName)
{
    return new(std::nothrow) Psql::Trigger(ipName, Database::instance()->findSchema(ipSchemaName));
}

/*!
 * \param[in] ipName - Name of trigger to construct
 * \param[in] ipSchemaName - Name of schema containing the procedure
 *
 * \return Pointer to MySQL trigger object
 */
//Mysql::Trigger*
//Trigger::createMysqlTrigger(const QString &ipName, const QString &ipSchemaName)
//{
//    return new Mysql::Trigger(ipName, Database::instance()->findSchema(ipSchemaName));
//}

} // namespace Factory

} // namespace DbObjects


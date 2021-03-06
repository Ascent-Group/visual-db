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
#include <factory/Procedure.h>
#include <psql/Procedure.h>

using namespace DbObjects::Common;

namespace DbObjects
{

namespace Factory
{

/*!
 * \param[in] ipName - Name of the procedure to construct
 * \param[in] ipSchemaName - Name of schema containing the procedure
 *
 * \return Database procedure object
 */
DbObjects::Common::DbProcedure*
Procedure::createProcedure(const QString &ipName, const QString &ipSchemaName)
{
    using namespace DbObjects::Common;

    DbProcedure *procedure = 0;

    switch (Database::instance()->sqlDriver()) {
        case Database::PostgreSQL:
                procedure = createPsqlProcedure(ipName, ipSchemaName);
                break;
        case Database::MySQL:
//                procedure = createMysqlProcedure(ipName, ipSchemaName);
                break;
        case Database::Oracle:
        case Database::SQLite:
        case Database::Unknown:
        default:
                break;
    }

    // if the procedure creation failed or procedure inforamtion could not be read from database.
//    if (!procedure || !procedure->loadData()) {
//        delete procedure;
//        procedure = 0;
//    }

    return procedure;
}

/*!
 * \param[in] ipName - Name of procedure to construct
 * \param[in] ipSchemaName - Name of schema containing the procedure
 *
 * \return Pointer to PostgreSQL procedure object
 */
Psql::Procedure*
Procedure::createPsqlProcedure(const QString &ipName, const QString &ipSchemaName)
{
    // \note we duplicate some code so that we don't make our signature (and therefore,
    // headers be dependent on DbObjectPtr
    // \todo but it should take dbschemaptr
    return new(std::nothrow) Psql::Procedure(ipName, Database::instance()->findSchema(ipSchemaName));
}

/*!
 * \param[in] ipName - Name of procedure to construct
 * \param[in] ipSchemaName - Name of schema containing the procedure
 *
 * \return Pointer to MySQL procedure object
 */
//Mysql::Procedure*
//Procedure::createMysqlProcedure(const QString &ipName, const QString &ipSchemaName)
//{
//    return new Mysql::Procedure(ipName, Database::instance()->findSchema(mSchemaName));
//}

} // namespace Factory

} // namespace DbObjects


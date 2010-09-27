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
#include <factory/Table.h>
//#include <mysql/Table.h>
#include <psql/Table.h>

namespace DbObjects
{

namespace Factory
{

/*!
 * \param[in] ipName - Name of the table to construct
 *
 * \return Database table objects
 */
DbObjects::Common::DbTable*
Table::createTable(const QString &ipSchemaName, const QString &ipName)
{
    using namespace DbObjects::Common;

    DbTable *table = 0;

    switch (Database::instance()->sqlDriver()) {
        case Database::PostgreSQL:
                table = createPsqlTable(ipSchemaName, ipName);
                break;
        case Database::MySQL:
//                table = createMysqlTable(ipName);
                break;
        case Database::Oracle:
        case Database::SQLite:
        case Database::Unknown:
        default:
                break;
    }

    //if the table creation failed or table inforamtion could not be read from database.
    if (!table || !table->loadData()) {
        delete table;
        table = 0;
    }

    return table;
}

/*!
 * \param[in] ipName - Name of table to construct
 * \return Pointer to PostgreSQL table object
 */
Psql::Table*
Table::createPsqlTable(const QString &ipSchemaName, const QString &ipName)
{
    return new Psql::Table(ipSchemaName, ipName);
}

/*!
 *
 * \param[in] ipName - Name of table to construct
 * \return Pointer to MySQL table object
 */
//Mysql::Table*
//Table::createMysqlTable(const QString &ipSchemaName, const QString &ipName)
//{
//    return new Mysql::Table(ipSchemaName, ipName);
//}

} // namespace Factory

} // namespace DbObjects


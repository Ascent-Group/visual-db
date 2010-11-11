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
#include <factory/Schema.h>
//#include <mysql/Schema.h>
#include <psql/Schema.h>

namespace DbObjects
{

namespace Factory
{

/*!
 * \param[in] ipName - Name of the schema to construct
 *
 * \return Database schema objects
 */
Common::DbSchema*
Schema::createSchema(const QString &ipName)
{
    using namespace DbObjects::Common;

    DbSchema *schema = 0;

    switch (Database::instance()->sqlDriver()) {
        case Database::PostgreSQL:
                schema = createPsqlSchema(ipName);
                break;
        case Database::MySQL:
//                schema = createMysqlSchema(ipName);
                break;
        case Database::Oracle:
        case Database::SQLite:
        case Database::Unknown:
        default:
                break;
    }

    // if the schema creation failed or schema inforamtion could not be read from database.
//    if (!schema || !schema->loadData()) {
//        delete schema;
//        schema = 0;
//    }

    qDebug() << "---------------" << schema;
    return schema;
}

/*!
 * \param[in] ipName - Name of schema to construct
 * \return Pointer to PostgreSQL schema object
 */
Psql::Schema*
Schema::createPsqlSchema(const QString &ipName)
{
    return new(std::nothrow) Psql::Schema(ipName);
}

/*!
 *
 * \param[in] ipName - Name of schema to construct
 * \return Pointer to MySQL schema object
 */
//Mysql::Schema*
//Schema::createMysqlSchema(const QString &ipName)
//{
//    return new Mysql::Schema(ipName);
//}

} // namespace Factory

} // namespace DbObjects


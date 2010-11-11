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
#include <factory/Index.h>
//#include <mysql/Index.h>
#include <psql/Index.h>

namespace DbObjects
{

namespace Factory
{

/*!
 * \param[in] ipName - Name of the index to construct
 *
 * \return Database index objects
 */
DbObjects::Common::DbIndex*
Index::createIndex(const QString &ipName)
{
    using namespace DbObjects::Common;

    DbIndex *index = 0;

    switch (Database::instance()->sqlDriver()) {
        case Database::PostgreSQL:
                index = createPsqlIndex(ipName);
                break;
        case Database::MySQL:
//                index = createMysqlIndex(ipName);
                break;
        case Database::Oracle:
        case Database::SQLite:
        case Database::Unknown:
        default:
                break;
    }

    // if the index creation failed or index inforamtion could not be read from database.
//    if (!index || !index->loadData()) {
//        delete index;
//        index = 0;
//    }

    return index;
}

/*!
 * \param[in] ipName - Name of index to construct
 * \return Pointer to PostgreSQL index object
 */
Psql::Index*
Index::createPsqlIndex(const QString &ipName)
{
    return new(std::nothrow) Psql::Index(ipName);
}

/*!
 *
 * \param[in] ipName - Name of index to construct
 * \return Pointer to MySQL index object
 */
//Mysql::Index*
//Index::createMysqlIndex(const QString &ipName)
//{
//    return new Mysql::Index(ipName);
//}

} // namespace Factory

} // namespace DbObjects


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
#include <factory/View.h>
#include <psql/View.h>

namespace DbObjects
{

namespace Factory
{

/*!
 * \param[in] ipName - Name of the view to construct
 *
 * \return Database view objects
 */
DbObjects::Common::DbView*
View::createView(const QString &ipSchemaName, const QString &ipName)
{
    using namespace DbObjects::Common;

    DbView *view = 0;

    switch (Database::instance()->sqlDriver()) {
        case Database::PostgreSQL:
                view = createPsqlView(ipSchemaName, ipName);
                break;
        case Database::MySQL:
//                view = createMysqlView(ipName);
                break;
        case Database::Oracle:
        case Database::SQLite:
        case Database::Unknown:
        default:
                break;
    }

    //if the view creation failed or view inforamtion could not be read from database.
    if (!view || !view->loadData()) {
        delete view;
        view = 0;
    }

    return view;
}

/*!
 * \param[in] ipName - Name of view to construct
 * \return Pointer to PostgreSQL view object
 */
Psql::View*
View::createPsqlView(const QString &ipSchemaName, const QString &ipName)
{
    return new Psql::View(ipSchemaName, ipName);
}

/*!
 *
 * \param[in] ipName - Name of view to construct
 * \return Pointer to MySQL view object
 */
//Mysql::View*
//View::createMysqlView(const QString &ipSchemaName, const QString &iName)
//{
//    return new Mysql::View(ipSchemaName, iName);
//}

} // namespace Factory

} // namespace DbObjects

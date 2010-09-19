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
#include <factory/Role.h>
#include <psql/Role.h>

#include <QtDebug>

namespace DbObjects
{

namespace Factory
{

/*!
 * \param[in] ipName - Name of the role to be constructed
 * \return Pointer to abstract database role object
 */
DbObjects::Common::DbRole*
Role::createRole(const QString &ipName)
{
    using namespace DbObjects::Common;
    Database::SqlDriverType drv = Database::instance()->sqlDriver();

    DbRole *role = 0;

    switch (drv) {
        case Database::Unknown:
                        qDebug() << __PRETTY_FUNCTION__ << "> SqlDriver was not set";
                        break;
        case Database::PostgreSQL:
                        role =  createPsqlRole(ipName);
                        break;
        case Database::MySQL:
//                        role = createMysqlRole();
                        break;
        case Database::Oracle:
        case Database::SQLite:
        default:
                        /* temporarily no support for these DBMS */
                        break;
    }

    // if role creation failed or role information cannont be read from database
    if (!role || !role->loadData()) {
        delete role;
        role = 0;
    }

    return role;
}

/*!
 * \param[in] ipName - Name of the rle to construct
 * \return Pointer to PostgreSQL role object whose name is ipName
 */
DbObjects::Psql::Role*
Role::createPsqlRole(const QString &ipName)
{
    return new(std::nothrow) Psql::Role(ipName);
}

} // namespace Factory

} // namespace DbObjects


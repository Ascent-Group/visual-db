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

#ifndef DBOBJECTS_PSQL_TOOLS_H
#define DBOBJECTS_PSQL_TOOLS_H

#include <QSqlDatabase>
#include <QStringList>
#include <QtCore/qglobal.h>

namespace DbObjects
{

namespace Psql
{

/*!
 * \namespace Tools
 * \headerfile psql/Tools.h
 * \brief Contains helper functions for PostgreSQL
 */
namespace Tools
{

    /*!
     * \enum Psql::Tools::Version
     * \brief Defines PostgreSQL version
     */
    enum Version {
        /*! Denotes that PostgreSQL version couldn't be recognized */
        PostgreSQL_Unknown = 0,
        /*! PostgreSQL version is >= 8 */
        PostgreSQL_8,
        /*! PostgreSQL version is >= 9 */
        PostgreSQL_9
    };

    Tools::Version version(const QSqlDatabase &);

    quint32 schemasList(const QSqlDatabase &, QStringList &oList);
    quint32 indicesList(const QSqlDatabase &, QStringList &oList);
    quint32 languagesList(const QSqlDatabase &, QStringList &oList);
    quint32 proceduresList(const QSqlDatabase &, const QString &iSchemaName, QStringList &oList);
    quint32 rolesList(const QSqlDatabase &, QStringList &oList);
    quint32 tablesList(const QSqlDatabase &, const QString &iSchemaName, QStringList &oList);
    quint32 triggersList(const QSqlDatabase &, const QString &iSchemaName, QStringList &oList);
    quint32 viewsList(const QSqlDatabase &, const QString &iSchemaName, QStringList &oList);

    quint32 objectNamesList(const QSqlDatabase &, const QString &iQstr, QStringList &oList);

} // namespace Tools

} // namespace Psql

} // namespace DbObjects

#endif // DBOBJECTS_PSQL_TOOLS_H


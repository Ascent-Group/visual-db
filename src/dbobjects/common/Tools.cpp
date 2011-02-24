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

#include <common/Tools.h>

#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>

#include <QtDebug>

namespace DbObjects
{
namespace Common
{

/*!
 * Constructor
 */
Tools::Tools()
{

}

/*!
 * Destructor
 */
Tools::~Tools()
{

}

/*!
 * \brief Reads thelist of objects' names
 *
 * \note The functions looks for a field by name 'name' !!!
 *
 * \param[in] iQstr - The query used to select names of objects
 * \param[out] oList - List that will hold objects' names
 *
 * \return The number of names in the list
 */
quint32
Tools::objectNamesList(const QSqlDatabase &iDbHandle, const QString &iQstr, QStringList &oList)
{
#if DEBUG_QUERY
    qDebug() << "DbObjects::Psql::Tools::objectNamesList> " << iQstr;
#endif

    QSqlQuery query(iDbHandle);

    // if query failed
    if (!query.exec(iQstr)) {
        qDebug() << query.lastError().text();
        return 0;
    }

    // if query didn't retrieve a row
    if (!query.first()) {
        return 0;
    }

    quint32 count = 0;
    qint32 colId = query.record().indexOf("name");

    do {
        oList.append(query.value(colId).toString());
        ++count;
    } while (query.next());

    // we return count instead of oList.count() because oList might come not empty to
    // this function (since it is passed by reference
    return count;
}

} // namespace Common
} // namespace DbObjects


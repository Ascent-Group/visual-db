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

#include <mysql/Tools.h>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QString>
#include <QVariant>

#include <QtDebug>

namespace DbObjects
{

namespace Mysql
{

namespace Tools
{

/*!
 * \todo Implement
 * \return The version of MySQL that is currently used
 */
Tools::Version
version()
{
    QSqlDatabase db = QSqlDatabase::database("mainConnect");
    QSqlQuery query(db);
    QString qstr("SELECT version();");

    if (!query.exec(qstr) || !query.first()) {
        return Tools::MySQL_Unknown;
    }

    QString versionString = query.value(0).toString();
    /* \todo detect the version */

    return Tools::MySQL_Unknown;
}

/*!
 * \brief Read schemas that are available in the database
 *
 * \param[out] oList - The list that will containt schemas' names
 *
 * \return The number of schemas read.
 */
quint32
schemasList(QStringList &oList)
{
    QString qstr = QString("SELECT schema_name AS name FROM information_schema.schemata;");

    return objectNamesList(qstr, oList);
}

/*!
 * \todo Implement
 * \brief Read indices that are in available in the database
 *
 * \param[out] oList - The list that will contain indices' names
 *
 * \return The number of indices read by the query
 */
quint32
indicesList(QStringList &oList)
{
    QString qstr = QString("");

    return objectNamesList(qstr, oList);
}

/*!
 * \todo Implement
 */
quint32
languagesList(QStringList &oList)
{
    QString qstr("");

    return objectNamesList(qstr, oList);
}

/*!
 * \todo Implement
 */
quint32
proceduresList(QStringList &oList)
{

}

/*!
 * \todo Implement
 * \brief Read roles that are in available in the database
 *
 * \param[out] oList - The list that will contain roles' names
 *
 * \return The number of roles read by the query
 */
quint32
rolesList(QStringList &oList)
{
    QString qstr = QString("");

    return objectNamesList(qstr, oList);
}

/*!
 * \todo Implement
 */
quint32
objectNamesList(const QString &iQstr, QStringList &oList)
{
    QSqlDatabase db = QSqlDatabase::database("mainConnect");
    QSqlQuery query(db);

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

    return count;
}

/*!
 * \todo Implement
 */
quint32
tablesList(QStringList &oList)
{

}

/*!
 * \todo Implement
 */
quint32
triggersList(QStringList &oList)
{

}

/*!
 * \todo Implement
 */
quint32
viewsList(QStringList &oList)
{

}

} // namespace Tools

} // namespace Mysql

} // namespace DbObjects


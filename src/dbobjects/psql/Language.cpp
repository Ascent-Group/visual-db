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
#include <psql/Language.h>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>

#include <QtDebug>

namespace DbObjects
{

namespace Psql
{

/*!
 * Constructor
 *
 * \param[in] iName - Name of the language
 */
Language::Language(QString iName)
    : DbLanguage(iName)
{

}

/*!
 * Destructor
 */
Language::~Language()
{

}

/*
 * Loads language definition
 */
bool
Language::loadData()
{
    if (mIsLoaded) {
        return true;
    }

    QSqlQuery query(mDatabase->dbHandle());
    QString qstr;

    qstr = QString("SELECT "
                        "l.lanname AS name, "
                        "l.lanowner AS owner, "
                        "l.lanispl AS ispl, "
                        "l.lanpltrusted AS trusted, "
                        "l.lanplcallfoid AS executor, "
                        "l.lanvalidator AS validator, "
                        "l.lanacl AS acl "
                    "FROM "
                        "pg_catalog.pg_language l "
                    "WHERE "
                        "l.lanname = '%1';")
            .arg(mName);

#ifdef DEBUG_QUERY
    qDebug() << "Language::loadData> " << qstr;
#endif

    // if query execution failed
    if (!query.exec(qstr)) {
        qDebug() << query.lastError().text();

        return false;
    }

    // if query returned nothing
    if (!query.first()) {
        qDebug() << "Language::loadData> Language not found.";

        return false;
    }

    qint32 colId;

    /*
    colId = query.record().indexOf("name");
    Q_ASSERT(colId > 0);
    mName = query.value(colId).toBool();
    */

    colId = query.record().indexOf("trusted");
    Q_ASSERT(colId > 0);
    mIsTrusted = query.value(colId).toBool();

#if DEBUG_TRACE
    qDebug() << "Language::loadData> name = " << mName;
    qDebug() << "Language::loadData> trusted = " << mIsTrusted;
#endif

    return DbLanguage::loadData();
}


} // namespace Psql

} // namespace DbObjects


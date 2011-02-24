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
#include <common/DbObjectPtr.h>
#include <common/DbSchema.h>
#include <mysql/Index.h>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QStringList>
#include <QVariant>

#include <QtDebug>

namespace DbObjects
{

namespace Mysql
{

/*!
 * Constructor
 */
Index::Index(QString iName)
    : DbIndex(iName)
{
}

/*!
 * Destructor
 */
Index::~Index()
{

}

/*
 * Loads the index's definition
 */
bool
Index::loadData()
{
    if (mIsLoaded) {
        return true;
    }

    QSqlQuery query(mDatabase->dbHandle());
    QString qstr;

    // construct the query
    qstr = QString("MYSQL specific query");

#ifdef DEBUG_QUERY
    qDebug() << "Index::loadData> " << qstr;
#endif

    // if query execuition failed
    if (!query.exec(qstr)) {
        qDebug() << query.lastError().text();
        return false;
    }

    // if query didn't retrieve a row
    if (!query.first()) {
        return false;
    }

    qint32 colId;
    /*
       colId= query.record().indexOf("name");
       Q_ASSERT(colId > 0);
       mName = query.value(colId).toString();
       */

    colId = query.record().indexOf("unique");
    Q_ASSERT(colId > 0);
    mIsUnique = query.value(colId).toBool();

    colId = query.record().indexOf("primary");
    Q_ASSERT(colId > 0);
    mIsPrimary = query.value(colId).toBool();

    colId = query.record().indexOf("clustered");
    Q_ASSERT(colId > 0);
    mIsClustered = query.value(colId).toBool();

    colId = query.record().indexOf("valid");
    Q_ASSERT(colId > 0);
    mIsValid = query.value(colId).toBool();

    colId = query.record().indexOf("xmin");
    Q_ASSERT(colId > 0);
    mChecksXMin = query.value(colId).toBool();

    colId = query.record().indexOf("ready");
    Q_ASSERT(colId > 0);
    mIsReady = query.value(colId).toBool();

    colId = query.record().indexOf("table");
    Q_ASSERT(colId > 0);
    QString tableName = query.value(colId).toString();

    colId = query.record().indexOf("schema");
    Q_ASSERT(colId > 0);
    QString schemaName = query.value(colId).toString();

    using namespace DbObjects::Common;
    DbSchemaPtr schema = mDatabase->findSchema(schemaName);

    if (schema.valid()) {
        DbTablePtr table = schema->findTable(tableName);

        if (table.valid()) {
            setSchema(schema);
            setTable(table);
        }
    }

    colId = query.record().indexOf("colcount");
    Q_ASSERT(colId > 0);
    mColumnsCount = query.value(colId).toInt();

    // get the list of column numbers
    colId = query.record().indexOf("fields");
    Q_ASSERT(colId > 0);
    QString str = query.value(colId).toString();

    // populate the number
    foreach (QString section, str.split(" ")) {
        addColumnNumber(section.toInt());
    }


    /* temporary debug output */
#if DEBUG_TRACE
    qDebug() << "mName: " << mName;
    qDebug() << "mIsUnique: " << mIsUnique;
    qDebug() << "mIsPrimary: " << mIsPrimary;
    qDebug() << "mIsClustered: " << mIsClustered;
    qDebug() << "mIsValid: " << mIsValid;
    qDebug() << "mChecksXMin: " << mChecksXMin;
    qDebug() << "mIsReady: " << mIsReady;
    qDebug() << "mTableName: " << mTable->name();
    qDebug() << "mSchemaName: " << mSchema->name();
    qDebug() << "mColumnsCount: " << mColumnsCount;
    qDebug() << "mColumnsNumbers: " << mColumnsNumbers;
#endif

    return DbIndex::loadData();
}

} // namespace Mysql
} // namespace DbObjects


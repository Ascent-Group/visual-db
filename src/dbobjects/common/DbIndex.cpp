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
#include <common/DbIndex.h>
#include <common/DbSchema.h>

#include <QtDebug>

/*
 * Ctor
 */
DbIndex::DbIndex(QString ipName)
    : DbObject(ipName)
{

}

/*
 * Dtor
 */
DbIndex::~DbIndex()
{

}

/*
 * Returns the table name this index is for
 */
QString
DbIndex::tableName() const
{
    if (mTable) {
        return mTable->name();
    }
    return mTableName;
}

/*
 * Sets the table this index is for
 */
void
DbIndex::setTableName(const QString &ipTableName)
{
    mTableName = ipTableName;

    if (mTable) {
        mTable->setName(ipTableName);
    }
}

/*
 * Returns the name of schema
 */
QString
DbIndex::schemaName() const
{
    if (mSchema) {
        return mSchema->name();
    }

    return mSchemaName;
}

/*
 * Sets the name of schema
 */
void
DbIndex::setSchemaName(const QString &ipSchemaName)
{
    mSchemaName = ipSchemaName;

    if (mSchema) {
        mSchema->setName(ipSchemaName);
    }
}

/*
 * Returns the table
 */
DbTable*
DbIndex::table() const
{
    return mTable;
}

/*
 * Sets the table
 */
void
DbIndex::setTable(DbTable *ipTable)
{
    mTable = ipTable;
}

/*
 * Returns the schema
 */
DbSchema*
DbIndex::schema() const
{
    return mSchema;
}

/*
 * Sets the schema
 */
void
DbIndex::setSchema(DbSchema *ipSchema)
{
    mSchema = ipSchema;
}

/*
 * Returns the number of columns in the ginev db index
 */
int
DbIndex::columnsCount() const
{
    return mColumnsCount;
}

/*
 * Sets the number of columns in the given index
 */
void
DbIndex::setColumnsCount(int ipCount)
{
    mColumnsCount = ipCount;
}

/*
 * Returns the list of table's columns numbers
 */
QVector<int>
DbIndex::columnsNumbers() const
{
    return mColumnsNumbers;
}

/*
 * Adds a table's coolumn number
 */
void
DbIndex::addColumnNumber(int ipNum)
{
    mColumnsNumbers.push_back(ipNum);
}

/*
 * Returns true if the index is unique
 */
bool
DbIndex::isUnique() const
{
    return mIsUnique;
}

/*
 * Sets the unique flag
 */
void
DbIndex::setUnique(bool ipFlag)
{
    mIsUnique = ipFlag;
}

/*
 * Returns true if the index represents the primary key in the table
 */
bool
DbIndex::isPrimary() const
{
    return mIsPrimary;
}

/*
 * Sets the primary flag
 */
void
DbIndex::setPrimary(bool ipFlag)
{
    mIsPrimary = ipFlag;
}

/*
 * Returns true if the table was clustered on this index
 */
bool
DbIndex::isClustered() const
{
    return mIsClustered;
}

/*
 * Sets the clustered flag
 */
void
DbIndex::setClustered(bool ipFlag)
{
    mIsClustered = ipFlag;
}

/*
 * Returns true if the index is currently valid for queries
 */
bool
DbIndex::isValid() const
{
    return mIsValid;
}

/*
 * Sets the valid flag
 */
void
DbIndex::setValid(bool ipFlag)
{
    mIsValid = ipFlag;
}

/*
 * Returns true if queries must not use this index if xmin < query.transaction_xmin
 */
bool
DbIndex::checksXMin() const
{
    return mChecksXMin;
}

/*
 * Sets the checksXmin flag
 */
void
DbIndex::setChecksXMin(bool ipFlag)
{
    mChecksXMin = ipFlag;
}

/*
 * Returns true if the index is ready for queries
 */
bool
DbIndex::isReady() const
{
    return mIsReady;
}

/*
 * Sets the ready flag
 */
void
DbIndex::setReady(bool ipFlag)
{
    mIsReady = ipFlag;
}

/*
 * Returns the id of database object
 */
int
DbIndex::objectId()
{
    return Database::IndexObject;
}

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

namespace DbObjects
{

namespace Common
{

/*!
 * Ctor
 */
DbIndex::DbIndex(QString ipName)
    : DbObject(ipName)
{

}

/*!
 * Dtor
 */
DbIndex::~DbIndex()
{

}

/*!
 * \return Table name this index is fassigned to
 */
QString
DbIndex::tableName() const
{
    if (mTable) {
        return mTable->name();
    }
    return mTableName;
}

/*!
 * \brief Sets the table this index is for
 *
 * \param[in] ipTableName
 */
void
DbIndex::setTableName(const QString &ipTableName)
{
    mTableName = ipTableName;

    if (mTable) {
        mTable->setName(ipTableName);
    }
}

/*!
 * \return Parent schema name or name stored within DbIndex object
 *         if the parent schema object is NULL.
 *
 * \note Schema name stored within the object can be set with the
 *       help of setSchemaName().
 *
 * \see setSchemaName()
 */
QString
DbIndex::schemaName() const
{
    if (mSchema) {
        return mSchema->name();
    }

    return mSchemaName;
}

/*!
 * \brief Sets the name of schema
 *
 * \param[in] ipSchemaName
 *
 * \note Doesn't set the name for the parent schema object
 */
void
DbIndex::setSchemaName(const QString &ipSchemaName)
{
    mSchemaName = ipSchemaName;

    if (mSchema) {
        mSchema->setName(ipSchemaName);
    }
}

/*!
 * \return Table
 */
DbTable*
DbIndex::table() const
{
    return mTable;
}

/*!
 * \brief Sets the table
 *
 * \param[in] ipTable
 */
void
DbIndex::setTable(DbTable *ipTable)
{
    mTable = ipTable;
}

/*!
 * \return Parent schema
 */
DbSchema*
DbIndex::schema() const
{
    return mSchema;
}

/*!
 * \brief Sets the schema
 *
 * \param[in] ipSchema - Parent schema of the given index
 */
void
DbIndex::setSchema(DbSchema *ipSchema)
{
    mSchema = ipSchema;
}

/*!
 * \return The number of columns covered by the given db index
 */
quint16
DbIndex::columnsCount() const
{
    return mColumnsCount;
}

/*!
 * \brief Sets the number of columns in the given index
 *
 * \param[in] ipCount - Number of columns covered by the given index
 */
void
DbIndex::setColumnsCount(quint16 ipCount)
{
    mColumnsCount = ipCount;
}

/*!
 * \return The list of table's columns numbers
 */
QVector<qint16>
DbIndex::columnsNumbers() const
{
    return mColumnsNumbers;
}

/*!
 * \brief Adds a table's column number
 *
 * \param[in] ipNum - Column number the given index is assigned to
 */
void
DbIndex::addColumnNumber(qint16 ipNum)
{
    mColumnsNumbers.push_back(ipNum);
}

/*!
 * \return true - If the index is unique
 * \return false - Otherwise
 */
bool
DbIndex::isUnique() const
{
    return mIsUnique;
}

/*!
 * \brief Sets the unique flag
 *
 * param[in] ipFlag
 */
void
DbIndex::setUnique(bool ipFlag)
{
    mIsUnique = ipFlag;
}

/*!
 * \return true - If the index represents the primary key in the table
 * \return false - Otherwise
 */
bool
DbIndex::isPrimary() const
{
    return mIsPrimary;
}

/*!
 * \brief Sets the primary flag
 *
 * \param[in] ipFlag
 */
void
DbIndex::setPrimary(bool ipFlag)
{
    mIsPrimary = ipFlag;
}

/*!
 * \return true - If the table was clustered on this index
 * \return false - Otherwise
 */
bool
DbIndex::isClustered() const
{
    return mIsClustered;
}

/*!
 * \brief Sets the clustered flag
 *
 * \param[in] ipFlag
 */
void
DbIndex::setClustered(bool ipFlag)
{
    mIsClustered = ipFlag;
}

/*!
 * \return true - If the index is currently valid for queries
 * \return falase - Otherwise
 */
bool
DbIndex::isValid() const
{
    return mIsValid;
}

/*!
 * \brief Sets the valid flag
 *
 * \param[in] ipFlag
 */
void
DbIndex::setValid(bool ipFlag)
{
    mIsValid = ipFlag;
}

/*!
 * \return true - If queries must not use this index if xmin < query.transaction_xmin
 * \return false - Otherwise
 */
bool
DbIndex::checksXMin() const
{
    return mChecksXMin;
}

/*!
 * \brief Sets the checksXmin flag
 *
 * \param[in] ipFlag
 */
void
DbIndex::setChecksXMin(bool ipFlag)
{
    mChecksXMin = ipFlag;
}

/*!
 * \return true - If the index is ready for queries
 * \return false - Otherwise
 */
bool
DbIndex::isReady() const
{
    return mIsReady;
}

/*!
 * \brief Sets the ready flag
 *
 * \param[in] ipFlag
 */
void
DbIndex::setReady(bool ipFlag)
{
    mIsReady = ipFlag;
}

/*!
 * \return Database object type
 */
DbObject::Type
DbIndex::type() const
{
    return DbObject::IndexObject;
}

} // namespace Common

} // namespace DbObjects


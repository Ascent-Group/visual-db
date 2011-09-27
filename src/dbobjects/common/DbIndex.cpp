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
DbIndex::DbIndex(const QString &iName)
    : DbObject(iName),
      mTable(),
      mSchema(),
      mColumnsCount(0),
      mColumnsNumbers(),
      mIsUnique(false),
      mIsPrimary(false),
      mIsClustered(false),
      mIsValid(false),
      mChecksXMin(false),
      mIsReady(false)
{

}

/*!
 * Dtor
 */
DbIndex::~DbIndex()
{

}

/*!
 * \return Table
 */
DbTablePtr
DbIndex::table() const
{
    return mTable;
}

/*!
 * \brief Sets the table
 *
 * \param[in] iTable
 */
void
DbIndex::setTable(const DbTablePtr &iTable)
{
    mTable = iTable;
}

/*!
 * \return Parent schema
 */
DbSchemaPtr
DbIndex::schema() const
{
    return mSchema;
}

/*!
 * \brief Sets the schema
 *
 * \param[in] iSchema - Parent schema of the given index
 */
void
DbIndex::setSchema(const DbSchemaPtr &iSchema)
{
    mSchema = iSchema;
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
 * \param[in] iCount - Number of columns covered by the given index
 */
void
DbIndex::setColumnsCount(quint16 iCount)
{
    mColumnsCount = iCount;
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
 * \param[in] iNum - Column number the given index is assigned to
 */
void
DbIndex::addColumnNumber(qint16 iNum)
{
    mColumnsNumbers.push_back(iNum);
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
 * \param[in] iFlag
 */
void
DbIndex::setUnique(bool iFlag)
{
    mIsUnique = iFlag;
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
 * \param[in] iFlag
 */
void
DbIndex::setPrimary(bool iFlag)
{
    mIsPrimary = iFlag;
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
 * \param[in] iFlag
 */
void
DbIndex::setClustered(bool iFlag)
{
    mIsClustered = iFlag;
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
 * \param[in] iFlag
 */
void
DbIndex::setValid(bool iFlag)
{
    mIsValid = iFlag;
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
 * \param[in] iFlag
 */
void
DbIndex::setChecksXMin(bool iFlag)
{
    mChecksXMin = iFlag;
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
 * \param[in] iFlag
 */
void
DbIndex::setReady(bool iFlag)
{
    mIsReady = iFlag;
}

/*!
 * \brief Reset index description read from database
 */
void
DbIndex::resetData()
{
    mTable = DbTablePtr();
    mSchema = DbSchemaPtr();
    mColumnsCount = 0;
    mColumnsNumbers.clear();
    mIsUnique = false;
    mIsPrimary = false;
    mIsClustered = false;
    mIsValid = false;
    mChecksXMin = false;
    mIsReady = false;

    DbObject::resetData();
}

/*!
 * \return Database object type
 */
ObjectType
DbIndex::type() const
{
    return IndexObject;
}

} // namespace Common

} // namespace DbObjects


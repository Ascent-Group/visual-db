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

#ifndef DBOBJECTS_COMMON_DBINDEX_H
#define DBOBJECTS_COMMON_DBINDEX_H

#include <common/DbObject.h>
#include <common/DbObjectPtr.h>

#include <QVector>

namespace DbObjects
{

namespace Common
{

template<typename T> class DbObjectPtr;
class DbSchema;
class DbTable;
//template class DbObjectPtr<DbTable>;
//template class DbObjectPtr<DbSchema>;
typedef DbObjectPtr<DbSchema> DbSchemaPtr;
typedef DbObjectPtr<DbTable> DbTablePtr;

/*!
 * \class DbIndex
 * \headerfile common/DbIndex.h
 *
 * \brief Defines an index stored in DB
 */
class DbIndex : public DbObject
{
    public:
        virtual ~DbIndex();

        DbTablePtr table() const;
        void setTable(const DbTablePtr &iTable);

        DbSchemaPtr schema() const;
        void setSchema(const DbSchemaPtr &iSchema);

        quint16 columnsCount() const;
        void setColumnsCount(quint16 iCount);

        QVector<qint16> columnsNumbers() const;
        void addColumnNumber(qint16 iNum);

        bool isUnique() const;
        void setUnique(bool iFlag);

        bool isPrimary() const;
        void setPrimary(bool iFlag);

        bool isClustered() const;
        void setClustered(bool iFlag);

        bool isValid() const;
        void setValid(bool iFlag);

        bool checksXMin() const;
        void setChecksXMin(bool iFlag);

        bool isReady() const;
        void setReady(bool iFlag);

        virtual void resetData();

        virtual DbObject::Type type() const;

    protected:
        /*! Table this index is assigned to */
        DbTablePtr mTable;
        /*! Schema containing this index */
        DbSchemaPtr mSchema;
        /*! Number of columns covered by this index */
        quint16 mColumnsCount;
        /*! Columns numbers */
        // qint16 because columns can be addressed via negative ids
        QVector<qint16> mColumnsNumbers;
        /*! If true, this is a unique index */
        bool mIsUnique;
        /*! If true, this index represents the primary key of the table
         * (indisunique should always be true when this is true)
         */
        bool mIsPrimary;
        /*! If true, the table was last clustered on this index */
        bool mIsClustered;
        /*! If true, the index is currently valid for queries. */
        bool mIsValid;
        /*!
         * If true, queries must not use the index until the xmin of this
         * pg_index  row is below their TransactionXmin event horizon, because
         * the table may contain broken HOT chains with incompatible rows that
         * they can see
         */
        bool mChecksXMin;
        /*! is currently ready for inserts */
        bool mIsReady;

    protected:
        DbIndex(const QString &iName);
};

} // namespace Common

} // namespace DbObjects

#endif // DBOBJECTS_COMMON_DBINDEX_H


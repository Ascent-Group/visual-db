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

#ifndef DBINDEX_H
#define DBINDEX_H

#include <DbObject.h>
#include <QVector>

class DbSchema;
class DbTable;

class DbIndex : public DbObject
{
    public:
        virtual ~DbIndex();

        QString tableName() const;
        void setTableName(const QString &ipTableName);

        QString schemaName() const;
        void setSchemaName(const QString &ipSchemaName);

        DbTable* table() const;
        void setTable(DbTable *ipTable);

        DbSchema* schema() const;
        void setSchema(DbSchema *ipSchema);

        int columnsCount() const;
        void setColumnsCount(int ipCount);

        QVector<int> columnsNumbers() const;
        void addColumnNumber(int ipNum);

        bool isUnique() const;
        void setUnique(bool ipFlag);

        bool isPrimary() const;
        void setPrimary(bool ipFlag);

        bool isClustered() const;
        void setClustered(bool ipFlag);

        bool isValid() const;
        void setValid(bool ipFlag);

        bool checksXMin() const;
        void setChecksXMin(bool ipFlag);

        bool isReady() const;
        void setReady(bool ipFlag);

        virtual void loadData() = 0;

        virtual int objectId();

    protected:
        QString mTableName;
        QString mSchemaName;
        DbTable *mTable;
        DbSchema *mSchema;
        int mColumnsCount;
        QVector<int> mColumnsNumbers;
        bool mIsUnique;
        bool mIsPrimary;
        bool mIsClustered;
        bool mIsValid;
        bool mChecksXMin;
        bool mIsReady;

    protected:
        DbIndex(QString ipName);
};

#endif // DBINDEX_H

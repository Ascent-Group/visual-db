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

#ifndef DBTABLE_H
#define DBTABLE_H

#include <DbObject.h>
#include <QStringList>
#include <QVector>

class DbIndex;
class DbSchema;

class DbTable : public DbObject
{
    public:
        virtual ~DbTable();

        int columnsCount() const;
        
        DbSchema* schema() const;
        void setSchema(DbSchema *ipSchema);

        QString schemaName() const;
        QString fullName() const;

	// params: [in] int ipColId
	// specifies the index of a column
	//  e.g.
	// (+)id 0 1 2 3 4  5   6  7
	// col   A B C D E  F   G  H
	// (-)id 0 . . . . -3  -2 -1
        QString columnName(int ipColId) const;
        QString columnType(int ipColId) const;
        bool isColumnNullable(int ipColId) const;
        bool isColumnPrimaryKey(int ipColId) const;
        bool isColumnForeignKey(int ipColId) const;
        QString foreignSchemaName(int ipColId) const;
        QString foreignTableName(int ipColId) const;
        QStringList foreignFields(int ipColId) const;
        bool isColumnUnique(int ipColId) const;

        virtual void loadData() = 0;
        // lyuts: for further functionality
        // lyuts: will save the changes made to the table
        // lyuts: virtual void save() = 0;
        virtual bool checkPrimaryKey(const QString &) const = 0;
        virtual bool checkForeignKey(const QString &, QString *, QString *, QStringList *) const = 0;
        virtual bool checkUnique(const QString &) const = 0;

        int getIndices(QVector<DbIndex*> &ipIndicesList);

        virtual int objectId();

    protected:
        struct ColumnDefinition
        {
            QString name;
            QString type;
            bool isNullable;
            bool isPrimaryKey;
            bool isForeignKey;
            QString foreignSchemaName;
	    QString foreignTableName;
            QStringList foreignFieldNames;
            bool isUnique;
        };

        QString mSchemaName; // for internal use only
        DbSchema *mSchema;
        QVector<ColumnDefinition> mColumnDefs;

    protected:
        DbTable(QString ipSchemaName, QString ipTableName);
};

#endif // DBTABLE_H

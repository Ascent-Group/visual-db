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

#ifndef DBOBJECTS_COMMON_DBTABLE_H
#define DBOBJECTS_COMMON_DBTABLE_H

#include <common/DbObject.h>
#include <common/DbObjectPtr.h>
#include <QStringList>
#include <QVector>

namespace DbObjects
{

namespace Common
{

template<typename T> class DbObjectPtr;
class DbIndex;
class DbSchema;
typedef DbObjectPtr<DbIndex> DbIndexPtr;
typedef DbObjectPtr<DbSchema> DbSchemaPtr;

/*!
 * \class DbTable
 * \headerfile common/DbTable.h
 * \brief Defines databse table object
 */
class DbTable : public Common::DbObject
{
    public:
        virtual ~DbTable();

        quint16 columnsCount() const;

        DbSchemaPtr schema() const;
        void setSchema(const DbSchemaPtr &iSchema);

        // \todo do we need these functions?
        QString fullName() const;

        // params: [in] int iColId
        // specifies the index of a column
        //  e.g.
        // (+)id 0 1 2 3 4  5   6  7
        // col   A B C D E  F   G  H
        // (-)id 0 . . . . -3  -2 -1
        QString columnName(qint16 iColId) const;
        QString columnType(qint16 iColId) const;
        bool isColumnNullable(qint16 iColId) const;
        bool isColumnPrimaryKey(qint16 iColId) const;
        bool isColumnForeignKey(qint16 iColId) const;
        QString foreignSchemaName(qint16 iColId) const;
        QString foreignTableName(qint16 iColId) const;
        QStringList foreignFields(qint16 iColId) const;
        bool isColumnUnique(qint16 iColId) const;

        virtual void resetData();
        // lyuts: for further functionality
        // lyuts: will save the changes made to the table
        // lyuts: virtual void save() = 0;
        /*! \return true - if the column is PK */
        virtual bool checkPrimaryKey(const QString &) const = 0;
        /*! \return true - if the column is FK */
        virtual bool checkForeignKey(const QString &, QString &, QString &, QStringList &) const = 0;
        /*! \return true - if the column is unique */
        virtual bool checkUnique(const QString &) const = 0;

        quint64 getIndices(QVector<DbIndexPtr> &iIndicesList);

        virtual ObjectType type() const;

        quint32 parentTables(QVector<DbTablePtr> &oList) const;

    protected:
        /*!
         * \struct ColumnDefinition
         * \brief Db table column's properties
         */
        struct ColumnDefinition
        {
            /*! Column name */
            QString name;
            /*! Column type (just like it is stored in system catalog) */
            QString type;
            /*! Indicates whether the field can be nullable */
            bool isNullable;
            /*! Indicates whether the field is a primary key */
            bool isPrimaryKey;
            /*! Indicates whether the field is a foreign key */
            bool isForeignKey;
            /*! Name of the foreign schema name */
            QString foreignSchemaName;
            /*! Name of the table tied with a FK constraint */
            QString foreignTableName;
            /*! Name of the column which references this column, when it is a FK */
            QStringList foreignFieldNames;
            /*! Indicates whether the field has to be unique */
            bool isUnique;
        };

        /*! Parent schema handle */
        DbSchemaPtr mSchema;
        /*! Column definitions */
        QVector<ColumnDefinition> mColumnDefs;
        /*! Parent tables */
        QVector<DbTablePtr> mParentTables;
        /*! Comment on table */
        // \todo add member comment

    protected:
        DbTable(QString iName, const DbSchemaPtr &iSchema);
};

} // namespace Common

} // namespace DbObjects

#endif // DBOBJECTS_COMMON_DBTABLE_H


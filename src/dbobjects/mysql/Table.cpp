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

//#include <mysql/Schema.h>
#include <mysql/Table.h>
#include <mysql/Tools.h>
//#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

#include <QtDebug>

namespace DbObjects
{

namespace Mysql
{

/*!
 * Constructor
 *
 * \param[in] iName - Name of the given table
 * \param[in] iSchema - Handle to schema containing the table
 */
Table::Table(QString iName, const Common::DbSchemaPtr &iSchema)
    : DbTable(iName, iSchema)
{
}

/*!
 * Destructor
 */
Table::~Table()
{

}

/*!
 * \brief Load column definitions data
 *
 * \return true - If column definitions have been successfully read
 * \return false - Otherwise
 */
bool
Table::loadData()
{
    if (mIsLoaded) {
        return true;
    }

    // \todo or maybe pass QSqlDatabase as input parameter.
    QSqlDatabase db = mDatabase->dbHandle();
    QSqlQuery query(db);
    QString qstr;

    /*! \todo get version */
    Tools::Version version = Tools::version(db);

    /*! \todo do version check */
    /*! \todo do version specific actions */

    /*if (mTableName.contains(QString('.'))) {
        mSchemaName = mTableName.section('.', 0);
        mTableName = mTableName.section('.', 1);
    }*/

    // create a query
    qstr = QString("SELECT "
            "c.column_name, "
            "c.column_type, "
            "c.is_nullable, "
            "pkc.constraint_name as PK, "
            "fkc.constraint_name as FK, "
            "uc.constraint_name as UK "
            "FROM "
            "information_schema.columns c "
            "LEFT OUTER JOIN information_schema.table_constraints pkc ON "
            "c.column_name = pkc.constraint_name "
            "AND c.table_name = pkc.table_name "
            "AND UPPER(pkc.constraint_type) = 'PRIMARY KEY' "
            "LEFT OUTER JOIN information_schema.table_constraints fkc ON "
            "c.column_name = fkc.constraint_name "
            "AND c.table_name = fkc.table_name "
            "AND UPPER(fkc.constraint_type) = 'FOREIGN KEY' "
            "LEFT OUTER JOIN information_schema.table_constraints uc ON "
            "c.column_name = uc.constraint_name "
            "AND c.table_name = uc.table_name "
            "AND UPPER(uc.constraint_type) = 'UNIQUE' "
            "WHERE "
            "c.table_name = '%1';")
            .arg(mName);

    // if query execution failed
    if (!query.exec(qstr)) {
        qDebug() << query.lastError().text();
        return false;
    }

    // if query result is not empty
    if (query.first()) {
        // for each retrieved row
        do {
            /* temporary debug output */
#if DEBUG_TRACE
            qDebug() << query.value(0).toString();
            qDebug() << query.value(1).toString();
            qDebug() << query.value(2).toString();
            qDebug() << query.value(3).toString();
            qDebug() << query.value(4).toString();
            qDebug() << query.value(5).toString();
#endif

            // create column definition
            struct ColumnDefinition cDef;

            // populate column definition
            cDef.name = query.value(0).toString();
            cDef.type = query.value(1).toString();
            // unlike psql stores "is_nullable" flag instead of "not_null"
            cDef.isNullable = query.value(2).toBool();
            cDef.isPrimaryKey = (!query.value(3).toString().isEmpty()) ? true : false;
            cDef.isForeignKey = (!query.value(4).toString().isEmpty()) ? true : false;
            cDef.isUnique = (!query.value(5).toString().isEmpty()) ? true : false;

            /* temporary debug output */
#if DEBUG_TRACE
            qDebug() << "isPrimaryKey " << cDef.isPrimaryKey;
            qDebug() << "isForeignKey " << cDef.isForeignKey;
            qDebug() << "isUnique " << cDef.isUnique;
#endif

            // add coumn definition
            mColumnDefs.push_back(cDef);
        } while (query.next());

        return true;
    }

    return DbTable::loadData();
}

/*!
 * Checks if a column is a primary key for the given table
 *
 * \param[in] iColumnName - Name of a column
 *
 * \return true If a column is a primary key
 * \return false Otherwise
 */
bool
Table::checkPrimaryKey(const QString &iColumnName) const
{
    QSqlQuery query(mDatabase->dbHandle());

    // create query
    QString qstr = QString("SELECT "
                                "tc.constraint_name "
                           "FROM "
                                "information_schema.table_constraints tc "
                           "WHERE "
                                "tc.table_name = '%1' "
                                "AND tc.constraint_name = '%2' "
                                "AND UPPER(tc.constraint_type) = 'PRIMARY KEY';")
                .arg(mName)
                .arg(iColumnName);

#ifdef DEBUG_QUERY
    qDebug() << "Table::checkPrimaryKey> " << qstr;
#endif

    // if query execution failed
    if (!query.exec(qstr)) {
        qDebug() << query.lastError().text();

        return false;
    }

    // primary key constraint definitino found
    return query.first();
}

/*!
 * Checks if column is a foreign key for the given table
 *
 * \param[in] iColumnName - Name of a column
 * \param[out] oForeignSchemaName - Name of a schema containing the referenced table
 * \param[out] oForeignTableName - Name of a table references by this foreign key
 *
 * \return true If a column is a foreign key
 * \return false Otherwise
 */
bool
Table::checkForeignKey(const QString &iColumnName, QString &oForeignSchemaName, QString &oForeignTableName, QStringList &) const
{
    Q_UNUSED(oForeignSchemaName);
    Q_UNUSED(oForeignTableName);

    QSqlQuery query(mDatabase->dbHandle());

    // create a query
    QString qstr = QString("SELECT "
                                "tc.constraint_name "
                           "FROM "
                                "information_schema.table_constraints tc "
                           "WHERE "
                                "tc.table_name = '%1' "
                                "AND tc.constraint_name = '%2' "
                                "AND UPPER(tc.constraint_type) = 'FOREIGN KEY';")
                .arg(mName)
                .arg(iColumnName);

#ifdef DEBUG_QUERY
    qDebug() << "Table::checkForeignKey> " << qstr;
#endif

    // if query execution failed
    if (!query.exec(qstr)) {
        qDebug() << query.lastError().text();

        return false;
    }

    // that foreign key constraint definition found
    return query.first();
}

/*!
 * Checks if a column is unique for the given table
 *
 * \param[in] iColumnName - Name of a column
 *
 * \return true If the column has UNIQUE constraint
 * \return false Otherwise
 */
bool
Table::checkUnique(const QString &iColumnName) const
{
    QSqlQuery query(mDatabase->dbHandle());

    // create query
    QString qstr = QString("SELECT "
                                "tc.constraint_name "
                           "FROM "
                                "information_schema.table_constraints tc "
                           "WHERE "
                                "tc.table_name = '%1' "
                                "AND tc.constraint_name = '%2' "
                                "AND UPPER(tc.constraint_type) = 'UNIQUE';")
                .arg(mName)
                .arg(iColumnName);

#ifdef DEBUG_QUERY
    qDebug() << "Table::checkUnique> " << qstr;
#endif

    // if query execution failed
    if (!query.exec(qstr)) {
        qDebug() << query.lastError().text();

        return false;
    }

    // unique constraint definiton found
    return query.first();
}

} // namespace Mysql

} // namespace DbObjects


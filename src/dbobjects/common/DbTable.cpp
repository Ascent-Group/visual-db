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
#include <common/DbSchema.h>
#include <common/DbTable.h>
#include <QStringList>

#include <QtDebug>

/*!
 * Constructor
 *
 * \param[in] ipSchemaName - Parent schema name
 * \param[in] ipTableName - Table name
 */
DbTable::DbTable(QString ipSchemaName, QString ipTableName)
    : DbObject(ipTableName), mSchemaName(ipSchemaName)
{
    setSchema(Database::instance()->findSchema(mSchemaName));
}

/*!
 * Destructor
 */
DbTable::~DbTable()
{

}

/*!
 * \return Number of columns
 */
quint16
DbTable::columnsCount() const
{
    return mColumnDefs.size();
}

/*!
 * \return tParent schema handle
 */
DbSchema*
DbTable::schema() const
{
    return mSchema;
}

/*!
 * \param[in] ipSchema - Parent schema
 */
void
DbTable::setSchema(DbSchema *ipSchema)
{
    mSchema = ipSchema;
}

/*!
 * \return Parent schema's name
 */
QString
DbTable::schemaName() const
{
    if (mSchema) {
        return mSchema->name();
    }

    return mSchemaName;
}

/*!
 * \return Fhe full name of the given table in a 'schema.name' format
 */
QString
DbTable::fullName() const
{
    return QString("%1.%2").arg(mSchemaName).arg(mName);
}

/*!
 * \param[in] ipColId - Column id.
 *
 * \return Column name
 */
QString
DbTable::columnName(qint16 ipColId) const
{
    // if id is negative
    if (ipColId < 0) {
        // consider it as a id going from right to left
        ipColId += mColumnDefs.count();
    }

    // if id is out of bounds
    if (0 > ipColId || ipColId >= mColumnDefs.count()) {
    qDebug() << QString("DbTable::columnName> For %1 id %2 is out of bounds!!!")
        .arg(mName)
        .arg(ipColId);
        return QString("");
    }

    return mColumnDefs.at(ipColId).name;
}

/*!
 * \param[in] ipColId - Column id.
 *
 * \return The type of the column with ipColId id.
 */
QString
DbTable::columnType(qint16 ipColId) const
{
    // if id is negative
    if (ipColId < 0) {
        // consider it as a id going from right to left
        ipColId += mColumnDefs.count();
    }

    // if id is out of bounds
    if (0 > ipColId || ipColId >= mColumnDefs.count()) {    
    qDebug() << QString("DbTable::columnType> For %1 id %2 is out of bounds!!!")
        .arg(mName)
        .arg(ipColId);

        return QString("");
    }

    return mColumnDefs.at(ipColId).type;
}

/*!
 * \param[in] ipColId - Column id.
 *
 * \return true - If a column specified by ipColId is nullable
 * \return false - Otherwise
 */
bool
DbTable::isColumnNullable(qint16 ipColId) const
{
    // if id is negative
    if (ipColId < 0) {
        // consider it as a id going from right to left
        ipColId += mColumnDefs.count();
    }

    // if id is out of bounds
    if (0 > ipColId || ipColId >= mColumnDefs.count()) {
    qDebug() << QString("DbTable::isColumnNullable> For %1 id %2 is out of bounds!!!")
        .arg(mName)
        .arg(ipColId);
    return false;
    }

    return mColumnDefs.at(ipColId).isNullable;
}

/*!
 * \param[in] ipColId - Column id.
 *
 * \return true - If a column specified by ipColId is a primary key
 * \return false - Otherwise
 */
bool
DbTable::isColumnPrimaryKey(qint16 ipColId) const
{
    // if id is negative
    if (ipColId < 0) {
        // consider it as a id going from right to left
        ipColId += mColumnDefs.count();
    }

    // if id is out of bounds
    if (0 > ipColId || ipColId >= mColumnDefs.count()) {
    qDebug() << QString("DbTable::isColumnPrimaryKey> For %1 id %2 is out of bounds!!!")
        .arg(mName)
        .arg(ipColId);

        return false;
    }

    return mColumnDefs.at(ipColId).isPrimaryKey;
}

/*!
 * \param[in] ipColId - Column id.
 *
 * \return true - If a column specified by ipColId is a foreign key
 * \return false - Otherwise
 */
bool
DbTable::isColumnForeignKey(qint16 ipColId) const
{
    // if id is negative
    if (ipColId < 0) {
        // consider it as a id going from right to left
        ipColId += mColumnDefs.count();
    }

    // if id is out of bounds
    if (0 > ipColId || ipColId >= mColumnDefs.count()) {
    qDebug() << QString("DbTable::isColumnForeignKey> For %1 id %2 is out of bounds!!!")
        .arg(mName)
        .arg(ipColId);

        return false;
    }

    return mColumnDefs.at(ipColId).isForeignKey;
}

/*!
 * \param[in] ipColId - Column id
 *
 * \return Foreign table's schema name
 */
QString
DbTable::foreignSchemaName(qint16 ipColId) const
{
    // if id is negative
    if (ipColId < 0) {
        // consider it as a id going from right to left
        ipColId += mColumnDefs.count();
    }

    // if id is out of bounds
    if (0 > ipColId || ipColId >= mColumnDefs.count()) {
    qDebug() << QString("DbTable::foreignSchemaName> For %1 id %2 is out of bounds!!!")
        .arg(mName)
        .arg(ipColId);

        return QString("");
    }

    return mColumnDefs.at(ipColId).foreignSchemaName;
}

/*!
 * \param[in] ipColId - Column id
 *
 * \return A table name pointed to by a foregin key
 */
QString
DbTable::foreignTableName(qint16 ipColId) const
{
    // if id is negative
    if (ipColId < 0) {
        // consider it as a id going from right to left
        ipColId += mColumnDefs.count();
    }

    // if id is out of bounds
    if (0 > ipColId || ipColId >= mColumnDefs.count()) {
    qDebug() << QString("DbTable::foreignTableName> For %1 id %2 is out of bounds!!!")
        .arg(mName)
        .arg(ipColId);

        return QString("");
    }

    return mColumnDefs.at(ipColId).foreignTableName;
}

/*!
 * \param[in] ipColId - Column id
 *
 * \return The list of referenced fields by a foreign key column that has
 * ipColId id.
 */
QStringList
DbTable::foreignFields(qint16 ipColId) const
{
    //quint16 count = mColumnDefs.at(ipColId).foreignFieldNames.count();

    //QStringList list;

    //for (quint16 i = 0; i < count; ++i) {
    //    list << mColumnDefs.at(ipColId).foreignFieldNames.at(i);
    //}

    //return list;
    return mColumnDefs.at(ipColId).foreignFieldNames;
}

/*!
 * \param[in] ipColId - Column id
 *
 * \return true - If column with id = ipColId has unique constraint
 * \return false - Otherwise
 */
bool
DbTable::isColumnUnique(qint16 ipColId) const
{
    // if id is negative
    if (ipColId < 0) {
        // consider it as a id going from right to left
        ipColId += mColumnDefs.count();
    }

    // if id is out of bounds
    if (0 > ipColId || ipColId >= mColumnDefs.count()) {
    qDebug() << QString("DbTable::isColumnUnique> For %1 id %2 is out of bounds!!!")
        .arg(mName)
        .arg(ipColId);

        return false;
    }

    return mColumnDefs.at(ipColId).isUnique;
}

/*!
 * \param[out] opIndicesList - A list of indices assigned to the given table
 *
 * \returns The number of indices for the given table and populates the
 * input list with pointers to indices
 */
quint64
DbTable::getIndices(QVector<DbIndex*> &opIndicesList)
{
    return Database::instance()->findTableIndices(this, opIndicesList);
}

/*!
 * \return Database object type identifier
 */
DbObject::Type
DbTable::type() const
{
    return DbObject::TableObject;
}


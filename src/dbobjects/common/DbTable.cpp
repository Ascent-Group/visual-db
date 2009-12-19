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

#include <Database.h>
#include <DbSchema.h>
#include <DbTable.h>
#include <QStringList>

#include <QtDebug>

/*
 * Constructor
 */
DbTable::DbTable(QString ipSchemaName, QString ipTableName)
    : DbObject(ipTableName), mSchemaName(ipSchemaName)
{
    setSchema(Database::instance()->findSchema(mSchemaName));
}

/*
 * Destructor
 */
DbTable::~DbTable()
{

}

/*
 * Returns the number of columns
 */
int
DbTable::columnsCount() const
{
    return mColumnDefs.size();
}

/*
 * Returns table's schema
 */
DbSchema*
DbTable::schema() const
{
    return mSchema;
}

/*
 * Sets the schema
 */
void
DbTable::setSchema(DbSchema *ipSchema)
{
    mSchema = ipSchema;
}

/*
 * Returns a name of table's schema
 */
QString
DbTable::schemaName() const
{
    if (mSchema) {
        return mSchema->name();
    }

    return mSchemaName;
}

/*
 * Returns the full name of the given table in a 'schema.name' representation
 */
QString
DbTable::fullName() const
{
    return QString("%1.%2").arg(mSchemaName).arg(mName);
}

/*
 * Returns a column name by its index
 */
QString
DbTable::columnName(int ipColId) const
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

/*
 * Returns the column type by its index
 */
QString
DbTable::columnType(int ipColId) const
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

/*
 * Returns true if column is nullable
 */
bool
DbTable::isColumnNullable(int ipColId) const
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

/*
 * Returns true if column is a primary key
 */
bool
DbTable::isColumnPrimaryKey(int ipColId) const
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

/*
 * Returns true if column is a foreign key
 */
bool
DbTable::isColumnForeignKey(int ipColId) const
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

/*
 * Returns foreign table's schema name pointed by a foreign key
 */
QString
DbTable::foreignSchemaName(int ipColId) const
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

/*
 * Returns a table name pointed to by a foregin key
 */
QString
DbTable::foreignTableName(int ipColId) const
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

/*
 * Returns the list of referenced fields by a foreign key
 */
QStringList
DbTable::foreignFields(int colId) const
{
    int count = mColumnDefs.at(colId).foreignFieldNames.count();

    QStringList list;

    for (int i = 0; i < count; ++i) {
        list << mColumnDefs.at(colId).foreignFieldNames.at(i);
    }

    return list;
}

/*
 * Returns true if column has unique constraint
 */
bool
DbTable::isColumnUnique(int ipColId) const
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

/*
 * Returns the number of indices for the given table
 * and populates the input list with pointers to indices
 */
int
DbTable::getIndices(QVector<DbIndex*> &ipIndicesList)
{
    return Database::instance()->findTableIndices(this, ipIndicesList);
}

/*
 * Returns the id of database object type
 */
DbObject::Type
DbTable::type()
{
    return DbObject::TableObject;
}

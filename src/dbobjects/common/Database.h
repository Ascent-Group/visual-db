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

#ifndef DBOBJECTS_COMMON_DATABASE_H
#define DBOBJECTS_COMMON_DATABASE_H

#include <QVector>
#include <common/DbTable.h>

namespace DbObjects
{

namespace Common
{

class DbIndex;
class DbLanguage;
class DbRole;
class DbSchema;

/*!
 * \class Database
 * \headerfile common/Database.h
 * \brief Class (singleton) that holds and knows how to operate with all
 *        cached db objects
 */
class Database
{
    public:
        static Database* instance();

        /*!
         * \enum SqlDriverType
         * Sql driver indentifiers
         */
        enum SqlDriverType
        {
            Unknown = 0,
            PostgreSQL,
            MySQL,
            Oracle,
            SQLite
        };

        bool registerNew(const DbObject *ipObject);
        bool registerModified(const DbObject *ipObject);
        bool registerDeleted(const DbObject *ipObject);

    public:
        bool addSchema(DbSchema *);
        bool addRole(DbRole *);
        bool addIndex(DbIndex *);
        bool addLanguage(DbLanguage *);

        void schemasList(QStringList *ipList) const;
        quint64 schemasCount() const;

        void rolesList(QStringList *ipList) const;
        quint64 rolesCount() const;

        void indicesList(QStringList *ipList) const;
        quint64 indicesCount() const;

        void languagesList(QStringList *ipList) const;
        quint8 languagesCount() const;

        DbSchema* findSchema(const QString &ipSchemaName) const;

        // \todo lyuts: maybe we will need this
        //Schema* findSchema(DbTable *ipTable) const;
        //DbTable* findTable(QString ipTableName) const;

        DbRole* findRole(const QString &ipRoleName) const;
        DbIndex* findIndex(const QString &ipIndexName) const;
        DbLanguage* findLanguage(const QString &ipLangName) const;

        quint64 findTableIndices(const DbTable *ipTable, QVector<DbIndex*> &ipList) const;

        void setSqlDriver(const QString &ipSqlDriverName);
        SqlDriverType sqlDriver() const;

        void readSchemas();
        void readRoles();
        void readIndices();
        void readLanguages();

        virtual bool loadData();
        virtual void resetData();

    protected:
        Database();
        virtual ~Database();

    private:
        static Database *mInstance;
        QVector<DbSchema*> mSchemas;
        QVector<DbRole*> mRoles;
        QVector<DbIndex*> mIndices;
        QVector<DbLanguage*> mLanguages;
        SqlDriverType mSqlDriver;

        QVector<const DbObject*> mNewObjects;
        QVector<const DbObject*> mModifiedObjects;
        QVector<const DbObject*> mDeletedObjects;

        friend class DatabaseManager;

    private:
        Q_DISABLE_COPY(Database);

        DbObject* findObject(const QString &ipObjName, DbObject::Type) const;
};

/*!
 * \class DatabaseManager
 * \headerfile common/Database.h
 * \brief Singleton manager that knows how to utilize the Database instance
 *
 * The main purpose for this class is to handle Database's instance
 * deletion. That's why it is Database's friend, so that it has
 * access to Database::mInstance;
 */
class DatabaseManager
{
    public:
        void flush();

};

} // namespace Common

} // namespace DbObjects

#endif // DBOBJECTS_COMMON_DATABASE_H


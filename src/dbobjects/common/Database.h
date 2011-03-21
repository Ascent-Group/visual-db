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

//#include <common/DbObjectPtr.h>

#include <QSqlDatabase>
#include <QStringList>
#include <QVector>

namespace DbObjects
{

namespace Common
{

template<typename T> class DbObjectPtr;
class DbIndex;
class DbLanguage;
class DbRole;
class DbSchema;
class DbTable;
class Factories;
class Tools;
typedef DbObjectPtr<DbIndex> DbIndexPtr;
typedef DbObjectPtr<DbLanguage> DbLanguagePtr;
typedef DbObjectPtr<DbRole> DbRolePtr;
typedef DbObjectPtr<DbSchema> DbSchemaPtr;
typedef DbObjectPtr<DbTable> DbTablePtr;

/*!
 * \class Database
 * \headerfile common/Database.h
 * \brief Class (singleton) that holds and knows how to operate with all
 *        cached db objects
 */
class Database
{
    public:
        Database(const QSqlDatabase &iDb);
        virtual ~Database();

        // \todo remove this func
        static Database* instance();

        /*!
         * \enum SqlDriverType
         * \brief Sql driver indentifiers
         */
        enum SqlDriverType
        {
            Unknown = 0,
            PostgreSQL,
            MySQL,
            Oracle,
            SQLite
        };

        /*!
         * \enum DBMSVersion
         * \brief Version of DBMS
         */
        enum DBMSVersion
        {
            DBMS_Unknown = 0,
            /*! PostgreSQL */
            PostgreSQL_Unknown,
            PostgreSQL_8,
            PostgreSQL_9,
            /*! MySQL */
            MySQL_Unknown,
            MySQL_5
        };

//        bool registerNew(const DbObject *iObject);
//        bool registerModified(const DbObject *iObject);
//        bool registerDeleted(const DbObject *iObject);

    public:
        DBMSVersion version() const;
        QSqlDatabase dbHandle() const;

        bool addSchema(const DbSchemaPtr &);
        bool addRole(const DbRolePtr &);
        bool addIndex(const DbIndexPtr &);
        bool addLanguage(const DbLanguagePtr &);

        void schemasList(QStringList &oList) const;
        quint64 schemasCount() const;

        void rolesList(QStringList &oList) const;
        quint64 rolesCount() const;

        void indicesList(QStringList &oList) const;
        quint64 indicesCount() const;

        void languagesList(QStringList &oList) const;
        quint8 languagesCount() const;

        DbSchemaPtr findSchema(const QString &iSchemaName) const;

        // \todo lyuts: maybe we will need this
        //Schema* findSchema(DbTable *iTable) const;
        //DbTable* findTable(QString iTableName) const;

        DbRolePtr findRole(const QString &iRoleName) const;
        DbIndexPtr findIndex(const QString &iIndexName) const;
        DbLanguagePtr findLanguage(const QString &iLangName) const;

        quint64 findTableIndices(const DbTablePtr &iTable, QVector<DbIndexPtr> &iList) const;

        SqlDriverType sqlDriver() const;
        void setSqlDriver(const QString &iSqlDriverName);

        void readSchemas(Factories *iFactories, Tools *iTools);
        void readRoles(Factories *iFactories, Tools *iTools);
        void readIndices(Factories *iFactories, Tools *iTools);
        void readLanguages(Factories *iFactories, Tools *iTools);

        virtual bool loadData(Factories *iFactories, Tools *iTools);
        virtual void resetData();

        QString lastError() const;

    private:
        // \todo add function for query execution that will use this handle
        QSqlDatabase mDbHandle;
        DBMSVersion mVersion;
//        static Database *mInstance;
        QVector<DbSchemaPtr> mSchemas;
        QVector<DbRolePtr> mRoles;
        QVector<DbIndexPtr> mIndices;
        QVector<DbLanguagePtr> mLanguages;
        SqlDriverType mSqlDriver;

        /*! \todo Investigate on const DbObject * vs DbObjectPtr */
        // \todo The next UnitOfWork vector will probably be separate for different object
        // types, i.e. QVector<DbTablePtr> mNewTables;

//        QVector<const DbObject*> mNewObjects;
//        QVector<const DbObject*> mModifiedObjects;
//        QVector<const DbObject*> mDeletedObjects;

//        friend class DatabaseManager;

    private:
        Q_DISABLE_COPY(Database);

};

} // namespace Common

} // namespace DbObjects

#endif // DBOBJECTS_COMMON_DATABASE_H


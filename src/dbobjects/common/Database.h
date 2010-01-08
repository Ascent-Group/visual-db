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

#ifndef DATABASE_H
#define DATABASE_H

#include <QVector>
#include <common/DbTable.h>

class DbIndex;
class DbLanguage;
class DbRole;
class DbSchema;

class Database
{
    public:
        static Database* instance();

        enum SqlDriverType
        {
            Unknown = 0,
            PostgreSQL,
            MySQL,
            Oracle,
            SQLite
        };

    public:
        void addSchema(DbSchema *);
        void addRole(DbRole *);
        void addIndex(DbIndex *);
        void addLanguage(DbLanguage *);

        void schemasList(QStringList *ipList) const;
        quint64 schemasCount() const;

        void rolesList(QStringList *ipList) const;
        quint64 rolesCount() const;

        void indicesList(QStringList *ipList) const;
        quint64 indicesCount() const;

        void languagesList(QStringList *ipList) const;
        quint8 languagesCount() const;

        DbSchema* findSchema(const QString &ipSchemaName) const;

        // lyuts: maybe we will need this
        //Schema* findSchema(DbTable *ipTable) const;
        //DbTable* findTable(QString ipTableName) const;

        // lyuts:
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

        void cleanup();



    protected:
        Database();
        virtual ~Database();

    private:
        static Database	*mInstance;
        QVector<DbSchema*> mSchemas;
        QVector<DbRole*> mRoles;
        QVector<DbIndex*> mIndices;
        QVector<DbLanguage*> mLanguages;
        SqlDriverType mSqlDriver;

	friend class DatabaseManager;

    private:
        DbObject* findObject(const QString &ipObjName, DbObject::Type) const;
};

/*
 * Class: DatabaseManager
 * Descr: The main purpose for this class is to handle Database's instance
 *        deletion. That's why it is Database's friend, so that it has
 *        access to Database::mInstance;
 */
class DatabaseManager
{
    public:
        void flush();

};

#endif // DATABASE_H

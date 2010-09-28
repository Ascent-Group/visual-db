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

#ifndef DBOBJECTS_COMMON_DBSCHEMA_H
#define DBOBJECTS_COMMON_DBSCHEMA_H

#include <QStringList>
#include <QVector>
#include <common/Database.h>
#include <common/DbObject.h>

namespace DbObjects
{

namespace Common
{

class DbProcedure;
class DbTable;
class DbTrigger;
class DbView;

/*!
 * \class DbSchema
 * \brief Defines database schema object
 * \headerfile DbSchema.h <common/DbSchema.h>
 */
class DbSchema : public DbObject
{
    public:
        DbSchema(QString ipName, DbRole *ipOwner);
        virtual ~DbSchema();

        bool addTable(DbTable *ipTable);
        bool addView(DbView *ipView);
        bool addProcedure(DbProcedure *ipProc);
        bool addTrigger(DbTrigger *);

        /*! \todo pass list by reference */
        void tablesList(QStringList *) const;
        quint64 tablesCount() const;

        void viewsList(QStringList *) const;
        quint64 viewsCount() const;

        void proceduresList(QStringList *) const;
        quint64 proceduresCount() const;

        void triggersList(QStringList *) const;
        quint16 triggersCount() const;

        DbTable* findTable(const QString &ipTableName) const;
        DbView* findView(const QString &ipViewName) const;
        DbProcedure* findProcedure(const QString &ipProcName) const;
        DbTrigger* findTrigger(const QString &ipTrigName) const;

        void readTables();
        void readViews();
        void readProcedures();
        void readTriggers();

        void cleanup();

        virtual DbObject::Type type() const;

        QString ownerName() const;

        QString description() const;
        void setDescription(const QString & ipDescription);

        virtual bool loadData();
        virtual void resetData();

    private:
        DbRole *mOwner;
        QString mDescription;
        QVector<DbTable*> mTables;
        QVector<DbView*> mViews;
        QVector<DbProcedure*> mProcedures;
        QVector<DbTrigger*> mTriggers;

    private:
        DbObject* findObject(const QString &ipObjName, DbObject::Type) const;
};

} // namespace Common

} // namespace DbObjects

#endif // DBOBJECTS_COMMON_DBSCHEMA_H


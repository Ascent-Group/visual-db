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

#ifndef DBTRIGGER_H
#define DBTRIGGER_H

#include <common/DbObject.h>
#include <common/DbProcedure.h>
#include <common/DbTable.h>

/*!
 * \class DbTrigger
 * \headerfile <common/DbTrigger.h>
 * \brief Represents database trigger object
 */
class DbTrigger : public DbObject
{
    public:
        virtual ~DbTrigger();

        DbSchema* schema() const;
        void setSchema(DbSchema *ipSchema);

        DbTable* table() const;
        void setTable(DbTable *ipTable);

        DbProcedure* procedure() const;
        void setProcedure(DbProcedure *ipProcedure);

        QChar enabled() const;
        void setEnabled(const QChar &ipFlag);

        bool isConstraint() const;
        void setConstraint(bool ipFlag);

        QString constraintName() const;
        void setConstraintName(const QString &ipName);

        DbTable* referencedTable() const;
        void setReferencedTable(DbTable *ipTable);

        bool isDeferrable() const;
        void setDeferrable(bool ipFlag);

        bool isInitiallyDeferred() const;
        void setInitiallyDeferred(bool ipFlag);

        quint16 numArgs() const;
        void setNumArgs(const quint16 &ipNum);

        virtual DbObject::Type type() const;
        /*!
         * \see PsqlTrigger::loadData()
         * \see MysqlTrigger::loadData()
         */
        virtual void loadData() = 0;

        QString schemaName() const;
        QString fullName() const;

    protected:
        DbTrigger(QString ipSchemaName, QString ipName);

        /*! Parent schema name */
        QString mSchemaName; // for internal use only
        /*! Table */
        DbTable *mTable;
        /*! Parent schema */
        DbSchema *mSchema;
        /*! ??? */
        DbProcedure *mProcedure;
        /*! Controls in which session_replication_role modes the trigger fires
         * O - origin and local modes
         * D - disabled
         * R - replica mode
         * A - always
         */
        QChar mEnabled;
        /*! True if trigger is a "constraint trigger" */
        bool mIsConstraint;
        /*! Constraint name, if a constraint trigger */
        QString mConstraintName;
        /*! The table referenced by a referential integrity constraint */
        DbTable *mReferencedTable;
        /*! True if constraint trigger is deferrable */
        bool mIsDeferrable;
        /*! True if constraint trigger is initially deferred */
        bool mIsInitiallyDeferred;
        /*! Number of argument strings passed to trigger function */
        quint16 mNumArgs;

};

#endif // DBTRIGGER_H


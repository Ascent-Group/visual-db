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

#include <DbObject.h>
#include <DbProcedure.h>
#include <DbTable.h>

class DbTrigger : public DbObject
{
    public:
        virtual ~DbTrigger();

        DbTable* table() const;
        void setTable(DbTable *ipTable);

        DbProcedure* procedure() const;
        void setProcedure(DbProcedure *ipProcedure);

        QChar enabled() const;
        void setEnabled(const QChar ipFlag);

        bool isConstraint() const;
        void setConstraint(const bool ipFlag);

        QString constraintName() const;
        void setConstraintName(const QString &ipName);

        DbTable* referencedTable() const;
        void setReferencedTable(DbTable *ipTable);

        bool isDeferrable() const;
        void setDeferrable(const bool ipFlag);

        bool isInitiallyDeferred() const;
        void setInitiallyDeferred(const bool ipFlag);

        int numArgs() const;
        void setNumArgs(const int &ipNum);

        virtual int objectId();
        virtual void loadData() = 0;

    protected:
        DbTrigger(QString ipName);

        DbTable *mTable;
        DbProcedure *mProcedure;
        // controls in which session_replication_role modes the trigger fires
        // O - origin and local modes
        // D - disabled
        // R - replica mode
        // A - always
        QChar mEnabled;
        bool mIsConstraint;
        QString mConstraintName; // name of constr if mIsConstraint is true
        DbTable *mReferencedTable;
        //DbConstraint
        bool mIsDeferrable;
        bool mIsInitiallyDeferred;
        int mNumArgs;

};

#endif // DBTRIGGER_H

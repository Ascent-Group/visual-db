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

#ifndef DBTRIGGERTEST_H
#define DBTRIGGERTEST_H

#include <QtTest/QtTest>
#include <dbobjects/common/Database.h>

typedef DbObjects::Common::DbObjectPtr<DbObjects::Common::DbProcedure> DbProcedurePtr;
typedef DbObjects::Common::DbObjectPtr<DbObjects::Common::DbSchema> DbSchemaPtr;
typedef DbObjects::Common::DbObjectPtr<DbObjects::Common::DbTable> DbTablePtr;
typedef DbObjects::Common::DbObjectPtr<DbObjects::Common::DbTrigger> DbTriggerPtr;

class DbTriggerTest : public QObject
{
    Q_OBJECT

    private slots:
        void initTestCase();
        void cleanupTestCase();

        void init();
        void cleanup();

        void enabledTest();
        void isConstraintTest();
        void isDeferrableTest();
        void isInitiallyDeferredTest();
        void loadDataTest();
        void numArgsTest();
        void typeTest();
        void procedureTest();
        void constraintNameTest();
        void referencedTableTest();
        void schemaTest();
        void setConstraintNameTest();
        void setConstraintTest();
        void setDeferrableTest();
        void setEnabledTest();
        void setInitiallyDeferredTest();
        void setNumArgsTest();
        void setProcedureTest();
        void setReferencedTableTest();
        void setSchemaTest();
        void setTableTest();
        void tableTest();

    private:
        QString mSchemaName;
        QString mTriggerName;
        QString mProcedureName;
        QString mTableName;
        QString mConstraintName;
        QString mRefTableName;
        DbTriggerPtr mTrigger;

        DbObjects::Common::Database *mDbInst;
};

#endif // DBTRIGGERTEST_H


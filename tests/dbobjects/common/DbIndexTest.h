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

#ifndef DBINDEXTEST_H
#define DBINDEXTEST_H

#include <QtTest/QtTest>

namespace DbObjects
{
namespace Common
{
class Database;
}
}

class DbIndexTest : public QObject
{
    Q_OBJECT

    private slots:
        void initTestCase();
        void cleanupTestCase();

        void init();
        void cleanup();

        void addColumnNumberTest();
        void checksXMinTest();
        void columnsCountTest();
        void columnsNumbersTest();
        void isClusteredTest();
        void isPrimaryTest();
        void isReadyTest();
        void isUniqueTest();
        void isValidTest();
        void loadDataTest();
        void typeTest();
        void schemaNameTest();
        void schemaTest();
        void setChecksXMinTest();
        void setClusteredTest();
        void setColumnsCountTest();
        void setPrimaryTest();
        void setReadyTest();
        void setSchemaNameTest();
        void setSchemaTest();
        void setTableNameTest();
        void setTableTest();
        void setUniqueTest();
        void setValidTest();
        void tableNameTest();
        void tableTest();

    private:
        DbObjects::Common::Database *mDbInst;
};

#endif // DBINDEXTEST_H


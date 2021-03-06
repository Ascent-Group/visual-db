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

#ifndef DATABASETEST_H
#define DATABASETEST_H

#include <QtTest/QtTest>

namespace DbObjects
{
namespace Common
{
class Database;
}

}

// \todo Add test for resetData and loadData

class DatabaseTest : public QObject
{
    Q_OBJECT

    private slots:
        /* QTestLib slots - BEGIN */
        void initTestCase();
        void cleanupTestCase();

        void init();
        void cleanup();
        /* QTestLib slots - END */

        void addIndexTest();
        void addLanguageTest();
        void addRoleTest();
        void addSchemaTest();
        void resetDataTest();
        void findIndexTest();
        void findLanguageTest();
        void findRoleTest();
        void findSchemaTest();
        void findTableIndicesTest();
        void flushTest();
        void indicesCountTest();
        void indicesListTest();
        void instanceTest();
        void languagesCountTest();
        void languagesListTest();
        void readIndicesTest();
        void readLanguagesTest();
        void readRolesTest();
        void readSchemasTest();
        void rolesCountTest();
        void rolesListTest();
        void schemasCountTest();
        void schemasListTest();
        void setSqlDriverTest();
        void sqlDriverTest();

    private:
        DbObjects::Common::Database *mDbInst;
        static const quint8 LANGUAGES_COUNT;
        static const quint64 SCHEMAS_COUNT;
        static const quint64 ROLES_COUNT;
        static const quint64 INDICES_COUNT;

};

#endif // DATABASETEST_H


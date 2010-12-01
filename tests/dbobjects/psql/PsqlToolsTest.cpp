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

#include <dbobjects/psql/Tools.h>
#include <dbobjects/psql/PsqlToolsTest.h>

using namespace DbObjects::Psql;

void
PsqlToolsTest::initTestCase()
{
    mIndicesNamesList << "ind_artists"
                      << "ind_albums"
                      << "ind_tracks"
                      << "ind_users"
                      << "ind_locations";

    mLanguagesNamesList << "plpgsql"
                        << "c";

    mProceduresNamesList << "insert_album"
                         << "insert_track"
                         << "check_location"
                         << "check_release_date";

    mRolesNamesList << "postgres"
                    << "music_user";

    mSchemasNamesList << "vtunes"
                      << "pg_catalog"
                      << "information_schema";

    mTablesNamesList << "artists"
                     << "genres"
                     << "albums"
                     << "tracks"
                     << "locations"
                     << "users"
                     << "playlists"
                     << "extended_playlists";

    mTriggersNamesList << "albums_biu"
                       << "locations_biu";

    mViewsNamesList << "users_playlists"
                    << "artists_tracks";
}

void
PsqlToolsTest::cleanupTestCase()
{
}

void
PsqlToolsTest::schemasListTest()
{
    QStringList actualSchemasNames;
    Tools::schemasList(actualSchemasNames);

    foreach (const QString &name, mSchemasNamesList) {
        QVERIFY(actualSchemasNames.contains(name));
    }
}


void
PsqlToolsTest::indicesListTest()
{
    QStringList actualIndicesNames;
    Tools::indicesList(actualIndicesNames);

    foreach (const QString &name, mIndicesNamesList) {
        QVERIFY(actualIndicesNames.contains(name));
    }
}


void
PsqlToolsTest::languagesListTest()
{
    QStringList actualLanguagesNames;
    Tools::languagesList(actualLanguagesNames);

    foreach (const QString &name, mLanguagesNamesList) {
        QVERIFY(actualLanguagesNames.contains(name));
    }
}


void
PsqlToolsTest::proceduresListTest()
{
    QStringList actualProceduresNames;
    Tools::proceduresList("vtunes", actualProceduresNames);

    foreach (const QString &name, mProceduresNamesList) {
        QVERIFY(actualProceduresNames.contains(name));
    }
}


void
PsqlToolsTest::rolesListTest()
{
    QStringList actualRolesNames;
    Tools::rolesList(actualRolesNames);

    foreach (const QString &name, mRolesNamesList) {
        QVERIFY(actualRolesNames.contains(name));
    }
}


void
PsqlToolsTest::tablesListTest()
{
    QStringList actualTablesNames;
    Tools::tablesList("vtunes", actualTablesNames);

    foreach (const QString &name, mTablesNamesList) {
        QVERIFY(actualTablesNames.contains(name));
    }
}


void
PsqlToolsTest::triggersListTest()
{
    QStringList actualTriggersNames;
    Tools::triggersList("vtunes", actualTriggersNames);

    foreach (const QString &name, mTriggersNamesList) {
        QVERIFY(actualTriggersNames.contains(name));
    }
}


void
PsqlToolsTest::viewsListTest()
{
    QStringList actualViewsNames;
    Tools::viewsList("vtunes", actualViewsNames);

    foreach (const QString &name, mViewsNamesList) {
        QVERIFY(actualViewsNames.contains(name));
    }
}



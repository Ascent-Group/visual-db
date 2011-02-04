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

#include <control/Config.h>
#include <control/ConfigTest.h>

using namespace Control;

void
ConfigTest::initTestCase()
{

}

void
ConfigTest::cleanupTestCase()
{

}

void
ConfigTest::languageTest()
{
    QLocale::Language minLocale = QLocale::C;
    QLocale::Language maxLocale = QLocale::Shambala;
    QLocale::Language newLocale = QLocale::Korean;

    Config cfg;

    QVERIFY(cfg.language() >= minLocale);
    QVERIFY(cfg.language() <= maxLocale);

    QCOMPARE(Config().language(), cfg.language());

    cfg.setLanguage(newLocale);

    QCOMPARE(cfg.language(), newLocale);
    QCOMPARE(Config().language(), cfg.language());
}

void
ConfigTest::sessionDirTest()
{
    QString newSessionDir("/home/user/.sessions");

    Config cfg;

    QCOMPARE(Config().sessionDir(), cfg.sessionDir());

    cfg.setSessionDir(newSessionDir);

    QCOMPARE(cfg.sessionDir(), newSessionDir);
    QCOMPARE(Config().sessionDir(), cfg.sessionDir());
}

void
ConfigTest::newTabAutoSwitchTest()
{
    Config cfg;

    bool flag = !cfg.newTabAutoSwitch();

    QCOMPARE(Config().newTabAutoSwitch(), cfg.newTabAutoSwitch());

    cfg.setNewTabAutoSwitch(flag);

    QCOMPARE(cfg.newTabAutoSwitch(), flag);
    QCOMPARE(Config().newTabAutoSwitch(), cfg.newTabAutoSwitch());
}

void
ConfigTest::loadLastSessionTest()
{
    Config cfg;

    bool flag = !cfg.loadLastSession();

    QCOMPARE(Config().loadLastSession(), cfg.loadLastSession());

    cfg.setLoadLastSession(flag);

    QCOMPARE(cfg.loadLastSession(), flag);
    QCOMPARE(Config().loadLastSession(), cfg.loadLastSession());
}

void
ConfigTest::savedSessionsNumberTest()
{
    Config cfg;

    // \note * 2 is not a magic number, we just need a new value
    quint32 num = cfg.savedSessionsNumber() * 2;

    QCOMPARE(Config().savedSessionsNumber(), cfg.savedSessionsNumber());

    cfg.setSavedSessionsNumber(num);

    QCOMPARE(cfg.savedSessionsNumber(), num);
    QCOMPARE(Config().savedSessionsNumber(), cfg.savedSessionsNumber());
}

void
ConfigTest::showIndicesTest()
{
    Config cfg;

    bool flag = !cfg.showIndices();

    QCOMPARE(Config().showIndices(), cfg.showIndices());

    cfg.setShowIndices(flag);

    QCOMPARE(cfg.showIndices(), flag);
    QCOMPARE(Config().showIndices(), cfg.showIndices());
}

void
ConfigTest::backgroundColorTest()
{
    Config cfg;

    QColor color("#0000ff");

    QCOMPARE(Config().backgroundColor(), cfg.backgroundColor());

    cfg.setBackgroundColor(color);

    QCOMPARE(cfg.backgroundColor(), color);
    QCOMPARE(Config().backgroundColor(), cfg.backgroundColor());
}

void
ConfigTest::tableColorTest()
{
    Config cfg;

    QColor color("#00ff00");

    QCOMPARE(Config().tableColor(), cfg.tableColor());

    cfg.setTableColor(color);

    QCOMPARE(cfg.tableColor(), color);
    QCOMPARE(Config().tableColor(), cfg.tableColor());
}

void
ConfigTest::fontColorTest()
{
    Config cfg;

    QColor color("#ff0000");

    QCOMPARE(Config().fontColor(), cfg.fontColor());

    cfg.setFontColor(color);

    QCOMPARE(cfg.fontColor(), color);
    QCOMPARE(Config().fontColor(), cfg.fontColor());
}

void
ConfigTest::arrowColorTest()
{
    Config cfg;

    QColor color("#ff00ff");

    QCOMPARE(Config().arrowColor(), cfg.arrowColor());

    cfg.setArrowColor(color);

    QCOMPARE(cfg.arrowColor(), color);
    QCOMPARE(Config().arrowColor(), cfg.arrowColor());
}

void
ConfigTest::borderColorTest()
{
    Config cfg;

    QColor color("#ff00ff");

    QCOMPARE(Config().borderColor(), cfg.borderColor());

    cfg.setBorderColor(color);

    QCOMPARE(cfg.borderColor(), color);
    QCOMPARE(Config().borderColor(), cfg.borderColor());
}

void
ConfigTest::alignToGridTest()
{
    Config cfg;

    bool flag = !cfg.alignToGrid();

    QCOMPARE(Config().alignToGrid(), cfg.alignToGrid());

    cfg.setAlignToGrid(flag);

    QCOMPARE(cfg.alignToGrid(), flag);
    QCOMPARE(Config().alignToGrid(), cfg.alignToGrid());
}

void
ConfigTest::showGridTest()
{
    Config cfg;

    bool flag = !cfg.showGrid();

    QCOMPARE(Config().showGrid(), cfg.showGrid());

    cfg.setShowGrid(flag);

    QCOMPARE(cfg.showGrid(), flag);
    QCOMPARE(Config().showGrid(), cfg.showGrid());
}

void
ConfigTest::divideIntoPagesTest()
{
    Config cfg;

    bool flag = !cfg.divideIntoPages();

    QCOMPARE(Config().divideIntoPages(), cfg.divideIntoPages());

    cfg.setDivideIntoPages(flag);

    QCOMPARE(cfg.divideIntoPages(), flag);
    QCOMPARE(Config().divideIntoPages(), cfg.divideIntoPages());
}

void
ConfigTest::savedSessionTest()
{
    Config cfg;

    cfg.setSavedSessionsNumber(0);
    cfg.setSavedSessionsNumber(10);

    quint32 sessionsCount = cfg.savedSessionsNumber();

    QVERIFY(0 < sessionsCount);

    // test plain setting with explicit index specification
    for (qint32 i = 0; i < sessionsCount; ++i) {
        QString sessionFilePath(QString("/some/path/session_%1.vdb").arg(i));

        QCOMPARE(cfg.setSavedSession(sessionFilePath, i), i);

        QCOMPARE(cfg.savedSession(i), sessionFilePath);
        QCOMPARE(Config().divideIntoPages(), cfg.divideIntoPages());
    }

    // get a session for a wrong index
    QCOMPARE(cfg.savedSession(sessionsCount), QString(""));

    // erase sessions
    for (qint32 i = 0; i < sessionsCount; ++i) {
        QCOMPARE(cfg.setSavedSession("", i), i);
    }

    qint32 id;
    // save session without speifying its index, it should take the very first place
    id = 0;
    QCOMPARE(cfg.setSavedSession("session1"), id);
    QCOMPARE(cfg.savedSession(id), QString("session1"));

    // save session with a specific index, e.g. 3, check it has taken right place
    id = 2;
    QCOMPARE(cfg.setSavedSession("session2", id), id);
    QCOMPARE(cfg.savedSession(id), QString("session2"));

    // save session without speifying its index, it should take the second place
    id = 1;
    QCOMPARE(cfg.setSavedSession("session3"), id);
    QCOMPARE(cfg.savedSession(id), QString("session3"));

    // save session with specifying an index greater than the savedSessionsNumber, set should fail
    id = sessionsCount;
    QCOMPARE(cfg.setSavedSession("sessionN", id), -1);
    QCOMPARE(cfg.savedSession(id), QString(""));

    // erase sessions
    for (qint32 i = 0; i < sessionsCount; ++i) {
        QCOMPARE(cfg.setSavedSession("", i), i);
    }

    // fill all sessions
    for (qint32 i = 0; i < sessionsCount; ++i) {
        QString sessionFilePath(QString("/some/path/session_%1.vdb").arg(i));

        QCOMPARE(cfg.setSavedSession(sessionFilePath, i), i);

        QCOMPARE(cfg.savedSession(i), sessionFilePath);
        QCOMPARE(Config().divideIntoPages(), cfg.divideIntoPages());
    }

    // try to save a session without a specific index, this should fail
    QCOMPARE(cfg.setSavedSession("sessionN"), -1);


    // explicit overriding of a saved session
    QCOMPARE(cfg.setSavedSession("OverridingSession.vdb", 0), 0);
    QCOMPARE(cfg.savedSession(0), QString("OverridingSession.vdb"));
}


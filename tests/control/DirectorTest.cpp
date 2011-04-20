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

#include <control/Context.h>
#include <control/Director.h>
#include <control/DirectorTest.h>

#include <QLabel>

using namespace Control;

void
DirectorTest::initTestCase()
{
}

void
DirectorTest::init()
{
    mDirector = new Control::Director();

    Q_CHECK_PTR(mDirector);

    mDirector->initialize();
}

void
DirectorTest::cleanup()
{
    delete mDirector;
    mDirector = 0;
}

void
DirectorTest::cleanupTestCase()
{

}

void
DirectorTest::addTest()
{
    int count = 0;
    QCOMPARE(mDirector->mRegistry.size(), count);

    QLabel *label1 = new QLabel();
    QLabel *label2 = new QLabel();
    Context *ctx = new Context();

//    mDirector->add(label1, 0);
//    QCOMPARE(mDirector->mRegistry.size(), 0);

//    mDirector->add(0, ctx);
//    QCOMPARE(mDirector->mRegistry.size(), 0);

    mDirector->add(label1, ctx);
    QCOMPARE(mDirector->mRegistry.size(), ++count);

    mDirector->add(label1, ctx);
    QCOMPARE(mDirector->mRegistry.size(), count);

    mDirector->add(label2, ctx);
    QCOMPARE(mDirector->mRegistry.size(), ++count);
}

void
DirectorTest::removeWidgetTest()
{
    int count = 0;
    QCOMPARE(mDirector->mRegistry.size(), count);

    QLabel *label1 = new QLabel();
    QLabel *label2 = new QLabel();
    QLabel *label3 = 0;
    Context *ctx = new Context();

    mDirector->add(label1, ctx);
    QCOMPARE(mDirector->mRegistry.size(), ++count);

    mDirector->add(label2, ctx);
    QCOMPARE(mDirector->mRegistry.size(), ++count);

    mDirector->remove(label3);
    QCOMPARE(mDirector->mRegistry.size(), count);

    mDirector->remove(label1);
    QCOMPARE(mDirector->mRegistry.size(), --count);

    mDirector->remove(label1);
    QCOMPARE(mDirector->mRegistry.size(), count);
}

void
DirectorTest::removeContextTest()
{
    int count = 0;
    QCOMPARE(mDirector->mRegistry.size(), count);

    QLabel *label1 = new QLabel();
    QLabel *label2 = new QLabel();
    Context *ctx = new Context();
    Context *uninitializedCtx = 0;

    mDirector->add(label1, ctx);
    QCOMPARE(mDirector->mRegistry.size(), ++count);

    mDirector->add(label2, ctx);
    QCOMPARE(mDirector->mRegistry.size(), ++count);

    mDirector->remove(uninitializedCtx);
    QCOMPARE(mDirector->mRegistry.size(), count);

    mDirector->remove(ctx);
    QCOMPARE(mDirector->mRegistry.size(), 0);
}

void
DirectorTest::findContextTest()
{
    QVERIFY(0);
}

void
DirectorTest::findWidgetsTest()
{
    QVERIFY(0);
}

void
DirectorTest::findTreeTest()
{
    QVERIFY(0);
}

void
DirectorTest::findSceneTest()
{
    QVERIFY(0);
}

void
DirectorTest::showConnectionDialogTest()
{
    QVERIFY(0);
}

void
DirectorTest::disconnectRequestedTest()
{
    QVERIFY(0);
}


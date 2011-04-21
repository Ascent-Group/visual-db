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
#include <gui/TreeWidget.h>
#include <gui/SceneWidget.h>

#include <QLabel>

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
    using namespace Control;
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
    using namespace Control;

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

//    mDirector->remove(label1);
//    QCOMPARE(mDirector->mRegistry.size(), count);
}

void
DirectorTest::removeContextTest()
{
    using namespace Control;

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
    using namespace Control;

    int count = 0;
    QCOMPARE(mDirector->mRegistry.size(), count);

    Context *ctx1 = new Context();
    Context *ctx2 = new Context();

    QLabel *label1 = new QLabel();
    QLabel *label2 = new QLabel();
    QLabel *label3 = 0;
    QLabel *label4 = new QLabel();

    QCOMPARE(mDirector->add(label1, ctx1), true);
    QCOMPARE(mDirector->mRegistry.size(), ++count);

    QCOMPARE(mDirector->add(label2, ctx2), true);
    QCOMPARE(mDirector->mRegistry.size(), ++count);

    QCOMPARE(mDirector->findContext(label1), ctx1);
    QCOMPARE(mDirector->findContext(label2), ctx2);
    QCOMPARE(mDirector->findContext(label3), (Context *)0);
    QCOMPARE(mDirector->findContext(label4), (Context *)0);
}

void
DirectorTest::findWidgetsTest()
{
    using namespace Control;

    QCOMPARE(mDirector->mRegistry.size(), 0);

    Context *ctx1 = new Context();
    Context *ctx2 = new Context();

    QLabel *label1 = new QLabel();
    QLabel *label2 = new QLabel();
    QLabel *label3 = new QLabel();
    QLabel *label4 = new QLabel();

    QCOMPARE(mDirector->add(label1, ctx1), true);
    QCOMPARE(mDirector->add(label2, ctx1), true);
    QCOMPARE(mDirector->add(label3, ctx1), true);
    QCOMPARE(mDirector->add(label4, ctx2), true);

    QVector<QWidget*> widgets;

    QCOMPARE(mDirector->findWidgets(ctx1, widgets), (quint32)3);
    QCOMPARE(widgets.size(), 3);
    QVERIFY(widgets.contains(label1));
    QVERIFY(widgets.contains(label2));
    QVERIFY(widgets.contains(label3));

    widgets.clear();
    QCOMPARE(mDirector->findWidgets(ctx2, widgets), (quint32)1);
    QCOMPARE(widgets.size(), 1);
    QVERIFY(widgets.contains(label4));
}

void
DirectorTest::findTreeTest()
{
    using namespace Control;
    using namespace Gui;

    Context *ctx = new Context();

    QLabel *label1 = new QLabel();
    QLabel *label2 = new QLabel();
    TreeWidget *tree = new TreeWidget();
    QLabel *label4 = new QLabel();

    QCOMPARE(mDirector->add(label1, ctx), true);
    QCOMPARE(mDirector->add(label2, ctx), true);
    QCOMPARE(mDirector->add(tree, ctx), true);
    QCOMPARE(mDirector->add(label4, ctx), true);

    QCOMPARE(mDirector->findTree(ctx), tree);
}

void
DirectorTest::findSceneTest()
{
    using namespace Control;
    using namespace Gui;

    Context *ctx = new Context();

    QLabel *label1 = new QLabel();
    QLabel *label2 = new QLabel();
    SceneWidget *scene = new SceneWidget();
    QLabel *label4 = new QLabel();

    QCOMPARE(mDirector->add(label1, ctx), true);
    QCOMPARE(mDirector->add(label2, ctx), true);
    QCOMPARE(mDirector->add(scene, ctx), true);
    QCOMPARE(mDirector->add(label4, ctx), true);

    QCOMPARE(mDirector->findScene(ctx), scene);
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


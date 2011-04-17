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

#include <gui/strategy/EdgeTest.h>
#include <gui/strategy/Edge.h>
#include <gui/strategy/Node.h>

void
EdgeTest::initTestCase()
{
}

void
EdgeTest::cleanupTestCase()
{
}

void
EdgeTest::endTest()
{
    Node start(1);
    Node end(2);
    Edge edge(start, end);
    QVERIFY(&start == &edge.start());
}

void
EdgeTest::setWeightTest()
{
    Node start(1);
    Node end(2);
    Edge edge1(start, end);
    QVERIFY(0 == edge1.weight());

    edge1.setWeight(1);
    QVERIFY(1 == edge1.weight());

    Edge edge2(start, end, 2);
    QVERIFY(2 == edge2.weight());
}

void
EdgeTest::startTest()
{
    Node start(1);
    Node end(2);
    Edge edge(start, end);
    QVERIFY(&end == &edge.end());
}

void
EdgeTest::weightTest()
{
    setWeightTest();
}


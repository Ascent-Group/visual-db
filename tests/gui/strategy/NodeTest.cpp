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

#include <QtDebug>
#include <gui/strategy/NodeTest.h>
#include <gui/strategy/Node.h>
#include <gui/strategy/Edge.h>
#include <gui/strategy/Graph.h>

void
NodeTest::initTestCase()
{
}

void
NodeTest::cleanupTestCase()
{
}

void
NodeTest::idTest()
{
    Node node(1);
    QVERIFY(1 == node.id());
}

void
NodeTest::labelTest()
{
    Node node(1);
    QVERIFY(0 == node.label());

    node.setLabel(3);
    QVERIFY(3 == node.label());
}

void
NodeTest::setLabelTest()
{
    Node node(1);
    node.setLabel(3);
    QVERIFY(3 == node.label());
}

void
NodeTest::maxTest()
{
    Node node10(10);
    Node node1(1);
    Node node2(2);
    Node node7(7);
   
    QVERIFY(0 == node1.max());

    Edge edge1_10(node1, node10);
    Edge edge2_10(node2, node10);
    Edge edge7_10(node7, node10);

    node10.mInEdgeSet.insert(edge1_10.key(), &edge1_10);
    node10.mInEdgeSet.insert(edge7_10.key(), &edge7_10);
    node10.mInEdgeSet.insert(edge2_10.key(), &edge2_10);

    QVERIFY(0 != node10.max());
    QVERIFY(&edge7_10 == node10.max());
}

void
NodeTest::medianTest()
{
    Graph graph;
    graph.addEdge(1, 2);
    graph.addEdge(1, 3);

    QVERIFY(0 == graph.node(1)->median());

    graph.addEdge(1, 4);

    QVERIFY(1 == graph.node(1)->median());
}

void
NodeTest::moveTest()
{
    Node node(1);
    QVERIFY(0 == node.mX && 0 == node.mY);

    node.move(2, 3);
    QVERIFY(2 == node.mX && 3 == node.mY);
}

void
NodeTest::operatorLessTest()
{
    Node node9(9);

    Node node10(10);
    Node node1(1);
    Node node2(2);
    Node node7(7);
    
    Edge edge1_10(node1, node10);
    Edge edge2_10(node2, node10);
    Edge edge7_10(node7, node10);

    node10.mInEdgeSet.insert(edge1_10.key(), &edge1_10);
    node10.mInEdgeSet.insert(edge7_10.key(), &edge7_10);
    node10.mInEdgeSet.insert(edge2_10.key(), &edge2_10);

    // {} < {1,2,7}
    QVERIFY(node9 < node10);
    QVERIFY(!(node10 < node9));

    Node node11(11);
    Node node3(3);
    Node node8(8);

    Edge edge3_11(node3, node11);
    Edge edge8_11(node8, node11);
    
    node11.mInEdgeSet.insert(edge3_11.key(), &edge3_11);
    node11.mInEdgeSet.insert(edge8_11.key(), &edge8_11);

    // {1,2,7} < {3,8}
    QVERIFY(node10 < node11);
    QVERIFY(!(node11 < node10));

    Node node12(12);

    Edge edge1_12(node1, node12);
    Edge edge3_12(node3, node12);
    Edge edge7_12(node7, node12);

    node12.mInEdgeSet.insert(edge1_12.key(), &edge1_12);
    node12.mInEdgeSet.insert(edge3_12.key(), &edge3_12);
    node12.mInEdgeSet.insert(edge7_12.key(), &edge7_12);

    // {1,2,7} < {1,3,7}
    QVERIFY(node10 < node12);
    QVERIFY(!(node12 < node10));
}

void
NodeTest::operatorEqualTest()
{
    Node node1(1);
    Node node2(1);

    QVERIFY(node1 == node2);

    Node node3(3);
    Node node4(4);

    QVERIFY(!(node3 == node4));
}

void
NodeTest::lessThanLexicorgraphicalTest()
{
    operatorLessTest();
}

void
NodeTest::lessThanMedianTest()
{
    // null pointers
    QVERIFY(!lessThanMedian(0, 0));

    Graph graph;
    graph.addEdge(1, 2);
    graph.addEdge(1, 3);
    graph.addEdge(5, 2);
    graph.addEdge(5, 3);
    graph.addEdge(5, 4);

    // check node1 < node5 in with respect to medians
    QVERIFY(lessThanMedian(graph.node(1), graph.node(5)));
}

void
NodeTest::lessThanOutMinusInEdgesTest()
{
    // null pointers
    QVERIFY(!lessThanOutMinusInEdges(0, 0));

    Graph graph;
    graph.addEdge(1, 2);
    graph.addEdge(1, 3);
    graph.addEdge(2, 4);
    graph.addEdge(3, 4);

    // node2 < node1
    QVERIFY(lessThanOutMinusInEdges(graph.node(2), graph.node(1)));
    // node3 < node1
    QVERIFY(lessThanOutMinusInEdges(graph.node(3), graph.node(1)));
    // node4 < node3
    QVERIFY(lessThanOutMinusInEdges(graph.node(4), graph.node(3)));
}

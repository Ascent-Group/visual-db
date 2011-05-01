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

#include <gui/strategy/GraphTest.h>
#include <gui/strategy/Graph.h>
#include <gui/strategy/Node.h>
#include <gui/strategy/Edge.h>

void
GraphTest::initTestCase()
{
}

void
GraphTest::cleanupTestCase()
{
}

void
GraphTest::addEdgeTest()
{
    Graph graph;
    graph.addEdge(1, 2);

    QVERIFY(1 == graph.edgeCount());

    // edges should be unique
    graph.addEdge(1, 2);

    QVERIFY(1 == graph.edgeCount());
}

void
GraphTest::addNodeTest()
{
    Graph graph;
    graph.addNode(1);
    
    QVERIFY(1 == graph.nodeCount());

    graph.addNode(1);

    // nodes should be unique
    QVERIFY(1 == graph.nodeCount());
}

void
GraphTest::removeEdgeTest()
{
    Graph graph;
    graph.addEdge(1, 2);
    
    QVERIFY(1 == graph.mEdgeSet.size());

    graph.removeEdge(1, 3);
    
    // test not presnted edge removing
    QVERIFY(1 == graph.mEdgeSet.size());

    graph.removeEdge(1, 2);
    
    // test presented edge removing
    QVERIFY(0 == graph.mEdgeSet.size());
}

void
GraphTest::removeNodeTest()
{
    Graph graph;

    graph.addNode(1);

    QVERIFY(1 == graph.nodeCount());

    graph.removeNode(2);

    // test not presented node removing
    QVERIFY(1 == graph.nodeCount());

    graph.removeNode(1);

    // test presented node removing
    QVERIFY(0 == graph.nodeCount());
}

void
GraphTest::prepareForDrawingTest()
{
    QVERIFY(0);
}

void
GraphTest::cycleRemovalTest()
{
    Graph graph;
    graph.addEdge(1, 2);
    graph.addEdge(1, 4);
    graph.addEdge(2, 3);
    graph.addEdge(2, 5);
    graph.addEdge(3, 6);
    graph.addEdge(4, 5);
    graph.addEdge(5, 8);
    graph.addEdge(6, 5);
    graph.addEdge(6, 9);
    graph.addEdge(7, 4);
    graph.addEdge(8, 7);
    graph.addEdge(8, 9);

    graph.cycleRemoval();

    QVERIFY(2 == graph.mFeedbackArcSet.size());
}

void
GraphTest::removeTwoCyclesTest()
{
    Graph graph;
    graph.addEdge(1, 2);
    graph.addEdge(2, 1);

    QVERIFY(2 == graph.edgeCount() && 0 == graph.mRemovedEdges.size());
    
    graph.removeTwoCycles();

    QVERIFY(1 == graph.edgeCount() && 1 == graph.mRemovedEdges.size());
}

void
GraphTest::coffmanGrahamTest()
{
    Graph graph;
    graph.addNode(2);
    graph.addEdge(1, 3);
    graph.addEdge(1, 4);
    graph.addEdge(3, 10);
    graph.addEdge(3, 5);
    graph.addEdge(3, 8);
    graph.addEdge(4, 6);
    graph.addEdge(4, 7);
    graph.addEdge(4, 8);
    graph.addEdge(4, 9);
    graph.addEdge(5, 9);
    graph.addEdge(7, 11);
    graph.addEdge(8, 10);
    graph.addEdge(9, 12);
    graph.addEdge(10, 11);
    graph.addEdge(10, 12);
    graph.addEdge(11, 13);
    graph.addEdge(12, 13);

    graph.coffmanGraham(3);

    QVERIFY(0 == graph.node(13)->level());
    QVERIFY(1 == graph.node(12)->level());
    QVERIFY(1 == graph.node(11)->level());
    QVERIFY(2 == graph.node(10)->level());
    QVERIFY(2 == graph.node(9)->level());
    QVERIFY(3 == graph.node(8)->level());
    QVERIFY(2 == graph.node(7)->level());
    QVERIFY(0 == graph.node(6)->level());
    QVERIFY(3 == graph.node(5)->level());
    QVERIFY(4 == graph.node(4)->level());
    QVERIFY(4 == graph.node(3)->level());
    QVERIFY(0 == graph.node(2)->level());
    QVERIFY(5 == graph.node(1)->level());
}

void
GraphTest::crossingReductionTest()
{
    Graph graph;
    graph.crossingReduction();

    foreach (QList<Node *> *level, graph.mLevels) {
        foreach (Node *node, *level) {
            qDebug() << node->level() << " : " << node->id();
        }
    }
}

void
GraphTest::horizontalCoordinatsAssignmentTest()
{
    QVERIFY(0);
}

void
GraphTest::restoreTest()
{
    QVERIFY(0);
}

void
GraphTest::selectNodeTest()
{
    Node node1(1);
    Node node2(2);
    Node node3(3);
    Node node4(4);
    Node node5(5);
    Node node6(6);
    Node node7(7);
    Node node8(8);
    Node node9(9);

    node1.setLabel(10);
    node2.setLabel(10);
    node3.setLabel(10);
    node4.setLabel(10);
    node5.setLabel(10);
    node6.setLabel(10);
    node7.setLabel(10);
    node8.setLabel(10);
    node9.setLabel(10);

    Edge edge1_4(node1, node4);
    Edge edge2_4(node2, node4);
    Edge edge3_4(node3, node4);

    Edge edge5_9(node5, node9);
    Edge edge6_9(node6, node9);
    Edge edge7_9(node7, node9);
    Edge edge8_9(node8, node9);

    QList<Node *>U;
    U.append(&node1);
    U.append(&node2);
    U.append(&node3);
    U.append(&node4);

    QList<Node *>V;
    V.append(&node1);
    V.append(&node2);
    V.append(&node3);
    V.append(&node4);
    V.append(&node5);
    V.append(&node6);
    V.append(&node7);
    V.append(&node8);
    V.append(&node9);

    Graph graph;
//    Node *node = graph.selectNode(U, V);
    QVERIFY(0);
}


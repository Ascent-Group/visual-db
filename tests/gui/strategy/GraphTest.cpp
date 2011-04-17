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
    Node node1(1);
    Node node2(2);
    Edge edge1_2(node1, node2);
    Graph graph;
    graph.addEdge(edge1_2);

    QVERIFY(&edge1_2 == *(graph.mEdgeSet.begin()));
    QVERIFY(1 == graph.mEdgeSet.size());
}

void
GraphTest::addNodeTest()
{
    Node node1(1);
    Graph graph;

    graph.addNode(node1);
    QVERIFY(1 == graph.mNodeSet.size());
}

void
GraphTest::coffmanGrahamTest()
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
    Node node10(10);
    Node node11(11);
    Node node12(12);
    Node node13(13);

    Edge edge1_3(node1, node3);
    Edge edge1_4(node1, node4);
    Edge edge3_5(node3, node5);
    Edge edge3_8(node3, node8);
    Edge edge3_10(node3, node10);
    Edge edge4_6(node4, node6);
    Edge edge4_7(node4, node7);
    Edge edge4_8(node4, node8);
    Edge edge4_9(node4, node9);
    Edge edge5_9(node5, node9);
    Edge edge7_11(node7, node11);
    Edge edge8_10(node8, node10);
    Edge edge9_12(node9, node12);
    Edge edge10_11(node10, node11);
    Edge edge10_12(node10, node12);
    Edge edge11_13(node11, node13);
    Edge edge12_13(node12, node13);

    Graph graph;

    graph.addNode(node1);
    graph.addNode(node2);
    graph.addNode(node3);
    graph.addNode(node4);
    graph.addNode(node5);
    graph.addNode(node6);
    graph.addNode(node7);
    graph.addNode(node8);
    graph.addNode(node9);
    graph.addNode(node10);
    graph.addNode(node11);
    graph.addNode(node12);
    graph.addNode(node13);

    graph.addEdge(edge1_3);
    graph.addEdge(edge1_4);
    graph.addEdge(edge3_10);
    graph.addEdge(edge3_5);
    graph.addEdge(edge3_8);
    graph.addEdge(edge4_6);
    graph.addEdge(edge4_7);
    graph.addEdge(edge4_8);
    graph.addEdge(edge4_9);
    graph.addEdge(edge5_9);
    graph.addEdge(edge7_11);
    graph.addEdge(edge8_10);
    graph.addEdge(edge9_12);
    graph.addEdge(edge10_11);
    graph.addEdge(edge10_12);
    graph.addEdge(edge11_13);
    graph.addEdge(edge12_13);

//    QVERIFY(!(node1 < node2));
//    QVERIFY(node2 < node3);
//    QVERIFY(!(node3 < node4));
//    QVERIFY(node4 < node5);
//    QVERIFY(node5 < node6);
//    QVERIFY(!(node6 < node7));
//    QVERIFY(node7 < node8);
//    QVERIFY(node8 < node9);
//    QVERIFY(node9 < node10);
//    QVERIFY(node10 < node11);
//    QVERIFY(node11 < node12);
//    QVERIFY(node12 < node13);

    graph.coffmanGraham(3);
    
    foreach (Node *node, graph.mNodeSet) {
        qDebug() << node->id() << node->label();
    }

    QVERIFY(0);
}

void
GraphTest::removeEdgeTest()
{
    Node node1(1);
    Node node2(2);
    Node node3(3);
    
    Edge edge1_2(node1, node2);
    Edge edge1_3(node1, node3);
    
    Graph graph;
    graph.addEdge(edge1_2);
    QVERIFY(1 == graph.mEdgeSet.size());

    graph.removeEdge(edge1_3);
    QVERIFY(1 == graph.mEdgeSet.size());

    graph.removeEdge(edge1_2);
    QVERIFY(0 == graph.mEdgeSet.size());
}

void
GraphTest::removeNodeTest()
{
    Node node1(1);
    Node node2(2);
    Graph graph;

    graph.addNode(node1);
    QVERIFY(1 == graph.mNodeSet.size());

    graph.removeNode(node2);
    QVERIFY(1 == graph.mNodeSet.size());

    graph.removeNode(node1);
    QVERIFY(0 == graph.mNodeSet.size());
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


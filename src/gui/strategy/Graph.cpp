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
#include <gui/strategy/Graph.h>

Graph::Graph()
    : mNodeSet(), mEdgeSet()
{
}

Graph::~Graph()
{
}

void
Graph::addEdge(Edge &iEdge)
{
    mEdgeSet.append(&iEdge);
    iEdge.start().mOutEdgeSet.insert(&iEdge);
    iEdge.end().mInEdgeSet.insert(&iEdge);
}

void
Graph::addNode(Node &iNode)
{
    mNodeSet.append(&iNode);
}

void
Graph::removeEdge(Edge &iEdge)
{
    iEdge.start().mOutEdgeSet.remove(&iEdge);
    iEdge.end().mInEdgeSet.remove(&iEdge);
    mEdgeSet.removeOne(&iEdge);
}

void
Graph::removeNode(Node &iNode)
{
    foreach (Edge *edge, iNode.mInEdgeSet) {
        removeEdge(*edge);
    }
    foreach (Edge *edge, iNode.mOutEdgeSet) {
        removeEdge(*edge);
    }

    mNodeSet.removeOne(&iNode);
}

void
Graph::draw()
{
    prepareForDrawing();
    cycleRemoval();
    coffmanGraham(3);
    crossingReduction();
    horizontalCoordinatsAssignment();
    restore();
}

void
Graph::prepareForDrawing()
{
    mLevels.clear();
    mRemovedEdges.clear();
    mFeedbackArcSet.clear();
}

void
Graph::cycleRemoval()
{
    removeTwoCycles();

    QList<Node *> sequenceLeft;
    QList<Node *> sequenceRight;
    Graph graph(*this);

    foreach (Node *node, graph.mNodeSet) {
        if (0 == node->mOutEdgeSet.size()) {
            sequenceRight.append(node);
            graph.removeNode(*node);
        }
    }

    foreach (Node *node, graph.mNodeSet) {
        if (0 == node->mInEdgeSet.size()) {
            sequenceLeft.append(node);
            graph.removeNode(*node);
        }
    }

    if (graph.mNodeSet.size() > 0) {
        for (qint32 i = graph.mNodeSet.size() - 1; i >= 0; --i) {
            Node *node = graph.maxOutMinusInDegree();
            sequenceLeft.append(node);
            graph.removeNode(*node);
        }
    }

    sequenceLeft.append(sequenceRight);
    
    foreach (Node *node, sequenceLeft) {
        qDebug() << "Node: " << node->id() << " out: " << node->mOutEdgeSet.size() << " in: " << node->mInEdgeSet.size();
        foreach (Edge *edge, node->mOutEdgeSet) {
            if (sequenceLeft.indexOf(&edge->end()) < sequenceLeft.indexOf(node)) {
                qDebug() << "Edge: " << node->id() << ":" << edge->end().id();
                edge->revert();
                mFeedbackArcSet.append(edge);
            }
        }
    }
}

Node *
Graph::maxOutMinusInDegree()
{
    if (mNodeSet.size() <= 0)
        return 0;

    Node *max = mNodeSet.at(0);

    foreach (Node *node, mNodeSet) {
//        qDebug() << node->id() << " out - in: " << node->mOutEdgeSet.size() - node->mInEdgeSet.size();
        if (node->mOutEdgeSet.size() - node->mInEdgeSet.size() > 
                max->mOutEdgeSet.size() - max->mInEdgeSet.size()) {
            max = node;
        }
    }

    return max;
}

void Graph::removeTwoCycles()
{
    for (qint32 i = 0; i < mEdgeSet.size() - 1; ++i) {
        for (qint32 j = i + 1; j < mEdgeSet.size(); ++j) {
            if (mEdgeSet.at(i)->start() == mEdgeSet.at(j)->end() && 
                    mEdgeSet.at(i)->end() == mEdgeSet.at(j)->start()) {
                mRemovedEdges.append(mEdgeSet.at(i));
            }
        }
    }

    foreach (Edge *edge, mRemovedEdges) {
        mEdgeSet.removeOne(edge);
    }
}

void
Graph::coffmanGraham(quint32 iWidth)
{
    // initialize all nodes
    foreach (Node *node, mNodeSet) {
        node->setLabel(mNodeSet.size() + 1);
    }

    // The first phase of the algorithm orders the virtices by assigning a 
    // positive integer label to each vertex. First label 1 is assigned to a source u, 
    // so that label(u) = 1. After labels 1,2,...,k-1 have been assigned to vertices, 
    // label k is assigned to a vertex v, such that:
    //
    // (1) No label has been assigned to v yet.
    // (2) Labels have been assigned to all vertices u for which (u,v) is in E.
    // (3) Among vertices satisfying (1) and (2) abouve, the set of labels of immediate
    //     predecessors of v (that is , {label(u) : (u,v) is in E}) is minimized it the 
    //     lexicographic ordering "<".
    //
    // Lexicographic ordering: let S is a finite set of positive integers, then let mas(S) 
    // denote the largest element of S. Then S<T if eigher:
    //
    //  - S=0 and T!=0, or
    //  - S!=0, T!=0 and max(S) < max(T), or
    //  - S!=0, T!=0, max(S) = max(T), and S-{max(S)} < T-{max(T}).

    QList<Node *> allNodes = mNodeSet;
    qSort(allNodes.begin(), allNodes.end(), lessThanLexicorgraphical);
    
    quint32 i = 1;
    foreach (Node *node, allNodes) {
        if (mNodeSet.size() + 1 == node->label()) {
            node->setLabel(i++);
        }
    }
    
    // The second phase of algorithm fills the layers with vertices,
    // ensuring that no layer receives more than W vertices. We start with
    // the bottom layer L1 and proceed to the top layer Lh. To fill Lk, we
    // choose a vertex u which has not been placed in a layer yet,
    // and for which all vertices u with (v,u) from E have been placed in one of the
    // layers L1,L2,...,Lk-1. If there is more than one such vertex, 
    // then we choose the one with the largest label. If there are no such vertices 
    // or layher Lk bocomes full (|Lk|=W), then we proceed to the next layer Lk+1.

    quint32 k = 0;
    QList<QList<Node *> *> mLevels;
    mLevels.append(new QList<Node *>());
    
    QList<Node *> unlabeledNodes;
    QList<Node *> currentLevelNodes;

    while (unlabeledNodes.size() != allNodes.size()) {
        // We choose u from unlabeledNodes such that every vertex in {v | (u,v) from E} is in V\U
        // and label(u) is maximized
        Node *u = selectNode(unlabeledNodes, currentLevelNodes, allNodes);
        
        bool condition = true;

        if (0 != u) {
            QList<QList<Node *> *>::const_iterator level;
            QList<Node *> levelsKminus1;
            for (level = mLevels.constBegin(); level != mLevels.constEnd() - 1; ++level) {
                levelsKminus1.append(**level);
            }

            // ...check if N+(u) is in L1 U L2 U ... U Lk-1 ...
            foreach (const Edge *edge, u->mOutEdgeSet) {
                if (!levelsKminus1.contains(&edge->end())) {
                    condition = false;
                    break;
                }
            }
        }

        // if |Lk| >= w or there is some v : (u,v) from E that is not in unlabeledNodes - create next level 
        if (0 == u || currentLevelNodes.size() >= iWidth || !condition) {
            k++;
            mLevels.append(new QList<Node *>());
            unlabeledNodes.append(currentLevelNodes);
            currentLevelNodes.clear();
        }

        if (0 != u) {
            // append node to current level
            u->setLevel(k);
            mLevels.at(k)->append(u);
//            qDebug() << "id: " << u->id() << " level: " << k;

            // removeOne node from unlabeledNodes set
            currentLevelNodes.append(u);
        }
    }
}

void
Graph::crossingReduction()
{
    for (int i = mLevels.size() - 1; i >=0; --i) {
        qSort(mLevels.at(i)->begin(), mLevels.at(i)->end(), lessThanMedian);
    }
}

void
Graph::horizontalCoordinatsAssignment()
{
    
}

void
Graph::restore()
{
    foreach (Edge *edge, mEdgeSet) {
        // this function garantees unreverting only for reverted edges
        edge->unrevert();
    }

    foreach (Edge *edge, mRemovedEdges) {
        mEdgeSet.append(edge);
    }
}

/*!
 * We choose u from unlabeledNodes such that every vertex in {v | (u,v) from E} is in V\U
 * and label(u) is maximized
 */
Node *
Graph::selectNode(const QList<Node *> &iAlreadyLeveledNodes, const QList<Node *> &iCurrentLevelNodes, const QList<Node *> &iAllNodes)
{
    // iAlreadyLeveledNodes should be already sorted
    
    // construct unleveledNodes set
    QList<Node *> unleveledNodes = iAllNodes;
    foreach (Node *node, iAlreadyLeveledNodes) {
        unleveledNodes.removeOne(node);
    }
    foreach (Node *node, iCurrentLevelNodes) {
        unleveledNodes.removeOne(node);
    }
    
    qint32 i = unleveledNodes.size() - 1;

    // so we start from the last element and move to the first
    while (i >= 0) {

        // check that each vertex in {v | (u,v) from E} is in AlreadyLeveledNodes
        bool condition = true;
        foreach (const Edge *edge, unleveledNodes.at(i)->mOutEdgeSet) {
            if (!iAlreadyLeveledNodes.contains(&edge->end())) {
                condition = false;
                break;
            }
        }
        
        // if yes - return found node
        if (condition) {
            return unleveledNodes.at(i);
        }

        // else try next node
        i--;
    }

    return 0;
}


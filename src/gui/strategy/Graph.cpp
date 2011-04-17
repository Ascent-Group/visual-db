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
    mEdgeSet.insert(&iEdge);
    iEdge.start().mOutEdgeSet.insert(&iEdge);
    iEdge.end().mInEdgeSet.insert(&iEdge);
}

void
Graph::addNode(Node &iNode)
{
    mNodeSet.insert(&iNode);
}

void
Graph::removeEdge(Edge &iEdge)
{
    mEdgeSet.remove(&iEdge);    
}

void
Graph::removeNode(Node &iNode)
{
    mNodeSet.remove(&iNode);
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

    QList<Node *> nodeList = mNodeSet.toList();
    quicksort(nodeList, 0, nodeList.size() - 1);
    
    quint32 i = 1;
    foreach (Node *node, nodeList) {
        if (mNodeSet.size() + 1 == node->label()) {
            node->setLabel(i++);
            qDebug() << node->id() << ":" << node->label();
        }
    }
    return;
    
    // The second phase of algorithm fills the layers with vertices,
    // ensuring that no layer receives more than W vertices. We start with
    // the bottom layer L1 and proceed to the top layer Lh. To fill Lk, we
    // choose a vertex u which has not been placed in a layer yet,
    // and for which all vertices u with (v,u) from E have been placed in one of the
    // layers L1,L2,...,Lk-1. If there is more than one such vertex, 
    // then we choose the one with the largest label. If there are no such vertices 
    // or layher Lk bocomes full (|Lk|=W), then we proceed to the next layer Lk+1.

    quint32 k = 0;
    QList<QSet<Node *> *> levels;
    levels.append(new QSet<Node *>());
    
    QList<Node *> U = nodeList;

    while (!U.empty()) {
        // We choose u from U such that every vertex in {v | (u,v) from E} is in V\U
        // and label(u) is maximized
        Node *u = selectNode(U, nodeList);
        if (u == 0) {
            qDebug() << "null";
        }
//        Q_ASSERT(u);

        qDebug() << U.size();
        if (U.size() <= 5)
            abort();
        // if |Lk| < w... 
        if (levels.at(k)->size() < iWidth) {
            if (levels.size() > 1) {
                QList<QSet<Node *> *>::const_iterator level;
                QList<Node *> levelsKminus1;
                for (level = levels.constBegin(); level < levels.constEnd() - 1; ++level) {
                    levelsKminus1.append((*level)->toList());
                }

                // ...check if N+(u) is in L1 U L2 U ... U Lk-1 ...
                bool condition = true;
                foreach (const Edge *edge, u->mOutEdgeSet) {
                    if (!levelsKminus1.contains(&edge->end())) {
                        condition = false;
                        break;
                    }
                }

                // if not - create next level
                if (!condition) {
                    k++;
                    levels.append(new QSet<Node *>());
                }
            }

            // append node to current level
            levels.at(k)->insert(u);
        }    

        // remove node from U set
        U.removeOne(u);
    }
}

/*!
 * We choose u from U such that every vertex in {v | (u,v) from E} is in V\U
 * and label(u) is maximized
 */
Node *
Graph::selectNode(const QList<Node *> U, const QList<Node *> V)
{
    Q_ASSERT(!U.empty());

    // U should be already sorted
//    QList<Node *>::const_iterator node = U.constEnd() - 1;
    qint32 i = U.size() - 1;

    // so we start from the last element and move to the first
    while (i >= 0) {
        // construct V\U set
        QList<Node *> vMinusU = V;
        foreach (Node *node, U) {
            vMinusU.removeOne(node);
        }
        qDebug() << vMinusU;

        // check that each vertex in {v | (u,v) from E} is in V\U
        bool condition = true;
        foreach (const Edge *edge, U.at(i)->mInEdgeSet) {
            if (vMinusU.contains(&edge->start())) {
                condition = false;
                break;
            }
        }
        
        // if yes - return found node
        if (condition) {
            return U.at(i);
        }

        // else try next node
        i--;
    }

    return 0;
}

void
quicksort(QList<Node *> &iNodeList, quint32 iLeft, quint32 iRight)
{
    if (iLeft < iRight) {
        quint32 pivot = partition(iNodeList, iLeft, iRight);
        quicksort(iNodeList, iLeft, pivot - 1);
        quicksort(iNodeList, pivot + 1, iRight);
    }
}

quint32
partition(QList<Node *> &iNodeList, quint32 iLeft, quint32 iRight)
{
    quint32 i = iLeft;

    for (quint32 j = iLeft + 1; j <= iRight; ++j) {
        if (*iNodeList.at(j) < *iNodeList.at(iLeft)) {
            ++i;
            swap(iNodeList, i, j);
        }
    }

    swap(iNodeList, i, iLeft);

    return i + 1;
}

void
swap(QList<Node *> &iNodeList, quint32 iI, quint32 iJ)
{
    Node *tmp = iNodeList.at(iI);
    iNodeList.insert(iI, iNodeList.at(iJ));
    iNodeList.insert(iJ, tmp);
}

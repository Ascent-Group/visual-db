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

#ifndef GRAPH_H
#define GRAPH_H

#include <gui/strategy/Node.h>
#include <gui/strategy/Edge.h>
#include <QSet>

class Graph
{
    public:
        Graph();
        ~Graph();

        void addEdge(Edge &iEdge);
        void addNode(Node &iNode);

        void removeEdge(Edge &iEdge);
        void removeNode(Node &iNode);

        void draw();

    private:
//        Graph(const Graph &iGraph);

        void prepareForDrawing();
        void cycleRemoval();
        void removeTwoCycles();
        void coffmanGraham(quint32 iWidth);
        void crossingReduction();
        void horizontalCoordinatsAssignment();
        void restore();

        Node *selectNode(const QList<Node *> &U, const QList<Node *> &iCurrentLevelNodes, const QList<Node *> &V);

    private:
        QList<Node *> mNodeSet;
        QList<Edge *> mEdgeSet;
        QList<QList<Node *> *> mLevels;
//        QList<const Edge *> mFeedbackArcSet;
        QList<Edge *> mRemovedEdges;

        friend class GraphTest;
};

#endif // GRAPH_H

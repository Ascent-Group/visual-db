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

#ifndef NODE_H
#define NODE_H

#include <QtGlobal>
#include <QSet>

class Edge;
//#include <gui/strategy/Edge.h>

class Node
{
    public:
        Node(quint32 iId);
        Node(const Node &iNode);
        ~Node();

        quint32 id() const;
        void move(quint32 iX, quint32 iY);

        void setLabel(quint32 iLabel);
        quint32 label() const;

        void setLevel(quint32 iLevel);
        quint32 level() const;

        quint32 median() const;

        bool operator<(const Node &iNode) const;
        bool operator==(const Node &iNode) const;

    private:

//        void addInEdge(const Edge &iEdge);
//        void addOutEdge(const Edge &iEdge);

        Edge *max() const;

    private:
        quint32 mId;
        quint32 mLabel;
        quint32 mLevel;
        quint32 mX;
        quint32 mY;

        QSet<Edge *> mInEdgeSet;
        QSet<Edge *> mOutEdgeSet;

        friend class Graph;
        friend class NodeTest;

        friend bool lessThanLexicorgraphical(const Node *iNode1, const Node *iNode2);
        friend bool lessThanMedian(const Node *iNode1, const Node *iNode2);
        friend bool lessThanOutMinusInEdges(const Node *iNode1, const Node *iNode2);
};

bool lessThanLexicorgraphical(const Node *iNode1, const Node *iNode2);
bool lessThanMedian(const Node *iNode1, const Node *iNode2);
bool lessThanOutMinusInEdges(const Node *iNode1, const Node *iNode2);

#endif // NODE_H

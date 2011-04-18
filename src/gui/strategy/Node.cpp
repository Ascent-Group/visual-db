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

#include <gui/strategy/Node.h>
#include <gui/strategy/Edge.h>
#include <QtDebug>

Node::Node(quint32 iId)
    : mId(iId), mLabel(0), mX(0), mY(0), mInEdgeSet(), mOutEdgeSet()
{
}

Node::Node(const Node &iNode)
    : mId(iNode.mLabel), mLabel(iNode.mLabel), mX(iNode.mX), mY(iNode.mY)
    , mInEdgeSet(iNode.mInEdgeSet), mOutEdgeSet(iNode.mOutEdgeSet)
{
}

Node::~Node()
{
}

quint32
Node::id() const
{
    return mId;
}

void 
Node::move(quint32 iX, quint32 iY)
{
    mX = iX;
    mY = iY;
}

void
Node::setLabel(quint32 iLabel)
{
    mLabel = iLabel;
}

quint32
Node::label() const
{
    return mLabel;
}

bool
Node::operator<(const Node &iNode) const
{
    if (this->mInEdgeSet.empty() && !iNode.mInEdgeSet.empty()) {
        return true;
    }

    if (!mInEdgeSet.empty() && !iNode.mInEdgeSet.empty()) {
        if (this->max()->start().id() < iNode.max()->start().id()) {
            return true;
        }
    }

    if (!mInEdgeSet.empty() && !iNode.mInEdgeSet.empty()) {
        if (this->max()->start().id() == iNode.max()->start().id()) {
            Node nodeThis = *this;
            Node nodeOther = iNode;
            
            nodeThis.mInEdgeSet.remove(nodeThis.max());
            nodeOther.mInEdgeSet.remove(nodeOther.max());

            return nodeThis < nodeOther;
        }
    }

    return false;
}

bool
lessThan(const Node *iNode1, const Node *iNode2)
{
    return *iNode1 < *iNode2;
}

const Edge *
Node::max() const
{
    Q_ASSERT(!mInEdgeSet.empty());

    QSet<const Edge *>::const_iterator iter = mInEdgeSet.begin();
    const Edge *max = *iter;

    for (iter = mInEdgeSet.begin() + 1; iter != mInEdgeSet.end(); ++iter) {
        if ((*iter)->start().id() > max->start().id()) {
            max = *iter;
        }
    }

    return max;
}

//void
//Node::addInEdge(const Edge &iEdge)
//{
//    mInEdgeSet.insert(&iEdge);
//}
//
//void
//Node::addOutEdge(const Edge &iEdge)
//{
//    mOutEdgeSet.insert(&iEdge);
//}

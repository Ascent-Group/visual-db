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

#include <gui/strategy/Edge.h>
#include <QtDebug>

Edge::Edge(Node &iStart, Node &iEnd, qint32 iWeight)
    : mStart(&iStart), mEnd(&iEnd), mWeight(iWeight), mReverted(false)
{
}

Edge::~Edge()
{
}

const QString
Edge::key() const
{
    return key(mStart->id(), mEnd->id());
}

const QString
Edge::key(quint32 iStart, quint32 iEnd)
{
    return QString::number(iStart) + "_" + QString::number(iEnd);
}

Node &
Edge::start() const
{
    return *mStart;
}

Node &
Edge::end() const
{
    return *mEnd;
}

void
Edge::setWeight(qint32 iWeight)
{
    mWeight = iWeight;
}

qint32
Edge::weight() const
{
    return mWeight;
}

void
Edge::revert()
{
    mReverted = true;
    swapNodes();
}

void
Edge::unrevert()
{
    if (mReverted) {
        mReverted = false;
        swapNodes();
    }
}

void
Edge::swapNodes()
{
    Node *tmp = mStart;
    mStart = mEnd;
    mEnd = tmp;
}

QDebug 
operator<<(QDebug ioDbg, const Edge &iEdge)
{
    ioDbg.nospace() << "Edge: [" << iEdge.start().id() << "->" << iEdge.end().id() << "]";
    return ioDbg.space();
}


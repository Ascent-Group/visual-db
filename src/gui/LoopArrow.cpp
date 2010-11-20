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

#include <gui/LoopArrow.h>
#include <QPainter>

/*!
 * Constructor
 */
LoopArrow::LoopArrow(TableItem *ipStartItem,
                                   TableItem *ipEndItem,
                                   QString ipTitle,
                                   QGraphicsItem *ipParent,
                                   QGraphicsScene *ipScene)
    : ForeignArrow(ipStartItem, ipEndItem, ipTitle, ipParent, ipScene)
{
}

/*!
 * Destructor
 */
LoopArrow::~LoopArrow()
{
}

/*!
 * \overload
 */
QRectF
LoopArrow::boundingRect() const
{
    QPointF center = startItem()->pos() + QPointF(startItem()->x() + startItem()->width(), startItem()->y());
    return QRectF(center - QPointF(20, 20), QSizeF(50, 55));
}

/*!
 * \brief Draws line
 *
 * \param[in] ipPainter - Painter
 */
void
LoopArrow::paintLine(QPainter *ipPainter)
{
    QPointF center = startItem()->pos() + QPointF(startItem()->x() + startItem()->width(), startItem()->y());

    QPainterPath path;
    path.moveTo(center);
    path.arcTo(QRectF(center - QPointF(20, 20), QSizeF(50, 50)), -120, 360);

    ipPainter->setBrush(QColor(0, 0, 0, 0));
    ipPainter->drawPath(path);

    QPointF headPoint = QPointF(center + QPointF(0, 30));
    setLine(QLineF(headPoint, headPoint + QPointF(10, 0)));

    ipPainter->setBrush(QColor("black"));
    ipPainter->drawPolygon(makeHead(line(), headPoint));
}

/*!
 * \brief Draws line head
 *
 * \param[in] ipPainter - Painter
 */
void
LoopArrow::paintHead(QPainter *ipPainter)
{
    ipPainter->setBrush(QColor("black"));
    setHead(makeHead(line(), line().p1()));
    ipPainter->drawPolygon(head());
}

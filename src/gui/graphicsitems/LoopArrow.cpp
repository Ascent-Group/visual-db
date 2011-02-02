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

#include <gui/graphicsitems/LoopArrow.h>
#include <QPainter>

namespace Gui {

namespace GraphicsItems {

/*!
 * Constructor
 */
LoopArrow::LoopArrow(TableItem *iStartItem,
                                   TableItem *iEndItem,
                                   QString iTitle,
                                   QGraphicsItem *iParent,
                                   QGraphicsScene *iScene)
    : ForeignArrow(iStartItem, iEndItem, iTitle, iParent, iScene)
{
}

/*!
 * Destructor
 */
LoopArrow::~LoopArrow()
{
}

/*!
 * \brief Update the arrow's position
 */
QPainterPath
LoopArrow::line()
{
    QPainterPath path;
    path.moveTo(centerPoint());
    path.arcTo(QRectF(centerPoint() - QPointF(20, 20), QSizeF(50, 50)), -120, 360);
    return path;
}

/*!
 * \brief Update the arrow's position
 */
QPolygonF
LoopArrow::head()
{
    QPointF headPoint = QPointF(centerPoint() + QPointF(0, 30));
    QLineF line(headPoint, headPoint + QPointF(10, 0));
    return makeHead(line, headPoint);
}

/*!
 * \brief Returns center point of the start item
 *
 * \return Center point
 */
QPointF
LoopArrow::centerPoint() const
{
    return startItem()->pos() + QPointF(startItem()->x() + startItem()->width(), startItem()->y());
}

}

}

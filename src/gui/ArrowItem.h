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

#ifndef ARROW_H
#define ARROW_H

#include <gui/TableItem.h>
#include <QGraphicsLineItem>

class QGraphicsPolygonItem;
class QGraphicsScene;
class QGraphicsSceneMouseEvent;
class QPainterPath;
class QRectF;

/*!
 * \class ArrowItem
 * \headerfile gui/ArrowItem.h
 * \brief Implement the visual relations between tables
 */
class ArrowItem : public QGraphicsPathItem
{
    public:
        enum { Type = UserType + 4 };

        static const int ARROW_SIZE = 12;

    public:
        ArrowItem(TableItem *, TableItem *, QString, QGraphicsItem *iParent = 0, QGraphicsScene *iScene = 0);
        virtual ~ArrowItem();

        virtual int type() const;
        virtual QRectF boundingRect() const;

        virtual TableItem *startItem() const;
        virtual TableItem *endItem() const;

        virtual void updatePosition();

    protected:
        virtual QColor brushColor() const;
        virtual void setBrushColor(const QColor &);

        virtual QPainterPath line();
        virtual QPolygonF head() = 0;

        virtual QPointF startPoint() const;
        virtual QPointF endPoint() const;

    private:
        TableItem *mStartItem;
        TableItem *mEndItem;

        QPointF mStartPoint;
        QPointF mEndPoint;
        QPainterPath mPath;

        QColor mColor;
        QColor mBrushColor;
        QPolygonF mArrowItemHead;

        QString mTitle;

    private:
        virtual QLineF makeLine();
        void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *iWidget = 0);
};

QPolygonF makeHead(const QLineF &, const QPointF &);

#endif // ARROW_H


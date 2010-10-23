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

#ifndef GRAPHICSITEM_H
#define GRAPHICSITEM_H

#include <QGraphicsPolygonItem>
#include <QSettings>

class QGraphicsTextItem;

/*!
 * \class GraphicsItem
 * \headerfile gui/GraphicsItem.h
 * \brief Parent class for graphics items on the scene
 */
class GraphicsItem : public QGraphicsPolygonItem
{
    public:
        static const int INTERVAL = 4;
        static const int FIELD_HEIGHT = 10;
        static const int MIN_HEIGHT = FIELD_HEIGHT + INTERVAL * 2;
        static const int MIN_WIDTH = 50;
        static const int DEFAULT_WIDTH = 150;
        static const int IMG_WIDTH = FIELD_HEIGHT;
        static const int IMG_HEIGHT = IMG_WIDTH;

    public:
        GraphicsItem();
        virtual ~GraphicsItem();

        virtual void setX(qreal);
        virtual qreal x() const;

        virtual void setY(qreal);
        virtual qreal y() const;

        virtual void setWidth(qreal);
        virtual qreal width() const;

        virtual void setHeight(qreal);
        virtual qreal height() const;

        virtual void setItemColor(QColor);
        virtual QColor itemColor() const;

        virtual void setBorderColor(QColor);
        virtual QColor borderColor() const;

        virtual void setFontColor(QColor);
        virtual QColor fontColor() const;

        virtual void setTitleText(QString);
        virtual QString titleText() const;

        virtual void setFieldText(int, QString);
        virtual QString fieldText(int) const;

        virtual void setTitleItem(QGraphicsTextItem *);
        virtual QGraphicsTextItem *titleItem() const;

        virtual bool isRightBottomCorner(const QPointF &ipPos) const;
        virtual bool isLeftBottomCorner(const QPointF &ipPos) const;
        virtual bool isLeftTopCorner(const QPointF &ipPos) const;
        virtual bool isRightTopCorner(const QPointF &ipPos) const;

        virtual bool isLeftVerticalBorder(const QPointF &ipPos) const;
        virtual bool isRightVerticalBorder(const QPointF &ipPos) const;
        virtual bool isTopHorizontalBorder(const QPointF &ipPos) const;
        virtual bool isBottomHorizontalBorder(const QPointF &ipPos) const;

        virtual QPointF centerPoint() const;

        virtual QRectF rect() const;

        virtual int countFields() const;

        virtual QList<QGraphicsTextItem *> fieldItems() const;

        virtual void addFieldItem(QGraphicsTextItem *);
        virtual void clearFields();
        virtual void updatePolygon();
        virtual void adjustSize();

    private:
        QSettings mSettings;

        QGraphicsTextItem *mTitleItem;
        QList<QGraphicsTextItem *> mFieldItems;

        QPointF mLeftTopPoint;
        QPointF mRightBottomPoint;

        QColor mItemColor;
        QColor mFontColor;
        QColor mBorderColor;

        QPolygonF mPolygon;

        static const int BORDER_WIDTH = 5;
};

#endif // GRAPHICSITEM_H


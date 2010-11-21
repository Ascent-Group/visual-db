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

#include <consts.h>
#include <gui/ArrowItem.h>
#include <QDebug>
#include <QPainterPath>
#include <QGraphicsLineItem>
#include <QGraphicsPolygonItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QRectF>
#include <math.h>


static QPointF findIntersection(const TableItem *, const QLineF &);

const qreal Pi = 3.14;

/*!
 * Constructor
 */
ArrowItem::ArrowItem(TableItem *ipStartItem,
             TableItem *ipEndItem,
             QString ipTitle,
             QGraphicsItem *ipParent,
             QGraphicsScene *ipScene)
    : QGraphicsLineItem(ipParent, ipScene)
    , mStartItem(ipStartItem), mEndItem(ipEndItem)
    , mTitle(ipTitle)
{

    // make arrow selectable
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    // set the style of the arrow
    mColor = mSettings.value(Consts::COLOR_GRP + "/" + Consts::ARROW_SETTING, Qt::black).value<QColor>();
    mBrushColor = mColor;
    setPen(QPen(mColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
}

/*!
 * Destructor
 */
ArrowItem::~ArrowItem()
{
}

/*!
 * \brief Define the bounding that arrow covers
 *
 * \return Bounding that arrow covers
 */
QRectF
ArrowItem::boundingRect() const
{
    qreal extra = (pen().width() + 20) / 2.0;

    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
    line().p2().y() - line().p1().y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

/*!
 * \brief Get the arrow head path
 *
 * \return Arrow head path
 */
QPainterPath
ArrowItem::shape() const
{
    QPainterPath path = QGraphicsLineItem::shape();
    path.addPolygon(mArrowItemHead);
    return path;
}

/*!
 * \brief Update the arrow's position
 */
void
ArrowItem::updatePosition()
{
    setLine(makeLine());
}

/*!
 * \brief Set brush color
 *
 * \param[in] ipColor - Brush color
 */
void
ArrowItem::setBrushColor(const QColor &ipColor)
{
    mBrushColor = ipColor;
}

/*!
 * \brief Get brush color
 *
 * \return Brush color
 */
QColor
ArrowItem::brushColor() const
{
    return mBrushColor;
}

/*!
 * \brief Paint the line
 */
void
ArrowItem::paintLine(QPainter *ipPainter)
{
    // draw the line
    setLine(makeLine());
    ipPainter->drawLine(line());
}

/*!
 * \override
 */
void
ArrowItem::paint(QPainter *ipPainter, const QStyleOptionGraphicsItem *, QWidget *)
{
    // return if start and end tables collides
    if (mStartItem != mEndItem && mStartItem->collidesWithItem(mEndItem)) {
        return;
    }

    // set arrow's preoperties
    ipPainter->setPen(pen());

    // draw the line
    paintLine(ipPainter);

    // draw the head
    paintHead(ipPainter);

    // draw the arrow title
    ipPainter->drawText(line().p1(), mTitle);

    // handle the selection of the arrow
    if (isSelected()) {
        ipPainter->setPen(QPen(mColor, 1, Qt::DashLine));
        QLineF selectedLine = line();
        selectedLine.translate(0, 4.0);
        ipPainter->drawLine(selectedLine);
        selectedLine.translate(0, -8.0);
        ipPainter->drawLine(selectedLine);
    }
}

/*!
 * \brief Set the head
 *
 * \param[in] ipHead - Line head
 */
void
ArrowItem::setHead(const QPolygonF &ipHead)
{
    mArrowItemHead = ipHead;
}

/*!
 * \brief Get the head
 *
 * \return Line head
 */
QPolygonF
ArrowItem::head() const
{
    return mArrowItemHead;
}

/*!
 * \brief Get the intersection point between given item and line
 *
 * \param[in] ipItem - Pointer to the table item
 * \param[in] ipLine - Line
 *
 * \return Intersection point if there is an intersection
 */
static QPointF
findIntersection(const TableItem *ipItem, const QLineF &ipLine)
{
    // find intersection between center line and  table border
    QPolygonF polygon = ipItem->polygon();
    QPointF point1 = polygon.first() + ipItem->scenePos();
    QPointF intersectPoint;
    for (int i = 1; i < polygon.count(); ++i) {
        QPointF point2 = polygon.at(i) + ipItem->scenePos();
        QLineF polyLine = QLineF(point1, point2);
        QLineF::IntersectType intersectType = polyLine.intersect(ipLine, &intersectPoint);
        if (intersectType == QLineF::BoundedIntersection) {
            break;
        }
        point1 = point2;
    }

    return intersectPoint;
}

/*!
 * \brief Make head for given arrow
 *
 * \param[in] ipLine - The line we are making the arrow head for
 *
 * \return Polygon that describes the arrow head
 */
QPolygonF
makeHead(const QLineF &ipLine, const QPointF &ipStartPoint)
{
    QPolygonF head;

    // make arrow head
    double angle = ::acos(ipLine.dx() / ipLine.length());
    if (ipLine.dy() >= 0) {
        angle = (Pi * 2) - angle;
    }

    QPointF arrowP1 = ipStartPoint + QPointF(sin(angle + Pi / 3) * ArrowItem::ARROW_SIZE,
                    cos(angle + Pi / 3) * ArrowItem::ARROW_SIZE);
    QPointF arrowP2 = ipStartPoint + QPointF(sin(angle + Pi - Pi / 3) * ArrowItem::ARROW_SIZE,
                    cos(angle + Pi - Pi / 3) * ArrowItem::ARROW_SIZE);

    head << ipStartPoint << arrowP1 << arrowP2;

    return head;
}

/*!
 * \brief Creates line from the start item to the end item (from border to border)
 *
 * \return Created line from the start item to the end item
 */
QLineF
ArrowItem::makeLine() const
{
    QLineF centerLine(mapFromItem(mStartItem, mStartItem->centerPoint()),
        mapFromItem(mEndItem, mEndItem->centerPoint()));

    QPointF startPoint = findIntersection(mStartItem, centerLine);
    QPointF endPoint = findIntersection(mEndItem, centerLine);

    return QLineF(endPoint, startPoint);
}

/*!
 * \brief Get the start item of the arrow
 *
 * \return Start item for the arrow
 */
TableItem *
ArrowItem::startItem() const
{
    return mStartItem;
}

/*!
 * \brief Get the end item of the arrow
 *
 * \return End item for the arrow
 */
TableItem *
ArrowItem::endItem() const
{
    return mEndItem;
}

/*!
 * \brief Get the type of the arrow
 *
 * \return Arrow type
 */
int
ArrowItem::type() const
{
    return Type;
}


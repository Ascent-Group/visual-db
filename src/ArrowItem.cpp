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

#include <QDebug>
#include <QGraphicsLineItem>
#include <QGraphicsPolygonItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QRectF>
#include <math.h>

#include <ArrowItem.h>

static QPointF findIntersection(const TableItem *, const QLineF &);
static QPolygonF makeHead(const QLineF &);

const qreal Pi = 3.14;

/*
 * Constructor
 */
ArrowItem::ArrowItem(TableItem *ipStartItem, 
		     TableItem *ipEndItem, 
		     QGraphicsItem *ipParent, 
		     QGraphicsScene *ipScene)
    : QGraphicsLineItem(ipParent, ipScene), 
      mStartItem(ipStartItem), mEndItem(ipEndItem)
{

    // make arrow selectable
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    // set the style of the arrow
    mColor = mSettings.value("Color/ArrowItem", Qt::black).value<QColor>();
    setPen(QPen(mColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
}

/*
 * Destructor
 */
ArrowItem::~ArrowItem()
{
}

/*
 * Define the bounding that arrow covers
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

/*
 * Get the arrow head path 
 */
QPainterPath 
ArrowItem::shape() const
{
    QPainterPath path = QGraphicsLineItem::shape();
    path.addPolygon(mArrowItemHead);
    return path;
}

/*
 * Update the arrow's position
 */
void 
ArrowItem::updatePosition()
{
    setLine(makeLine());
}

/*
 * Draw the arrow
 */
void 
ArrowItem::paint(QPainter *ipPainter, const QStyleOptionGraphicsItem *, QWidget *)
{
    // return if start and end tables collides
    if (mStartItem->collidesWithItem(mEndItem)) {
        return;
    }

    // set arrow's preoperties
    ipPainter->setPen(mColor);
    ipPainter->setBrush(mColor);

    setLine(makeLine());
    mArrowItemHead = makeHead(line());
 
    // draw the arrow with the head
    ipPainter->drawLine(line());
    ipPainter->drawPolygon(mArrowItemHead);

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

/*
 * Get the intersection point between given item and line
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

/*
 * Make head for given arrow
 */
static QPolygonF
makeHead(const QLineF &ipLine)
{
    QPolygonF head;

    // make arrow head
    double angle = ::acos(ipLine.dx() / ipLine.length());
    if (ipLine.dy() >= 0) {
        angle = (Pi * 2) - angle;
    }

    QPointF arrowP1 = ipLine.p1() + QPointF(sin(angle + Pi / 3) * ArrowItem::ARROW_SIZE,
				    cos(angle + Pi / 3) * ArrowItem::ARROW_SIZE);
    QPointF arrowP2 = ipLine.p1() + QPointF(sin(angle + Pi - Pi / 3) * ArrowItem::ARROW_SIZE,
				    cos(angle + Pi - Pi / 3) * ArrowItem::ARROW_SIZE);

    head << ipLine.p1() << arrowP1 << arrowP2;

    return head;
}

/*
 * Line from the start's item to the end's item (from border to border)
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

/*
 * Get the start item of the arrow
 */
TableItem *
ArrowItem::startItem() const 
{ 
    return mStartItem; 
}

/*
 * Get the end item of the arrow
 */
TableItem *
ArrowItem::endItem() const 
{ 
    return mEndItem; 
}

/*
 * Get the type of the arrow
 */
int
ArrowItem::type() const
{
    return Type;
}

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
 *     * Neither the name of the Ascent Group nor the names of its contributor
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

#include <QBrush>
#include <QGraphicsTextItem>
#include <QTextDocument>
#include <gui/GraphicsItem.h>

/*
 * Constructor
 */
GraphicsItem::GraphicsItem()
    : QGraphicsPolygonItem(), mLeftTopPoint(0, 0), mRightBottomPoint(0, 0)
{
    mFieldItems = QList<QGraphicsTextItem *>();
    // get selected color
    mItemColor = mSettings.value("Color/Table", Qt::white).value<QColor>();
    mBorderColor = mSettings.value("Color/Border", Qt::black).value<QColor>();
}

/*
 * Destructor
 */
GraphicsItem::~GraphicsItem()
{
}


/*
 * Check if the poition is under left vertical border
 */
bool
GraphicsItem::isLeftVerticalBorder(qreal ipX) const
{
    return ipX > mLeftTopPoint.x() && ipX < mLeftTopPoint.x() + BORDER_WIDTH;
}

/*
 * Check if the position is under right vertical border
 */
bool
GraphicsItem::isRightVerticalBorder(qreal ipX) const
{
    return ipX < mRightBottomPoint.x() && ipX > mRightBottomPoint.x() - BORDER_WIDTH;
}

/*
 * Check if the positon is under top horizontal border
 */
bool
GraphicsItem::isTopHorizontalBorder(qreal ipY) const
{
    return ipY > mLeftTopPoint.y() && ipY < mLeftTopPoint.y() + BORDER_WIDTH;
}

/*
 * Check if the position is under bottom horizontal border
 */
bool
GraphicsItem::isBottomHorizontalBorder(qreal ipY) const
{
    return ipY < mRightBottomPoint.y() && ipY > mRightBottomPoint.y() - BORDER_WIDTH;
}

/*
 * Check if the position is under the right bottom corner
 */
bool
GraphicsItem::isRightBottomCorner(qreal ipX, qreal ipY) const
{
    return isBottomHorizontalBorder(ipY) && isRightVerticalBorder(ipX);
}

/*
 * Check if the position is under the left bottom corner
 */
bool
GraphicsItem::isLeftBottomCorner(qreal ipX, qreal ipY) const
{
    return isTopHorizontalBorder(ipY) && isRightVerticalBorder(ipX);
}

/*
 * Check if the position is under the left top corner
 */
bool
GraphicsItem::isLeftTopCorner(qreal ipX, qreal ipY) const
{
    return isTopHorizontalBorder(ipY) && isLeftVerticalBorder(ipX);
}

/*
 * Check if the position is under the right top corner
 */
bool
GraphicsItem::isRightTopCorner(qreal ipX, qreal ipY) const
{
    return isBottomHorizontalBorder(ipY) && isLeftVerticalBorder(ipX);
}

/*
 * Update the border polygon
 */
void
GraphicsItem::updatePolygon()
{
    mPolygon.clear();
    mPolygon
	<< QPointF(mLeftTopPoint)
	<< QPointF(mRightBottomPoint.x(), mLeftTopPoint.y())
	<< QPointF(mRightBottomPoint)
	<< QPointF(mLeftTopPoint.x(), mRightBottomPoint.y())
	<< QPointF(mLeftTopPoint);
    setPolygon(mPolygon);
}

/*
 * Get item color
 */
QColor
GraphicsItem::itemColor() const
{
    return mItemColor;
}

/*
 * Return rectangle
 */
QRectF
GraphicsItem::rect() const
{
    return QRectF(x(), y(), width(), height());
}

/*
 * Get x coordinate of left top corner
 */
qreal
GraphicsItem::x() const
{
    return mLeftTopPoint.x();
}

/*
 * Get y coordinate of left top corner
 */
qreal
GraphicsItem::y() const
{
    return mLeftTopPoint.y();
}

/*
 * Return width of the item
 */
qreal
GraphicsItem::width() const
{
    return mRightBottomPoint.x() - mLeftTopPoint.x();
}

/*
 * Return height of the item
 */
qreal
GraphicsItem::height() const
{
    return mRightBottomPoint.y() - mLeftTopPoint.y();
}

/*
 * Set x coordinate of left top corner
 */
void
GraphicsItem::setX(qreal ipX)
{
    mLeftTopPoint.setX(ipX);
}

/*
 * Set y coordinate of left top corner
 */
void
GraphicsItem::setY(qreal ipY)
{
    mLeftTopPoint.setY(ipY);
}

/*
 * Set width of the item
 */
void
GraphicsItem::setWidth(qreal ipWidth)
{
    mRightBottomPoint.setX(x() + ipWidth);
}

void
GraphicsItem::setHeight(qreal ipHeight)
{
    mRightBottomPoint.setY(y() + ipHeight);
}

/*
 * Get the center point of the item in item's system coordinate
 */
QPointF
GraphicsItem::centerPoint() const
{
    return QPointF((mRightBottomPoint.x() + mLeftTopPoint.x()) / 2, (mRightBottomPoint.y() + mLeftTopPoint.y()) / 2);
}

/*
 * Set table color
 */
void
GraphicsItem::setItemColor(QColor ipColor)
{
    mItemColor = ipColor;
    setBrush(QBrush(mItemColor));

    int red = mItemColor.red();
    int green = mItemColor.green();
    int blue = mItemColor.blue();
    mFontColor = QColor(255 - red, 255 - green, 255 - blue);

    update();
}

/*
 * Get border color
 */
QColor
GraphicsItem::borderColor() const
{
    return mBorderColor;
}

/*
 * Set border color
 */
void
GraphicsItem::setBorderColor(QColor ipColor)
{
    mBorderColor = ipColor;
}

/*
 * Get font color
 */
QColor
GraphicsItem::fontColor() const
{
    return mFontColor;
}

/*
 * Set font color
 */
void
GraphicsItem::setFontColor(QColor ipColor)
{
    mFontColor = ipColor;
}

/*
 * Add field item to the list of field
 */
void
GraphicsItem::addFieldItem(QGraphicsTextItem *ipItem)
{
    mFieldItems << ipItem;
}

/*
 * Set title item
 */
void
GraphicsItem::setTitleItem(QGraphicsTextItem *ipItem)
{
    mTitleItem = ipItem;
}

/*
 * Get title text
 */
QString
GraphicsItem::titleText() const
{
    return mTitleItem->toPlainText();
}

/*
 * Set title text
 */
void
GraphicsItem::setTitleText(QString ipText)
{
    mTitleItem->setPlainText(ipText);
}

/*
 * Get field text
 */
QString
GraphicsItem::fieldText(int ipI) const
{
    return mFieldItems.at(ipI)->toPlainText();
}

/*
 * Get count field
 */
int
GraphicsItem::countFields() const
{
    return mFieldItems.size();
}

/*
 * Get all field
 */
QList<QGraphicsTextItem *>
GraphicsItem::fieldItems() const
{
    return mFieldItems;
}

/*
 * Get the title item
 */
QGraphicsTextItem *
GraphicsItem::titleItem() const
{
    return mTitleItem;
}
/*
 * Set field text
 */
void
GraphicsItem::setFieldText(int ipI, QString ipText)
{
    mFieldItems.at(ipI)->setPlainText(ipText);
}

/*
 * Clear field
 */
void
GraphicsItem::clearFields()
{
    mFieldItems.clear();
}

/*
 * Adjust the size of the item
 */
void
GraphicsItem::adjustSize()
{
    // find the maximum width among the item's field
    qreal optimalWidth = MIN_WIDTH;
    if (optimalWidth < mTitleItem->document()->idealWidth()) {
	optimalWidth = mTitleItem->document()->idealWidth();
    }
    for (int i = 0; i < mFieldItems.size(); ++i) {
	QGraphicsTextItem *fieldItem = mFieldItems.at(i);
	if (optimalWidth < fieldItem->document()->idealWidth()) {
	    optimalWidth = fieldItem->document()->idealWidth();
	}
    }

    // calculate optimal width
    optimalWidth += INTERVAL * 3 + IMG_WIDTH;
    // calculate optimal height
    qreal optimalHeight = (mFieldItems.size() + 1) * (FIELD_HEIGHT + INTERVAL) + INTERVAL * 2;

    // resize the item's size with optimal width and height
    QPointF optimalRightBottomCorner(optimalWidth, optimalHeight);
    setWidth(optimalRightBottomCorner.x());
    setHeight(optimalRightBottomCorner.y());
    updatePolygon();
}


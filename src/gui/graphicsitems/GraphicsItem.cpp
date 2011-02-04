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
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsTextItem>
#include <QMenu>
#include <QPainter>
#include <QPen>
#include <QPixmap>
#include <QTextDocument>
#include <control/Config.h>
#include <gui/graphicsitems/ArrowItem.h>
#include <gui/graphicsitems/GraphicsItem.h>
#include <gui/GraphicsScene.h>

namespace Gui {

namespace GraphicsItems {

int GraphicsItem::mSeek = 80;

/*!
 * Constructor
 */
GraphicsItem::GraphicsItem(QMenu *iMenu)
    : QGraphicsPolygonItem(),
      mTitleItem(0),
      mFieldItems(),
      mLeftTopPoint(0, 0),
      mRightBottomPoint(0, 0),
      mFont("Arial", 8),
      mMode(GraphicsItem::MOVE),
      mContextMenu(iMenu)
{
    // get selected color
    Control::Config cfg;
    mItemColor = cfg.tableColor();
    mBorderColor = cfg.borderColor();

    mShadowColor = QColor(60, 60, 60, 175);
}

/*!
 * Destructor
 */
GraphicsItem::~GraphicsItem()
{
    delete mTitleItem;
    qDeleteAll(mFieldItems);
}

/*!
 * \brief Check if the poition is under left vertical border
 *
 * \param[in] iPos - Position we are checking
 *
 * \return True if the poition is under left vertical border, false otherwise
 */
bool
GraphicsItem::isLeftVerticalBorder(const QPointF &iPos) const
{
    return iPos.x() > mLeftTopPoint.x() && iPos.x() < mLeftTopPoint.x() + BORDER_WIDTH;
}

/*!
 * \brief Check if the position is under right vertical border
 *
 * \param[in] iPos - Position we are checking
 *
 * \return True if the position is under right vertical border, false otherwise
 */
bool
GraphicsItem::isRightVerticalBorder(const QPointF &iPos) const
{
    return iPos.x() < mRightBottomPoint.x() && iPos.x() > mRightBottomPoint.x() - BORDER_WIDTH;
}

/*!
 * \brief Check if the positon is under top horizontal border
 *
 * \param[in] iPos - Position we are checking
 *
 * \return True if the positon is under top horizontal border, false otherwise
 */
bool
GraphicsItem::isTopHorizontalBorder(const QPointF &iPos) const
{
    return iPos.y() > mLeftTopPoint.y() && iPos.y() < mLeftTopPoint.y() + BORDER_WIDTH;
}

/*!
 * \brief Check if the position is under bottom horizontal border
 *
 * \param[in] iPos - Position we are checking
 *
 * \return True if the position is under bottom horizontal border, false otherwise
 */
bool
GraphicsItem::isBottomHorizontalBorder(const QPointF &iPos) const
{
    return iPos.y() < mRightBottomPoint.y() && iPos.y() > mRightBottomPoint.y() - BORDER_WIDTH;
}

/*!
 * \brief Check if the position is under the right bottom corner
 *
 * \param[in] iPos - Position we are checking
 *
 * \return True if the position is under the right bottom corner, false otherwise
 */
bool
GraphicsItem::isRightBottomCorner(const QPointF &iPos) const
{
    return isBottomHorizontalBorder(iPos) && isRightVerticalBorder(iPos);
}

/*!
 * \brief Check if the position is under the left bottom corner
 *
 * \param[in] iPos - Position we are checking
 *
 * \return True if the position is under the left bottom corner, false otherwise
 */
bool
GraphicsItem::isLeftBottomCorner(const QPointF &iPos) const
{
    return isTopHorizontalBorder(iPos) && isRightVerticalBorder(iPos);
}

/*!
 * \brief Check if the position is under the left top corner
 *
 * \param[in] iPos - Position we are checking
 *
 * \return True if the position is under the left top corner, false otherwise
 */
bool
GraphicsItem::isLeftTopCorner(const QPointF &iPos) const
{
    return isTopHorizontalBorder(iPos) && isLeftVerticalBorder(iPos);
}

/*!
 * \brief Check if the position is under the right top corner
 *
 * \param[in] iPos - Position we are checking
 *
 * \return True if position is under the right top corner, false otherwise
 */
bool
GraphicsItem::isRightTopCorner(const QPointF &iPos) const
{
    return isBottomHorizontalBorder(iPos) && isLeftVerticalBorder(iPos);
}

/*!
 * \brief Update the border polygon
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

/*!
 * \brief Get item color
 *
 * \return Item color
 */
QColor
GraphicsItem::itemColor() const
{
    return mItemColor;
}

/*!
 * \brief Return rectangle
 *
 * \return Item rectangle
 */
QRectF
GraphicsItem::rect() const
{
    return QRectF(x(), y(), width(), height());
}

/*!
 * \brief Get x coordinate of left top corner
 *
 * \return X coordinate of left top corner
 */
qreal
GraphicsItem::x() const
{
    return mLeftTopPoint.x();
}

/*!
 * \brief Get y coordinate of left top corner
 *
 * \return Y coordinate of left top corner
 */
qreal
GraphicsItem::y() const
{
    return mLeftTopPoint.y();
}

/*!
 * \brief Return width of the item
 *
 * \return Item width
 */
qreal
GraphicsItem::width() const
{
    return mRightBottomPoint.x() - mLeftTopPoint.x();
}

/*!
 * \brief Return height of the item
 *
 * \return Item height
 */
qreal
GraphicsItem::height() const
{
    return mRightBottomPoint.y() - mLeftTopPoint.y();
}

/*!
 * \brief Set x coordinate of left top corner
 *
 * \param[in] iX - X coordinate of left top corner
 */
void
GraphicsItem::setX(qreal iX)
{
    mLeftTopPoint.setX(iX);
}

/*!
 * \brief Set y coordinate of left top corner
 *
 * \param[in] iY - Y coordinate of left top corner
 */
void
GraphicsItem::setY(qreal iY)
{
    mLeftTopPoint.setY(iY);
}

/*!
 * \brief Set width of the item
 *
 * \param[in] iWidth - Item width
 */
void
GraphicsItem::setWidth(qreal iWidth)
{
    mRightBottomPoint.setX(x() + iWidth);
}

/*!
 * \brief Set height of the item
 *
 * \param[in] iHeight - Item height
 */
void
GraphicsItem::setHeight(qreal iHeight)
{
    mRightBottomPoint.setY(y() + iHeight);
}

/*!
 * \brief Get the center point of the item in item's system coordinate
 *
 * \return Center point of the item
 */
QPointF
GraphicsItem::centerPoint() const
{
    return QPointF((mRightBottomPoint.x() + mLeftTopPoint.x()) / 2, (mRightBottomPoint.y() + mLeftTopPoint.y()) / 2);
}

/*!
 * \brief Set item color
 *
 * \param[in] iColor - Item color
 */
void
GraphicsItem::setItemColor(const QColor &iColor)
{
    mItemColor = iColor;
    setBrush(QBrush(mItemColor));

    int red = mItemColor.red();
    int green = mItemColor.green();
    int blue = mItemColor.blue();
    mFontColor = QColor(255 - red, 255 - green, 255 - blue);

    update();
}

/*!
 * \brief Get border color
 *
 * \return Border color
 */
QColor
GraphicsItem::borderColor() const
{
    return mBorderColor;
}

/*!
 * \brief Set border color
 *
 * \param[in] iColor - Border color
 */
void
GraphicsItem::setBorderColor(const QColor &iColor)
{
    mBorderColor = iColor;
}

/*!
 * \brief Get font color
 *
 * \return Font color
 */
QColor
GraphicsItem::fontColor() const
{
    return mFontColor;
}

/*!
 * \brief Set font color
 *
 * \param[in] iColor - Font color
 */
void
GraphicsItem::setFontColor(const QColor &iColor)
{
    mFontColor = iColor;
}

/*!
 * \brief Add field item to the list of field
 *
 * \param[in] iItem - Pointer to the field item
 */
void
GraphicsItem::addFieldItem(QGraphicsTextItem *iItem)
{
    mFieldItems << iItem;
}

/*!
 * \brief Set title item
 *
 * \param[in] iItem - Pointer to the title item
 */
void
GraphicsItem::setTitleItem(QGraphicsTextItem *iItem)
{
    mTitleItem = iItem;
}

/*!
 * \brief Get title text
 *
 * \return Title text
 */
QString
GraphicsItem::titleText() const
{
    return mTitleItem->toPlainText();
}

/*!
 * \brief Set title text
 *
 * \param[in] iText - Title text
 */
void
GraphicsItem::setTitleText(const QString &iText)
{
    mTitleItem->setPlainText(iText);
}

/*!
 * \brief Get field text
 *
 * \param[in] iI - Field index
 *
 * \return Field text
 */
QString
GraphicsItem::fieldText(int iI) const
{
    return mFieldItems.at(iI)->toPlainText();
}

/*!
 * \brief Get count fields
 *
 * \return Count fields
 */
int
GraphicsItem::countFields() const
{
    return mFieldItems.size();
}

/*!
 * \brief Get all field
 *
 * \return Field list
 */
QList<QGraphicsTextItem *>
GraphicsItem::fieldItems() const
{
    return mFieldItems;
}

/*!
 * \brief Get the title item
 *
 * \return Title item
 */
QGraphicsTextItem *
GraphicsItem::titleItem() const
{
    return mTitleItem;
}

/*!
 * \brief Set field text
 *
 * \param[in] iI - Field index
 * \param[in] iText - Field text
 */
void
GraphicsItem::setFieldText(int iI, const QString &iText)
{
    mFieldItems.at(iI)->setPlainText(iText);
}

/*!
 * \brief Clear field
 */
void
GraphicsItem::clearFields()
{
    qDeleteAll(mFieldItems);
    mFieldItems.clear();
}

/*!
 * \brief Adjust the size of the item
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
    setWidth(optimalWidth);
    setHeight(optimalHeight);
    updatePolygon();
}

/*!
 * \overload
 */
QRectF
GraphicsItem::boundingRect() const
{
    return QRectF(x(), y(), width() + SHADOW_SIZE, height() + SHADOW_SIZE);
}

/*!
 * \overload
 */
void
GraphicsItem::paint(QPainter *iPainter, const QStyleOptionGraphicsItem *iItem, QWidget *iWidget)
{
    // set the color of painting
    iPainter->setPen(fontColor());
    iPainter->setFont(mFont);

    paintBorder(iPainter, iItem, iWidget);
    paintTitle(iPainter);
    paintTitleImage(iPainter);
    paintTitleText(iPainter);
    paintFields(iPainter);
    paintAdditionalInfo(iPainter);
    paintAnchor(iPainter);
}

/*!
 * \brief Paints border for item
 *
 * \param[in] iPainter - Painter
 * \param[in] iItem - Style option item
 * \param[in] iWidget - Widget
 */
void
GraphicsItem::paintBorder(QPainter *iPainter, const QStyleOptionGraphicsItem *iItem, QWidget *iWidget)
{
    // draw the board of the table
    QGraphicsPolygonItem::paint(iPainter, iItem, iWidget);
    iPainter->fillRect((int)x() + SHADOW_SIZE, (int)y() + (int)height() + 1, (int)width() + 1, SHADOW_SIZE, mShadowColor);
    iPainter->fillRect((int)x() + (int)width() + 1, (int)y() + SHADOW_SIZE, SHADOW_SIZE, (int)height() - SHADOW_SIZE + 1, mShadowColor);
}

/*!
 * \brief Paints title for item
 *
 * \param[in] iPainter - Painter
 */
void
GraphicsItem::paintTitle(QPainter *iPainter)
{
    // fill title with a little darker color then another table
    iPainter->fillRect((int)x() + 1, (int)y() + 1,
            (int)width() - 1, (int)y() + FIELD_HEIGHT + INTERVAL * 2 - (int)y() - 1,
            QColor(abs(itemColor().red() - 80), abs(itemColor().green() - 80), abs(itemColor().blue() - 80)));

    // draw line under the title
    iPainter->drawLine((int)x(), (int)y() + FIELD_HEIGHT + INTERVAL * 2,
            (int)(x() + width()), (int)y() + FIELD_HEIGHT + INTERVAL * 2);
}

/*!
 * \brief Paints title image
 *
 * \param[in] iPainter - Painter
 */
void
GraphicsItem::paintTitleImage(QPainter *iPainter)
{
}

/*!
 * \brief Paints title text
 *
 * \param[in] iPainter - Painter
 */
void
GraphicsItem::paintTitleText(QPainter *iPainter)
{
    // draw the title aligned on the center in upper case
    iPainter->drawText((int)x() + IMG_WIDTH + 3 * INTERVAL, (int)y() + INTERVAL,
            (int)width() - IMG_WIDTH - INTERVAL * 4, FIELD_HEIGHT + INTERVAL,
            Qt::AlignCenter, titleText());
}

/*!
 * \brief Paints all fields for item
 *
 * \param[in] iPainter - Painter
 */
void
GraphicsItem::paintFields(QPainter *iPainter)
{
    // row in the graphic table (some items may be missed)
    // draw each field
    for (int i = 0; i < countFields(); ++i) {
        // break drawing if we have reached the board
        if (height() < (FIELD_HEIGHT + INTERVAL) * (i + 2) + INTERVAL) {
            break;
        }

        paintFieldImage(iPainter, i);
        paintFieldText(iPainter, i);
    }
}

/*!
 * \brief Paints field text
 *
 * \param[in] iPainter - Painter
 * \param[in] iIdx - Field index
 */
void
GraphicsItem::paintFieldText(QPainter *iPainter, int iIdx)
{
    // draw field name with margins = INTERVAL for top, bottom, left and right sizes
    iPainter->drawText((int)x() + IMG_WIDTH + 3 * INTERVAL, (int)y() + (FIELD_HEIGHT + INTERVAL) * (iIdx + 1) + INTERVAL * 2,
            (int)width() - IMG_WIDTH - INTERVAL * 4, FIELD_HEIGHT + INTERVAL * 2,
            Qt::AlignLeft,
            fieldText(iIdx));
}

}

}

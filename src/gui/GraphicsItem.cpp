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
#include <QGraphicsTextItem>
#include <QMenu>
#include <QPainter>
#include <QPen>
#include <QPixmap>
#include <QTextDocument>
#include <QTextDocument>
#include <consts.h>
#include <gui/ArrowItem.h>
#include <gui/GraphicsItem.h>
#include <gui/GraphicsScene.h>

int GraphicsItem::mSeek = 80;

/*!
 * Constructor
 */
GraphicsItem::GraphicsItem(QMenu *ipMenu)
    : QGraphicsPolygonItem(),
      mLeftTopPoint(0, 0),
      mRightBottomPoint(0, 0),
      mFont("Arial", 10), 
      mMode(GraphicsItem::MOVE),
      mFieldsTypesVisible(true),
      mContextMenu(ipMenu)
{
    mFieldItems = QList<QGraphicsTextItem *>();
    // get selected color
    mItemColor = mSettings.value(Consts::COLOR_GRP + "/" + Consts::TABLE_SETTING, Qt::white).value<QColor>();
    mBorderColor = mSettings.value(Consts::COLOR_GRP + "/" + Consts::BORDER_SETTING, Qt::black).value<QColor>();

    // preload of images
    mTableImage = new QImage(":/img/table.png");
    mFieldImage = new QImage(":/img/field.png");
    mAnchorImage = new QImage(":/img/anchor.png");

    mShadowColor = QColor(60, 60, 60, 175);
}

/*!
 * Destructor
 */
GraphicsItem::~GraphicsItem()
{
}

/*!
 * \brief Check if the poition is under left vertical border
 *
 * \param[in] ipPos - Position we are checking
 *
 * \return True if the poition is under left vertical border, false otherwise
 */
bool
GraphicsItem::isLeftVerticalBorder(const QPointF &ipPos) const
{
    return ipPos.x() > mLeftTopPoint.x() && ipPos.x() < mLeftTopPoint.x() + BORDER_WIDTH;
}

/*!
 * \brief Check if the position is under right vertical border
 *
 * \param[in] ipPos - Position we are checking
 *
 * \return True if the position is under right vertical border, false otherwise
 */
bool
GraphicsItem::isRightVerticalBorder(const QPointF &ipPos) const
{
    return ipPos.x() < mRightBottomPoint.x() && ipPos.x() > mRightBottomPoint.x() - BORDER_WIDTH;
}

/*!
 * \brief Check if the positon is under top horizontal border
 *
 * \param[in] ipPos - Position we are checking
 *
 * \return True if the positon is under top horizontal border, false otherwise
 */
bool
GraphicsItem::isTopHorizontalBorder(const QPointF &ipPos) const
{
    return ipPos.y() > mLeftTopPoint.y() && ipPos.y() < mLeftTopPoint.y() + BORDER_WIDTH;
}

/*!
 * \brief Check if the position is under bottom horizontal border
 *
 * \param[in] ipPos - Position we are checking
 *
 * \return True if the position is under bottom horizontal border, false otherwise
 */
bool
GraphicsItem::isBottomHorizontalBorder(const QPointF &ipPos) const
{
    return ipPos.y() < mRightBottomPoint.y() && ipPos.y() > mRightBottomPoint.y() - BORDER_WIDTH;
}

/*!
 * \brief Check if the position is under the right bottom corner
 *
 * \param[in] ipPos - Position we are checking
 *
 * \return True if the position is under the right bottom corner, false otherwise
 */
bool
GraphicsItem::isRightBottomCorner(const QPointF &ipPos) const
{
    return isBottomHorizontalBorder(ipPos) && isRightVerticalBorder(ipPos);
}

/*!
 * \brief Check if the position is under the left bottom corner
 *
 * \param[in] ipPos - Position we are checking
 *
 * \return True if the position is under the left bottom corner, false otherwise
 */
bool
GraphicsItem::isLeftBottomCorner(const QPointF &ipPos) const
{
    return isTopHorizontalBorder(ipPos) && isRightVerticalBorder(ipPos);
}

/*!
 * \brief Check if the position is under the left top corner
 *
 * \param[in] ipPos - Position we are checking
 *
 * \return True if the position is under the left top corner, false otherwise
 */
bool
GraphicsItem::isLeftTopCorner(const QPointF &ipPos) const
{
    return isTopHorizontalBorder(ipPos) && isLeftVerticalBorder(ipPos);
}

/*!
 * \brief Check if the position is under the right top corner
 *
 * \param[in] ipPos - Position we are checking
 *
 * \return True if position is under the right top corner, false otherwise
 */
bool
GraphicsItem::isRightTopCorner(const QPointF &ipPos) const
{
    return isBottomHorizontalBorder(ipPos) && isLeftVerticalBorder(ipPos);
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
 * \param[in] ipX - X coordinate of left top corner
 */
void
GraphicsItem::setX(qreal ipX)
{
    mLeftTopPoint.setX(ipX);
}

/*!
 * \brief Set y coordinate of left top corner
 *
 * \param[in] ipY - Y coordinate of left top corner
 */
void
GraphicsItem::setY(qreal ipY)
{
    mLeftTopPoint.setY(ipY);
}

/*!
 * \brief Set width of the item
 *
 * \param[in] ipWidth - Item width
 */
void
GraphicsItem::setWidth(qreal ipWidth)
{
    mRightBottomPoint.setX(x() + ipWidth);
}

/*!
 * \brief Set height of the item
 *
 * \param[in] ipHeight - Item height
 */
void
GraphicsItem::setHeight(qreal ipHeight)
{
    mRightBottomPoint.setY(y() + ipHeight);
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
 * \param[in] ipColor - Item color
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
 * \param[in] ipColor - Border color
 */
void
GraphicsItem::setBorderColor(QColor ipColor)
{
    mBorderColor = ipColor;
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
 * \param[in] ipColor - Font color
 */
void
GraphicsItem::setFontColor(QColor ipColor)
{
    mFontColor = ipColor;
}

/*!
 * \brief Add field item to the list of field
 *
 * \param[in] ipItem - Pointer to the field item
 */
void
GraphicsItem::addFieldItem(QGraphicsTextItem *ipItem)
{
    mFieldItems << ipItem;
}

/*!
 * \brief Set title item
 *
 * \param[in] ipItem - Pointer to the title item
 */
void
GraphicsItem::setTitleItem(QGraphicsTextItem *ipItem)
{
    mTitleItem = ipItem;
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
 * \param[in] ipTexx - Title text
 */
void
GraphicsItem::setTitleText(QString ipText)
{
    mTitleItem->setPlainText(ipText);
}

/*!
 * \brief Get field text
 *
 * \param[in] ipI - Field index
 *
 * \return Field text
 */
QString
GraphicsItem::fieldText(int ipI) const
{
    return mFieldItems.at(ipI)->toPlainText();
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
 * \param[in] ipI - Field index
 * \param[in] ipText - Field text
 */
void
GraphicsItem::setFieldText(int ipI, QString ipText)
{
    mFieldItems.at(ipI)->setPlainText(ipText);
}

/*!
 * \brief Clear field
 */
void
GraphicsItem::clearFields()
{
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
    QPointF optimalRightBottomCorner(optimalWidth, optimalHeight);
    setWidth(optimalRightBottomCorner.x());
    setHeight(optimalRightBottomCorner.y());
    updatePolygon();
}

/*
 *
 */
QRectF
GraphicsItem::boundingRect() const
{
    return QRectF(x(), y(), width() + SHADOW_SIZE, height() + SHADOW_SIZE);
}

/*
 * Paint the table. Height is adjusting, but width is setted to default.
 */
void
GraphicsItem::paint(QPainter *ipPainter, const QStyleOptionGraphicsItem *ipItem, QWidget *ipWidget)
{
    // set the color of painting
    ipPainter->setPen(fontColor());
    ipPainter->setFont(mFont);

    paintBorder(ipPainter, ipItem, ipWidget);
    paintTitle(ipPainter);
    paintTitleImage(ipPainter);
    paintTitleText(ipPainter);
    paintFields(ipPainter);
    paintAdditionalInfo(ipPainter);
    paintAnchor(ipPainter);
}

void
GraphicsItem::paintBorder(QPainter *ipPainter, const QStyleOptionGraphicsItem *ipItem, QWidget *ipWidget)
{
    // draw the board of the table
    QGraphicsPolygonItem::paint(ipPainter, ipItem, ipWidget);
    ipPainter->fillRect((int)x() + SHADOW_SIZE, (int)y() + (int)height() + 1, width() + 1, SHADOW_SIZE, mShadowColor);
    ipPainter->fillRect((int)x() + (int)width() + 1, (int)y() + SHADOW_SIZE, SHADOW_SIZE, (int)height() - SHADOW_SIZE + 1, mShadowColor);
}

void
GraphicsItem::paintTitle(QPainter *ipPainter)
{
    // fill title with a little darker color then another table
    ipPainter->fillRect((int)x() + 1, (int)y() + 1,
            (int)width() - 1, (int)y() + FIELD_HEIGHT + INTERVAL * 2 - (int)y() - 1,
            QColor(abs(itemColor().red() - 80), abs(itemColor().green() - 80), abs(itemColor().blue() - 80)));

    // draw line under the title
    ipPainter->drawLine((int)x(), (int)y() + FIELD_HEIGHT + INTERVAL * 2,
            (int)(x() + width()), (int)y() + FIELD_HEIGHT + INTERVAL * 2);
}

void
GraphicsItem::paintTitleImage(QPainter *ipPainter)
{
    // draw image for table
    QRectF target((int)x() + INTERVAL, (int)y() + INTERVAL,
            IMG_HEIGHT + INTERVAL, IMG_HEIGHT + INTERVAL);
    QRectF source(0.0, 0.0, mTableImage->width(), mTableImage->height());
    ipPainter->drawImage(target, *mTableImage, source);
}

void
GraphicsItem::paintTitleText(QPainter *ipPainter)
{
    // draw the title aligned on the center in upper case
    ipPainter->drawText((int)x() + IMG_WIDTH + 2 * INTERVAL, (int)y() + INTERVAL,
            (int)width() - IMG_WIDTH - INTERVAL * 3, FIELD_HEIGHT + INTERVAL,
            Qt::AlignCenter,
            titleText());
}

void
GraphicsItem::paintAnchor(QPainter *ipPainter)
{
    // if anchor was setted for this table - draw the anchor
    if (!(flags() & QGraphicsItem::ItemIsMovable)) {
        QRectF target(x() + width() - IMG_WIDTH - INTERVAL, y() + height() - IMG_HEIGHT - INTERVAL, IMG_WIDTH, IMG_HEIGHT);
        QRectF source(0.0, 0.0, mAnchorImage->width(), mAnchorImage->height());
        ipPainter->drawImage(target, *mAnchorImage, source);
    }
}

void
GraphicsItem::paintFields(QPainter *ipPainter)
{
    // row in the graphic table (some items may be missed)
    // draw each field
    for (int i = 0; i < countFields(); ++i) {
        // break drawing if we have reached the board
        if (height() < (FIELD_HEIGHT + INTERVAL) * (i + 2) + INTERVAL) {
            break;
        }

        paintFieldImage(ipPainter, i);
        paintFieldText(ipPainter, i);
    }
}

void
GraphicsItem::paintFieldText(QPainter *ipPainter, int ipIdx)
{
    // draw field name with margins = INTERVAL for top, bottom, left and right sizes
    ipPainter->drawText((int)x() + IMG_WIDTH + 2 * INTERVAL, (int)y() + (FIELD_HEIGHT + INTERVAL) * (ipIdx + 1) + INTERVAL,
            (int)width() - IMG_WIDTH - INTERVAL * 3, FIELD_HEIGHT + INTERVAL * 2,
            Qt::AlignLeft,
            fieldText(ipIdx));
}


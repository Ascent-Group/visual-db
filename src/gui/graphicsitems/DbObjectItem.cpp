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
#include <control/Config.h>
#include <gui/graphicsitems/ArrowItem.h>
#include <gui/graphicsitems/DbObjectItem.h>
#include <gui/GraphicsScene.h>

namespace Gui {

namespace GraphicsItems {

/*!
 * Constructor
 */
DbObjectItem::DbObjectItem(QMenu *iMenu)
    : GraphicsItem(),
      mFieldsTypesVisible(true),
      mContextMenu(iMenu)
//      mMode(MOVE)
{
    // preload of images
    mFieldImage = new QImage(":/img/field.png");
    mAnchorImage = new QImage(":/img/anchor.png");
}

/*!
 * Destructor
 */
DbObjectItem::~DbObjectItem()
{
    // we cannot delete arrows here because both source and destination tables hold the
    // pointer to the given arrow in its own arrow items list. In this case we cannot
    // handle a proper destruction of an arrow, because delete on the same arrows will be
    // called twice, and this may result in crash
//    qDeleteAll(mArrowItems);
    mArrowItems.clear();

    delete mFieldImage;
    delete mAnchorImage;

}

/*!
 * \overload
 */
void
DbObjectItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *iEvent)
{
    if (mContextMenu) {
        setSelected(true);
        // show context menu
        mContextMenu->exec(iEvent->screenPos());
    }
}

/*!
 * \brief Add given arrow to the list of arrows related with this item
 *
 * \param[in] arrow - Arrow we will add
 */
void
DbObjectItem::addArrowItem(ArrowItem *arrow)
{
    mArrowItems.append(arrow);
}

/*!
 * \brief Remove given arrow from the list of arrows related with this item
 *
 * \param[in] iArrowItem - Arrow we will remove
 */
void
DbObjectItem::removeArrowItem(ArrowItem *iArrowItem)
{
    int index = mArrowItems.indexOf(iArrowItem);
    if (index != -1) {
        mArrowItems.removeAt(index);
    }
}

/*!
 * \brief Remove all arrows from the list of arrows related with this item
 */
void
DbObjectItem::removeArrowItems()
{
    for (QList<ArrowItem *>::iterator iter = mArrowItems.begin(); iter != mArrowItems.end(); ++iter) {
        DbObjectItem *startItem = (*iter)->startItem();
        DbObjectItem *endItem = (*iter)->endItem();

        if (startItem) {
            startItem->removeArrowItem(*iter);
        }

        if (endItem && endItem != startItem) {
            endItem->removeArrowItem(*iter);
        }

        if (scene()) {
            scene()->removeItem(*iter);
        }
        // FIXME we must delete this item but program chashed in this case...
//        delete const_cast<ArrowItem*>(*iter);
//        mArrowItems.erase(iter);
        mArrowItems.removeOne(*iter);
    }
}

/*!
 * \overload
 */
void
DbObjectItem::mousePressEvent(QGraphicsSceneMouseEvent *iEvent)
{
    if (isRightBottomCorner(iEvent->pos())) {
        mMode = DbObjectItem::RIGHT_BOTTOM_CORNER_RESIZE;
    } else if (isLeftBottomCorner(iEvent->pos())) {
        mMode = DbObjectItem::LEFT_BOTTOM_CORNER_RESIZE;
    } else if (isLeftTopCorner(iEvent->pos())) {
        mMode = DbObjectItem::LEFT_TOP_CORNER_RESIZE;
    } else if (isRightTopCorner(iEvent->pos())) {
        mMode = DbObjectItem::RIGHT_TOP_CORNER_RESIZE;
    } else if (isLeftVerticalBorder(iEvent->pos())) {
        mMode = DbObjectItem::LEFT_VERTICAL_RESIZE;
    } else if (isRightVerticalBorder(iEvent->pos())) {
        mMode = DbObjectItem::RIGHT_VERTICAL_RESIZE;
    } else if (isBottomHorizontalBorder(iEvent->pos())) {
        mMode = DbObjectItem::BOTTOM_HORIZONTAL_RESIZE;
    } else if (isTopHorizontalBorder(iEvent->pos())) {
        mMode = DbObjectItem::TOP_HORIZONTAL_RESIZE;
    } else {
        mMode = DbObjectItem::MOVE;
    }

    QGraphicsItem::mousePressEvent(iEvent);
}

/*!
 * \overload
 */
void
DbObjectItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *iEvent)
{
    QGraphicsItem::mouseReleaseEvent(iEvent);
    if (Control::Config().alignToGrid()) {
        moveBy(-(int)pos().x() % GraphicsScene::LOW_GRID_DX, -(int)pos().y() % GraphicsScene::LOW_GRID_DY);
    }
}

/*!
 * \overload
 */
void
DbObjectItem::hoverMoveEvent(QGraphicsSceneHoverEvent *iEvent)
{
    if (isRightBottomCorner(iEvent->pos())) {
        setCursor(Qt::SizeFDiagCursor);
    } else if (isLeftBottomCorner(iEvent->pos())) {
        setCursor(Qt::SizeBDiagCursor);
    } else if (isLeftTopCorner(iEvent->pos())) {
        setCursor(Qt::SizeFDiagCursor);
    } else if (isRightTopCorner(iEvent->pos())) {
        setCursor(Qt::SizeBDiagCursor);
    } else if (isLeftVerticalBorder(iEvent->pos())) {
        setCursor(Qt::SizeHorCursor);
    } else if (isRightVerticalBorder(iEvent->pos())) {
        setCursor(Qt::SizeHorCursor);
    } else if (isBottomHorizontalBorder(iEvent->pos())) {
        setCursor(Qt::SizeVerCursor);
    } else if (isTopHorizontalBorder(iEvent->pos())) {
        setCursor(Qt::SizeVerCursor);
    } else {
        setCursor(Qt::SizeAllCursor);
    }

    QGraphicsItem::hoverMoveEvent(iEvent);
}

/*!
 * \overload
 */
void
DbObjectItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *iEvent)
{
//    setCursor(Qt::OpenHandCursor);
//    setCursor(Qt::ArrowCursor);
    QGraphicsItem::hoverLeaveEvent(iEvent);
}

/*!
 * \overload
 * \brief Handler for mouse move event. Analyze a mode of an event and
 *        change the position of the item if mode = move or resize it if mode = resize
 */
void
DbObjectItem::mouseMoveEvent(QGraphicsSceneMouseEvent *iEvent)
{
    if (mMode == DbObjectItem::RIGHT_BOTTOM_CORNER_RESIZE) {
        if (iEvent->pos().x() - x() < MIN_WIDTH || iEvent->pos().y() - y() < MIN_HEIGHT) return;
        setWidth((int)iEvent->pos().x() - x());
        setHeight((int)iEvent->pos().y() - y());
        updatePolygon();
    } else if (mMode == DbObjectItem::LEFT_BOTTOM_CORNER_RESIZE) {
        if (iEvent->pos().x() - x() < MIN_WIDTH || height() + y() - iEvent->pos().y() < MIN_HEIGHT) return;
        setWidth((int)iEvent->pos().x() - x());
        setY((int)iEvent->pos().y());
        updatePolygon();
    } else if (mMode == DbObjectItem::LEFT_TOP_CORNER_RESIZE) {
        if (width() + x() - iEvent->pos().x() < MIN_WIDTH || height() + y() - iEvent->pos().y() < MIN_HEIGHT) return;
        setX((int)iEvent->pos().x());
        setY((int)iEvent->pos().y());
        updatePolygon();
    } else if (mMode == DbObjectItem::RIGHT_TOP_CORNER_RESIZE) {
        if (width() + x() - iEvent->pos().x() < MIN_WIDTH || iEvent->pos().y() - y() < MIN_HEIGHT) return;
        setX((int)iEvent->pos().x());
        setHeight((int)iEvent->pos().y() - y());
        updatePolygon();
    } else if (mMode == DbObjectItem::LEFT_VERTICAL_RESIZE) {
        if (width() + x() - iEvent->pos().x() < MIN_WIDTH) return;
        setX((int)iEvent->pos().x());
        updatePolygon();
    } else if (mMode == DbObjectItem::RIGHT_VERTICAL_RESIZE) {
        if (iEvent->pos().x() - x() < MIN_WIDTH) return;
        setWidth((int)iEvent->pos().x() - x());
        updatePolygon();
    } else if (mMode == DbObjectItem::BOTTOM_HORIZONTAL_RESIZE) {
        if (iEvent->pos().y() - y() < MIN_HEIGHT) return;
        setHeight((int)iEvent->pos().y() - y());
        updatePolygon();
    } else if (mMode == DbObjectItem::TOP_HORIZONTAL_RESIZE) {
        if (height() + y() - iEvent->pos().y() < MIN_HEIGHT) return;
        setY((int)iEvent->pos().y());
        updatePolygon();
    } else {
        QGraphicsItem::mouseMoveEvent(iEvent);
//        qDebug() << rect() << scenePos();
        foreach (ArrowItem *arrow, mArrowItems) {
            arrow->updatePosition();
        }
    }
//    QGraphicsItem::mouseMoveEvent(iEvent);
}

/*!
 * \brief According to the given flag show or hide fields' types
 *
 * \param[in] iFlag - True if we want to show field types, false otherwise
 */
void
DbObjectItem::setFieldsTypesVisible(bool iFlag)
{
    mFieldsTypesVisible = iFlag;
    update(x(), y(), width(), height());
}

/*!
 * \brief Get all arrows related to this item
 *
 * \return List of arrows connected to or connected with this item
 */
QList<ArrowItem *>
DbObjectItem::arrows() const
{
    return mArrowItems;
}

/*!
 *
 */
void
DbObjectItem::paintAnchor(QPainter *iPainter)
{
    // if anchor was setted for this table - draw the anchor
    if (!(flags() & QGraphicsItem::ItemIsMovable)) {
        QRectF target(x() + width() - IMG_WIDTH - INTERVAL, y() + height() - IMG_HEIGHT - INTERVAL, IMG_WIDTH, IMG_HEIGHT);
        QRectF source(0.0, 0.0, mAnchorImage->width(), mAnchorImage->height());
        iPainter->drawImage(target, *mAnchorImage, source);
    }
}

/*!
 * \brief Convert item to the xml format
 *
 * \param[in] iDoc - Xml document
 * \param[in] elementName - Element name in the xml
 *
 * \return Xml dom element with item information into it
 */
QDomElement
DbObjectItem::toXml(QDomDocument &iDoc, const QString &elementName) const
{
    QDomElement element = iDoc.createElement(elementName);
    element.setAttribute("schema", schemaName());
    element.setAttribute("name", name());
    QPointF point = mapToScene(QPointF(x(), y()));
    element.setAttribute("x", (int)point.x());
    element.setAttribute("y", (int)point.y());
    element.setAttribute("width", width());
    element.setAttribute("height", height());
    element.setAttribute("red", itemColor().red());
    element.setAttribute("green", itemColor().green());
    element.setAttribute("blue", itemColor().blue());
    return element;
}

/*!
 * \brief Set the seek
 *
 * \param[in] iSeek - Item seek
 */
void
DbObjectItem::setSeek(int iSeek)
{
    mSeek = iSeek;
}

/*!
 * \brief Get the seek
 *
 * \return Item seek
 */
int
DbObjectItem::seek()
{
    return mSeek;
}

/*!
 * \brief Get mouse mode (resizing or movement)
 *
 * \return Mouse mode
 */
DbObjectItem::Mode
DbObjectItem::mouseMode() const
{
    return mMode;
}

/*!
 * \brief Convert input item to DbObject
 *
 * \param[in] iItem - Item we will convert
 *
 * \return Converted to DbObject item of 0
 */
DbObjectItem *
toDbObject(QGraphicsItem *iItem)
{
//    return qgraphicsitem_cast<DbObjectItem *>(iItem);
    return dynamic_cast<DbObjectItem *>(iItem);
}

}

}

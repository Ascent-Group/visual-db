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
#include <gui/DbObjectItem.h>
#include <gui/GraphicsScene.h>

int DbObjectItem::mSeek = 80;

/*!
 * Constructor
 */
DbObjectItem::DbObjectItem(QMenu *ipMenu)
    : GraphicsItem(), mFieldsTypesVisible(true), mContextMenu(ipMenu), mMode(MOVE)
{
}

/*!
 * Destructor
 */
DbObjectItem::~DbObjectItem()
{
}

/*!
 * \overload
 */
void
DbObjectItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *ipEvent)
{
    if (mContextMenu) {
        setSelected(true);
        // show context menu
        mContextMenu->exec(ipEvent->screenPos());
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
 * \param[in] ipArrowItem - Arrow we will remove
 */
void
DbObjectItem::removeArrowItem(ArrowItem *ipArrowItem)
{
    int index = mArrowItems.indexOf(ipArrowItem);
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
    for (QList<ArrowItem *>::const_iterator iter = mArrowItems.constBegin(); iter != mArrowItems.constEnd(); ++iter) {
        (*iter)->startItem()->removeArrowItem(*iter);
        (*iter)->endItem()->removeArrowItem(*iter);
        if (scene()) {
            scene()->removeItem(*iter);
        }
        // FIXME we must delete this item but program chashed in this case...
//        delete (*iter);
    }
}

/*!
 * \overload
 */
void
DbObjectItem::mousePressEvent(QGraphicsSceneMouseEvent *ipEvent)
{
    if (isRightBottomCorner(ipEvent->pos())) {
        mMode = DbObjectItem::RIGHT_BOTTOM_CORNER_RESIZE;
    } else if (isLeftBottomCorner(ipEvent->pos())) {
        mMode = DbObjectItem::LEFT_BOTTOM_CORNER_RESIZE;
    } else if (isLeftTopCorner(ipEvent->pos())) {
        mMode = DbObjectItem::LEFT_TOP_CORNER_RESIZE;
    } else if (isRightTopCorner(ipEvent->pos())) {
        mMode = DbObjectItem::RIGHT_TOP_CORNER_RESIZE;
    } else if (isLeftVerticalBorder(ipEvent->pos())) {
        mMode = DbObjectItem::LEFT_VERTICAL_RESIZE;
    } else if (isRightVerticalBorder(ipEvent->pos())) {
        mMode = DbObjectItem::RIGHT_VERTICAL_RESIZE;
    } else if (isBottomHorizontalBorder(ipEvent->pos())) {
        mMode = DbObjectItem::BOTTOM_HORIZONTAL_RESIZE;
    } else if (isTopHorizontalBorder(ipEvent->pos())) {
        mMode = DbObjectItem::TOP_HORIZONTAL_RESIZE;
    } else {
        mMode = DbObjectItem::MOVE;
    }
    
    QGraphicsItem::mousePressEvent(ipEvent);
}

/*!
 * \overload
 */
void
DbObjectItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *ipEvent)
{
    QGraphicsItem::mouseReleaseEvent(ipEvent);
    if (mSettings.value(Consts::VIEW_GRP + "/" + Consts::ALIGN_TO_GRID_SETTING, false).toBool()) {
        moveBy(-(int)pos().x() % GraphicsScene::LOW_GRID_DX, -(int)pos().y() % GraphicsScene::LOW_GRID_DY);
    }
}

/*!
 * \overload
 */
void
DbObjectItem::hoverMoveEvent(QGraphicsSceneHoverEvent *ipEvent)
{
    if (isRightBottomCorner(ipEvent->pos())) {
        setCursor(Qt::SizeFDiagCursor);
    } else if (isLeftBottomCorner(ipEvent->pos())) {
        setCursor(Qt::SizeBDiagCursor);
    } else if (isLeftTopCorner(ipEvent->pos())) {
        setCursor(Qt::SizeFDiagCursor);
    } else if (isRightTopCorner(ipEvent->pos())) {
        setCursor(Qt::SizeBDiagCursor);
    } else if (isLeftVerticalBorder(ipEvent->pos())) {
        setCursor(Qt::SizeHorCursor);
    } else if (isRightVerticalBorder(ipEvent->pos())) {
        setCursor(Qt::SizeHorCursor);
    } else if (isBottomHorizontalBorder(ipEvent->pos())) {
        setCursor(Qt::SizeVerCursor);
    } else if (isTopHorizontalBorder(ipEvent->pos())) {
        setCursor(Qt::SizeVerCursor);
    } else {
        setCursor(Qt::SizeAllCursor);
    }

    QGraphicsItem::hoverMoveEvent(ipEvent);
}

/*!
 * \overload
 */
void
DbObjectItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *)
{
    setCursor(Qt::ArrowCursor);
}

/*!
 * \overload
 * \brief Handler for mouse move event. Analyze a mode of an event and
 *        change the position of the item if mode = move or resize it if mode = resize
 */
void
DbObjectItem::mouseMoveEvent(QGraphicsSceneMouseEvent *ipEvent)
{
    if (mMode == DbObjectItem::RIGHT_BOTTOM_CORNER_RESIZE) {
        if (ipEvent->pos().x() - x() < MIN_WIDTH || ipEvent->pos().y() - y() < MIN_HEIGHT) return;
        setWidth((int)ipEvent->pos().x() - x());
        setHeight((int)ipEvent->pos().y() - y());
        updatePolygon();
    } else if (mMode == DbObjectItem::LEFT_BOTTOM_CORNER_RESIZE) {
        if (ipEvent->pos().x() - x() < MIN_WIDTH || height() + y() - ipEvent->pos().y() < MIN_HEIGHT) return;
        setWidth((int)ipEvent->pos().x() - x());
        setY((int)ipEvent->pos().y());
        updatePolygon();
    } else if (mMode == DbObjectItem::LEFT_TOP_CORNER_RESIZE) {
        if (width() + x() - ipEvent->pos().x() < MIN_WIDTH || height() + y() - ipEvent->pos().y() < MIN_HEIGHT) return;
        setX((int)ipEvent->pos().x());
        setY((int)ipEvent->pos().y());
        updatePolygon();
    } else if (mMode == DbObjectItem::RIGHT_TOP_CORNER_RESIZE) {
        if (width() + x() - ipEvent->pos().x() < MIN_WIDTH || ipEvent->pos().y() - y() < MIN_HEIGHT) return;
        setX((int)ipEvent->pos().x());
        setHeight((int)ipEvent->pos().y() - y());
        updatePolygon();
    } else if (mMode == DbObjectItem::LEFT_VERTICAL_RESIZE) {
        if (width() + x() - ipEvent->pos().x() < MIN_WIDTH) return;
        setX((int)ipEvent->pos().x());
        updatePolygon();
    } else if (mMode == DbObjectItem::RIGHT_VERTICAL_RESIZE) {
        if (ipEvent->pos().x() - x() < MIN_WIDTH) return;
        setWidth((int)ipEvent->pos().x() - x());
        updatePolygon();
    } else if (mMode == DbObjectItem::BOTTOM_HORIZONTAL_RESIZE) {
        if (ipEvent->pos().y() - y() < MIN_HEIGHT) return;
        setHeight((int)ipEvent->pos().y() - y());
        updatePolygon();
    } else if (mMode == DbObjectItem::TOP_HORIZONTAL_RESIZE) {
        if (height() + y() - ipEvent->pos().y() < MIN_HEIGHT) return;
        setY((int)ipEvent->pos().y());
        updatePolygon();
    } else {
        QGraphicsItem::mouseMoveEvent(ipEvent);
//        qDebug() << rect() << scenePos();
        foreach (ArrowItem *arrow, mArrowItems) {
            arrow->updatePosition();
        }
    }
//    QGraphicsItem::mouseMoveEvent(ipEvent);
}

/*!
 * \brief According to the given flag show or hide fields' types
 *
 * \param[in] ipFlag - True if we want to show field types, false otherwise
 */
void
DbObjectItem::setFieldsTypesVisible(bool ipFlag)
{
    // TODO: implement it according new class structure
//    for (int i = 0; i < countFields(); ++i) {
//        if (ipFlag) {
//            setFieldText(i, mModel->columnName(i) + ": " + mModel->columnType(i));
//        } else {
//            setFieldText(i, mModel->columnName(i));
//        }
//    }
//    mFieldsTypesVisible = ipFlag;
//    update(x(), y(), width(), height());
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
 * \brief Convert item to the xml format
 *
 * \param[in] ipDoc - Xml document
 * \param[in] elementName - Element name in the xml
 *
 * \return Xml dom element with item information into it
 */
QDomElement 
DbObjectItem::toXml(QDomDocument &ipDoc, const QString &elementName) const
{
    QDomElement element = ipDoc.createElement(elementName);
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
 * \param[in] ipSeek - Item seek
 */
void
DbObjectItem::setSeek(int ipSeek)
{
    mSeek = ipSeek;
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
 * \param[in] ipItem - Item we will convert
 *
 * \return Converted to DbObject item of 0
 */
DbObjectItem *
toDbObject(QGraphicsItem *ipItem)
{
//    return qgraphicsitem_cast<DbObjectItem *>(ipItem);
    return dynamic_cast<DbObjectItem *>(ipItem);
}

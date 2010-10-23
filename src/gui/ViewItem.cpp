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

#include <QDomDocument>
#include <QDomElement>
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsTextItem>
#include <QMenu>
#include <QPainter>
#include <QPen>
#include <QPixmap>
#include <QTextDocument>
#include <QtDebug>
#include <common/Database.h>
#include <common/DbIndex.h>
#include <common/DbSchema.h>
#include <common/DbView.h>
#include <consts.h>
#include <gui/ArrowItem.h>
#include <gui/GraphicsScene.h>
#include <gui/ViewItem.h>
#include <math.h>
#include <psql/View.h>

int ViewItem::mSeek = 80;

/*
 * Constructor
 */
ViewItem::ViewItem(const QString &ipSchemaName, const QString &ipViewName, QMenu *ipMenu, const QPoint &ipPos)
    : GraphicsItem(), mContextMenu(ipMenu), mMode(ViewItem::MOVE), mFieldsTypesVisible(true), mIndicesVisible(true), mFont("Arial", 10)
{
    using namespace DbObjects::Common;
    Database *dbInst = Database::instance();

    // find schema
    DbSchema *schema = dbInst->findSchema(ipSchemaName);

    // if foung
    if (schema) {
        // find view in this schema
        DbView *view = schema->findView(ipViewName);

        // if found
        if (view) {
            mViewModel = view;
        } else {
            qDebug() << "Cann't find this view: " << ipViewName;
            return;
        }
        // lyuts: maybe else we should do this
        // mViewModel = new PsqlView(ipSchemaName, ipViewName);
    } else {
        qDebug() << "Cann't find this schema: " << ipSchemaName;
        return;
    }

    // create title item
    setTitleItem(new QGraphicsTextItem(ipSchemaName.toUpper() + "." + ipViewName.toUpper()));

    // create field items
    // FIXME DbView is not implemented yet
//    for (int i = 0; i < mViewModel->columnsCount(); ++i) {
//        addFieldItem(new QGraphicsTextItem(mViewModel->columnName(i) + ": " + mViewModel->columnType(i)));
//    }
//
//    dbInst->findViewIndices(mViewModel, mIndices);
//    foreach (DbIndex *index, mIndices) {
//        addIndexItem(new QGraphicsTextItem(index->name()));
//    }

    // set left top point coordinates
    if (ipPos.x() == 0 && ipPos.y() == 0) {
        setX(ipPos.x() + mSeek);
        setY(ipPos.y() + mSeek);

        ViewItem::mSeek += SEEK_STEP;
    } else {
        setX(ipPos.x());
        setY(ipPos.y());
    }

    // set width and height
    setWidth(DEFAULT_WIDTH);

    // FIXME DbView is not implemented yet
//    mIndicesVisible = mSettings.value(Consts::PREFS_GRP + "/" + Consts::SHOW_INDICES_SETTING, false).toBool();
//    if (mIndicesVisible) {
//        setHeight((mViewModel->columnsCount() + mIndices.count() + 1) * (FIELD_HEIGHT + INTERVAL) + INTERVAL * 3);
//    } else {
//        setHeight((mViewModel->columnsCount() + 1) * (FIELD_HEIGHT + INTERVAL) + INTERVAL * 3);
//    }
    setHeight((FIELD_HEIGHT + INTERVAL) + INTERVAL * 2);

    updatePolygon();

    // set the color of the view
    setBrush(itemColor());
    setPen(borderColor());

    setZValue(0);

    // allow selecting and moving of the view
    setAcceptsHoverEvents(true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    // preload of images
    mViewImage = new QImage(":/img/view.png");
    mKeyImage = new QImage(":/img/key.png");
    mForeignKeyImage = new QImage(":/img/foreignkey.png");
    mFieldImage = new QImage(":/img/field.png");
    mAnchorImage = new QImage(":/img/anchor.png");
}

/*
 * Destructor
 */
ViewItem::~ViewItem()
{
}

/*
 * Return the type
 */
int
ViewItem::type() const
{
    return Type;
}

/*
 *
 */
void
ViewItem::addIndexItem(QGraphicsTextItem *ipIndexItem)
{
    mIndexItems << ipIndexItem;
}

/*
 * Paint the view. Height is adjusting, but width is setted to default.
 */
void
ViewItem::paint(QPainter *ipPainter, const QStyleOptionGraphicsItem *ipItem, QWidget *ipWidget)
{
    // draw the board of the view
    QGraphicsPolygonItem::paint(ipPainter, ipItem, ipWidget);

    // fill title with a little darker color then another view
    ipPainter->fillRect((int)x() + 1, (int)y() + 1,
            (int)width() - 1, (int)y() + FIELD_HEIGHT + INTERVAL * 2 - (int)y() - 1,
            QColor(abs(itemColor().red() - 80), abs(itemColor().green() - 80), abs(itemColor().blue() - 80)));

    // draw line under the title
    ipPainter->drawLine((int)x(), (int)y() + FIELD_HEIGHT + INTERVAL * 2,
            (int)(x() + width()), (int)y() + FIELD_HEIGHT + INTERVAL * 2);

    // set the color of painting
    ipPainter->setPen(fontColor());
    ipPainter->setFont(mFont);

    // draw image for view
    QRectF target((int)x() + INTERVAL, (int)y() + INTERVAL,
            IMG_HEIGHT + INTERVAL, IMG_HEIGHT + INTERVAL);
    QRectF source(0.0, 0.0, mViewImage->width(), mViewImage->height());
    ipPainter->drawImage(target, *mViewImage, source);

    // draw the title aligned on the center in upper case
    ipPainter->drawText((int)x() + IMG_WIDTH + 2 * INTERVAL, (int)y() + INTERVAL,
            (int)width() - IMG_WIDTH - INTERVAL * 3, FIELD_HEIGHT + INTERVAL,
            Qt::AlignCenter,
            titleText());

    // row in the graphic view (some items may be missed)
    // draw each field
    for (int i = 0; i < countFields(); ++i) {
        // break drawing if we have reached the board
        if (height() < (FIELD_HEIGHT + INTERVAL) * (i + 2) + INTERVAL) {
            break;
        }

        QImage *image = 0;
        // draw image for primary key field with margins = INTERVAL for top, bottom, left and right sizes
        // FIXME DbView is not implemented yet
//        if (mViewModel->isColumnPrimaryKey(i)) {
//            image = mKeyImage;
//        } else if (mViewModel->isColumnForeignKey(i)) {
//            image = mForeignKeyImage;
//        }
        if (image) {
            QRectF target((int)x() + INTERVAL, (int)y() + (FIELD_HEIGHT + INTERVAL) * (i + 1) + INTERVAL,
                    IMG_WIDTH + INTERVAL, IMG_HEIGHT + INTERVAL);
            QRectF source(0.0, 0.0, image->width(), image->height());
            ipPainter->drawImage(target, *image, source);
        }

        // draw field name with margins = INTERVAL for top, bottom, left and right sizes
        ipPainter->drawText((int)x() + IMG_WIDTH + 2 * INTERVAL, (int)y() + (FIELD_HEIGHT + INTERVAL) * (i + 1) + INTERVAL,
                (int)width() - IMG_WIDTH - INTERVAL * 3, FIELD_HEIGHT + INTERVAL * 2,
                Qt::AlignLeft,
                fieldText(i));
    }

    // if we need to show indeces
    // FIXME DbView is not implemented yet
//    if (mIndicesVisible) {
//        for (int i = 0; i < mIndexItems.size(); ++i) {
//            // break drawing if we have reached the board
//            if (height() < (FIELD_HEIGHT + INTERVAL) * (countFields() + i + 2) + INTERVAL) {
//                break;
//            }
//            ipPainter->drawText((int)x() + IMG_WIDTH + 2 * INTERVAL,
//                    (int)y() + (FIELD_HEIGHT + INTERVAL) * (countFields() + i + 1) + INTERVAL,
//                    (int)width() - IMG_WIDTH - INTERVAL * 3,
//                    FIELD_HEIGHT + INTERVAL * 2, Qt::AlignLeft,
//                    mIndexItems.at(i)->toPlainText());
//        }
//    }

    // if anchor was setted for this view - draw the anchor
    if (!(flags() & QGraphicsItem::ItemIsMovable)) {
        QRectF target(x() + width() - IMG_WIDTH - INTERVAL, y() + height() - IMG_HEIGHT - INTERVAL, IMG_WIDTH, IMG_HEIGHT);
        QRectF source(0.0, 0.0, mAnchorImage->width(), mAnchorImage->height());
        ipPainter->drawImage(target, *mAnchorImage, source);
    }
}

/*
 * Handler of the right mouse button click
 */
void
ViewItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *ipEvent)
{
    setSelected(true);
    // show context menu
    mContextMenu->exec(ipEvent->screenPos());
}

/*
 * Add given arrow to the list of arrows related with this view
 */
void
ViewItem::addArrowItem(ArrowItem *arrow)
{
    mArrowItems.append(arrow);
}

/*
 * Remove given arrow from the list of arrows related with this view
 */
void
ViewItem::removeArrowItem(ArrowItem *ipArrowItem)
{
    int index = mArrowItems.indexOf(ipArrowItem);
    if (index != -1) {
        mArrowItems.removeAt(index);
    }
}

/*
 * Remove all arrows from the list of arrows related with this view
 */
void
ViewItem::removeArrowItems()
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

/*
 * Handler for item change event
 */
QVariant
ViewItem::itemChange(GraphicsItemChange ipChange, const QVariant &ipValue)
{
    // if we change the position of the view - redraw all related arrows
    if (ipChange == QGraphicsItem::ItemPositionChange) {
        foreach (ArrowItem *arrow, mArrowItems) {
            arrow->updatePosition();
        }
    }

    return ipValue;
}

/*
 * Handler for a mouse press event. Analyze a position of an event and change a mode according to it
 */
void
ViewItem::mousePressEvent(QGraphicsSceneMouseEvent *ipEvent)
{
    if (isRightBottomCorner(ipEvent->pos())) {
        mMode = ViewItem::RIGHT_BOTTOM_CORNER_RESIZE;
    } else if (isLeftBottomCorner(ipEvent->pos())) {
        mMode = ViewItem::LEFT_BOTTOM_CORNER_RESIZE;
    } else if (isLeftTopCorner(ipEvent->pos())) {
        mMode = ViewItem::LEFT_TOP_CORNER_RESIZE;
    } else if (isRightTopCorner(ipEvent->pos())) {
        mMode = ViewItem::RIGHT_TOP_CORNER_RESIZE;
    } else if (isLeftVerticalBorder(ipEvent->pos())) {
        mMode = ViewItem::LEFT_VERTICAL_RESIZE;
    } else if (isRightVerticalBorder(ipEvent->pos())) {
        mMode = ViewItem::RIGHT_VERTICAL_RESIZE;
    } else if (isBottomHorizontalBorder(ipEvent->pos())) {
        mMode = ViewItem::BOTTOM_HORIZONTAL_RESIZE;
    } else if (isTopHorizontalBorder(ipEvent->pos())) {
        mMode = ViewItem::TOP_HORIZONTAL_RESIZE;
    } else {
        mMode = ViewItem::MOVE;
    }

    //    setZValue(1);

    QGraphicsItem::mousePressEvent(ipEvent);
}

/*
 * Handler for mouse realease event
 */
void
ViewItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *ipEvent)
{
    //    setZValue(0);

    QGraphicsItem::mouseReleaseEvent(ipEvent);
    if (mSettings.value(Consts::VIEW_GRP + "/" + Consts::ALIGN_TO_GRID_SETTING, false).toBool()) {
        moveBy(-(int)pos().x() % GraphicsScene::LOW_GRID_DX, -(int)pos().y() % GraphicsScene::LOW_GRID_DY);
    }
}

/*
 * Handler for mouse hover event. Analyze a position of an event and change a cursor
 */
void
ViewItem::hoverMoveEvent(QGraphicsSceneHoverEvent *ipEvent)
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

/*
 * Handle for mouse hover leave event
 */
void
ViewItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *)
{
    setCursor(Qt::ArrowCursor);
}

/*
 * Handler for mouse move event. Analyze a mode of an event and change the position of the view if mode = move or resize it if mode = resize
 */
void
ViewItem::mouseMoveEvent(QGraphicsSceneMouseEvent *ipEvent)
{
    if (mMode == ViewItem::RIGHT_BOTTOM_CORNER_RESIZE) {
        if (ipEvent->pos().x() - x() < MIN_WIDTH || ipEvent->pos().y() - y() < MIN_HEIGHT) return;
        setWidth((int)ipEvent->pos().x() - x());
        setHeight((int)ipEvent->pos().y() - y());
        updatePolygon();
    } else if (mMode == ViewItem::LEFT_BOTTOM_CORNER_RESIZE) {
        if (ipEvent->pos().x() - x() < MIN_WIDTH || height() + y() - ipEvent->pos().y() < MIN_HEIGHT) return;
        setWidth((int)ipEvent->pos().x() - x());
        setY((int)ipEvent->pos().y());
        updatePolygon();
    } else if (mMode == ViewItem::LEFT_TOP_CORNER_RESIZE) {
        if (width() + x() - ipEvent->pos().x() < MIN_WIDTH || height() + y() - ipEvent->pos().y() < MIN_HEIGHT) return;
        setX((int)ipEvent->pos().x());
        setY((int)ipEvent->pos().y());
        updatePolygon();
    } else if (mMode == ViewItem::RIGHT_TOP_CORNER_RESIZE) {
        if (width() + x() - ipEvent->pos().x() < MIN_WIDTH || ipEvent->pos().y() - y() < MIN_HEIGHT) return;
        setX((int)ipEvent->pos().x());
        setHeight((int)ipEvent->pos().y() - y());
        updatePolygon();
    } else if (mMode == ViewItem::LEFT_VERTICAL_RESIZE) {
        if (width() + x() - ipEvent->pos().x() < MIN_WIDTH) return;
        setX((int)ipEvent->pos().x());
        updatePolygon();
    } else if (mMode == ViewItem::RIGHT_VERTICAL_RESIZE) {
        if (ipEvent->pos().x() - x() < MIN_WIDTH) return;
        setWidth((int)ipEvent->pos().x() - x());
        updatePolygon();
    } else if (mMode == ViewItem::BOTTOM_HORIZONTAL_RESIZE) {
        if (ipEvent->pos().y() - y() < MIN_HEIGHT) return;
        setHeight((int)ipEvent->pos().y() - y());
        updatePolygon();
    } else if (mMode == ViewItem::TOP_HORIZONTAL_RESIZE) {
        if (height() + y() - ipEvent->pos().y() < MIN_HEIGHT) return;
        setY((int)ipEvent->pos().y());
        updatePolygon();
    } else {
        QGraphicsItem::mouseMoveEvent(ipEvent);
    }
}

/*
 * According to the given flag show or hide fields' types
 */
void
ViewItem::setFieldsTypesVisible(bool ipFlag)
{
    // FIXME DbView is not implemented yet
//    for (int i = 0; i < countFields(); ++i) {
//        if (ipFlag) {
//            setFieldText(i, mViewModel->columnName(i) + ": " + mViewModel->columnType(i));
//        } else {
//            setFieldText(i, mViewModel->columnName(i));
//        }
//    }
    mFieldsTypesVisible = ipFlag;
    update(x(), y(), width(), height());
}

/*
 * Show or hide indices
 */
void
ViewItem::setIndicesVisible(bool ipFlag)
{
    mIndicesVisible = ipFlag;
    update(x(), y(), width(), height());
}

/*
 * Get the name of the schema
 */
QString
ViewItem::schemaName() const
{
    // FIXME DbView is not implemented yet
//    return mViewModel->schemaName();
    return "";
}

/*
 * Get the name of the view item
 */
QString
ViewItem::viewName() const
{
    return mViewModel->name();
}

/*
 * Get the view model of this item
 */
DbObjects::Common::DbView *
ViewItem::viewModel() const
{
    return mViewModel;
}

/*
 * Get all arrows related to this view
 */
QList<ArrowItem *>
ViewItem::arrows() const
{
    return mArrowItems;
}

/*
 * Set the seek
 */
void
ViewItem::setSeek(int ipSeek)
{
    mSeek = ipSeek;
}

/*
 * Create the xml represantation for the view
 */
QDomElement
ViewItem::toXml(QDomDocument &ipDoc)
{
    QDomElement element = ipDoc.createElement("view");
    element.setAttribute("schema", schemaName());
    element.setAttribute("name", viewName());
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

/*
 * Check if input graphics item is view item
 */
bool
isView(QGraphicsItem *ipItem)
{
    return qgraphicsitem_cast<ViewItem *>(ipItem) != 0;
}

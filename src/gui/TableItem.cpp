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
#include <common/DbTable.h>
#include <gui/ArrowItem.h>
#include <gui/GraphicsScene.h>
#include <gui/TableItem.h>
#include <math.h>
#include <mysql/MysqlTable.h>
#include <psql/PsqlTable.h>

int TableItem::mSeek = 80;

/*
 * Constructor
 */
TableItem::TableItem(const QString &ipSchemaName, const QString &ipTableName, QMenu *ipMenu)
    : GraphicsItem(), mContextMenu(ipMenu), mMode(TableItem::MOVE), mFieldsTypesVisible(true), mIndicesVisible(true), mFont("Arial", 10)
{
    Database *dbInst = Database::instance();

    // find schema
    DbSchema *schema = dbInst->findSchema(ipSchemaName);

    // if foung
    if (schema) {
        // find table in this schema
        DbTable *table = schema->findTable(ipTableName);

        // if found
        if (table) {
            mTableModel = table;
        } else {
        qDebug() << "Cann't find this table: " << ipTableName;
        return;
    }
        // lyuts: maybe else we should do this
        // mTableModel = new PsqlTable(ipSchemaName, ipTableName);
    } else {
    qDebug() << "Cann't find this schema: " << ipSchemaName;
    return;
    }

    // create title item
    setTitleItem(new QGraphicsTextItem(ipSchemaName.toUpper() + "." + ipTableName.toUpper()));

    // create field items
    for (int i = 0; i < mTableModel->columnsCount(); ++i) {
    addFieldItem(new QGraphicsTextItem(mTableModel->columnName(i) + ": " + mTableModel->columnType(i)));
    }

    dbInst->findTableIndices(mTableModel, mIndices);
    foreach (DbIndex *index, mIndices) {
    addIndexItem(new QGraphicsTextItem(index->name()));
    }

    // set left top point coordinates
    setX(mSeek);
    setY(mSeek);

    // set width and height
    setWidth(DEFAULT_WIDTH);

    mIndicesVisible = mSettings.value("Preferences/ShowIndices", false).toBool();
    if (mIndicesVisible) {
        setHeight((mTableModel->columnsCount() + mIndices.count() + 1) * (FIELD_HEIGHT + INTERVAL) + INTERVAL * 3);
    } else {
    setHeight((mTableModel->columnsCount() + 1) * (FIELD_HEIGHT + INTERVAL) + INTERVAL * 3);
    }

    TableItem::mSeek += 20;

    updatePolygon();

    // set the color of the table
    setBrush(itemColor());
    setPen(borderColor());

    setZValue(0);

    // allow selecting and moving of the table
    setAcceptsHoverEvents(true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    // preload of images
    mTableImage = new QImage(":/img/table.png");
    mKeyImage = new QImage(":/img/key.png");
    mForeignKeyImage = new QImage(":/img/foreignkey.png");
    mFieldImage = new QImage(":/img/field.png");
    mAnchorImage = new QImage(":/img/anchor.png");
}

/*
 * Destructor
 */
TableItem::~TableItem()
{
}

/*
 * Return the type
 */
int
TableItem::type() const
{
    return Type;
}

/*
 * 
 */
void
TableItem::addIndexItem(QGraphicsTextItem *ipIndexItem)
{
    mIndexItems << ipIndexItem;
}

/*
 * Paint the table. Height is adjusting, but width is setted to default.
 */
void
TableItem::paint(QPainter *ipPainter, const QStyleOptionGraphicsItem *ipItem, QWidget *ipWidget) 
{
    // draw the board of the table
    QGraphicsPolygonItem::paint(ipPainter, ipItem, ipWidget);

    // fill title with a little darker color then another table
    ipPainter->fillRect((int)x() + 1, (int)y() + 1,
        (int)width() - 1, (int)y() + FIELD_HEIGHT + INTERVAL * 2 - (int)y() - 1, 
        QColor(abs(itemColor().red() - 80), abs(itemColor().green() - 80), abs(itemColor().blue() - 80)));

    // draw line under the title
    ipPainter->drawLine((int)x(), (int)y() + FIELD_HEIGHT + INTERVAL * 2,
        (int)(x() + width()), (int)y() + FIELD_HEIGHT + INTERVAL * 2);

    // set the color of painting
    ipPainter->setPen(fontColor());
    ipPainter->setFont(mFont);

    // draw image for table
    QRectF target((int)x() + INTERVAL, (int)y() + INTERVAL,
        IMG_HEIGHT + INTERVAL, IMG_HEIGHT + INTERVAL);
    QRectF source(0.0, 0.0, mTableImage->width(), mTableImage->height());
    ipPainter->drawImage(target, *mTableImage, source);

    // draw the title aligned on the center in upper case
    ipPainter->drawText((int)x() + IMG_WIDTH + 2 * INTERVAL, (int)y() + INTERVAL, 
        (int)width() - IMG_WIDTH - INTERVAL * 3, FIELD_HEIGHT + INTERVAL, 
        Qt::AlignCenter,
        titleText());

    // row in the graphic table (some items may be missed)
    // draw each field
    for (int i = 0; i < countFields(); ++i) {
    // break drawing if we have reached the board
    if (height() < (FIELD_HEIGHT + INTERVAL) * (i + 2) + INTERVAL) {
        break;
    }
    
    QImage *image = 0;
    // draw image for primary key field with margins = INTERVAL for top, bottom, left and right sizes
    if (mTableModel->isColumnPrimaryKey(i)) {
        image = mKeyImage;
    } else if (mTableModel->isColumnForeignKey(i)) {
        image = mForeignKeyImage;
    }
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
    if (mIndicesVisible) {
    for (int i = 0; i < mIndexItems.size(); ++i) {
        // break drawing if we have reached the board
        if (height() < (FIELD_HEIGHT + INTERVAL) * (countFields() + i + 2) + INTERVAL) {
        break;
        }
        ipPainter->drawText((int)x() + IMG_WIDTH + 2 * INTERVAL, 
            (int)y() + (FIELD_HEIGHT + INTERVAL) * (countFields() + i + 1) + INTERVAL, 
            (int)width() - IMG_WIDTH - INTERVAL * 3, 
            FIELD_HEIGHT + INTERVAL * 2, Qt::AlignLeft,
            mIndexItems.at(i)->toPlainText());
    }
    }

    // if anchor was setted for this table - draw the anchor
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
TableItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *ipEvent)
{
    setSelected(true);
    // show context menu
    mContextMenu->exec(ipEvent->screenPos());
}

/*
 * Add given arrow to the list of arrows related with this table
 */
void
TableItem::addArrowItem(ArrowItem *arrow)
{
    mArrowItems.append(arrow);
}

/*
 * Remove given arrow from the list of arrows related with this table
 */
void
TableItem::removeArrowItem(ArrowItem *ipArrowItem)
{
    int index = mArrowItems.indexOf(ipArrowItem);
    if (index != -1) {
    mArrowItems.removeAt(index);
    }
}

/*
 * Remove all arrows from the list of arrows related with this table
 */
void
TableItem::removeArrowItems()
{
    foreach (ArrowItem *arrow, mArrowItems) {
    // this way to handle self-referenced tables
    if (arrow->scene() && arrow) {
        arrow->startItem()->removeArrowItem(arrow);
        arrow->endItem()->removeArrowItem(arrow);
        scene()->removeItem(arrow);
        delete arrow;
    }
    }
}

/*
 * Handler for item change event
 */
QVariant
TableItem::itemChange(GraphicsItemChange ipChange, const QVariant &ipValue)
{
    // if we change the position of the table - redraw all related arrows
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
TableItem::mousePressEvent(QGraphicsSceneMouseEvent *ipEvent)
{
    if (isRightBottomCorner(ipEvent->pos().x(), ipEvent->pos().y())) {
        mMode = TableItem::RIGHT_BOTTOM_CORNER_RESIZE;
    } else if (isLeftBottomCorner(ipEvent->pos().x(), ipEvent->pos().y())) {
        mMode = TableItem::LEFT_BOTTOM_CORNER_RESIZE;
    } else if (isLeftTopCorner(ipEvent->pos().x(), ipEvent->pos().y())) {
        mMode = TableItem::LEFT_TOP_CORNER_RESIZE;
    } else if (isRightTopCorner(ipEvent->pos().x(), ipEvent->pos().y())) {
        mMode = TableItem::RIGHT_TOP_CORNER_RESIZE;
    } else if (isLeftVerticalBorder(ipEvent->pos().x())) {
        mMode = TableItem::LEFT_VERTICAL_RESIZE;
    } else if (isRightVerticalBorder(ipEvent->pos().x())) {
        mMode = TableItem::RIGHT_VERTICAL_RESIZE;
    } else if (isBottomHorizontalBorder(ipEvent->pos().y())) {
        mMode = TableItem::BOTTOM_HORIZONTAL_RESIZE;
    } else if (isTopHorizontalBorder(ipEvent->pos().y())) {
        mMode = TableItem::TOP_HORIZONTAL_RESIZE;
    } else {
        mMode = TableItem::MOVE;
    }

//    setZValue(1);

    QGraphicsItem::mousePressEvent(ipEvent);
}

/*
 * Handler for mouse realease event
 */
void
TableItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *ipEvent)
{
//    setZValue(0);

    QGraphicsItem::mouseReleaseEvent(ipEvent);
    if (mSettings.value("View/AlignToGrid", false).toBool()) {
    moveBy(-(int)pos().x() % GraphicsScene::LOW_GRID_DX, -(int)pos().y() % GraphicsScene::LOW_GRID_DY);
    }
}

/*
 * Handler for mouse hover event. Analyze a position of an event and change a cursor
 */
void
TableItem::hoverMoveEvent(QGraphicsSceneHoverEvent *ipEvent)
{
    if (isRightBottomCorner(ipEvent->pos().x(), ipEvent->pos().y())) {
    setCursor(Qt::SizeFDiagCursor);
    } else if (isLeftBottomCorner(ipEvent->pos().x(), ipEvent->pos().y())) {
    setCursor(Qt::SizeBDiagCursor);
    } else if (isLeftTopCorner(ipEvent->pos().x(), ipEvent->pos().y())) {
    setCursor(Qt::SizeFDiagCursor);
    } else if (isRightTopCorner(ipEvent->pos().x(), ipEvent->pos().y())) {
    setCursor(Qt::SizeBDiagCursor);
    } else if (isLeftVerticalBorder(ipEvent->pos().x())) {
    setCursor(Qt::SizeHorCursor);
    } else if (isRightVerticalBorder(ipEvent->pos().x())) {
    setCursor(Qt::SizeHorCursor);
    } else if (isBottomHorizontalBorder(ipEvent->pos().y())) {
    setCursor(Qt::SizeVerCursor);
    } else if (isTopHorizontalBorder(ipEvent->pos().y())) {
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
TableItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *)
{
    setCursor(Qt::ArrowCursor);
}

/*
 * Handler for mouse move event. Analyze a mode of an event and change the position of the table if mode = move or resize it if mode = resize
 */
void
TableItem::mouseMoveEvent(QGraphicsSceneMouseEvent *ipEvent)
{
    if (mMode == TableItem::RIGHT_BOTTOM_CORNER_RESIZE) {
    if (ipEvent->pos().x() - x() < MIN_WIDTH || ipEvent->pos().y() - y() < MIN_HEIGHT) return;
    setWidth((int)ipEvent->pos().x() - x());
    setHeight((int)ipEvent->pos().y() - y());
    updatePolygon();
    } else if (mMode == TableItem::LEFT_BOTTOM_CORNER_RESIZE) {
    if (ipEvent->pos().x() - x() < MIN_WIDTH || height() + y() - ipEvent->pos().y() < MIN_HEIGHT) return;
    setWidth((int)ipEvent->pos().x() - x());
    setY((int)ipEvent->pos().y());
    updatePolygon();
    } else if (mMode == TableItem::LEFT_TOP_CORNER_RESIZE) {
    if (width() + x() - ipEvent->pos().x() < MIN_WIDTH || height() + y() - ipEvent->pos().y() < MIN_HEIGHT) return;
    setX((int)ipEvent->pos().x());
    setY((int)ipEvent->pos().y());
    updatePolygon();
    } else if (mMode == TableItem::RIGHT_TOP_CORNER_RESIZE) {
    if (width() + x() - ipEvent->pos().x() < MIN_WIDTH || ipEvent->pos().y() - y() < MIN_HEIGHT) return;
    setX((int)ipEvent->pos().x());
    setHeight((int)ipEvent->pos().y() - y());
    updatePolygon();
    } else if (mMode == TableItem::LEFT_VERTICAL_RESIZE) {
    if (width() + x() - ipEvent->pos().x() < MIN_WIDTH) return;
    setX((int)ipEvent->pos().x());
    updatePolygon();
    } else if (mMode == TableItem::RIGHT_VERTICAL_RESIZE) {
    if (ipEvent->pos().x() - x() < MIN_WIDTH) return;
    setWidth((int)ipEvent->pos().x() - x());
    updatePolygon();
    } else if (mMode == TableItem::BOTTOM_HORIZONTAL_RESIZE) {
    if (ipEvent->pos().y() - y() < MIN_HEIGHT) return;
    setHeight((int)ipEvent->pos().y() - y());
    updatePolygon();
    } else if (mMode == TableItem::TOP_HORIZONTAL_RESIZE) {
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
TableItem::setFieldsTypesVisible(bool ipFlag)
{
    for (int i = 0; i < countFields(); ++i) {
    if (ipFlag) {
        setFieldText(i, mTableModel->columnName(i) + ": " + mTableModel->columnType(i));
    } else {
        setFieldText(i, mTableModel->columnName(i));
    }
    }
    mFieldsTypesVisible = ipFlag;
    update(x(), y(), width(), height());
/*
    if (ipFlag) {
    for (int i = 0; i < countFields(); ++i) {
        setFieldText(i, mTableModel->columnName(i) + ": " + mTableModel->columnType(i));
    }
    mFieldsTypesVisible = true;
    } else {
        for (int i = 0; i < countFields(); ++i) {
        setFieldText(i, mTableModel->columnName(i));
    }
    mFieldsTypesVisible = false;
    }
*/
}

/*
 * Show or hide indices
 */
void
TableItem::setIndicesVisible(bool ipFlag)
{
    mIndicesVisible = ipFlag;
    update(x(), y(), width(), height());
}

/*
 * Set the color of the table
 */
//void
//TableItem::setColor(QColor ipColor)
//{
//    mTableColor = ipColor;
//    setBrush(ipColor);
//
//    int red = mTableColor.red();
//    int green = mTableColor.green();
//    int blue = mTableColor.blue();
//    mFontColor = QColor(255 - red, 255 - green, 255 - blue);
//}

/*
 * Get the name of the schema
 */
QString
TableItem::schemaName() const
{
    return mTableModel->schemaName();
}

/*
 * Get the name of the table item
 */
QString
TableItem::tableName() const
{
    return mTableModel->name();
}

/*
 * Get the table model of this item
 */
DbTable *
TableItem::tableModel() const
{
    return mTableModel;
}

/*
 * Get all arrows related to this table
 */
QList<ArrowItem *>
TableItem::arrows() const
{
    return mArrowItems;
}

/*
 * Set the seek
 */
void
TableItem::setSeek(int ipSeek)
{
    mSeek = ipSeek;
}

/*
 * Adjust the size of the item
 */
void
TableItem::adjustSize()
{
    // find the maximum width among the item's fields
    qreal optimalWidth = MIN_WIDTH;
    if (optimalWidth < titleItem()->document()->idealWidth()) {
    optimalWidth = titleItem()->document()->idealWidth();
    }
    foreach (QGraphicsTextItem *fieldItem, fieldItems()) {
    if (optimalWidth < fieldItem->document()->idealWidth()) {
        optimalWidth = fieldItem->document()->idealWidth();
    }
    }

    // calculate optimal width
    optimalWidth += INTERVAL * 3 + IMG_WIDTH;
    // calculate optimal height
    int totalFields = fieldItems().size();
    if (mIndicesVisible) {
    totalFields += mIndexItems.size();
    }
    qreal optimalHeight = (totalFields + 1) * (FIELD_HEIGHT + INTERVAL) + INTERVAL * 2;

    // resize the item's size with optimal width and height
    setWidth(optimalWidth);
    setHeight(optimalHeight);
    updatePolygon();
}

/*
 * Create the xml represantation for the table
 */
QDomElement
TableItem::toXml(QDomDocument &ipDoc)
{
    QDomElement element = ipDoc.createElement("table");
    element.setAttribute("schema", schemaName());
    element.setAttribute("name", tableName());
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
 * Check if input graphics item is table item
 */
bool
isTable(QGraphicsItem *ipItem)
{
    return qgraphicsitem_cast<TableItem *>(ipItem) != 0;
}

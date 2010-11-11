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

#include <QColorDialog>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QMenu>
#include <QPainter>
#include <QPen>
#include <QPrinter>
#include <QTreeWidgetItem>
#include <common/Database.h>
#include <common/DbTable.h>
#include <consts.h>
#include <gui/ArrowItem.h>
#include <gui/ControlWidget.h>
#include <gui/DbObjectItem.h>
#include <gui/GraphicsScene.h>
#include <gui/Legend.h>
#include <gui/TableItem.h>
#include <gui/ItemGroup.h>
#include <gui/TreeWidget.h>
#include <gui/ViewItem.h>
#include <math.h>

#include <QDebug>

/*!
 * Constructor
 */
GraphicsScene::GraphicsScene()
: QGraphicsScene(), mSelectionPath(), mMoveMode(false), mOldPos(), mDiffX(0), mDiffY(0), mStartMovingTimer()
{
    setBackgroundBrush(QBrush(mSettings.value(Consts::COLOR_GRP + "/" + Consts::BACKGROUND_SETTING, Qt::white).value<QColor>()));
    setSceneRect(0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT);
    mLegend = new Legend();
    mLegend->setZValue(1000);

    connect(&mStartMovingTimer, SIGNAL(timeout()), this, SLOT(movingTimerExpired()));
}

/*!
 * Destructor
 */
GraphicsScene::~GraphicsScene()
{
}

/*!
 * @brief Set scene context menu
 *
 * @param[in] ipSchemeMenu - Context menu for schema
 */
void
GraphicsScene::setSchemeMenu(QMenu *ipSchemeMenu)
{
    mSchemeMenu = ipSchemeMenu;
}

/*!
 * @brief Set table context menu
 *
 * @param[in] ipTableMenu - Context menu for table
 */
void
GraphicsScene::setTableMenu(QMenu *ipTableMenu)
{
    mTableMenu = ipTableMenu;
}

//#include <QGraphicsItemAnimation>
//#include <QTimeLine>

/*!
 * @brief Add table from tree event to the scene
 *
 * @param[in] ipTreeItem - Tree item we need to show on the scene
 * @param[in] ipCol - Tree item column
 * @param[in] ipPos - Position where we should place new item
 *
 * @return List of added items
 */
QList<QGraphicsItem *>
GraphicsScene::showOnScene(QTreeWidgetItem *ipTreeItem, int ipCol, const QPoint &ipPos)
{
    QList<QGraphicsItem *> objectList;

    if (!ipTreeItem) {
        qDebug() << "[E][" << __func__ << "][" << __LINE__ << 
            "]Can't show selected items on the scene because input tree item is empty";   
        return objectList;
    }

    // get database object id
    int objId = ipTreeItem->text(TreeWidget::IdCol).toInt();

    // if schema or table or view were double clicked
    if (TreeWidget::SchemaItem == objId || TreeWidget::TableItem == objId || TreeWidget::ViewItem == objId) {
        // check whether item is a schema item
        if (TreeWidget::SchemaItem == objId) {
            // add all its table children to the scene
            for (int i = 0; i < ipTreeItem->childCount(); ++i) {
                if (TreeWidget::TableNode == ipTreeItem->child(i)->text(TreeWidget::IdCol).toInt() ||
                        TreeWidget::ViewNode == ipTreeItem->child(i)->text(TreeWidget::IdCol).toInt()) {
                    for (int j = 0; j < ipTreeItem->child(i)->childCount(); ++j) {
                        QPoint pos(ipPos.x() + j * SEEK_STEP, ipPos.y() + j * SEEK_STEP);
                        objectList << showOnScene(ipTreeItem->child(i)->child(j), /*TreeWidget::NameCol*/ipCol, pos);
                    }
                }
            }
            // don't add schema item itself
            return objectList;
        }

        if (TreeWidget::TableItem == objId) {
            TableItem *table = newTableItem(ipTreeItem->parent()->parent()->text(TreeWidget::NameCol),
                    ipTreeItem->text(TreeWidget::NameCol), mTableMenu, ipPos);

//            QTimeLine *timer = new QTimeLine(1000);
//            timer->setFrameRange(0, 100);

//            QGraphicsItemAnimation *animation = new QGraphicsItemAnimation;
//            animation->setItem(table);
//            animation->setTimeLine(timer);

//            for (int i = 0; i < 10; ++i) {
//                int dy = (i % 2) == 0 ? 10 / (-i + 1) : 0;
//                animation->setPosAt(i / 10.0, QPointF(0, dy));
//            }

//            timer->start();

            objectList.append(table);
        }

        if (TreeWidget::ViewItem == objId) {
            ViewItem *view = newViewItem(ipTreeItem->parent()->parent()->text(TreeWidget::NameCol),
                    ipTreeItem->text(TreeWidget::NameCol), mTableMenu, ipPos);
            objectList.append(view);
        }

    }
//    else if (TreeWidget::TableNode == objId) {
//        for (int i = 0; i < ipTreeItem->childCount(); ++i) {
//            showOnScene(ipTreeItem->child(i), ipCol, ipPos);
//        }
//    }

    return objectList;
}

/*!
 * @brief Create new table item if it doesn't exist or existent one
 *
 * @param[in] ipSchemaName - Schema name
 * @param[in] ipTableName - Table name
 * @param[in] ipMenu - Context menu for new item
 * @param[in] ipPos - Position of new item
 *
 * @return Existent or new table item
 */
// FIXME: ipSchemaName and ipTableName should be const reference
TableItem *
GraphicsScene::newTableItem(QString ipSchemaName, QString ipTableName, QMenu *ipMenu, const QPoint &ipPos)
{
    DbObjectItem *newItem = findItem(ipSchemaName, ipTableName);
    // check if such item is already on the scene
    if (toTable(newItem)) {
        return toTable(newItem);
    }

    // create new table
    return new TableItem(ipSchemaName, ipTableName, ipMenu, ipPos);
}

/*!
 * @brief Create new view item if it doesn't exist or existent one
 *
 * @param[in] ipSchemaName - Schema name
 * @param[in] ipViewName - View name
 * @param[in] ipMenu - Context menu for new item
 * @param[in] ipPos - Position of new item
 *
 * @return Existent or new view item
 */
// FIXME: ipSchemaName and ipViewName should be const reference
ViewItem *
GraphicsScene::newViewItem(QString ipSchemaName, QString ipViewName, QMenu *ipMenu, const QPoint &ipPos)
{
    DbObjectItem *newItem = findItem(ipSchemaName, ipViewName);
    // check if such item is already on the scene
    if (toView(newItem)) {
        return toView(newItem);
    }

    // create new view
    return new ViewItem(ipSchemaName, ipViewName, ipMenu, ipPos);
}

/*!
 * @brief Add given items to the scene
 *
 * @param[in] ipItems - List of items we need to add on the scene
 */
void
GraphicsScene::addItems(const QList<QGraphicsItem *> &ipItems)
{
    foreach (QGraphicsItem *item, ipItems) {
        if (toGroup(item)) {
            addItems(toGroup(item)->children());
            createItemGroup(toGroup(item)->children());
        } else if (toDbObject(item)) {
            addItem(item);
        }
    }
    drawRelations();
    clearSelection();
    update();
}

/*!
 * @brief Draw relations for all tables on the scene
 */
void
GraphicsScene::drawRelations()
{
    // draw all relations between new table and already added ones
    foreach (QGraphicsItem *item, items()) {
        if (toTable(item)) {
            createRelations(toTable(item));
        }
    }
}

/*!
 * @brief Create relations between given table item and anothers ones already painted
 *
 * @param[in] ipSourceItem - Source item
 */
void
GraphicsScene::createRelations(DbObjectItem *ipSourceItem)
{
    // TODO: implement this according to new class structure
    // find foreign keys and tables related to this keys
//    for (int i = 0; i < ipSourceItem->columnsCount(); ++i) {
//        if (ipSourceItem->isColumnForeignKey(i)) {
//            DbObjectItem *destItem = 0;
//
//            // if founded, create arrow
//            if ((destItem = findItem(ipSourceItem->foreignSchemaName(i), ipSourceItem->foreignTableName(i))) != 0) {
//                ArrowItem *arrow = new ArrowItem(ipSourceItem, destItem);
//                ipSourceItem->addArrowItem(arrow);
//                destItem->addArrowItem(arrow);
//                arrow->setZValue(-1000.0);
//                addItem(arrow);
//                arrow->updatePosition();
//            }
//        }
//    }
}

/*!
 * @brief Find item by his name on the scene
 *
 * @param[in] ipSchemaName - Schema name
 * @param[in] ipItemName - Item name
 *
 * @return Founded item if it presents in the DB, 0 otherwise
 */
DbObjectItem *
GraphicsScene::findItem(const QString &ipSchemaName, const QString &ipItemName)
{
    foreach (QGraphicsItem *item, items()) {
        DbObjectItem *dbItem = toDbObject(item);
        if (dbItem) {
            // TODO: check if the view and table may have equal names (if yes - code below isn't correct)
            if (dbItem->schemaName() == ipSchemaName && dbItem->name() == ipItemName) {
                return dbItem;
            }
        }
    }

    return 0;
}

/*!
 * @brief Handler for right mouse button click
 *
 * @param[in] ipEvent - Context menu event
 */
void
GraphicsScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *ipEvent)
{
    if (itemAt(ipEvent->scenePos())) {
        QGraphicsScene::contextMenuEvent(ipEvent);
        return;
    }

    mSchemeMenu->exec(ipEvent->screenPos());
}

/*!
 * @brief Handler for mouse press event
 *
 * @param[in] ipEvent - Mouse press event
 */
void
GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *ipEvent)
{
    if (!mMoveMode) {
        // if we pressed under item - do default actions and return
        QGraphicsItem *item = itemAt(ipEvent->scenePos());
        if (toDbObject(item) || toGroup(item)) {
            mOldPos = item->scenePos();
        } else {
            clearSelection();
            mStartSelect = ipEvent->scenePos();
        }
    }
    QGraphicsScene::mousePressEvent(ipEvent);
}

/*!
 * @brief Handler for mouse release event
 *
 * @param[in] ipEvent - Mouse release event
 */
void
GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *ipEvent)
{
    QGraphicsItem *item = itemAt(ipEvent->scenePos());
    if ((toDbObject(item) || toGroup(item)) && item->scenePos() != mOldPos) {
        emit itemMoved(selectedItems(), (int)(item->scenePos().x() - mOldPos.x()), (int)(item->scenePos().y() - mOldPos.y()));
    }

    if (!mMoveMode) {
        if (mSelectionPath) {
            removeItem(mSelectionPath);
            mSelectionPath = 0;
        }
    }
    QGraphicsScene::mouseReleaseEvent(ipEvent);
}

/*!
 * @brief Handler for mouse move event
 *
 * @param[in] ipEvent - Mouse move event
 */
void
GraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *ipEvent)
{
    if (mMoveMode) {
        QGraphicsScene::mouseMoveEvent(ipEvent);
        return;
    } else {
        // if we grabbered an item - do default actions and return
        if (mouseGrabberItem()) {
            QGraphicsScene::mouseMoveEvent(ipEvent);
            return;
        }

        // if left button was pressed - draw selection rectangle and set selection for all items under this rectangle
        if (ipEvent->buttons() == Qt::LeftButton) {
            mEndSelect = ipEvent->scenePos();

            if (mSelectionPath) {
                removeItem(mSelectionPath);
                mSelectionPath = 0;
            }

            // calculate coordinates of left top and right bottom corners
            qreal startX = fmin(mStartSelect.x(), mEndSelect.x());
            qreal startY = fmin(mStartSelect.y(), mEndSelect.y());
            qreal endX = fmax(mStartSelect.x(), mEndSelect.x());
            qreal endY = fmax(mStartSelect.y(), mEndSelect.y());

            qreal width = endX - startX;
            qreal height = endY - startY;

            // draw rectangle
            QPen pen(Qt::DashLine);
            pen.setColor(Qt::gray);

            QPainterPath path = QPainterPath();
            path.addRect(QRectF(startX, startY, width, height));
            mSelectionPath = addPath(path, pen);

            // select all items under rectangle
            setSelectionArea(path);
        }

        QGraphicsScene::mouseMoveEvent(ipEvent);
    }
}

/*!
 * @brief Handler for key press ipEvent
 * 	  + - maximize the scheme view
 * 	  - - minimize the scheme view
 * 	  arrow keys - move items to the appropriate sides
 *
 * @param[in] ipEvent - Key press event
 */
void
GraphicsScene::keyPressEvent(QKeyEvent *ipEvent)
{
    switch (ipEvent->key()) {
        case Qt::Key_Plus:
            //      scaleView(mPrevFactor + 1);
            break;
        case Qt::Key_Minus:
            //      scaleView(mPrevFactor - 1);
            break;
        case Qt::Key_Left:
            foreach (QGraphicsItem *item, selectedItems()) {
                if ((toDbObject(item) || toGroup(item))
                        && item->flags() & QGraphicsItem::ItemIsMovable) {
                    item->moveBy(-MOVE_STEP, 0);
                    mDiffX -= MOVE_STEP;
                    mStartMovingTimer.start(MOVE_INTERVAL);
                }
            }
            break;
        case Qt::Key_Right:
            foreach (QGraphicsItem *item, selectedItems()) {
                if ((toDbObject(item) || toGroup(item))
                        && item->flags() & QGraphicsItem::ItemIsMovable) {
                    item->moveBy(MOVE_STEP, 0);
                    mDiffX += MOVE_STEP;
                    mStartMovingTimer.start(MOVE_INTERVAL);
                }
            }
            break;
        case Qt::Key_Up:
            foreach (QGraphicsItem *item, selectedItems()) {
                if ((toDbObject(item) || toGroup(item))
                        && item->flags() & QGraphicsItem::ItemIsMovable) {
                    item->moveBy(0, -MOVE_STEP);
                    mDiffY -= MOVE_STEP;
                    mStartMovingTimer.start(MOVE_INTERVAL);
                }
            }
            break;
        case Qt::Key_Down:
            foreach (QGraphicsItem *item, selectedItems()) {
                if ((toDbObject(item) || toGroup(item))
                        && item->flags() & QGraphicsItem::ItemIsMovable) {
                    item->moveBy(0, MOVE_STEP);
                    mDiffY += MOVE_STEP;
                    mStartMovingTimer.start(MOVE_INTERVAL);
                }
            }
            break;
    }
}

/*!
 * @brief Emit signal when moving timer expired
 */
void
GraphicsScene::movingTimerExpired()
{
    mStartMovingTimer.stop();

    emit itemMoved(selectedItems(), mDiffX, mDiffY);
    mDiffX = 0;
    mDiffY = 0;
}

/*!
 * @brief Create item group from the given items
 *
 * @param[in] items - Items we will add to new group
 *
 * @return New items group
 */
ItemGroup *
GraphicsScene::createItemGroup(const QList<QGraphicsItem *> &items)
{
    clearSelection();
    // build a list of the first item's ancestors
    QList<QGraphicsItem *> ancestors;
    int n = 0;
    if (!items.isEmpty()) {
        QGraphicsItem *parent = items.at(n++);
        while ((parent = parent->parentItem())) {
            ancestors.append(parent);
        }
    }

    // find the common ancestor for all items
    QGraphicsItem *commonAncestor = 0;
    if (!ancestors.isEmpty()) {
        while (n < items.size()) {
            int commonIndex = -1;
            QGraphicsItem *parent = items.at(n++);
            do {
                int index = ancestors.indexOf(parent, qMax(0, commonIndex));
                if (index != -1) {
                    commonIndex = index;
                    break;
                }
            } while ((parent = parent->parentItem()));

            if (commonIndex == -1) {
                commonAncestor = 0;
                break;
            }

            commonAncestor = ancestors.at(commonIndex);
        }
    }

    // Create a new group at that level
    ItemGroup *group = new ItemGroup(commonAncestor);
    if (!commonAncestor) {
        addItem(group);
    }

    foreach (QGraphicsItem *item, items) {
        if (toDbObject(item) || toGroup(item)) {
            group->addToGroup(item);
        }
    }
    return group;
}

/*!
 * @brief Draw background
 *
 * @param[in] ipPainter - Painter
 */
void
GraphicsScene::drawBackground(QPainter *ipPainter, const QRectF &)
{
    using namespace Consts;
    bool needDrawGrid = mSettings.value(VIEW_GRP + "/" + SHOW_GRID_SETTING, true).value<bool>();
    if (needDrawGrid) {
        drawGrid(ipPainter);
    }

    bool needDivideIntoPages = mSettings.value(VIEW_GRP + "/" + DIVIDE_INTO_PAGES_SETTING, true).value<bool>();
    if (needDivideIntoPages) {
        divideIntoPages(ipPainter);
    }
}

/*!
 * @brief Draw the grid
 *
 * @param[in] ipPainter - Painter
 */
void
GraphicsScene::drawGrid(QPainter *ipPainter)
{
    // print the grid
    QPen pen = QPen();

    // draw vertical lines
    int x1 = 0;
    int x2 = x1;
    while (x1 < width()) {
        if (x1 % 50 == 0) {
            pen.setColor(Qt::gray);
            pen.setStyle(Qt::DashDotDotLine);
        } else {
            pen.setColor(QColor(210, 210, 210));
            pen.setStyle(Qt::DashDotDotLine);
        }
        ipPainter->setPen(pen);

        // draw line between (x1, 0) and (x2, height) points
        ipPainter->drawLine(x1, 0, x2, (int)height());
        x1 = x2 = x1 + LOW_GRID_DX;
    }

    // draw hogizontal lines
    int y1 = 0;
    int y2 = y1;
    while (y1 < height()) {
        if (y1 % 50 == 0) {
            pen.setColor(Qt::gray);
            pen.setStyle(Qt::DashDotDotLine);
        } else {
            pen.setColor(QColor(210, 210, 210));
            pen.setStyle(Qt::DashDotDotLine);
        }
        ipPainter->setPen(pen);

        // draw line between (0, y1) and (width, y2) points
        ipPainter->drawLine(0, y1, (int)width(), y2);
        y1 = y2 = y1 + LOW_GRID_DY;
    }
}

/*!
 * @brief Draw the page's bounds
 *
 * @param[in] ipPainter - Painter
 */
void
GraphicsScene::divideIntoPages(QPainter *ipPainter)
{
    // print A4 pages grid
    QPen pen = QPen();
    QPrinter printer(QPrinter::ScreenResolution);

    int a4width = printer.pageRect().width();
    int a4height = printer.pageRect().height();
    int maxI = (int)(width() / a4width + 1);
    int maxJ = (int)(height() / a4height + 1);

    pen.setColor(Qt::black);
    pen.setStyle(Qt::DashDotDotLine);
    ipPainter->setPen(pen);

    for (int i = 0; i < maxI; ++i) {
        ipPainter->drawLine(i * a4width, 0, i * a4width, (int)height());
    }

    for (int j = 0; j < maxJ; ++j) {
        ipPainter->drawLine(0, j * a4height, (int)width(), j * a4height);
    }
}

/*!
 * @brief Refresh legend
 */
void
GraphicsScene::refreshLegend()
{
    mLegend->refresh();
    updateLegend();
}

/*!
 * @brief Show/hide legend
 *
 * @param[in] ipFlag - True if we want to show legend, false otherwise
 */
void
GraphicsScene::showLegend(bool ipFlag)
{
    if (ipFlag) {
        addItem(mLegend);
    } else {
        if (mLegend->scene()) {
            removeItem(mLegend);
        }
    }
}

/*!
 * @brief Update legend
 */
void
GraphicsScene::updateLegend()
{
    mLegend->update(mLegend->rect());
}

/*!
 * @brief Delete given items (recursivelly for groups)
 *
 * @param[in] ipItems - List of items we want to remove from scene
 */
void
GraphicsScene::deleteItems(QList<QGraphicsItem *> &ipItems)
{
    foreach (QGraphicsItem *item, ipItems) {
        if (toLegend(item)) {
            continue;
        }
       
        if (toDbObject(item)) {
            toDbObject(item)->removeArrowItems();
        } else if (toGroup(item)) {
            QList<QGraphicsItem *> children = toGroup(item)->children();
            deleteItems(children);
        }
        removeItem(item);
    }

    updateLegend();
    update();
}

/*!
 * @brief Show field types for selected items
 */
void
GraphicsScene::setFieldsTypesVisible()
{
    setFieldsTypesVisible(selectedItems(), true);
}

/*!
 * @brief Hide field types for selected items
 */
void
GraphicsScene::setFieldsTypesInvisible()
{
    setFieldsTypesVisible(selectedItems(), false);
}


/*!
 * @brief Show/hide field types for given items (recursivelly for groups)
 *
 * @param[in] ipItems - List of items we want to show field types for
 * @param[in] ipFlag - True if we want to show field types, false otherwise
 */
void
GraphicsScene::setFieldsTypesVisible(QList<QGraphicsItem *> ipItems, bool ipFlag)
{
    foreach (QGraphicsItem *item, ipItems) {
        if (toDbObject(item)) {
            toDbObject(item)->setFieldsTypesVisible(ipFlag);
        } else if (toGroup(item)) {
            setFieldsTypesVisible(toGroup(item)->children(), ipFlag);
        }
    }
}

/*!
 * @brief Show/hide indices for selected tables
 *
 * @param[in] ipFlag - True if we want to show indeces, false otherwise
 */
void
GraphicsScene::setIndicesVisible(bool ipFlag)
{
    setIndicesVisible(selectedItems(), ipFlag);
}

/*!
 * @brief Show/hide indices for selected tables
 *
 * @param[in] ipItems - List of items we will show indeces for
 * @param[in] ipFlag - True if we want to show indeces, false otherwise
 */
void
GraphicsScene::setIndicesVisible(QList<QGraphicsItem *> ipItems, bool ipFlag)
{
    foreach (QGraphicsItem *item, ipItems) {
        if (toTable(item)) {
            toTable(item)->setIndicesVisible(ipFlag);
        } else if (toGroup(item)) {
            setIndicesVisible(toGroup(item)->children(), ipFlag);
        }
    }
}

/*!
 * @brief Set all selected items' color to user choosed color
 */
void
GraphicsScene::setItemColor()
{
    // prompt for color
    QColor color = QColorDialog::getColor(Qt::white);

    // if color is valid
    if (color.isValid()) {
        setItemColor(selectedItems(), color);
    }

    updateLegend();
}

/*!
 * @brief Colorize all items in the list (recursivelly for groups)
 *
 * @param[in] ipItems - List of items we will change color for
 * @param[in] ipColor - Items color
 */
void
GraphicsScene::setItemColor(QList<QGraphicsItem *> ipItems, QColor ipColor)
{
    // colorize each item that is on the scene
    foreach (QGraphicsItem *item, ipItems) {
        if (toDbObject(item)) {
            setItemColor(toDbObject(item), ipColor);
        } 
        
        if (toGroup(item)) {
            setItemColor(toGroup(item)->children(), ipColor);
        }
    }
}

/*!
 * @brief Set item color
 *
 * @param[in] ipItem - Graphics item
 * @param[in] ipColor - Item color
 */
void
GraphicsScene::setItemColor(DbObjectItem *ipItem, QColor ipColor)
{
    ipItem->setItemColor(ipColor);
}

/*!
 * @brief Select all items if ctrl-a is pressed
 */
void
GraphicsScene::selectAllItems()
{
    // items can mutate in the loop
    foreach (QGraphicsItem *item, items()) {
        if (toDbObject(item)) {
            toDbObject(item)->setSelected(true);
        }
    }
}

/*!
 * @brief Auto resize selected items' sizes to adjusted
 */
void
GraphicsScene::adjustItems()
{
    adjustItems(selectedItems());
}

/*!
 * @brief Auto resize items (recursivelly for groups)
 *
 * @param[in] ipItems - List of items we will adjust size for
 */
void
GraphicsScene::adjustItems(QList<QGraphicsItem *> ipItems)
{
    foreach (QGraphicsItem *item, ipItems) {
        if (toDbObject(item)) {
            toDbObject(item)->adjustSize();
        } else if (toGroup(item)) {
            adjustItems(toGroup(item)->children());
        }
    }
}

/*!
 * @brief Group items
 *
 * @param[in] ipList - List of items we will group
 */
void
GraphicsScene::groupItems(QList<QGraphicsItem *> ipItems)
{
    // if we try to group only one item - return
    if (1 == ipItems.count()) {
        return;
    }

    ItemGroup *group = createItemGroup(ipItems);
    group->setContextMenu(mTableMenu);
}

/*!
 * @brief Group items
 */
void
GraphicsScene::groupItems()
{
    groupItems(selectedItems());
}

/*!
 * @brief Ungroup items
 *
 * @param[in] ipList - List of items we will ungroup
 */
void
GraphicsScene::ungroupItems(QList<QGraphicsItem *> ipItems)
{
    foreach (QGraphicsItem *item, ipItems) {
        if (toGroup(item)) {
            destroyItemGroup(toGroup(item));
        }
    }
}

/*!
 * @brief Ungroup items
 */
void
GraphicsScene::ungroupItems()
{
    ungroupItems(selectedItems());
}

/*!
 * @brief Colorize items according schemas
 */
void
GraphicsScene::colorizeAccordingSchemas()
{
    Database *dbInst = Database::instance();

    QStringList schemasNames;
    dbInst->schemasList(schemasNames);

    for (int i = 0; i < schemasNames.count(); ++i) {
        // items can mutate in the loop
        foreach (QGraphicsItem *item, items()) {
            // FIXME doesn't work for views because schemaName returns empty string now for ViewItem
            if (toDbObject(item) && toDbObject(item)->schemaName() == schemasNames.at(i)) {
                // use only two colors from the palette
                int red = (0 == i % 3) ? 255 * (i + 1) / schemasNames.count() : 0;
                int green = (1 == i % 3) ? 255 * (i + 1) / schemasNames.count() : 0;
                int blue = (2 == i % 3) ? 255 * (i + 1) / schemasNames.count() : 0;

                setItemColor(toDbObject(item), QColor(red, green, blue));
            }
        }
    }

    updateLegend();
}

/*!
 * @brief Set the anchor for selected items
 *
 * @param[in] ipFlag - True if we want to enable anchor for items, false if disable
 */
void
GraphicsScene::setAnchor(bool ipFlag)
{
    setAnchor(selectedItems(), ipFlag);
}

/*!
 * @brief Set the anchor for selected items
 *
 * @param[in] ipItems - List of items we will enable or disable anchors for
 * @param[in] ipFlag - True if we want to enable anchor for items, false if disable
 */
void
GraphicsScene::setAnchor(QList<QGraphicsItem *> ipItems, bool ipFlag)
{
    foreach (QGraphicsItem *item, ipItems) {
        if (toGroup(item)) {
            item->setFlag(QGraphicsItem::ItemIsMovable, ipFlag);
            setAnchor(item->children(), ipFlag);
        } else if (toDbObject(item)) {
            item->setFlag(QGraphicsItem::ItemIsMovable, ipFlag);
        }
    }
}

/*!
 * @brief Show/hide grid
 *
 * @param[in] ipFlag - True if we want to want to show grid, false otherwise
 */
void
GraphicsScene::showGrid(bool ipFlag)
{
    // only remember given flag; all analyze will be done in scene class
    mSettings.setValue(Consts::VIEW_GRP + "/" + Consts::SHOW_GRID_SETTING, ipFlag);
    update(QRectF(0.0, 0.0, width(), height()));
}

/*!
 * @brief (Un)Align items to the grid
 *
 * @param[in] ipFlag - True if we want to align items according grid, false it we doesn't
 */
void
GraphicsScene::alignToGrid(bool ipFlag)
{
    mSettings.setValue(Consts::VIEW_GRP + "/" + Consts::ALIGN_TO_GRID_SETTING, ipFlag);
}

/*!
 * @brief Show/hide grid
 *
 * @param[in] ipFlag - True if we want to divide scene into pages, false otherwise
 */
void
GraphicsScene::divideIntoPages(bool ipFlag)
{
    // only remember given flag; all analyze will be done in scene class
    mSettings.setValue(Consts::VIEW_GRP + "/" + Consts::DIVIDE_INTO_PAGES_SETTING, ipFlag);
    update(QRectF(0.0, 0.0, width(), height()));
}

/*!
 * @brief Select all items in schema
 */
void
GraphicsScene::selectAllItemsInSchema()
{
    foreach (QGraphicsItem *itemFromSelected, selectedItems()) {
        // items can mutate in the loop
        foreach (QGraphicsItem *itemFromAll, items()) {
            if (toDbObject(itemFromAll) && toDbObject(itemFromSelected) &&
                    toDbObject(itemFromAll)->schemaName() ==
                    toDbObject(itemFromSelected)->schemaName()) {
                (itemFromAll)->setSelected(true);
            }
        }
    }
}


/*!
 * @brief Save scene to xml
 *
 * @param[in] ipDoc - Xml dom document
 * @param[in] ipShowGrid - True if we need to show grid, false otherwise
 * @param[in] ipDivideIntoPages - True if we need to divide scene into pages, false otherwise
 * @param[in] ipShowLegend - True if we need to show legend, false otherwise
 * @param[in] ipShowControlWidget - True if we need to show control widget, false otherwise
 *
 * @return Filled with scene info xml dom element
 */
QDomElement
GraphicsScene::toXml(QDomDocument &ipDoc, bool ipShowGrid, bool ipDivideIntoPages, bool ipShowLegend, bool ipShowControlWidget) const
{
    QDomElement element = ipDoc.createElement("scene");
    element.setAttribute("grid", ipShowGrid);
    element.setAttribute("divideIntoPages", ipDivideIntoPages);
    element.setAttribute("legend", ipShowLegend);
    element.setAttribute("controlWidget", ipShowControlWidget);

    foreach (QGraphicsItem *item, items()) {
        if (toGroup(item) && 0 == toGroup(item)->parentItem()) {
            element.appendChild(toGroup(item)->toXml(ipDoc));
        } else if (toDbObject(item) && 0 == toDbObject(item)->parentItem()) {
            element.appendChild(toDbObject(item)->toXml(ipDoc));
        }
    }

    return element;
}

/*!
 * @brief Restore scene from xml
 *
 * @param[in] ipNode - Dom node we will restore scene parameters from
 *
 * @return List of graphic items restored from xml
 */
QList<QGraphicsItem *>
GraphicsScene::fromXml(const QDomNode &ipNode)
{
    QList<QGraphicsItem *> itemList;
    QDomNode node = ipNode;
    // show all elements
    while (!node.isNull()) {
        QDomElement element = node.toElement();
        if (!element.isNull()) {
            // it's a table
            if (element.tagName() == "table") {
                itemList << TableItem::fromXml(element, this, mTableMenu);
            // it's a view
            } else if (element.tagName() == "view") {
//                itemList << ViewItem::fromXml(element, this, mTableMenu);
            // it's a table group
            } else if (element.tagName() == "tableGroup") {
                itemList << ItemGroup::fromXml(element, this, mTableMenu);
            }
        }
        // go to the next element
        node = node.nextSibling();
    }

    return itemList;
}

/*!
 * @brief Move the legend
 *
 * @param[in] ipDx - Gorizontal distance
 * @param[in] ipDy - Vertical distance
 */
void
GraphicsScene::moveLegend(int ipDx, int ipDy)
{
    mLegend->moveBy(ipDx, ipDy);
}

/*!
 * @brief Resize the scene
 *
 * @param ipFactor - Scale factor
 */
void
GraphicsScene::resize(int ipFactor)
{
    static int factor = 0;
    setSceneRect(0, 0, width() + (ipFactor - factor) * 10, height() + (ipFactor - factor) * 10);
    factor = ipFactor;
    update(QRectF(0.0, 0.0, width(), height()));
}

/*!
 * @brief Set move mode
 *
 * @param[in] ipFlag - True if we want to set move mode, false otherwise
 */
void
GraphicsScene::setMoveMode(bool ipFlag)
{
    mMoveMode = ipFlag;
}

/*!
 * @brief Get move mode
 *
 * @return True if move mode is enabled, false otherwise
 */
bool
GraphicsScene::moveMode() const
{
    return mMoveMode;
}

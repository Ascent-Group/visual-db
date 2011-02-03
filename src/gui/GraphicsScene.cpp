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
#include <common/Database.h>
#include <control/Config.h>
#include <gui/ControlWidget.h>
#include <gui/GraphicsScene.h>
#include <gui/TreeWidget.h>
#include <gui/graphicsitems/ForeignArrow.h>
#include <gui/graphicsitems/InheritanceArrow.h>
#include <gui/graphicsitems/ItemGroup.h>
#include <gui/graphicsitems/Legend.h>
#include <gui/graphicsitems/LoopArrow.h>
#include <gui/graphicsitems/TableItem.h>
#include <gui/graphicsitems/ViewItem.h>
#include <math.h>

#include <QDebug>

namespace Gui {

/*!
 * Constructor
 */
GraphicsScene::GraphicsScene(QObject *iParent)
    : QGraphicsScene(iParent),
      mSelectionPath(),
      mMoveMode(false),
      mOldPos(),
      mOldRect(),
      mDiffX(0),
      mDiffY(0),
      mStartMovingTimer()
{
    using namespace Gui::GraphicsItems;

    setBackgroundBrush(QBrush(Control::Config().backgroundColor()));
    setSceneRect(0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT);
    // \todo We probably should pass this to Legend's constructor, so that graphics scene
    // takes parentship of it.
    mLegend = new Legend();
    mLegend->setZValue(1000);

    connect(&mStartMovingTimer, SIGNAL(timeout()), this, SLOT(movingTimerExpired()));
}

/*!
 * Destructor
 */
GraphicsScene::~GraphicsScene()
{
    // remove items from scene, so that scene is not theri parent anymore
    QList<QGraphicsItem*> itemsList = items();
    deleteItems(itemsList);

    // we will handle items' destruction on our own
    qDeleteAll(mDbItems);
    qDeleteAll(mArrows);
    mDbItems.clear();
    mArrows.clear();
    delete mLegend;
}

/*!
 * \brief Set scene context menu
 *
 * \param[in] iSchemeMenu - Context menu for schema
 */
void
GraphicsScene::setSchemeMenu(QMenu *iSchemeMenu)
{
    mSchemeMenu = iSchemeMenu;
}

/*!
 * \brief Set table context menu
 *
 * \param[in] iTableMenu - Context menu for table
 */
void
GraphicsScene::setTableMenu(QMenu *iTableMenu)
{
    mTableMenu = iTableMenu;
}

//#include <QGraphicsItemAnimation>
//#include <QTimeLine>
/*!
 * \brief Add table from tree event to the scene
 *
 * \param[in] iTreeItem - Tree item we need to show on the scene
 * \param[in] iCol - Tree item column
 * \param[in] iPos - Position where we should place new item
 *
 * \return List of added items
 */
QList<QGraphicsItem *>
GraphicsScene::showOnScene(QTreeWidgetItem *iTreeItem, int iCol, const QPoint &iPos, bool iCenterOn)
{
    QList<QGraphicsItem *> objectList;

    if (!iTreeItem) {
        qDebug() << "[E][" << __func__ << "][" << __LINE__ <<
            "]Can't show selected items on the scene because input tree item is empty";
        return objectList;
    }

    // get database object id
    int objId = iTreeItem->text(TreeWidget::IdCol).toInt();

    // if schema or table or view were double clicked
    if (TreeWidget::SchemaItem == objId || TreeWidget::TableItem == objId || TreeWidget::ViewItem == objId) {
        // check whether item is a schema item
        if (TreeWidget::SchemaItem == objId) {
            // add all its table children to the scene
            for (int i = 0; i < iTreeItem->childCount(); ++i) {
                if (TreeWidget::TableNode == iTreeItem->child(i)->text(TreeWidget::IdCol).toInt() ||
                        TreeWidget::ViewNode == iTreeItem->child(i)->text(TreeWidget::IdCol).toInt()) {
                    for (int j = 0; j < iTreeItem->child(i)->childCount(); ++j) {
                        QPoint pos(iPos.x() + j * SEEK_STEP, iPos.y() + j * SEEK_STEP);
                        objectList << showOnScene(iTreeItem->child(i)->child(j), /*TreeWidget::NameCol*/iCol, pos, iCenterOn);
                    }
                }
            }
            // don't add schema item itself
            return objectList;
        }

        using namespace Gui::GraphicsItems;
        
        if (TreeWidget::TableItem == objId) {
            TableItem *table = newTableItem(iTreeItem->parent()->parent()->text(TreeWidget::NameCol),
                    iTreeItem->text(TreeWidget::NameCol), mTableMenu, iPos);
            objectList.append(table);
        }

        if (TreeWidget::ViewItem == objId) {
            ViewItem *view = newViewItem(iTreeItem->parent()->parent()->text(TreeWidget::NameCol),
                    iTreeItem->text(TreeWidget::NameCol), mTableMenu, iPos);
            objectList.append(view);
        }

    }

    return objectList;
}

/*!
 * \brief Create new table item if it doesn't exist or existent one
 *
 * \param[in] iSchemaName - Schema name
 * \param[in] iTableName - Table name
 * \param[in] iMenu - Context menu for new item
 * \param[in] iPos - Position of new item
 *
 * \return Existent or new table item
 */
Gui::GraphicsItems::TableItem *
GraphicsScene::newTableItem(const QString &iSchemaName, const QString &iTableName, QMenu *iMenu, const QPoint &iPos)
{
    using namespace Gui::GraphicsItems;

    DbObjectItem *newItem = findItem(iSchemaName, iTableName);

    // if item is currently not on scene
    if (!toTable(newItem)) {
        newItem = 0;

        // try to find it in registered items
        QSet<DbObjectItem*>::const_iterator iter = mDbItems.constBegin();

        while (0 == newItem && iter != mDbItems.constEnd()) {
            if (iTableName == (*iter)->name() && iSchemaName == (*iter)->schemaName()) {
                newItem = *iter;
                break;
            }
            ++iter;
        }

        // if found - just move to the given position (if position is correct)
        if (newItem) {
            if (iPos.x() > 0 && iPos.y() > 0) {
                newItem->setX(iPos.x());
                newItem->setY(iPos.y());
                newItem->adjustSize();
            }
        // if not found
        } else {
            // create new table
            newItem = new TableItem(iSchemaName, iTableName, iMenu, iPos);
            // register this item
            mDbItems.insert(newItem);
        }
    }

    return toTable(newItem);
}

/*!
 * \brief Create new view item if it doesn't exist or existent one
 *
 * \param[in] iSchemaName - Schema name
 * \param[in] iViewName - View name
 * \param[in] iMenu - Context menu for new item
 * \param[in] iPos - Position of new item
 *
 * \return Existent or new view item
 */
Gui::GraphicsItems::ViewItem *
GraphicsScene::newViewItem(const QString &iSchemaName, const QString &iViewName, QMenu *iMenu, const QPoint &iPos)
{
    using namespace Gui::GraphicsItems;

    DbObjectItem *newItem = findItem(iSchemaName, iViewName);

    // if item is currently not on scene
    if (!toView(newItem)) {
        newItem = 0;
        // try to find it in registered items
        QSet<DbObjectItem*>::const_iterator iter = mDbItems.constBegin();

        while (0 == newItem && iter != mDbItems.constEnd()) {
            if (iViewName == (*iter)->name() && iSchemaName == (*iter)->schemaName()) {
                newItem = *iter;
            }
            ++iter;
        }

        // if found - just move to the given position (if position is correct)
        if (newItem) {
            if (iPos.x() > 0 && iPos.y() > 0) {
                newItem->setX(iPos.x());
                newItem->setY(iPos.y());
                newItem->adjustSize();
            }
        // if not found
        } else {
            // create new view
            newItem = new ViewItem(iSchemaName, iViewName, iMenu, iPos);
            // register this item
            mDbItems.insert(newItem);
        }
    }

    return toView(newItem);
}

/*!
 * \brief Add given items to the scene
 *
 * \param[in] iItems - List of items we need to add on the scene
 */
void
GraphicsScene::addItems(const QList<QGraphicsItem *> &iItems)
{
    using namespace Gui::GraphicsItems;

    foreach (QGraphicsItem *item, iItems) {
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
 * \brief Draw relations for all tables on the scene
 */
void
GraphicsScene::drawRelations()
{
    using namespace Gui::GraphicsItems;

    // draw all relations between new table and already added ones
    foreach (QGraphicsItem *item, items()) {
        if (toTable(item)) {
            createRelations(toTable(item));
        }
    }
}

/*!
 * \brief Create relations between given table item and anothers ones already painted
 *
 * \param[in] iSourceItem - Source item
 */
void
GraphicsScene::createRelations(Gui::GraphicsItems::TableItem *iSourceItem)
{
    if (!iSourceItem) {
        return;
    }

    using namespace Gui::GraphicsItems;

    ArrowItem *arrow = 0;
    // FIXME: this code is applicable only for tables
    // find foreign keys and tables related to this keys
    for (int i = 0; i < iSourceItem->columnsCount(); ++i) {
        arrow = 0;
        if (iSourceItem->isColumnForeignKey(i)) {
            TableItem *destItem = toTable(findItem(iSourceItem->foreignSchemaName(i), iSourceItem->foreignTableName(i)));

            if (!destItem) {
                continue;
            }

            // find the arrow if it exists already
            QSet<ArrowItem*>::const_iterator iter = mArrows.constBegin();

            while (0 == arrow && iter != mArrows.constEnd()) {
                if (iSourceItem == (*iter)->startItem() && destItem == (*iter)->endItem()) {
                    arrow = (*iter);
                }
                ++iter;
            }

            // if arrow was not found
            if (!arrow) {
                // create an arrow
                // self referenced arrow
                if (iSourceItem == destItem) {
                    arrow = new LoopArrow(iSourceItem, destItem, QString(""));
                    // if founded, create arrow
                } else if (destItem) {
                    arrow = new ForeignArrow(iSourceItem, destItem, QString(""));
                }
            }
            createRelation(iSourceItem, destItem, arrow);
        }
    }

    // if table inherits from another table - paint inheritance arrow
    foreach (const TableItem::FullName &parentFullName, iSourceItem->parents()) {
        TableItem *destItem = toTable(findItem(parentFullName.mSchemaName, parentFullName.mTableName));

        arrow = 0;
        // find the arrow if it exists already
        QSet<ArrowItem*>::const_iterator iter = mArrows.constBegin();
        while (0 == arrow && iter != mArrows.constEnd()) {
            if (iSourceItem == (*iter)->startItem() && destItem == (*iter)->endItem()) {
                arrow = (*iter);
            }
            ++iter;
        }

        // if was not found
        if (!arrow && destItem) {
            arrow = new InheritanceArrow(iSourceItem, destItem, QString(""));
            createRelation(iSourceItem, destItem, arrow);
        }
    }
}

/*!
 * \brief Create an arrow between source and destination tables
 *
 * \param[in] iSource - Source table
 * \param[in] iDestination - Destination table
 * \param[in] iArrow - Arrow between source and destination tables
 */
void
GraphicsScene::createRelation(Gui::GraphicsItems::TableItem *iSource, Gui::GraphicsItems::TableItem *iDestination, Gui::GraphicsItems::ArrowItem *iArrow)
{
    // register the arrow
    mArrows.insert(iArrow);
    iSource->addArrowItem(iArrow);
    iDestination->addArrowItem(iArrow);
    // \todo Give an explanation of why -1000 is used
    iArrow->setZValue(-1000.0);
    addItem(iArrow);
    iArrow->updatePosition();
}

/*!
 * \brief Find item by his name on the scene
 *
 * \param[in] iSchemaName - Schema name
 * \param[in] iItemName - Item name
 *
 * \return Founded item if it presents in the DB, 0 otherwise
 */
Gui::GraphicsItems::DbObjectItem *
GraphicsScene::findItem(const QString &iSchemaName, const QString &iItemName)
{
    using namespace Gui::GraphicsItems;

    foreach (QGraphicsItem *item, items()) {
        DbObjectItem *dbItem = toDbObject(item);
        if (dbItem) {
            // TODO: check if the view and table may have equal names (if yes - code below isn't correct)
            if (dbItem->schemaName() == iSchemaName && dbItem->name() == iItemName) {
                return dbItem;
            }
        }
    }

    return 0;
}

/*!
 * \brief Handler for right mouse button click
 *
 * \param[in] iEvent - Context menu event
 */
void
GraphicsScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *iEvent)
{
    if (itemAt(iEvent->scenePos())) {
        QGraphicsScene::contextMenuEvent(iEvent);
        return;
    }

    mSchemeMenu->exec(iEvent->screenPos());
}

/*!
 * \brief Handler for mouse press event
 *
 * \param[in] iEvent - Mouse press event
 */
void
GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *iEvent)
{
    using namespace Gui::GraphicsItems;

    if (!mMoveMode) {
        // if we pressed under item - do default actions and return
        QGraphicsItem *item = itemAt(iEvent->scenePos());
        if (toDbObject(item) || toGroup(item)) {
            // it would be a resizing - remember current item geometry
            if (toDbObject(item)) {
                mOldRect = toDbObject(item)->boundingRect();
            }
            mOldPos = item->scenePos();
        } else {
            clearSelection();
            mStartSelect = iEvent->scenePos();
        }
    }
    QGraphicsScene::mousePressEvent(iEvent);
}

/*!
 * \brief Handler for mouse release event
 *
 * \param[in] iEvent - Mouse release event
 */
void
GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *iEvent)
{
    using namespace Gui::GraphicsItems;

    QGraphicsItem *item = itemAt(iEvent->scenePos());
    if ((toDbObject(item) || toGroup(item))) {
        // if it was resizing - emit resizing signal
        if (toDbObject(item) && toDbObject(item)->mouseMode() != DbObjectItem::MOVE) {
            emit itemResized(toDbObject(item), toDbObject(item)->boundingRect(), mOldRect);
        // else it is movement
        } else if (item->scenePos() != mOldPos) {
            emit itemMoved(selectedItems(), (int)(item->scenePos().x() - mOldPos.x()), (int)(item->scenePos().y() - mOldPos.y()));
        }
    }

    if (!mMoveMode) {
        if (mSelectionPath) {
            removeItem(mSelectionPath);
            mSelectionPath = 0;
        }
    }
    QGraphicsScene::mouseReleaseEvent(iEvent);
}

/*!
 * \brief Handler for mouse move event
 *
 * \param[in] iEvent - Mouse move event
 */
void
GraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *iEvent)
{
    // if we grabbered an item - do default actions and return
    if (!mMoveMode && !mouseGrabberItem()) {
        // if left button was pressed - draw selection rectangle and set selection for all items under this rectangle
        if (iEvent->buttons() == Qt::LeftButton) {
            mEndSelect = iEvent->scenePos();

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
    }

    QGraphicsScene::mouseMoveEvent(iEvent);
}

/*!
 * \brief Handler for key press iEvent
 * 	  + - maximize the scheme view
 * 	  - - minimize the scheme view
 * 	  arrow keys - move items to the appropriate sides
 *
 * \param[in] iEvent - Key press event
 */
void
GraphicsScene::keyPressEvent(QKeyEvent *iEvent)
{
    using namespace Gui::GraphicsItems;

    switch (iEvent->key()) {
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
 * \brief Emit signal when moving timer expired
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
 * \brief Create item group from the given items
 *
 * \param[in] items - Items we will add to new group
 *
 * \return New items group
 */
Gui::GraphicsItems::ItemGroup *
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

    using namespace Gui::GraphicsItems;

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
 * \brief Draw background
 *
 * \param[in] iPainter - Painter
 */
void
GraphicsScene::drawBackground(QPainter *iPainter, const QRectF &)
{
    Control::Config cfg;

    bool needDrawGrid = cfg.showGrid();
    if (needDrawGrid) {
        drawGrid(iPainter);
    }

    bool needDivideIntoPages = cfg.divideIntoPages();
    if (needDivideIntoPages) {
        divideIntoPages(iPainter);
    }
}

/*!
 * \brief Draw the grid
 *
 * \param[in] iPainter - Painter
 */
void
GraphicsScene::drawGrid(QPainter *iPainter)
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
        iPainter->setPen(pen);

        // draw line between (x1, 0) and (x2, height) points
        iPainter->drawLine(x1, 0, x2, (int)height());
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
        iPainter->setPen(pen);

        // draw line between (0, y1) and (width, y2) points
        iPainter->drawLine(0, y1, (int)width(), y2);
        y1 = y2 = y1 + LOW_GRID_DY;
    }
}

/*!
 * \brief Draw the page's bounds
 *
 * \param[in] iPainter - Painter
 */
void
GraphicsScene::divideIntoPages(QPainter *iPainter)
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
    iPainter->setPen(pen);

    for (int i = 0; i < maxI; ++i) {
        iPainter->drawLine(i * a4width, 0, i * a4width, (int)height());
    }

    for (int j = 0; j < maxJ; ++j) {
        iPainter->drawLine(0, j * a4height, (int)width(), j * a4height);
    }
}

/*!
 * \brief Refresh legend
 */
void
GraphicsScene::refreshLegend()
{
    mLegend->refresh();
    updateLegend();
}

/*!
 * \brief Show/hide legend
 *
 * \param[in] iFlag - True if we want to show legend, false otherwise
 */
void
GraphicsScene::showLegend(bool iFlag)
{
    if (iFlag) {
        addItem(mLegend);
    } else {
        if (mLegend->scene()) {
            removeItem(mLegend);
        }
    }

//    if (views().size() > 0) {
//        qDebug() << views().at(0)->width() << ":::" << views().at(0)->height();
//        mLegend->setX(views().at(0)->rect().width() - mLegend->width() - 20);
//        mLegend->setY(views().at(0)->rect().height() - mLegend->height() - 20);
//    }
}

/*!
 * \brief Update legend
 */
void
GraphicsScene::updateLegend()
{
    mLegend->update(mLegend->rect());
}

/*!
 * \brief Delete given items (recursivelly for groups)
 *
 * \param[in] iItems - List of items we want to remove from scene
 */
void
GraphicsScene::deleteItems(QList<QGraphicsItem *> &iItems)
{
    using namespace Gui::GraphicsItems;

    foreach (QGraphicsItem *item, iItems) {
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
 * \brief Show field types for selected items
 */
void
GraphicsScene::setFieldsTypesVisible()
{
    setFieldsTypesVisible(selectedItems(), true);
}

/*!
 * \brief Hide field types for selected items
 */
void
GraphicsScene::setFieldsTypesInvisible()
{
    setFieldsTypesVisible(selectedItems(), false);
}


/*!
 * \brief Show/hide field types for given items (recursivelly for groups)
 *
 * \param[in] iItems - List of items we want to show field types for
 * \param[in] iFlag - True if we want to show field types, false otherwise
 */
void
GraphicsScene::setFieldsTypesVisible(QList<QGraphicsItem *> iItems, bool iFlag)
{
    foreach (QGraphicsItem *item, iItems) {
        using namespace Gui::GraphicsItems;
        
        if (toDbObject(item)) {
            toDbObject(item)->setFieldsTypesVisible(iFlag);
        } else if (toGroup(item)) {
            setFieldsTypesVisible(toGroup(item)->children(), iFlag);
        }
    }
}

/*!
 * \brief Show/hide indices for selected tables
 *
 * \param[in] iFlag - True if we want to show indices, false otherwise
 */
void
GraphicsScene::setIndicesVisible(bool iFlag)
{
    setIndicesVisible(selectedItems(), iFlag);
}

/*!
 * \brief Show/hide indices for selected tables
 *
 * \param[in] iItems - List of items we will show indices for
 * \param[in] iFlag - True if we want to show indices, false otherwise
 */
void
GraphicsScene::setIndicesVisible(QList<QGraphicsItem *> iItems, bool iFlag)
{
    foreach (QGraphicsItem *item, iItems) {
        using namespace Gui::GraphicsItems;
        if (toTable(item)) {
            toTable(item)->setIndicesVisible(iFlag);
        } else if (toGroup(item)) {
            setIndicesVisible(toGroup(item)->children(), iFlag);
        }
    }
}

/*!
 * \brief Set all selected items' color to user choosed color
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
 * \brief Colorize all items in the list (recursivelly for groups)
 *
 * \param[in] iItems - List of items we will change color for
 * \param[in] iColor - Items color
 */
void
GraphicsScene::setItemColor(QList<QGraphicsItem *> iItems, const QColor &iColor)
{
    // colorize each item that is on the scene
    foreach (QGraphicsItem *item, iItems) {
        using namespace Gui::GraphicsItems;
        if (toDbObject(item)) {
            setItemColor(toDbObject(item), iColor);
        }

        if (toGroup(item)) {
            setItemColor(toGroup(item)->children(), iColor);
        }
    }
}

/*!
 * \brief Set item color
 *
 * \param[in] iItem - Graphics item
 * \param[in] iColor - Item color
 */
void
GraphicsScene::setItemColor(Gui::GraphicsItems::GraphicsItem *iItem, const QColor &iColor)
{
//    iItem->setItemColor(iColor);
    emit itemColorChanged(iItem, iColor, iItem->itemColor());
}

/*!
 * \brief Select all items if ctrl-a is pressed
 */
void
GraphicsScene::selectAllItems()
{
    // items can mutate in the loop
    foreach (QGraphicsItem *item, items()) {
        using namespace Gui::GraphicsItems;
        if (toDbObject(item)) {
            toDbObject(item)->setSelected(true);
        }
    }
}

/*!
 * \brief Auto resize selected items' sizes to adjusted
 */
void
GraphicsScene::adjustItems()
{
    adjustItems(selectedItems());
}

/*!
 * \brief Auto resize items (recursivelly for groups)
 *
 * \param[in] iItems - List of items we will adjust size for
 */
void
GraphicsScene::adjustItems(QList<QGraphicsItem *> iItems)
{
    foreach (QGraphicsItem *item, iItems) {
        using namespace Gui::GraphicsItems;
        if (toDbObject(item)) {
            mOldRect = toDbObject(item)->boundingRect();
            toDbObject(item)->adjustSize();
            emit itemResized(toDbObject(item), toDbObject(item)->boundingRect(), mOldRect);
        } else if (toGroup(item)) {
            adjustItems(toGroup(item)->children());
        }
    }
}

/*!
 * \brief Group items
 *
 * \param[in] iList - List of items we will group
 */
void
GraphicsScene::groupItems(QList<QGraphicsItem *> iItems)
{
    // if we try to group only one item - return
    if (1 == iItems.count()) {
        return;
    }

    using namespace Gui::GraphicsItems;
    
    ItemGroup *group = createItemGroup(iItems);
    group->setContextMenu(mTableMenu);
}

/*!
 * \brief Group items
 */
void
GraphicsScene::groupItems()
{
    groupItems(selectedItems());
}

/*!
 * \brief Ungroup items
 *
 * \param[in] iList - List of items we will ungroup
 */
void
GraphicsScene::ungroupItems(QList<QGraphicsItem *> iItems)
{
    foreach (QGraphicsItem *item, iItems) {
        using namespace Gui::GraphicsItems;
        if (toGroup(item)) {
            destroyItemGroup(toGroup(item));
        }
    }
}

/*!
 * \brief Ungroup items
 */
void
GraphicsScene::ungroupItems()
{
    ungroupItems(selectedItems());
}

/*!
 * \brief Colorize items according schemas
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
            using namespace Gui::GraphicsItems;
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
 * \brief Set the anchor for selected items
 *
 * \param[in] iFlag - True if we want to enable anchor for items, false if disable
 */
void
GraphicsScene::setAnchor(bool iFlag)
{
    setAnchor(selectedItems(), iFlag);
}

/*!
 * \brief Set the anchor for selected items
 *
 * \param[in] iItems - List of items we will enable or disable anchors for
 * \param[in] iFlag - True if we want to enable anchor for items, false if disable
 */
void
GraphicsScene::setAnchor(QList<QGraphicsItem *> iItems, bool iFlag)
{
    foreach (QGraphicsItem *item, iItems) {
        using namespace Gui::GraphicsItems;
        if (toGroup(item)) {
            item->setFlag(QGraphicsItem::ItemIsMovable, iFlag);
            setAnchor(item->children(), iFlag);
        } else if (toDbObject(item)) {
            item->setFlag(QGraphicsItem::ItemIsMovable, iFlag);
        }
    }
}

/*!
 * \brief Show/hide grid
 *
 * \param[in] iFlag - True if we want to want to show grid, false otherwise
 */
void
GraphicsScene::showGrid(bool iFlag)
{
    // only remember given flag; all analyze will be done in scene class
    Control::Config().setShowGrid(iFlag);
    update(QRectF(0.0, 0.0, width(), height()));
}

/*!
 * \brief (Un)Align items to the grid
 *
 * \param[in] iFlag - True if we want to align items according grid, false it we doesn't
 */
void
GraphicsScene::alignToGrid(bool iFlag)
{
    Control::Config().setAlignToGrid(iFlag);
}

/*!
 * \brief Show/hide grid
 *
 * \param[in] iFlag - True if we want to divide scene into pages, false otherwise
 */
void
GraphicsScene::divideIntoPages(bool iFlag)
{
    // only remember given flag; all analyze will be done in scene class
    Control::Config().setDivideIntoPages(iFlag);
    update(QRectF(0.0, 0.0, width(), height()));
}

/*!
 * \brief Select all items in schema
 */
void
GraphicsScene::selectAllItemsInSchema()
{
    foreach (QGraphicsItem *itemFromSelected, selectedItems()) {
        // items can mutate in the loop
        foreach (QGraphicsItem *itemFromAll, items()) {
            using namespace Gui::GraphicsItems;
            if (toDbObject(itemFromAll) && toDbObject(itemFromSelected) &&
                    toDbObject(itemFromAll)->schemaName() ==
                    toDbObject(itemFromSelected)->schemaName()) {
                (itemFromAll)->setSelected(true);
            }
        }
    }
}


/*!
 * \brief Save scene to xml
 *
 * \param[in] iDoc - Xml dom document
 * \param[in] iShowGrid - True if we need to show grid, false otherwise
 * \param[in] iDivideIntoPages - True if we need to divide scene into pages, false otherwise
 * \param[in] iShowLegend - True if we need to show legend, false otherwise
 * \param[in] iShowControlWidget - True if we need to show control widget, false otherwise
 *
 * \return Filled with scene info xml dom element
 */
QDomElement
GraphicsScene::toXml(QDomDocument &iDoc, bool iShowGrid, bool iDivideIntoPages, bool iShowLegend, bool iShowControlWidget) const
{
    QDomElement element = iDoc.createElement("scene");
    element.setAttribute("grid", iShowGrid);
    element.setAttribute("divideIntoPages", iDivideIntoPages);
    element.setAttribute("legend", iShowLegend);
    element.setAttribute("controlWidget", iShowControlWidget);

    foreach (QGraphicsItem *item, items()) {
        using namespace Gui::GraphicsItems;
        if (toGroup(item) && 0 == toGroup(item)->parentItem()) {
            element.appendChild(toGroup(item)->toXml(iDoc));
        } else if (toDbObject(item) && 0 == toDbObject(item)->parentItem()) {
            element.appendChild(toDbObject(item)->toXml(iDoc));
        }
    }

    return element;
}

/*!
 * \brief Restore scene from xml
 *
 * \param[in] iNode - Dom node we will restore scene parameters from
 *
 * \return List of graphic items restored from xml
 */
QList<QGraphicsItem *>
GraphicsScene::fromXml(const QDomNode &iNode)
{
    QList<QGraphicsItem *> itemList;
    QDomNode node = iNode;
    // show all elements
    while (!node.isNull()) {
        QDomElement element = node.toElement();
        if (!element.isNull()) {
            using namespace Gui::GraphicsItems;
            
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
 * \brief Move the legend
 *
 * \param[in] iDx - Gorizontal distance
 * \param[in] iDy - Vertical distance
 */
void
GraphicsScene::moveLegend(int iDx, int iDy)
{
    mLegend->moveBy(iDx, iDy);
}

/*!
 * \brief Resize the scene
 *
 * \param iFactor - Scale factor
 */
void
GraphicsScene::resize(int iFactor)
{
    static int factor = 0;
    setSceneRect(0, 0, width() + (iFactor - factor) * 10, height() + (iFactor - factor) * 10);
    factor = iFactor;
    update(QRectF(0.0, 0.0, width(), height()));
}

/*!
 * \brief Set move mode
 *
 * \param[in] iFlag - True if we want to set move mode, false otherwise
 */
void
GraphicsScene::setMoveMode(bool iFlag)
{
    mMoveMode = iFlag;
}

/*!
 * \brief Get move mode
 *
 * \return True if move mode is enabled, false otherwise
 */
bool
GraphicsScene::moveMode() const
{
    return mMoveMode;
}

/*!
 * \brief Deletes all cached items that were created since last flush
 * \note The function is to be used for destroying the cached objects on such events as
 *       disconnecting, reconnecting, reloading data from db, etc.
 */
void
GraphicsScene::flushCache()
{
    QList<QGraphicsItem*> itemsList = items();
    deleteItems(itemsList);
    qDeleteAll(mDbItems);
    mDbItems.clear();
    qDeleteAll(mArrows);
    mArrows.clear();
}

}

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
#include <QMenu>
#include <QPainter>
#include <QPen>
#include <QPrinter>
#include <QTreeWidgetItem>
#include <common/Database.h>
#include <common/DbTable.h>
#include <gui/ArrowItem.h>
#include <gui/ControlWidget.h>
#include <gui/GraphicsScene.h>
#include <gui/Legend.h>
#include <gui/TableItem.h>
#include <gui/TableItemGroup.h>
#include <gui/TreeWidget.h>
#include <math.h>

#include <QDebug>

/*
 * Constructor
 */
GraphicsScene::GraphicsScene()
    : QGraphicsScene(), mSelectionPath(), mMoveMode(false), mOldPos()
{
    setBackgroundBrush(QBrush(mSettings.value("Color/Background", Qt::white).value<QColor>()));
    setSceneRect(0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT);
    mLegend = new Legend();
    mLegend->setZValue(1000);
}

/*
 * Destructor
 */
GraphicsScene::~GraphicsScene()
{
}

/*
 * Set scene context menu
 */
void
GraphicsScene::setSchemeMenu(QMenu *ipSchemeMenu)
{
    mSchemeMenu = ipSchemeMenu;
}

void
GraphicsScene::setTableMenu(QMenu *ipTableMenu)
{
    mTableMenu = ipTableMenu;
}

/*
 * Add table from tree event to the scene
 */
QList<QGraphicsItem *>
GraphicsScene::showOnScene(QTreeWidgetItem *ipTreeItem, int ipCol)
{ 
    QList<QGraphicsItem *> tableList;

    if (0 == ipTreeItem) {
        return tableList;
    }
   
    // get database object id
    int objId = ipTreeItem->text(TreeWidget::IdCol).toInt();

    // if schema or table were double clicked
    if (TreeWidget::SchemaItem == objId || TreeWidget::TableItem == objId) {
        // check whether item is a schema item
        if (TreeWidget::SchemaItem == ipTreeItem->text(TreeWidget::IdCol).toInt()) {
            // add all its table children to the scene
            for (int i = 0; i < ipTreeItem->childCount(); ++i) {
                if (TreeWidget::TableNode == ipTreeItem->child(i)->text(TreeWidget::IdCol).toInt()) {
                    for (int j = 0; j < ipTreeItem->child(i)->childCount(); ++j) {
                        tableList << showOnScene(ipTreeItem->child(i)->child(j), /*TreeWidget::NameCol*/ipCol);
                    }
                }
            }
            // don't add schema item itself
            return tableList;
        }

	TableItem *table = newTableItem(ipTreeItem->parent()->parent()->text(TreeWidget::NameCol),
		ipTreeItem->text(TreeWidget::NameCol), mTableMenu);
	tableList.append(table);

    } else if (TreeWidget::TableNode == objId) {
        for (int i = 0; i < ipTreeItem->childCount(); ++i) {
            showOnScene(ipTreeItem->child(i), ipCol);
        }
    }

    return tableList;
}

/*
 * Create new table item
 */
TableItem *
GraphicsScene::newTableItem(QString ipSchemaName, QString ipTableName, QMenu *ipMenu)
{
    TableItem *newItem = findTableItem(ipSchemaName, ipTableName);
    // check if such item is already on the scene
    if (newItem) {
	return newItem;
    }

    // create new table
    newItem = new TableItem(ipSchemaName, ipTableName, ipMenu);
    return newItem;
}

/*
 * Add existent table items to the scene
 */
void
GraphicsScene::addTableItems(const QList<QGraphicsItem *> &ipItems) 
{
    foreach (QGraphicsItem *item, ipItems) {
	if (qgraphicsitem_cast<TableItemGroup *>(item)) {
	    TableItemGroup *group = qgraphicsitem_cast<TableItemGroup *>(item);
            addTableItems(group->children());
	    createItemGroup(group->children());
	} else if (qgraphicsitem_cast<TableItem *>(item)) {
	    addItem(item);
	}
    }
    drawRelations();
    clearSelection();
    update();
}

/*
 * Draw relations for all tables on the scene
 */
void
GraphicsScene::drawRelations()
{
    // draw all relations between new table and already added ones
    foreach (QGraphicsItem *item, items()) {
	if (qgraphicsitem_cast<TableItem *>(item)) {
	    createRelations(qgraphicsitem_cast<TableItem *>(item));
	}
    }  
}

/*
 * Create relations between given table item and anothers ones already painted
 */
void
GraphicsScene::createRelations(TableItem *ipSourceItem)
{
    DbTable *tableModel = ipSourceItem->tableModel();
    // find foreign keys and tables related to this keys
    for (int i = 0; i < tableModel->columnsCount(); ++i) {
	if (tableModel->isColumnForeignKey(i)) {
	    TableItem *destItem = 0;

	    // if founded, create arrow
	    if ((destItem = findTableItem(tableModel->foreignSchemaName(i), tableModel->foreignTableName(i))) != 0) {
		ArrowItem *arrow = new ArrowItem(ipSourceItem, destItem);
		ipSourceItem->addArrowItem(arrow);
		destItem->addArrowItem(arrow);
		arrow->setZValue(-1000.0);
		addItem(arrow);
		arrow->updatePosition();
	    }
	}
    }
}

/*
 * Find table by his name on the scene
 */
TableItem *
GraphicsScene::findTableItem(const QString &ipSchemaName, const QString &ipTableName)
{
    foreach (QGraphicsItem *item, items()) {
	if (qgraphicsitem_cast<TableItem *>(item)) {
	    TableItem *tableItem = qgraphicsitem_cast<TableItem *>(item);
	    if (tableItem->schemaName() == ipSchemaName && tableItem->tableName() == ipTableName) {
		return tableItem;
	    }
	}
    }

    return 0;
}

/*
 * Handler for right mouse button click
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

/*
 * Handler for mouse press event
 */
void
GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *ipEvent)
{
    if (mMoveMode) {
	QGraphicsScene::mousePressEvent(ipEvent);
	return;
    } else {
	// if we pressed under item - do default actions and return
	QGraphicsItem *item = itemAt(ipEvent->scenePos());
	if (item && qgraphicsitem_cast<TableItem *>(item) || qgraphicsitem_cast<TableItemGroup *>(item)) {
	    mOldPos = item->scenePos();
	    QGraphicsScene::mousePressEvent(ipEvent);
	    return;
	}
     
	clearSelection();
	mStartSelect = ipEvent->scenePos();
	   
	QGraphicsScene::mousePressEvent(ipEvent);
    }
}

/*
 * Handler for mouse release event
 */
void
GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *ipEvent)
{
    QGraphicsItem *item = itemAt(ipEvent->scenePos());
    qDebug() << item << mOldPos.x() << ipEvent->scenePos().x();
    if (item && (qgraphicsitem_cast<TableItem *>(item) || qgraphicsitem_cast<TableItemGroup *>(item)) && 
	    ipEvent->scenePos() != mOldPos) {
//	emit tableMoved(selectedItems(), mOldPos - item->scenePos());
    }

    if (mMoveMode) {
	QGraphicsScene::mouseReleaseEvent(ipEvent);
	return;
    } else {
	if (mSelectionPath) {
	    removeItem(mSelectionPath);
	    mSelectionPath = 0;
	}

	QGraphicsScene::mouseReleaseEvent(ipEvent);
    }
}

/*
 * Handler for mouse move event
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

/*
 * Create table item group from the given items
 */
TableItemGroup *
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
    TableItemGroup *group = new TableItemGroup(commonAncestor);
    if (!commonAncestor) {
        addItem(group);
    }

    foreach (QGraphicsItem *item, items) {
	if (qgraphicsitem_cast<TableItem *>(item) || qgraphicsitem_cast<TableItemGroup *>(item)) {
	    group->addToGroup(item);
	}
    }
    return group;
}

/*
 * Draw background grid
 */
void
GraphicsScene::drawBackground(QPainter *ipPainter, const QRectF &)
{
    bool needDrawGrid = mSettings.value("View/ShowGrid", true).value<bool>();
    if (needDrawGrid) {
	drawGrid(ipPainter);
    }
   
    bool needDivideIntoPages = mSettings.value("View/DivideIntoPages", true).value<bool>();
    if (needDivideIntoPages) {
	divideIntoPages(ipPainter);
    }
}

/*
 * Draw the background grid
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

/*
 * Draw the page's bounds
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

/*
 * Refresh legend
 */
void
GraphicsScene::refreshLegend()
{
    mLegend->refresh();
    updateLegend();
}

/*
 * Show/hide legend
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

/*
 * Update legend
 */
void
GraphicsScene::updateLegend()
{
    mLegend->update(mLegend->rect());
}

/*
 * Delete given items (recursivelly for groups)
 */
void
GraphicsScene::deleteTableItems(QList<QGraphicsItem *> ipItems)
{
    foreach (QGraphicsItem *item, ipItems) {
        if (qgraphicsitem_cast<TableItem *>(item)) {
            qgraphicsitem_cast<TableItem *>(item)->removeArrowItems();
        } else if (qgraphicsitem_cast<TableItemGroup *>(item)) {
            deleteTableItems(qgraphicsitem_cast<TableItemGroup *>(item)->children());
	}
        removeItem(item);
    }

    updateLegend();
    update();
}

/*
 * Show field types for selected items
 */
void
GraphicsScene::setFieldsTypesVisible()
{
    setFieldsTypesVisible(selectedItems(), true);
}

/*
 * Hide field types for selected items
 */
void
GraphicsScene::setFieldsTypesInvisible()
{
    setFieldsTypesVisible(selectedItems(), false);
}


/*
 * Show/hide field types for given items (recursivelly for groups)
 */
void
GraphicsScene::setFieldsTypesVisible(QList<QGraphicsItem *> ipItems, bool ipFlag)
{
    foreach (QGraphicsItem *item, ipItems) {
        if (qgraphicsitem_cast<TableItem *>(item)) {
            qgraphicsitem_cast<TableItem *>(item)->setFieldsTypesVisible(ipFlag);
        } else if (qgraphicsitem_cast<TableItemGroup *>(item)) {
            setFieldsTypesVisible(qgraphicsitem_cast<TableItemGroup *>(item)->children(), ipFlag);
	}
    }
}

/*
 * Show/hide indices for selected tables
 */
void
GraphicsScene::setIndicesVisible(bool ipFlag)
{
    setIndicesVisible(selectedItems(), ipFlag);
}

/*
 * Show/hide indices for selected tables
 */
void
GraphicsScene::setIndicesVisible(QList<QGraphicsItem *> ipItems, bool ipFlag)
{
    foreach (QGraphicsItem *item, ipItems) {
        if (qgraphicsitem_cast<TableItem *>(item)) {
            qgraphicsitem_cast<TableItem *>(item)->setIndicesVisible(ipFlag);
        } else if (qgraphicsitem_cast<TableItemGroup *>(item)) {
            setIndicesVisible(qgraphicsitem_cast<TableItemGroup *>(item)->children(), ipFlag);
	}
    }
}

/*
 * Set all selected tables' color to user choosed color
 */
void
GraphicsScene::setTableColor()
{
    // prompt for color
    QColor color = QColorDialog::getColor(Qt::white);

    // if color is valid
    if (color.isValid()) {
	setTableColor(selectedItems(), color);
    }

    updateLegend();
}

/*
 * Set all items to given color (recursivelly for groups)
 */
void
GraphicsScene::setTableColor(QList<QGraphicsItem *> ipItems, QColor ipColor)
{
    // ipColorize each table that is on the scene
    foreach (QGraphicsItem *item, ipItems) {
        if (qgraphicsitem_cast<TableItem *>(item)) {
            setTableColor(qgraphicsitem_cast<TableItem *>(item), ipColor);
        } else if (qgraphicsitem_cast<TableItemGroup *>(item)) {
            setTableColor(qgraphicsitem_cast<TableItemGroup *>(item)->children(), ipColor);
	}
    }
}

/*
 * Set item to given color
 */
void
GraphicsScene::setTableColor(TableItem *ipItem, QColor ipColor)
{
    ipItem->setItemColor(ipColor);
}

/*
 * Select all tables if ctrl-a is pressed
 */
void
GraphicsScene::selectAllTables()
{
    // items can mutate in the loop
    foreach (QGraphicsItem *item, items()) {
        if (qgraphicsitem_cast<TableItem *>(item)) {
            qgraphicsitem_cast<TableItem *>(item)->setSelected(true);
	}
    }
}

/*
 * Auto resize selected tables' sizes to adjusted
 */
void
GraphicsScene::adjustTables()
{
    adjustTables(selectedItems());
}

/*
 * Auto resize items (recursivelly for groups)
 */
void
GraphicsScene::adjustTables(QList<QGraphicsItem *> ipItems)
{
    foreach (QGraphicsItem *item, ipItems) {
        if (qgraphicsitem_cast<TableItem *>(item)) {
            qgraphicsitem_cast<TableItem *>((item))->adjustSize();
        } else if (qgraphicsitem_cast<TableItemGroup *>(item)) {
            adjustTables(qgraphicsitem_cast<TableItemGroup *>((item))->children());
	}
    }
}

/*
 * Group items
 */
void
GraphicsScene::groupItems(QList<QGraphicsItem *> ipItems)
{
    // if we try to group only one item - return
    if (1 == ipItems.count()) {
	return;
    }

    TableItemGroup *group = createItemGroup(ipItems);
    group->setContextMenu(mTableMenu);
}

/*
 * Group items
 */
void
GraphicsScene::groupItems()
{
    groupItems(selectedItems());
}

/*
 * Ungroup items
 */
void
GraphicsScene::ungroupItems(QList<QGraphicsItem *> ipItems)
{
    foreach (QGraphicsItem *item, ipItems) {
        if (qgraphicsitem_cast<TableItemGroup *>(item)) {
            destroyItemGroup(qgraphicsitem_cast<TableItemGroup *>(item));
	}
    }
}

/*
 * Ungroup items
 */
void
GraphicsScene::ungroupItems()
{
    ungroupItems(selectedItems());
}

/*
 * Colorize tables according schemas
 */
void
GraphicsScene::colorizeAccordingSchemas()
{
    Database *dbInst = Database::instance();

    QStringList schemasNames;
    dbInst->schemasList(&schemasNames);

    for (int i = 0; i < schemasNames.count(); ++i) {
	// items can mutate in the loop
	foreach (QGraphicsItem *item, items()) {
	    TableItem *tableItem = qgraphicsitem_cast<TableItem *>(item);
	    if (tableItem && tableItem->schemaName() == schemasNames.at(i)) {
		// use only two colors from the palette
		int red = (0 == i % 3) ? 255 * (i + 1) / schemasNames.count() : 0;
		int green = (1 == i % 3) ? 255 * (i + 1) / schemasNames.count() : 0;
		int blue = (2 == i % 3) ? 255 * (i + 1) / schemasNames.count() : 0;
		setTableColor(tableItem, QColor(red, green, blue));
	    }
	}
    }

    updateLegend();
}

/*
 * Show/hide grid
 */
void
GraphicsScene::showGrid(bool ipFlag)
{
    // only remember given flag; all analyze will be done in scene class
    mSettings.setValue("View/ShowGrid", ipFlag);
    update(QRectF(0.0, 0.0, width(), height()));
}

/*
 * (Un)Align tables to the grid
 */
void
GraphicsScene::alignToGrid(bool ipFlag)
{
    mSettings.setValue("View/AlignToGrid", ipFlag);
}

/*
 * Show/hide grid
 */
void
GraphicsScene::divideIntoPages(bool ipFlag)
{
    // only remember given flag; all analyze will be done in scene class
    mSettings.setValue("View/DivideIntoPages", ipFlag);
    update(QRectF(0.0, 0.0, width(), height()));
}

/*
 * Select all tables in schema
 */
void
GraphicsScene::selectAllTablesInSchema()
{
    foreach (QGraphicsItem *itemFromSelected, selectedItems()) {
	// items can mutate in the loop
	foreach (QGraphicsItem *itemFromAll, items()) {
	    if (qgraphicsitem_cast<TableItem *>(itemFromAll) && 
		    qgraphicsitem_cast<TableItem *>(itemFromSelected) &&
		    qgraphicsitem_cast<TableItem *>(itemFromAll)->schemaName() ==
		    qgraphicsitem_cast<TableItem *>(itemFromSelected)->schemaName()) {
		(itemFromAll)->setSelected(true);
	    }
	}
    }
}

/*
 * Move the legend
 */
void
GraphicsScene::moveLegend(int ipDx, int ipDy)
{
    mLegend->moveBy(ipDx, ipDy);
}

/*
 * Resize the scene
 */
void
GraphicsScene::resize(int ipFactor)
{
    static int factor = 0;
    setSceneRect(0, 0, width() + (ipFactor - factor) * 10, height() + (ipFactor - factor) * 10);
    factor = ipFactor;
    update(QRectF(0.0, 0.0, width(), height()));
}

/*
 * Set move mode
 */
void
GraphicsScene::setMoveMode(bool ipFlag)
{
    mMoveMode = ipFlag;
}

/*
 * Get move mode
 */
bool
GraphicsScene::moveMode() const
{
    return mMoveMode;
}


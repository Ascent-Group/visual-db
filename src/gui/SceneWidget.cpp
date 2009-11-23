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
#include <QFileDialog>
#include <QGridLayout>
#include <QImage>
#include <QPainter>
#include <QPrinter>
#include <gui/ControlWidget.h>
#include <gui/GraphicsScene.h>
#include <gui/GraphicsView.h>
#include <gui/SceneWidget.h>
#include <gui/TableItem.h>
#include <gui/TableItemGroup.h>
#include <gui/behaviour/AddTableCommand.h>
#include <gui/behaviour/DeleteTableCommand.h>
#include <gui/behaviour/MoveTableCommand.h>

#include <QDebug>

/*
 * Constructor
 */
SceneWidget::SceneWidget(QWidget *ipParent, Qt::WindowFlags ipFlags)
    : QWidget(ipParent, ipFlags)
{
    mScene = new GraphicsScene();

    mView = new GraphicsView(mScene, this);
    mView->centerOn(0, 0);

    mControlWidget = new ControlWidget(GraphicsView::MINIMUM_FACTOR, GraphicsView::MAXIMUM_FACTOR);

    connect(mControlWidget, SIGNAL(valueChanged(int)), mView, SLOT(scaleView(int)));
    connect(mControlWidget, SIGNAL(movedUp()), mView, SLOT(moveUp()));
    connect(mControlWidget, SIGNAL(movedDown()), mView, SLOT(moveDown()));
    connect(mControlWidget, SIGNAL(movedLeft()), mView, SLOT(moveLeft()));
    connect(mControlWidget, SIGNAL(movedRight()), mView, SLOT(moveRight()));
    connect(mControlWidget, SIGNAL(settedMoveMode(bool)), mView, SLOT(setMoveMode(bool)));

    connect(mScene, SIGNAL(tableMoved(QGraphicsItem *, const QPointF &)), 
	    this, SLOT(sendTableMoved(QGraphicsItem *, const QPointF &)));

    mainLayout = new QGridLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);

    QGridLayout *controlLayout = new QGridLayout();
    controlLayout->addWidget(mControlWidget, 0, 0, Qt::AlignTop | Qt::AlignRight);
    controlLayout->setColumnMinimumWidth(1, 20);
    mainLayout->addLayout(controlLayout, 0, 0, Qt::AlignTop | Qt::AlignRight);
    mainLayout->addWidget(mView, 0, 0);

    setLayout(mainLayout);
}

/*
 * Destructor
 */
SceneWidget::~SceneWidget()
{
}

/*
 * Set the scene menu
 */
void
SceneWidget::setSchemeMenu(QMenu *ipMenu)
{
    mSchemeMenu = ipMenu;
    mScene->setSchemeMenu(ipMenu);
}

/*
 * Set the table menu
 */
void
SceneWidget::setTableMenu(QMenu *ipMenu)
{
    mTableMenu = ipMenu;
    mScene->setTableMenu(ipMenu);
}

/*
 * Add table from tree event to the scene
 */
void
SceneWidget::showOnScene(QTreeWidgetItem *ipTreeItem, int ipCol)
{
    QList<QGraphicsItem *> tableList = mScene->showOnScene(ipTreeItem, ipCol);
    emit tableActionDone(new AddTableCommand(mScene, tableList));
}

/*
 * Show/hide legend
 */
void
SceneWidget::showLegend(bool ipFlag)
{
    mScene->showLegend(ipFlag);
}

/*
 * Delete selected tables from scheme
 */
void
SceneWidget::deleteTableItem()
{
    if (mScene->selectedItems().count() > 0) { 
	emit tableActionDone(new DeleteTableCommand(mScene, mScene->selectedItems()));
    }
}

/*
 * Clean table scheme scene
 */
void
SceneWidget::cleanTableSchemeScene()
{
    if (mScene->items().count() > 0) {
	// first send the signal to remember deleted items
	emit tableActionDone(new DeleteTableCommand(mScene, mScene->items()));;
    }
}

/*
 * Show field types for selected items
 */
void
SceneWidget::setFieldsTypesVisible()
{
    mScene->setFieldsTypesVisible();
}

/*
 * Hide field types for selected items
 */
void
SceneWidget::setFieldsTypesInvisible()
{
    mScene->setFieldsTypesInvisible();
}

/*
 * Show indices for selected tables
 */
void
SceneWidget::setIndicesVisible()
{
    mScene->setIndicesVisible(true);
}

/*
 * Hide indices for selected tables
 */
void
SceneWidget::setIndicesInvisible()
{
    mScene->setIndicesVisible(false);
}

/*
 * Set all selected tables' color to user choosed color
 */
void
SceneWidget::setTableColor()
{
    mScene->setTableColor();
}

/*
 * Select all tables if ctrl-a is pressed
 */
void
SceneWidget::selectAllTables()
{
    mScene->selectAllTables();
}

/*
 * Auto resize selected tables' sizes to adjusted
 */
void
SceneWidget::adjustTables()
{
    mScene->adjustTables();
}

/*
 * Group items
 */
void
SceneWidget::groupItems()
{
    mScene->groupItems();
}

/*
 * Ungroup items
 */
void
SceneWidget::ungroupItems()
{
    mScene->ungroupItems();
}

/*
 * Anchor selected items
 */
void
SceneWidget::anchorTables()
{
    setAnchor(mScene->selectedItems(), false);
}

/*
 * Weight anchor selected items
 */
void
SceneWidget::weightAnchorTables()
{
    setAnchor(mScene->selectedItems(), true);
}

/*
 * Set the anchor for selected items
 */
void
SceneWidget::setAnchor(QList<QGraphicsItem *> ipItems, bool ipFlag)
{
    foreach (QGraphicsItem *item, ipItems) {
	if (qgraphicsitem_cast<TableItemGroup *>(item)) {
	    setAnchor(item->children(), ipFlag);
	} else if (qgraphicsitem_cast<TableItem *>(item)) {
	    item->setFlag(QGraphicsItem::ItemIsMovable, ipFlag);
	}
    }
}

/*
 * Colorize tables according schemas
 */
void
SceneWidget::colorizeAccordingSchemas()
{
    mScene->colorizeAccordingSchemas();
}

/*
 * Show/hide grid
 */
void
SceneWidget::showGrid(bool ipFlag)
{
    mScene->showGrid(ipFlag);
}

/*
 * Attach detach tables to the grid
 */
void
SceneWidget::attachToGrid(bool ipFlag)
{
    mScene->attachToGrid(ipFlag);
}

/*
 * Show/hide grid
 */
void
SceneWidget::divideOnPages(bool ipFlag)
{
    mScene->divideOnPages(ipFlag);
}

/*
 * Select all tables in schema
 */
void
SceneWidget::selectAllTablesInSchema()
{
    mScene->selectAllTablesInSchema();
}

/*
 * Refresh the legend rect
 */
void
SceneWidget::refreshLegend()
{
    mScene->refreshLegend();
}

/*
 * Update the legend's records
 */
void
SceneWidget::updateLegend()
{
    mScene->updateLegend();
}

/*
 * Create table item group from the given items
 */
TableItemGroup *
SceneWidget::createItemGroup(const QList<QGraphicsItem *> &ipItems)
{
    return mScene->createItemGroup(ipItems);
}

/*
 * Return all items for the scene
 */
QList<QGraphicsItem *>
SceneWidget::items () const
{
    return mScene->items();
}

/*
 * Save scheme to image
 */
void
SceneWidget::saveToImage()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save to image..."),
	    "untitled.png",
	    tr("Images (*.png *.jpg)"));
    // return if we don't select any file to save
    if (fileName == "") {
	return;
    }

    // create image
    QImage *image = new QImage((int)mScene->width(), (int)mScene->height(), QImage::Format_ARGB32);

    // print to image
    QPainter painter(image);
    mScene->render(&painter);

    // save
    image->save(fileName);
}

/*
 * Show/hide control widget
 */
void
SceneWidget::showControlWidget(bool ipFlag)
{
    mControlWidget->setVisible(ipFlag);
}

/*
 * Get xml structure of scene
 */
QDomElement
SceneWidget::toXml(QDomDocument &ipDoc, bool ipShowGrid, bool ipDivideOnPages, bool ipShowLegend, bool ipShowControlWidget)
{
    QDomElement element = ipDoc.createElement("scene");
    element.setAttribute("grid", ipShowGrid);
    element.setAttribute("divideOnPages", ipDivideOnPages);
    element.setAttribute("legend", ipShowLegend);
    element.setAttribute("controlWidget", ipShowControlWidget);
    
    foreach (QGraphicsItem *item, items()) {
	TableItem *tableItem;
	TableItemGroup *groupItem;
	if ((groupItem = qgraphicsitem_cast<TableItemGroup *>(item)) && 0 == groupItem->parentItem()) {
	    element.appendChild(groupItem->toXml(ipDoc));
	} else if ((tableItem = qgraphicsitem_cast<TableItem *>(item)) && 0 == tableItem->parentItem()) {
	    element.appendChild(tableItem->toXml(ipDoc));
	}
    }

    return element;
}

/*
 * Load scene from the xml file
 */
void
SceneWidget::fromXml(QDomElement &ipElement)
{
    QDomNode child = ipElement.firstChild();
    bool grid = ipElement.attribute("grid").toInt();
    mScene->showGrid(grid);
    bool divideOnPages = ipElement.attribute("divideOnPages").toInt();
    mScene->divideOnPages(divideOnPages);
    bool legend = ipElement.attribute("legend").toInt();
    mScene->showLegend(legend);
    bool controlWidget = ipElement.attribute("controlWidget").toInt();
    showControlWidget(controlWidget);

    QList<QGraphicsItem *> tableList;
    // show all elements
    while (!child.isNull()) {
	QDomElement element = child.toElement();
	if (!element.isNull()) {
	    // it's a table
	    if (element.tagName() == "table") {
		tableList << tableFromXml(element);
	    // it's a table group
	    } else if (element.tagName() == "tableGroup") {
		tableList << tableGroupFromXml(element);
	    }
	}
	// go to the next element
	child = child.nextSibling();
    }

    emit tableActionDone(new AddTableCommand(mScene, tableList));
}

/*
 * Load table from the xml file
 */
TableItem *
SceneWidget::tableFromXml(QDomElement &ipElement)
{
    // get table's coordinates
    TableItem *newTable = mScene->newTableItem(ipElement.attribute("schema"), 
	    ipElement.attribute("name"), mTableMenu);
    newTable->moveBy(ipElement.attribute("x").toInt() - newTable->x(),
	    ipElement.attribute("y").toInt() - newTable->y());
    
    // get table's size
    newTable->setWidth(ipElement.attribute("width").toInt());
    newTable->setHeight(ipElement.attribute("height").toInt());
    newTable->updatePolygon();

    // get table's color
    newTable->setItemColor(QColor(ipElement.attribute("red").toInt(),
		ipElement.attribute("green").toInt(), ipElement.attribute("blue").toInt()));

    return newTable;
}

/*
 * Load table group from the xml file
 */
QList<QGraphicsItem *>
SceneWidget::tableGroupFromXml(QDomElement &ipElement)
{
    QList<QGraphicsItem *> tableList;
    // loop for all childs of this group
    QDomNode child = ipElement.firstChild();
    while (!child.isNull()) {
	QDomElement element = child.toElement();
	if (!element.isNull()) {
	    if (element.tagName() == "table") {
		tableList << tableFromXml(element);
	    } else if (element.tagName() == "tableGroup") {
		tableList << tableGroupFromXml(element);
	    }
	}
	child = child.nextSibling();
    }

    TableItemGroup *group = mScene->createItemGroup(tableList);
    group->setContextMenu(mTableMenu);
    tableList << group;
    return tableList;
}

/*
 * Print the graphic widget
 */
void
SceneWidget::print(QPrinter *ipPrinter)
{
    QPainter painter(ipPrinter);
    painter.setFont(QFont("Arial", 10));
    int pageWidth = ipPrinter->pageRect().width();
    int pageHeight = ipPrinter->pageRect().height();
    int maxI = (int)(mScene->width() / pageWidth + 1);
    int maxJ = (int)(mScene->height() / pageHeight + 1);
    for (int i = 0; i < maxI; ++i) {
	for (int j = 0; j < maxJ; ++j) {
	    mScene->render(&painter, QRectF(), QRectF(i * pageWidth, j * pageHeight, pageWidth, pageHeight));
	    if (i != maxI - 1 || j != maxJ - 1) {
		ipPrinter->newPage();
	    }
    	}
    }
}

/*
 * Send 'table moved' signal
 */
void
SceneWidget::sendTableMoved(QGraphicsItem *ipItem, const QPointF &ipPos)
{
    emit tableActionDone(new MoveTableCommand(ipItem, ipPos));
}


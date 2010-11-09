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
#include <gui/ItemGroup.h>
#include <gui/behaviour/AddItemCommand.h>
#include <gui/behaviour/DeleteItemCommand.h>
#include <gui/behaviour/MoveItemCommand.h>

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

    mControlWidget = new ControlWidget(/*GraphicsView::MINIMUM_FACTOR, GraphicsView::MAXIMUM_FACTOR*/);
    mControlWidget->setMinZoom(GraphicsView::MINIMUM_FACTOR);
    mControlWidget->setMaxZoom(GraphicsView::MAXIMUM_FACTOR);
    mControlWidget->setZoom((GraphicsView::MAXIMUM_FACTOR + GraphicsView::MINIMUM_FACTOR) / 2);

    connect(mControlWidget, SIGNAL(valueChanged(int)), mView, SLOT(scaleView(int)));
    connect(mControlWidget, SIGNAL(movedUp()), mView, SLOT(moveUp()));
    connect(mControlWidget, SIGNAL(movedDown()), mView, SLOT(moveDown()));
    connect(mControlWidget, SIGNAL(movedLeft()), mView, SLOT(moveLeft()));
    connect(mControlWidget, SIGNAL(movedRight()), mView, SLOT(moveRight()));
    connect(mControlWidget, SIGNAL(moveModeSet(bool)), mView, SLOT(setMoveMode(bool)));

    connect(mScene, SIGNAL(itemMoved(QList <QGraphicsItem *>, int, int)),
            this, SLOT(sendItemMoved(QList <QGraphicsItem *>, int, int)));

    connect(mView, SIGNAL(valueIncreased()), mControlWidget, SLOT(increaseValue()));
    connect(mView, SIGNAL(valueDecreased()), mControlWidget, SLOT(decreaseValue()));
    connect(mView, SIGNAL(itemActionDone(QUndoCommand *)), this, SLOT(sendItemActionDone(QUndoCommand *)));

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
 * Add item from tree event to the scene
 */
void
SceneWidget::showOnScene(QTreeWidgetItem *ipTreeItem, int ipCol)
{
    QList<QGraphicsItem *> itemLIst = mScene->showOnScene(ipTreeItem, ipCol, QPoint(0, 0));
    emit itemActionDone(new AddItemCommand(mScene, itemLIst));
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
 * Delete selected items from scheme
 */
void
SceneWidget::deleteItem()
{
    if (mScene->selectedItems().count() > 0) {
        emit itemActionDone(new DeleteItemCommand(*mScene, mScene->selectedItems()));
    }
}

/*
 * Clean scheme scene
 */
void
SceneWidget::cleanSchemeScene()
{
    if (mScene->items().count() > 0) {
        // first send the signal to remember deleted items
        emit itemActionDone(new DeleteItemCommand(*mScene, mScene->items()));
    }
    TableItem::setSeek(20);
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
 * Show indices for selected items
 */
void
SceneWidget::setIndicesVisible()
{
    mScene->setIndicesVisible(true);
}

/*
 * Hide indices for selected items
 */
void
SceneWidget::setIndicesInvisible()
{
    mScene->setIndicesVisible(false);
}

/*
 * Set all selected items' color to user choosed color
 */
void
SceneWidget::setItemColor()
{
    mScene->setItemColor();
}

/*
 * Select all items if ctrl-a is pressed
 */
void
SceneWidget::selectAllItems()
{
    mScene->selectAllItems();
}

/*
 * Auto resize selected items' sizes to adjusted
 */
void
SceneWidget::adjustItems()
{
    mScene->adjustItems();
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
SceneWidget::anchorItems()
{
    mScene->setAnchor(false);
}

/*
 * Disable anchor selected items
 */
void
SceneWidget::disableAnchorItems()
{
    mScene->setAnchor(true);
}

/*
 * Colorize items according schemas
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
 * (Un)Align items to the grid
 */
void
SceneWidget::alignToGrid(bool ipFlag)
{
    mScene->alignToGrid(ipFlag);
}

/*
 * Show/hide grid
 */
void
SceneWidget::divideIntoPages(bool ipFlag)
{
    mScene->divideIntoPages(ipFlag);
}

/*
 * Select all items in schema
 */
void
SceneWidget::selectAllItemsInSchema()
{
    mScene->selectAllItemsInSchema();
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
 * Create item group from the given items
 */
ItemGroup *
SceneWidget::createItemGroup(const QList<QGraphicsItem *> &ipItems)
{
    return mScene->createItemGroup(ipItems);
}

/*
 * Return all items for the scene
 */
QList<QGraphicsItem *>
SceneWidget::items() const
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
    mScene->clearSelection();
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
SceneWidget::toXml(QDomDocument &ipDoc, bool ipShowGrid, bool ipDivideIntoPages, bool ipShowLegend, bool ipShowControlWidget)
{
    return mScene->toXml(ipDoc, ipShowGrid, ipDivideIntoPages, ipShowLegend, ipShowControlWidget);
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
    bool divideIntoPages = ipElement.attribute("divideIntoPages").toInt();
    mScene->divideIntoPages(divideIntoPages);
    bool legend = ipElement.attribute("legend").toInt();
    mScene->showLegend(legend);
    bool controlWidget = ipElement.attribute("controlWidget").toInt();
    showControlWidget(controlWidget);

    QList<QGraphicsItem *> itemLIst = mScene->fromXml(child);

    emit itemActionDone(new AddItemCommand(mScene, itemLIst));
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

    mScene->clearSelection();

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
 * Send 'item moved' signal
 */
void
SceneWidget::sendItemMoved(QList <QGraphicsItem *> ipItemList, int ipDiffX, int ipDiffY)
{
    emit itemActionDone(new MoveItemCommand(ipItemList, ipDiffX, ipDiffY));
}

void
SceneWidget::sendItemActionDone(QUndoCommand *ipCommand)
{
    emit itemActionDone(ipCommand);
}

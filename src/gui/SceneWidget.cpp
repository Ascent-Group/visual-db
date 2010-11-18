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
#include <gui/behaviour/SetColorItemCommand.h>
#include <gui/behaviour/ResizeItemCommand.h>

#include <QDebug>

/*!
 * Constructor
 */
SceneWidget::SceneWidget(QWidget *ipParent, Qt::WindowFlags ipFlags)
    : QWidget(ipParent, ipFlags),
      mScene(0),
      mView(0),
      mControlWidget(0),
      mainLayout(0),
      mTableMenu(0),
      mSchemeMenu(0)
{
    mScene = new GraphicsScene(this);

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
    connect(mScene, SIGNAL(itemColorChanged(GraphicsItem *, const QColor &, const QColor &)),
            this, SLOT(sendItemColorChanged(GraphicsItem *, const QColor &, const QColor &)));
    connect(mScene, SIGNAL(itemResized(GraphicsItem *, const QRectF &, const QRectF &)),
            this, SLOT(sendItemResized(GraphicsItem *, const QRectF &, const QRectF &)));

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

/*!
 * Destructor
 */
SceneWidget::~SceneWidget()
{
}

/*!
 * \brief Set the scene menu
 *
 * \param[in] ipMenu - Schema context menu
 */
void
SceneWidget::setSchemeMenu(QMenu *ipMenu)
{
    mSchemeMenu = ipMenu;
    mScene->setSchemeMenu(ipMenu);
}

/*!
 * \brief Set the table menu
 *
 * \param[in] ipMenu - Table context menu
 */
void
SceneWidget::setTableMenu(QMenu *ipMenu)
{
    mTableMenu = ipMenu;
    mScene->setTableMenu(ipMenu);
}

/*!
 * \brief Add item from tree event to the scene
 *
 * \param[in] ipTreeItem - Tree item we should show on the scene
 * \param[in] ipCol - Tree item column identifier
 */
void
SceneWidget::showOnScene(QTreeWidgetItem *ipTreeItem, int ipCol, bool ipCenterOn)
{
    QList<QGraphicsItem *> itemLIst = mScene->showOnScene(ipTreeItem, ipCol, QPoint(0, 0), ipCenterOn);
    emit itemActionDone(new AddItemCommand(mScene, itemLIst));
}

/*!
 * \brief Show/hide legend
 *
 * \param[in] ipFlag - True if we want to show legend, false otherwise
 */
void
SceneWidget::showLegend(bool ipFlag)
{
    mScene->showLegend(ipFlag);
}

/*!
 * \brief Delete selected items from scheme
 */
void
SceneWidget::deleteItem()
{
    if (mScene->selectedItems().count() > 0) {
        emit itemActionDone(new DeleteItemCommand(*mScene, mScene->selectedItems()));
    }
}

/*!
 * \brief Clean scheme scene
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

/*!
 * \brief Show field types for selected items
 */
void
SceneWidget::setFieldsTypesVisible()
{
    mScene->setFieldsTypesVisible();
}

/*!
 * \brief Hide field types for selected items
 */
void
SceneWidget::setFieldsTypesInvisible()
{
    mScene->setFieldsTypesInvisible();
}

/*!
 * \brief Show indices for selected items
 */
void
SceneWidget::setIndicesVisible()
{
    mScene->setIndicesVisible(true);
}

/*!
 * \brief Hide indices for selected items
 */
void
SceneWidget::setIndicesInvisible()
{
    mScene->setIndicesVisible(false);
}

/*!
 * \brief Set all selected items' color to user choosed color
 */
void
SceneWidget::setItemColor()
{
    mScene->setItemColor();
}

/*!
 * \brief Select all items if ctrl-a is pressed
 */
void
SceneWidget::selectAllItems()
{
    mScene->selectAllItems();
}

/*!
 * \brief Auto resize selected items' sizes to adjusted
 */
void
SceneWidget::adjustItems()
{
    mScene->adjustItems();
}

/*!
 * \brief Group items
 */
void
SceneWidget::groupItems()
{
    mScene->groupItems();
}

/*!
 * \brief Ungroup items
 */
void
SceneWidget::ungroupItems()
{
    mScene->ungroupItems();
}

/*!
 * \brief Anchor selected items
 */
void
SceneWidget::anchorItems()
{
    mScene->setAnchor(false);
}

/*!
 * \brief Disable anchor selected items
 */
void
SceneWidget::disableAnchorItems()
{
    mScene->setAnchor(true);
}

/*!
 * \brief Colorize items according schemas
 */
void
SceneWidget::colorizeAccordingSchemas()
{
    mScene->colorizeAccordingSchemas();
}

/*!
 * \brief Show/hide grid
 *
 * \param[in] ipFlag - True if we want to show grid, false otherwise
 */
void
SceneWidget::showGrid(bool ipFlag)
{
    mScene->showGrid(ipFlag);
}

/*!
 * \brief (Un)Align items to the grid
 *
 * \param[in] ipFlag - True if we want to align items according grid, false otherwise
 */
void
SceneWidget::alignToGrid(bool ipFlag)
{
    mScene->alignToGrid(ipFlag);
}

/*!
 * \brief Show/hide grid
 *
 * \param[in] ipFlag - True if we want to show pages, false otherwise
 */
void
SceneWidget::divideIntoPages(bool ipFlag)
{
    mScene->divideIntoPages(ipFlag);
}

/*!
 * \brief Select all items in schema
 */
void
SceneWidget::selectAllItemsInSchema()
{
    mScene->selectAllItemsInSchema();
}

/*!
 * \brief Refresh the legend rect
 */
void
SceneWidget::refreshLegend()
{
    mScene->refreshLegend();
}

/*!
 * \brief Update the legend's records
 */
void
SceneWidget::updateLegend()
{
    mScene->updateLegend();
}

/*!
 * \brief Create item group from the given items
 *
 * \param[in] ipItems - List of items we will add to new group
 *
 * \return New item group
 */
ItemGroup *
SceneWidget::createItemGroup(const QList<QGraphicsItem *> &ipItems)
{
    return mScene->createItemGroup(ipItems);
}

/*!
 * \brief Return all items for the scene
 *
 * \return List of all items on the scene
 */
QList<QGraphicsItem *>
SceneWidget::items() const
{
    return mScene->items();
}

/*!
 * \brief Save scheme to image
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

/*!
 * \brief Show/hide control widget
 *
 * \param[in] ipFlag - True if we want to show control widget, false otherwise
 */
void
SceneWidget::showControlWidget(bool ipFlag)
{
    mControlWidget->setVisible(ipFlag);
}

/*!
 * \brief Get xml structure of scene
 *
 * \param[in] ipDoc - Xml dom document
 * \param[in] ipShowGrid - Is grid shown or not
 * \param[in] ipDivideIntoPages - Is scene divided into pages or not
 * \param[in] ipShowLegend - Is legend shown or not
 * \param[in] ipShowControlWidget - Is control widget shown or not
 *
 * \return Filled with scene info xml dom element
 */
QDomElement
SceneWidget::toXml(QDomDocument &ipDoc, bool ipShowGrid, bool ipDivideIntoPages, bool ipShowLegend, bool ipShowControlWidget)
{
    return mScene->toXml(ipDoc, ipShowGrid, ipDivideIntoPages, ipShowLegend, ipShowControlWidget);
}

/*!
 * \brief Load scene from the xml file
 *
 * \param[in] ipElement - Xml dom element
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

/*!
 * \brief Print the graphic widget
 *
 * \param[in] ipPrinter - Printer
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

/*!
 * \brief Send 'item moved' signal
 *
 * \param[in] ipItemList - Item list were moved
 * \param[in] ipDiffX - Gorizontal shift
 * \param[in] ipDiffY - Vertical shift
 */
void
SceneWidget::sendItemMoved(QList <QGraphicsItem *> ipItemList, int ipDiffX, int ipDiffY)
{
    emit itemActionDone(new MoveItemCommand(ipItemList, ipDiffX, ipDiffY));
}

void
SceneWidget::sendItemColorChanged(GraphicsItem *ipItem, const QColor &ipNewColor, const QColor &ipOldColor)
{
    emit itemActionDone(new SetColorItemCommand(ipItem, ipNewColor, ipOldColor));
}

void
SceneWidget::sendItemResized(GraphicsItem *ipItem, const QRectF &ipNewRect, const QRectF &ipOldRect)
{
    emit itemActionDone(new ResizeItemCommand(ipItem, ipNewRect, ipOldRect));
}

/*!
 * \brief Send 'action was performed' signal
 *
 * \param[in] ipCommand - Command which describes action
 */
void
SceneWidget::sendItemActionDone(QUndoCommand *ipCommand)
{
    emit itemActionDone(ipCommand);
}

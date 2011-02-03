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
#include <gui/graphicsitems/ItemGroup.h>
#include <gui/SceneWidget.h>
#include <gui/graphicsitems/TableItem.h>
#include <gui/behaviour/AddItemCommand.h>
#include <gui/behaviour/DeleteItemCommand.h>
#include <gui/behaviour/MoveItemCommand.h>
#include <gui/behaviour/ResizeItemCommand.h>
#include <gui/behaviour/SetColorItemCommand.h>

#include <QDebug>

namespace Gui {

/*!
 * Constructor
 */
SceneWidget::SceneWidget(QWidget *iParent, Qt::WindowFlags iFlags)
    : QWidget(iParent, iFlags),
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

    mControlWidget = new ControlWidget(this/*GraphicsView::MINIMUM_FACTOR, GraphicsView::MAXIMUM_FACTOR*/);
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
    mainLayout->setContentsMargins(0, 0, 0, 0);

    QGridLayout *controlLayout = new QGridLayout();
    controlLayout->addWidget(mControlWidget, 0, 0, Qt::AlignTop | Qt::AlignRight);
    controlLayout->setColumnMinimumWidth(1, 20);
    controlLayout->setContentsMargins(0, 0, 0, 0);
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
 * \param[in] iMenu - Schema context menu
 */
void
SceneWidget::setSchemeMenu(QMenu *iMenu)
{
    mSchemeMenu = iMenu;
    mScene->setSchemeMenu(iMenu);
}

/*!
 * \brief Set the table menu
 *
 * \param[in] iMenu - Table context menu
 */
void
SceneWidget::setTableMenu(QMenu *iMenu)
{
    mTableMenu = iMenu;
    mScene->setTableMenu(iMenu);
}

/*!
 * \brief Add item from tree event to the scene
 *
 * \param[in] iTreeItem - Tree item we should show on the scene
 * \param[in] iCol - Tree item column identifier
 */
void
SceneWidget::showOnScene(QTreeWidgetItem *iTreeItem, int iCol, bool iCenterOn)
{
    QList<QGraphicsItem *> itemList = mScene->showOnScene(iTreeItem, iCol, QPoint(0, 0), iCenterOn);
    foreach (QGraphicsItem *item, itemList) {
        using namespace Gui::GraphicsItems;

        // add new item only if it isn't exist on the scene
        if (toDbObject(item) && !mScene->findItem(toDbObject(item)->schemaName(), toDbObject(item)->name())) {
            emit itemActionDone(new AddItemCommand(mScene, itemList));
            break;
        } else {
            mView->centerOn(item);
        }
    }
}

/*!
 * \brief Show/hide legend
 *
 * \param[in] iFlag - True if we want to show legend, false otherwise
 */
void
SceneWidget::showLegend(bool iFlag)
{
    mScene->showLegend(iFlag);
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

    using namespace Gui::GraphicsItems;

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
 * \param[in] iFlag - True if we want to show grid, false otherwise
 */
void
SceneWidget::showGrid(bool iFlag)
{
    mScene->showGrid(iFlag);
}

/*!
 * \brief (Un)Align items to the grid
 *
 * \param[in] iFlag - True if we want to align items according grid, false otherwise
 */
void
SceneWidget::alignToGrid(bool iFlag)
{
    mScene->alignToGrid(iFlag);
}

/*!
 * \brief Show/hide grid
 *
 * \param[in] iFlag - True if we want to show pages, false otherwise
 */
void
SceneWidget::divideIntoPages(bool iFlag)
{
    mScene->divideIntoPages(iFlag);
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
 * \note Just delegate flush request to scene.
 */
void
SceneWidget::flushCache()
{
    mScene->flushCache();
}

/*!
 * \brief Create item group from the given items
 *
 * \param[in] iItems - List of items we will add to new group
 *
 * \return New item group
 */
Gui::GraphicsItems::ItemGroup *
SceneWidget::createItemGroup(const QList<QGraphicsItem *> &iItems)
{
    return mScene->createItemGroup(iItems);
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
 * \param[in] iFlag - True if we want to show control widget, false otherwise
 */
void
SceneWidget::showControlWidget(bool iFlag)
{
    mControlWidget->setVisible(iFlag);
}

/*!
 * \brief Get xml structure of scene
 *
 * \param[in] iDoc - Xml dom document
 * \param[in] iShowGrid - Is grid shown or not
 * \param[in] iDivideIntoPages - Is scene divided into pages or not
 * \param[in] iShowLegend - Is legend shown or not
 * \param[in] iShowControlWidget - Is control widget shown or not
 *
 * \return Filled with scene info xml dom element
 */
QDomElement
SceneWidget::toXml(QDomDocument &iDoc, bool iShowGrid, bool iDivideIntoPages, bool iShowLegend, bool iShowControlWidget)
{
    return mScene->toXml(iDoc, iShowGrid, iDivideIntoPages, iShowLegend, iShowControlWidget);
}

/*!
 * \brief Load scene from the xml file
 *
 * \param[in] iElement - Xml dom element
 */
void
SceneWidget::fromXml(QDomElement &iElement)
{
    QDomNode child = iElement.firstChild();
    bool grid = iElement.attribute("grid").toInt();
    mScene->showGrid(grid);
    bool divideIntoPages = iElement.attribute("divideIntoPages").toInt();
    mScene->divideIntoPages(divideIntoPages);
    bool legend = iElement.attribute("legend").toInt();
    mScene->showLegend(legend);
    bool controlWidget = iElement.attribute("controlWidget").toInt();
    showControlWidget(controlWidget);

    QList<QGraphicsItem *> itemLIst = mScene->fromXml(child);

    emit itemActionDone(new AddItemCommand(mScene, itemLIst));
}

/*!
 * \brief Print the graphic widget
 *
 * \param[in] iPrinter - Printer
 */
void
SceneWidget::print(QPrinter *iPrinter)
{
    QPainter painter(iPrinter);
    painter.setFont(QFont("Arial", 10));

    int pageWidth = iPrinter->pageRect().width();
    int pageHeight = iPrinter->pageRect().height();
    int maxI = (int)(mScene->width() / pageWidth + 1);
    int maxJ = (int)(mScene->height() / pageHeight + 1);

    mScene->clearSelection();

    for (int i = 0; i < maxI; ++i) {
        for (int j = 0; j < maxJ; ++j) {
            mScene->render(&painter, QRectF(), QRectF(i * pageWidth, j * pageHeight, pageWidth, pageHeight));
            if (i != maxI - 1 || j != maxJ - 1) {
                iPrinter->newPage();
            }
        }
    }
}

/*!
 * \brief Send 'item moved' signal
 *
 * \param[in] iItemList - Item list were moved
 * \param[in] iDiffX - Gorizontal shift
 * \param[in] iDiffY - Vertical shift
 */
void
SceneWidget::sendItemMoved(QList <QGraphicsItem *> iItemList, int iDiffX, int iDiffY)
{
    emit itemActionDone(new MoveItemCommand(iItemList, iDiffX, iDiffY));
}

void
SceneWidget::sendItemColorChanged(Gui::GraphicsItems::GraphicsItem *iItem, const QColor &iNewColor, const QColor &iOldColor)
{
    emit itemActionDone(new SetColorItemCommand(iItem, iNewColor, iOldColor));
}

void
SceneWidget::sendItemResized(Gui::GraphicsItems::GraphicsItem *iItem, const QRectF &iNewRect, const QRectF &iOldRect)
{
    emit itemActionDone(new ResizeItemCommand(iItem, iNewRect, iOldRect));
}

/*!
 * \brief Send 'action was performed' signal
 *
 * \param[in] iCommand - Command which describes action
 */
void
SceneWidget::sendItemActionDone(QUndoCommand *iCommand)
{
    emit itemActionDone(iCommand);
}

}

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

#include <QApplication>
#include <QContextMenuEvent>
#include <QDebug>
#include <QMenu>
#include <QMessageBox>
#include <QScopedPointer>
#include <gui/TreeWidget.h>
#include <gui/viewcontroller/TreeViewController.h>
#include <gui/viewcontroller/TreeViewControllerFactory.h>

namespace Gui {

/*!
 * Constructor
 */
TreeWidget::TreeWidget(QWidget *iParent)
    : QTreeWidget(iParent),
      Gui::ContextMenuHolder(this),
      mMode(UnknownMode),
      mAllItemExpanded(false),
      mIndicesNode(0),
      mLanguagesNode(0),
      mRolesNode(0),
      mSchemasNode(0)
{
    setColumnCount(TreeWidget::ColumnsCount);
    setColumnHidden(TreeWidget::TypeCol, true);
    setColumnHidden(TreeWidget::SchemaCol, true);
    setHeaderLabel(QString(""));
    setHeaderHidden(true);
    setAnimated(true);
    setFont(QFont("Arial", 8));
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setContextMenuPolicy(Qt::DefaultContextMenu);
}

/*!
 * Desctuctor
 */
TreeWidget::~TreeWidget()
{
}

/*!
 *
 */
bool
TreeWidget::allItemsExpanded() const
{
    return mAllItemExpanded;
}

/*!
 *
 */
void
TreeWidget::display(const QList<Gui::TreeWidgetItem*> &iItems)
{
    using namespace Gui::ViewController;
    // \fixme mMode is set here for now
    mMode = DefaultMode;
    QScopedPointer<TreeViewController> viewController(TreeViewControllerFactory::create(mMode));
    if (!viewController.isNull()) {
        viewController->buildTree(this, iItems);
    }
}

/*!
 * \brief Handler for context menu event
 *
 * \param[in] iEvent - Context menu event
 */
void
TreeWidget::contextMenuEvent(QContextMenuEvent *iEvent)
{
    emit contextMenuRequest(iEvent);
}

/*!
 * \brief This function is called when we start draggin the tree element
 */
void
TreeWidget::startDrag(Qt::DropActions)
{
    QTreeWidgetItem *item = currentItem();

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);

    dataStream << item->data(TreeWidget::NameCol, Qt::DisplayRole).toString();
    qDebug() << "startDrag: " << item->data(TreeWidget::NameCol, Qt::DisplayRole).toString();

    QMimeData *mimeData = new QMimeData;
    mimeData->setData("table/x-table", itemData);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);

    drag->exec(Qt::MoveAction);
}

/*!
 * \brief Show the item on the scene when we double click on it
 *
 * \param[in] iItem - Item we clicked on
 * \param[in] iColumn - Column of the item
 */
//void
//TreeWidget::itemDoubleClicked(Gui::TreeWidgetItem * /*iItem*/, int /*iColumn*/)
//{
//    emit itemDoubleClicked();
//}

/*!
 * Finds the treewidget item with the specified text and whose parent is the one that we
 * said.
 *
 * \param[in] iParent - Parent of the item.
 * \param[in] iValue - Value of the item text.
 * \param[in] iColumn - ID of the column that should be considered during the search.
 *
 * \return Desired tree widget item if found.
 * \return 0 - Otherwise.
 */
Gui::TreeWidgetItem*
TreeWidget::findItem(Gui::TreeWidgetItem *iParent, const QString &iValue, int iColumn) const
{
    QList<QTreeWidgetItem*> items = findItems(iValue, Qt::MatchExactly | Qt::MatchRecursive, iColumn);
    QList<QTreeWidgetItem*>::iterator iter = items.begin();
    QTreeWidgetItem *item = 0;

    while (!item && iter != items.end()) {
        if ((*iter)->parent() == iParent) {
            item = (*iter);
        }
        ++iter;
    }

    return dynamic_cast<Gui::TreeWidgetItem *>(item);
}

/*!
 * Converts tree widget item id into a corresponding node id.
 *
 * \fixme This function is a complete magic. I think I should fix it... later.
 *
 * \param[in] iType - Tree widget item id.
 *
 * \return Tree widget node id.
 */
TreeWidget::Node
TreeWidget::nodeForItem(TreeWidget::Item iType) const
{
    return static_cast<TreeWidget::Node>(TreeWidget::UnkNode + iType);
}

/*!
 *
 * \param[in] iFlag - Indicates whether all item should be expanded (true) or collapse
 * (false).
 */
void
TreeWidget::setExpandAll(bool iFlag)
{
    if (iFlag) {
        expandAll();
    } else {
        collapseAll();
    }

    mAllItemExpanded = iFlag;
}

}


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

#include <control/ContextMenuManager.h>
#include <control/Director.h>
#include <gui/TreeWidget.h>
#include <gui/SceneWidget.h>
#include <gui/TriAction.h>
#include <QContextMenuEvent>
#include <QMenu>

#include <QtDebug>

namespace Control
{

/*!
 * Constructor
 */
ContextMenuManager::ContextMenuManager(const Control::Director *iDirector)
{
    try {
        createMenus(iDirector);

        // \todo set context menu for tree widget items
        // \todo set context menu for graphics items
        // \todo determine how to set context menu for instance for table item and view
        // node???
    } catch (...) {
    }
}

/*!
 * Destructor
 */
ContextMenuManager::~ContextMenuManager()
{
    qDeleteAll(mMenus);
}

/*!
 * \todo Implement
 *
 * \param[in] iDirector - Instance of Director.
 */
void
ContextMenuManager::createActions(const Control::Director *iDirector)
{
    mExpandAllAction = new QAction(tr("Expand/Collapse all"), this);
    mExpandAllAction->setCheckable(true);
    connect(mExpandAllAction, SIGNAL(toggled(bool)),
            this, SLOT(expandAllActionToggled(bool)));
    connect(this, SIGNAL(expandAllTreeItems(Gui::TreeWidget*, bool)),
            iDirector, SLOT(expandAllTreeItems(Gui::TreeWidget*, bool)));

    mAddAction = new QAction(tr("Add"), this);
    connect(mAddAction, SIGNAL(triggered()), iDirector, SLOT(addTreeItemsToScene()));

    mDescribeAction = new QAction(tr("Describe"), this);
    connect(mDescribeAction, SIGNAL(triggered()), iDirector, SLOT(describeItems()));

    mQueryAction = new QAction(tr("Query"), this);
    connect(mQueryAction, SIGNAL(triggered()), iDirector, SLOT(queryItems()));
}

/*!
 * Creates context menus for all widgets and establishes all required signal/slot
 * connections.
 *
 * \param[in] iDirector - Instance of Director.
 */
void
ContextMenuManager::createMenus(const Control::Director *iDirector)
{
    createActions(iDirector);

    QMenu *treeWidgetMenu = createTreeWidgetMenu(iDirector);
    if (treeWidgetMenu) {
        mMenus.insert(Tree, treeWidgetMenu);
    }


    QMenu *treeTableItemMenu = createTreeTableItemMenu(iDirector);
    if (treeTableItemMenu) {
        mMenus.insert(TreeItem | TableObject, treeTableItemMenu);
    }


    QMenu *treeViewItemMenu = createTreeViewItemMenu(iDirector);
    if (treeViewItemMenu) {
        mMenus.insert(TreeItem | ViewObject, treeViewItemMenu);
    }


    QMenu *sceneWidgetMenu = createSceneWidgetMenu(iDirector);
    if (sceneWidgetMenu) {
        mMenus.insert(Scene, sceneWidgetMenu);
    }
}

/*!
 * Creates a context menu applicable to tree widget.
 *
 * \param[in] iDirector - Instance of director. Used for connections.
 *
 * \return Construsted menu or 0 if an error occured.
 */
QMenu*
ContextMenuManager::createTreeWidgetMenu(const Control::Director *iDirector)
{
    QMenu *menu = new(std::nothrow) QMenu();

    if (menu) {
        menu->addAction(mExpandAllAction);
        // \todo Add more actions
    }

    return menu;
}

/*!
 * Construct a context menu for table items held by tree widget.
 *
 * \param[in] iDirector - Instance of director. Used for connections.
 *
 * \return Constructed menu or 0 in case of failure.
 */
QMenu*
ContextMenuManager::createTreeTableItemMenu(const Control::Director *iDirector)
{
    QMenu *menu = new(std::nothrow) QMenu();
    if (menu) {
        menu->addAction(mAddAction);
        menu->addAction(mDescribeAction);
        menu->addAction(mQueryAction);
    }

    return menu;
}

/*!
 * Construct a context menu for view items held by tree widget.
 *
 * \param[in] iDirector - Instance of director. Used for connections.
 *
 * \return Constructed menu or 0 in case of failure.
 */
QMenu*
ContextMenuManager::createTreeViewItemMenu(const Control::Director *iDirector)
{
    // \note Since views behave almost just like tables, we can use
    // createTreeTableItemMenu() function to create a context menu for view items and we
    // won't have code duplication.
    return createTreeTableItemMenu(iDirector);
}

/*!
 * \param[in] iDirector - Instance of director. Used for connections.
 *
 * \return Constructed context menu for a scene widget or 0 in case of failure.
 */
QMenu*
ContextMenuManager::createSceneWidgetMenu(const Control::Director *iDirector)
{
    QMenu *menu = new(std::nothrow) QMenu();

    if (menu) {
        // \todo populate with actions
    }

    return menu;
}

/*!
 * Slot. Handles context menu request emitted by a child of ContextMenuHolder.
 *
 * \param[in] iEvent - Context menu event.
 */
void
ContextMenuManager::contextMenuRequested(QContextMenuEvent *iEvent)
{
    using namespace Gui;

    const TreeWidget *tree = 0;
    const SceneWidget *scene = 0;

    const QMenu *menu = 0;

    if ((tree = dynamic_cast<TreeWidget*>(sender()))) {
        // detect selected items
        QList<QTreeWidgetItem *> items = tree->selectedItems();
        // if a single item is selected, then get its type and show its menu
        if (0 == items.size()) {
            menu = tree->contextMenu();
            // \fixme expandAllAction is obtained by its index assuming that we added it first
            menu->actions().at(0)->setChecked(tree->allItemsExpanded());
        // in case several items are selected, then we need to get types of these items.
        } else if (1 == items.size()) {
            menu = dynamic_cast<Gui::TreeWidgetItem*>(items.first())->contextMenu();
        } else {
            QSet<quint32> itemTypes;
            foreach (QTreeWidgetItem *item, items) {
                quint32 type = item->text(TreeWidget::TypeCol).toUInt();

                if (!itemTypes.contains(type)) {
                    itemTypes.insert(type);
                }
            }

            // \todo find intersection of menus
            // otherwise, build an intersection menu out of these types' menus
        }
    } else if ((scene = dynamic_cast<SceneWidget*>(sender()))) {
    } else {
        qDebug() << "This widget is not supported by ContextMenuManager!";
    }

    qDebug() << "ContextMenuManager::contextMenuRequested> Menu = " << menu;
    if (menu) {
//        menu->exec(iEvent->globalPos());
        const_cast<QMenu*>(menu)->exec(QCursor::pos());
    }
}

/*!
 * \param[in] iType - Type of context menu to find
 *
 * \return Context menu of the given type
 */
const QMenu*
ContextMenuManager::menu(int iType) const
{
    return mMenus.value(iType);
}

/*!
 * Slot. Handles expanding/collapsing of tree items. Executed when expandAll action is
 * toggled.
 */
void
ContextMenuManager::expandAllActionToggled(bool iFlag)
{
    QAction *action = dynamic_cast<QAction*>(sender());
    QMenu *menu = dynamic_cast<QMenu*>(action->parentWidget());
    QPoint point = menu->pos();
    Gui::TreeWidget *tree = dynamic_cast<Gui::TreeWidget*>(qApp->widgetAt(point)->parent());

    emit expandAllTreeItems(tree, iFlag);
}

} // namespace


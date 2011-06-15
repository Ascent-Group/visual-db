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
ContextMenuManager::ContextMenuManager()
{
    try {
        createMenus();

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
 *
 */
void
ContextMenuManager::createMenus()
{
    // Context menu for tree widget
    QMenu *treeWidgetMenu = new QMenu();
//    treeWidgetMenu->addAction(dynamic_cast<QAction*>(new Gui::TriAction("ttt", this)));
    mMenus.insert(MENU_TREE_WIDGET, treeWidgetMenu);

    // context menu for table item in tree widget
    QMenu *tableTreeItemMenu = new QMenu();
//    tableTreeItemMenu->addAction(ui.mAddTableAction);
//    tableTreeItemMenu->addAction(ui.mDescribeObjectAction);
//    tableTreeItemMenu->addAction(ui.mQueryDataAction);
    mMenus.insert(MENU_TREE_TABLE_ITEM, tableTreeItemMenu);

    // context menu for scene
    QMenu *sceneWidgetMenu = new QMenu();
    mMenus.insert(MENU_SCENE_WIDGET, sceneWidgetMenu);
}

/*!
 * \todo Comment
 */
void
ContextMenuManager::contextMenuRequested(QContextMenuEvent *iEvent)
{
    using namespace Gui;

    TreeWidget *tree = 0;
    SceneWidget *scene = 0;

    const QMenu *menu = 0;

    if ((tree = dynamic_cast<TreeWidget*>(sender()))) {
        // detect selected items
        QList<QTreeWidgetItem *> items = tree->selectedItems();
        qDebug() << items.size();
        // if a single item is selected, then get its type and show its menu
        if (0 == items.size()) {
            menu = tree->contextMenu();
//            int itemType = items.first()->text(TreeWidget::IdCol).toUInt();
//            menu = mMenus.value(treeItemType2MenuType(itemType));
        // in case several items are selected, then we need to get types of these items.
        } else {
            QSet<quint32> itemTypes;
            foreach (QTreeWidgetItem *item, items) {
                itemTypes.insert(item->text(TreeWidget::IdCol).toUInt());
            }
            // if only one type, then get its menu and show it
            // \todo find intersection of menus
            // otherwise, build an intersection menu out of these types' menus
        }
    } else if ((scene = dynamic_cast<SceneWidget*>(sender()))) {
    } else {
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
ContextMenuManager::menu(Control::ContextMenuManager::MenuType iType) const
{
    return mMenus.value(iType);
}

/*!
 *
 */
ContextMenuManager::MenuType
ContextMenuManager::treeItemType2MenuType(quint32 iType) const
{
    using namespace Gui;

    ContextMenuManager::MenuType menuType = MENU_UNKNOWN;

    switch (iType) {
        case TreeWidget::SchemaItem:
            break;
        case TreeWidget::TableItem:
            menuType = ContextMenuManager::MENU_TREE_TABLE_ITEM;
            break;
        case TreeWidget::ViewItem:
            break;
        case TreeWidget::RoleItem:
            break;
        case TreeWidget::TriggerItem:
            break;
        case TreeWidget::LanguageItem:
            break;
        case TreeWidget::IndexItem:
            break;
        case TreeWidget::ProcedureItem:
            break;
        case TreeWidget::SchemaNode:
            break;
        case TreeWidget::TableNode:
            break;
        case TreeWidget::ViewNode:
            break;
        case TreeWidget::RoleNode:
            break;
        case TreeWidget::TriggerNode:
            break;
        case TreeWidget::LanguageNode:
            break;
        case TreeWidget::IndexNode:
            break;
        case TreeWidget::ProcedureNode:
            break;
        case TreeWidget::UnkItem:
        case TreeWidget::UnkNode:
        default:
            break;

    }

    return menuType;

}

}


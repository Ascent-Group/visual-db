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

#ifndef GUI_TREEWIDGET_H
#define GUI_TREEWIDGET_H

#include <QTreeWidget>
#include <common.h>
#include <common/DbObject.h>
#include <control/ContextMenuManager.h>

class QContextMenuEvent;
class QMenu;

using namespace DbObjects::Common;

namespace Gui {

/*!
 * \class TreeWidget
 * \headerfile gui/TreeWidget.h
 * \brief Tree widget. Show all database objects on this tree.
 */
class TreeWidget : public QTreeWidget
{
    Q_OBJECT

    public:
        TreeWidget(QWidget *iParent = 0);
        ~TreeWidget();

        // it is highly recommended that we keep the same order as
        // in DbObject::Type enum (though we don't have to)
        enum Item {
            UnkItem = DbObject::UnkObject,
            SchemaItem = DbObject::SchemaObject,
            TableItem = DbObject::TableObject,
            ViewItem = DbObject::ViewObject,
            RoleItem = DbObject::RoleObject,
            TriggerItem = DbObject::TriggerObject,
            LanguageItem = DbObject::LanguageObject,
            IndexItem = DbObject::IndexObject,
            ProcedureItem = DbObject::ProcedureObject,
        };

        // Node's values MUST NOT intersect with Item's values
        enum Node {
            UnkNode = 100 + UnkItem,
            SchemaNode,
            TableNode,
            ViewNode,
            RoleNode,
            TriggerNode,
            LanguageNode,
            IndexNode,
            ProcedureNode
        };

        enum Columns {
            NameCol = 0,
            IdCol,

            ColumnsCount
        };

        void displayObjects(const Objects &iList);
//    signals:
//        void itemDoubleClicked();

    protected:
        void contextMenuEvent(QContextMenuEvent *);
        void startDrag(Qt::DropActions);
        void itemDoubleClicked(QTreeWidgetItem *, int);

    private:
        QTreeWidgetItem *mIndicesNode;
        QTreeWidgetItem *mLanguagesNode;
        QTreeWidgetItem *mRolesNode;
        QTreeWidgetItem *mSchemasNode;

    private:
        QTreeWidgetItem* findItem(QTreeWidgetItem *iParent, const QString &iValue, int iColumn) const;
        TreeWidget::Node nodeForItem(TreeWidget::Item type) const;
        QTreeWidgetItem* insertItem(QTreeWidgetItem *, const QString &, TreeWidget::Item, bool iDragEnabled = false);

    signals:
        // \todo This probable should be placed in an abstract type and should e inherited
        // just like DataHolder
        void contextMenuRequest(QContextMenuEvent *);
};

}

#endif // GUI_TREEWIDGET_H


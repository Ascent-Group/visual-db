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

#include <gui/TreeWidget.h>
#include <gui/viewcontroller/DefaultController.h>

#include <QtDebug>

namespace Gui {

/*!
 * Constructor
 */
DefaultController::DefaultController()
    : TreeViewController()
{

}

/*!
 * Destructor
 */
DefaultController::~DefaultController()
{
}

/*!
 * \todo comment
 * \see TreeViewController::buildTree
 */
void
DefaultController::buildTree(Gui::TreeWidget *iTree, const QList<Gui::TreeWidgetItem*> &iItems)
{
    using namespace Gui;

    // construct the tree skeleton
    TreeWidgetItem *indicesNode = createNode(0, QObject::tr("Indices"), TreeWidget::IndexNode);
    iTree->addTopLevelItem(indicesNode);

    TreeWidgetItem *languagesNode = createNode(0, QObject::tr("Languages"), TreeWidget::LanguageNode);
    iTree->addTopLevelItem(languagesNode);

    TreeWidgetItem *rolesNode = createNode(0, QObject::tr("Roles"), TreeWidget::RoleNode);
    iTree->addTopLevelItem(rolesNode);

    TreeWidgetItem *schemasNode = createNode(0, QObject::tr("Schemas"), TreeWidget::SchemaNode);
    iTree->addTopLevelItem(schemasNode);

    Gui::TreeWidgetItem *parentNode = 0;
    foreach(Gui::TreeWidgetItem *item, iItems) {
        QString name = item->text(TreeWidget::NameCol);
        QString parentName = item->text(TreeWidget::SchemaCol);
        TreeWidget::Item type = static_cast<TreeWidget::Item>(item->text(TreeWidget::TypeCol).toUInt());

        parentNode = 0;

        switch (type) {
            case  TreeWidget::IndexItem:
                parentNode = indicesNode;
                break;

            case  TreeWidget::LanguageItem:
                parentNode = languagesNode;
                break;

            case  TreeWidget::RoleItem:
                parentNode = rolesNode;
                break;

            case  TreeWidget::SchemaItem:
//                QApplication::processEvents();
                // if schema item already exists, then just skip it.
                // this may occur when schema's child comes earlier than the schema
                // itself.
                if (iTree->findItem(schemasNode, name, TreeWidget::NameCol)) {
                    continue;
                } else {
                    parentNode = schemasNode;
                }
                break;

            case  TreeWidget::ViewItem:
            case  TreeWidget::ProcedureItem:
            case  TreeWidget::TableItem:
            case  TreeWidget::TriggerItem:
            {
                // find parent schema item
                Gui::TreeWidgetItem *schemaItem = iTree->findItem(schemasNode, parentName, TreeWidget::NameCol);
                // or create it with subnodes
                if (!schemaItem) {
                    schemaItem = insertItem(schemasNode, parentName, TreeWidget::SchemaItem);

                    createNode(schemaItem, QObject::tr("Procedures"), TreeWidget::ProcedureNode);
                    createNode(schemaItem, QObject::tr("Tables"), TreeWidget::TableNode);
                    createNode(schemaItem, QObject::tr("Triggers"), TreeWidget::TriggerNode);
                    createNode(schemaItem, QObject::tr("Views"), TreeWidget::ViewNode);
                }

                // find nested node for the given parentNode type
                parentNode = iTree->findItem(schemaItem, QString::number((int)iTree->nodeForItem(type)), TreeWidget::TypeCol);
            }
                break;

            case  TreeWidget::UnkItem:
            default:
                qDebug() << "TreeWidget::displayObjects> Unknown object type: " << type;
                continue;
                break;
        }

        Q_ASSERT_X(0 != parentNode, "DefaultController::buildTree", QString("name = %1, type = %2").arg(name).arg(type).toAscii().data());
        if (parentNode) {
            insertItem(parentNode, name, type, true);
        }

    }

    // \todo sort the tree
//    QList<QTreeWidgetItem *> nodes = iTree->findItems("", Qt::MatchContains | Qt::MatchRecursive, TreeWidget::NameCol);
//    foreach (QTreeWidgetItem *node, nodes) {
//        if (node->text(TreeWidget::TypeCol).toUInt() > TreeWidget::UnkNode) {
//            node->sortChildren(TreeWidget::NameCol, Qt::AscendingOrder);
//        }
//    }

}


} // namespace Gui


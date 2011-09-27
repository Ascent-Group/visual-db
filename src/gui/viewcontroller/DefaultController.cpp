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

namespace Gui
{
namespace ViewController
{

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
 * Builds a tree with default item arrangement.
 *
 * \param[in] iTree - Tree that will display item hierarchy.
 * \param[in] iItems - List of items to build the hierarchy from.
 *
 * \see TreeViewController::buildTree
 */
void
DefaultController::buildTree(Gui::TreeWidget *iTree, const QList<Gui::TreeWidgetItem*> &iItems)
{
    using namespace Gui;

    // construct the tree skeleton
    TreeWidgetItem *indicesNode = createNode(0, QObject::tr("Indices"), TreeNode | IndexObject);
    iTree->addTopLevelItem(indicesNode);

    TreeWidgetItem *languagesNode = createNode(0, QObject::tr("Languages"), TreeNode | LanguageObject);
    iTree->addTopLevelItem(languagesNode);

    TreeWidgetItem *rolesNode = createNode(0, QObject::tr("Roles"), TreeNode | RoleObject);
    iTree->addTopLevelItem(rolesNode);

    TreeWidgetItem *schemasNode = createNode(0, QObject::tr("Schemas"), TreeNode | SchemaObject);
    iTree->addTopLevelItem(schemasNode);

    Gui::TreeWidgetItem *parentNode = 0;
    foreach(Gui::TreeWidgetItem *item, iItems) {
        QString name = item->text(TreeWidget::NameCol);
        QString parentName = item->text(TreeWidget::SchemaCol);
        int type = item->text(TreeWidget::TypeCol).toUInt();

        parentNode = 0;

        switch (type) {
            case IndexObject:
                parentNode = indicesNode;
                break;

            case LanguageObject:
                parentNode = languagesNode;
                break;

            case RoleObject:
                parentNode = rolesNode;
                break;

            case SchemaObject:
                {
//                QApplication::processEvents();
                // if schema item already exists, then just skip it.
                // this may occur when schema's child comes earlier than the schema
                // itself.

                QTreeWidgetItem *old = iTree->findItem(schemasNode, name, TreeWidget::NameCol);

                if (old) {
                    item->addChildren(old->takeChildren());
                    delete old;
                }
                parentNode = schemasNode;
                }
                break;

            case ViewObject:
            case ProcedureObject:
            case TableObject:
            case TriggerObject:
            {
                // find parent schema item
                Gui::TreeWidgetItem *schemaItem = iTree->findItem(schemasNode, parentName, TreeWidget::NameCol);
                // or create it with subnodes, but later we will replace it with an
                // original schema item
                if (!schemaItem) {
                    Gui::TreeWidgetItem *tempParent = new Gui::TreeWidgetItem();
                    tempParent->setText(TreeWidget::NameCol, parentName);
                    tempParent->setText(TreeWidget::TypeCol, QString::number(TreeNode | SchemaObject));

                    schemaItem = insertItem(schemasNode, tempParent);

                    createNode(schemaItem, QObject::tr("Procedures"), TreeNode | ProcedureObject);
                    createNode(schemaItem, QObject::tr("Tables"), TreeNode | TableObject);
                    createNode(schemaItem, QObject::tr("Triggers"), TreeNode | TriggerObject);
                    createNode(schemaItem, QObject::tr("Views"), TreeNode | ViewObject);
                }

                // find nested node for the given parentNode type
                parentNode = iTree->findItem(schemaItem, QString::number(TreeNode | type), TreeWidget::TypeCol);
            }
                break;

            case UnkObject:
            default:
                qDebug() << "TreeWidget::displayObjects> Unknown object type: " << type;
                continue;
                break;
        }

        Q_ASSERT_X(0 != parentNode, "DefaultController::buildTree", QString("name = %1, type = %2").arg(name).arg(type).toAscii().data());
        if (parentNode) {
            insertItem(parentNode, item, true);
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


} // namespace ViewController
} // namespace Gui


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
#include <gui/TreeWidget.h>

namespace Gui {

static void setBold(QTreeWidgetItem *, bool);
static QTreeWidgetItem* createNode(QTreeWidgetItem *, const QString &, TreeWidget::Node);

/*!
 * Constructor
 */
TreeWidget::TreeWidget(QWidget *iParent)
    : QTreeWidget(iParent),
      mIndicesNode(0),
      mLanguagesNode(0),
      mRolesNode(0),
      mSchemasNode(0)
{
    setColumnCount(TreeWidget::ColumnsCount);
    setColumnHidden(TreeWidget::IdCol, true);
    setHeaderLabel(QString(""));
    setHeaderHidden(true);
    setAnimated(true);
    setFont(QFont("Arial", 8));
}

/*!
 * Desctuctor
 */
TreeWidget::~TreeWidget()
{
}

/*!
 * Displays specified objects.
 *
 * \param[in] iList - A collection of objects which includes name, schema name and object
 * type.
 *
 * \note Maybe we should use TreeView and a standard model here
 */
void
TreeWidget::displayObjects(const Objects &iList)
{
    clear();

    // construct the tree skeleton
    mIndicesNode = createNode(0, tr("Indices"), TreeWidget::IndexNode);
    addTopLevelItem(mIndicesNode);

    mLanguagesNode = createNode(0, tr("Languages"), TreeWidget::LanguageNode);
    addTopLevelItem(mLanguagesNode);

    mRolesNode = createNode(0, tr("Roles"), TreeWidget::RoleNode);
    addTopLevelItem(mRolesNode);

    mSchemasNode = createNode(0, tr("Schemas"), TreeWidget::SchemaNode);
    addTopLevelItem(mSchemasNode);

    // preparing is done, get to displaying
    using namespace DbObjects::Common;

    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled;
    QTreeWidgetItem *parentNode = 0;

    foreach(const QString &name, iList.keys()) {
        QString parentName = iList.value(name).first;
        TreeWidget::Item type = static_cast<TreeWidget::Item>(iList.value(name).second);

        parentNode = 0;

        switch (type) {
            case IndexItem:
                parentNode = mIndicesNode;
                break;

            case LanguageItem:
                parentNode = mLanguagesNode;
                break;

            case RoleItem:
                parentNode = mRolesNode;
                break;

            case SchemaItem:
//                QApplication::processEvents();
                // if schema item already exists, then just skip it.
                // this may occur when schema's child comes earlier than the schema
                // itself.
                if (findItem(mSchemasNode, name, TreeWidget::NameCol)) {
                    continue;
                } else {
                    parentNode = mSchemasNode;
                }
                break;

            case ViewItem:
            case ProcedureItem:
            case TableItem:
            case TriggerItem:
            {
                // find parent schema item
                QTreeWidgetItem *schemaItem = findItem(mSchemasNode, parentName, TreeWidget::NameCol);
                // or create it with subnodes
                if (!schemaItem) {
                    schemaItem = insertItem(mSchemasNode, parentName, TreeWidget::SchemaItem);
                }

                // find nested node for the given parentNode type
                parentNode = findItem(schemaItem, QString::number((int)nodeForItem(type)), TreeWidget::IdCol);
            }
                break;

            case UnkItem:
            default:
                qDebug() << "TreeWidget::displayObjects> Unknown object type: " << type;
                continue;
                break;
        }

        Q_ASSERT_X(0 != parentNode, "displayObjects", QString("name = %1, type = %2").arg(name).arg(type).toAscii().data());
        if (parentNode) {
            insertItem(parentNode, name, type, true);
        }

    }

    // todo sort the tree
    QList<QTreeWidgetItem *> nodes = findItems("", Qt::MatchContains | Qt::MatchRecursive, TreeWidget::NameCol);
    foreach (QTreeWidgetItem *node, nodes) {
        if (node->text(TreeWidget::IdCol).toUInt() > TreeWidget::UnkNode) {
            node->sortChildren(TreeWidget::NameCol, Qt::AscendingOrder);
        }
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
    using namespace Control;
    // find out what kind of context menu we want to show
    ContextMenuManager::MenuType type = ContextMenuManager::MENU_UNKNOWN;
    emit contextMenuRequest(iEvent, type);
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
//TreeWidget::itemDoubleClicked(QTreeWidgetItem * /*iItem*/, int /*iColumn*/)
//{
//    emit itemDoubleClicked();
//}

/*!
 * \brief Set font to bold
 *
 * \param[in] iItem - Tree item we need to change the boldness
 * \param[in] iBold - True for bold font or false for normal one
 */
static void
setBold(QTreeWidgetItem *iItem, bool iBold)
{
    QFont font = iItem->font(TreeWidget::NameCol);
    font.setBold(iBold);
    iItem->setFont(TreeWidget::NameCol, font);
}

/*!
 * Creates a node for storing objects of specified type.
 *
 * \param[in] iParent - Parent item who will hold this node as a child.
 * \param[in] iName - Name of the node.
 * \param[in] iType - Type of a node that is going to be created.
 *
 * \return Newly created tree widget node if its creation succeeded.
 * \return 0 - Otherwise.
 */
static QTreeWidgetItem*
createNode(QTreeWidgetItem *iParent, const QString &iName, TreeWidget::Node iType)
{
    QTreeWidgetItem *node = new(std::nothrow) QTreeWidgetItem(iParent);

    if (node) {
        node->setText(TreeWidget::NameCol, iName);
        node->setText(TreeWidget::IdCol, QString::number(iType));
        setBold(node, true);
    }

    return node;
}

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
QTreeWidgetItem*
TreeWidget::findItem(QTreeWidgetItem *iParent, const QString &iValue, int iColumn) const
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

    return item;
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
 * \brief Insert a child item for the given parent
 *
 * \param[in] iParentNode - Parent item we will populate.
 * \param[in] iText - Text to display by the new item.
 * \param[in] iType - Item type.
 * \param[in] iDrabEnabled - Indicates whether these items should be draggable or not.
 *
 * \return Tree widget item that was created
 */
QTreeWidgetItem*
TreeWidget::insertItem(QTreeWidgetItem *iParentNode, const QString &iText, TreeWidget::Item iType, bool iDragEnabled)
{
    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    if (iDragEnabled) {
        flags |= Qt::ItemIsDragEnabled;
    }

    QTreeWidgetItem *item = new(std::nothrow) QTreeWidgetItem(iParentNode);

    if (item) {
        item->setFlags(flags);
        item->setText(TreeWidget::NameCol, iText);
        item->setText(TreeWidget::IdCol, QString::number(iType));
        item->setData(TreeWidget::NameCol, Qt::DisplayRole, iText);

        if (TreeWidget::SchemaItem == iType) {
            createNode(item, tr("Procedures"), TreeWidget::ProcedureNode);
            createNode(item, tr("Tables"), TreeWidget::TableNode);
            createNode(item, tr("Triggers"), TreeWidget::TriggerNode);
            createNode(item, tr("Views"), TreeWidget::ViewNode);
        }
    }

    return item;
}

}


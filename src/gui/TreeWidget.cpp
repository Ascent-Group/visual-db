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
TreeWidget::TreeWidget(/*QMenu *iMenu, */QWidget *iParent)
    : QTreeWidget(iParent),
      mContextMenu(0),
      mIndicesNode(0),
      mLanguagesNode(0),
      mRolesNode(0),
      mSchemasNode(0)
{
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
 * \brief Set context menu
 *
 * \param[in] iMenu - Context menu
 */
void
TreeWidget::setContextMenu(QMenu *iMenu)
{
    mContextMenu = iMenu;
}

/*!
 * \brief Read database and fill the tree
 */
#if 0
void
TreeWidget::refresh()
{
    QStringList::const_iterator schemaIter;
    // for each schema on the list
    for (schemaIter = schemasList.begin(); schemaIter != schemasList.end(); ++schemaIter) {
        // keep app responsive
        QApplication::processEvents();

        QString schemaName = *schemaIter;
        // create an item
        QTreeWidgetItem *schemaItem = new QTreeWidgetItem(schemasNode);
        schemaItem->setText(TreeWidget::NameCol, schemaName);
        schemaItem->setText(TreeWidget::IdCol, QString::number(TreeWidget::SchemaItem));

        // add to the scena subtree
        //addTopLevelItem(schemaItem);

        // create schema
        DbSchemaPtr schema = dbInst->findSchema(schemaName);

        // create tables node
        QTreeWidgetItem *tablesNode = new QTreeWidgetItem(schemaItem);
        tablesNode->setText(TreeWidget::NameCol, tr("Tables"));
        tablesNode->setText(TreeWidget::IdCol, QString::number(TreeWidget::TableNode));
        setBold(tablesNode, true);
        // create views node
        QTreeWidgetItem *viewsNode = new QTreeWidgetItem(schemaItem);
        viewsNode->setText(TreeWidget::NameCol, tr("Views"));
        viewsNode->setText(TreeWidget::IdCol, QString::number(TreeWidget::ViewNode));
        setBold(viewsNode, true);
        // create procedures node
        QTreeWidgetItem *procsNode = new QTreeWidgetItem(schemaItem);
        procsNode->setText(TreeWidget::NameCol, tr("Procedures"));
        procsNode->setText(TreeWidget::IdCol, QString::number(TreeWidget::ProcedureNode));
        setBold(procsNode, true);
        // create triggers node
        QTreeWidgetItem *trigsNode = new QTreeWidgetItem(schemaItem);
        trigsNode->setText(TreeWidget::NameCol, tr("Triggers"));
        trigsNode->setText(TreeWidget::IdCol, QString::number(TreeWidget::TriggerNode));
        setBold(trigsNode, true);

        // get tables list for the given schema
        QStringList tablesList;
        schema->tablesList(tablesList);
        insertItems(tablesNode, &tablesList, TreeWidget::TableItem, true);

        // get views list for the given schema
        QStringList viewsList;
        schema->viewsList(viewsList);
        insertItems(viewsNode, &viewsList, TreeWidget::ViewItem);

        // get procs list for the given schema
        QStringList procsList;
        schema->proceduresList(procsList);
        insertItems(procsNode, &procsList, TreeWidget::ProcedureItem);

        // get triggers list for the given schema
        QStringList trigsList;
        schema->triggersList(trigsList);
        insertItems(trigsNode, &trigsList, TreeWidget::TriggerItem);
    }
    insertItems(indicesNode, &indicesList, TreeWidget::IndexItem);
}
#endif

/*!
 * \todo comment
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
    QTreeWidgetItem *item = 0;

    foreach(const QString &name, iList.keys()) {
        QString parentName = iList.value(name).first;
        TreeWidget::Item type = static_cast<TreeWidget::Item>(iList.value(name).second);
        item = 0;

        switch (type) {
            case IndexItem:
                item = new(std::nothrow) QTreeWidgetItem(mIndicesNode);
                break;

            case LanguageItem:
                item = new(std::nothrow) QTreeWidgetItem(mLanguagesNode);
                break;

            case RoleItem:
                item = new(std::nothrow) QTreeWidgetItem(mRolesNode);
                break;

            case SchemaItem:
            {
                item = new(std::nothrow) QTreeWidgetItem(mSchemasNode);
                // create children
                createNode(item, tr("Procedures"), TreeWidget::ProcedureNode);
                createNode(item, tr("Tables"), TreeWidget::TableNode);
                createNode(item, tr("Triggers"), TreeWidget::TriggerNode);
                createNode(item, tr("Views"), TreeWidget::ViewNode);
            }
                break;

            case ViewItem:
            case ProcedureItem:
            case TableItem:
            case TriggerItem:
            {
                qDebug() << name;
                // find parent schema item
//                QTreeWidgetItem *schemaItem = findItem(mSchemasNode, parentName, TreeWidget::NameCol);
//                Q_ASSERT(0 != schemaItem);
                // find nested node for the given item type
//                item = findItem(schemaItem, QString::number((int)nodeForItem(type)), TreeWidget::IdCol);
                // \todo ??? if there is no such node, then create it
            }
                break;
            case UnkItem:
            default:
                break;
        }

        if (item) {
            item->setFlags(flags);
            item->setText(TreeWidget::NameCol, name);
            item->setText(TreeWidget::IdCol, QString::number(type));
            item->setData(TreeWidget::NameCol, Qt::DisplayRole, name);
        }

    }

    // \todo sort tree here
}

/*!
 * \brief Handler for context menu event
 *
 * \param[in] iEvent - Context menu event
 */
void
TreeWidget::contextMenuEvent(QContextMenuEvent *iEvent)
{
    mContextMenu->exec(iEvent->globalPos());
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
 * \todo comments
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
 * \param[in] iText - Text of the item.
 *
 * \return Desired tree widget item
 */
QTreeWidgetItem*
TreeWidget::findItem(QTreeWidgetItem *iParent, const QString &iValue, int iColumn) const
{
    QList<QTreeWidgetItem*> items = QTreeWidget::findItems(iValue, Qt::MatchExactly, iColumn);

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
 *
 */
TreeWidget::Node
TreeWidget::nodeForItem(TreeWidget::Item type) const
{
    return static_cast<TreeWidget::Node>(TreeWidget::UnkNode + type);
}

/*!
 * \brief Populate tree under parent-item with triggers.
 *
 * \param[in] iParentItem - Parent item we will populate.
 * \param[in] iList - The list of captions we need to fill with the parent item.
 * \param[in] iType - Item type.
 * \param[in] iDrabEnabled - Indicates whether these items should be draggable or not.
 */
//void
//TreeWidget::insertItems(QTreeWidgetItem *iParentItem, QStringList &iList, TreeWidget::Item iType, bool iDragEnabled)
//{
//    qDebug() << "=>" << iParentItem;
//    iList.sort();
//
//    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
//    if (iDragEnabled) {
//        flags |= Qt::ItemIsDragEnabled;
//    }
//
//    QTreeWidgetItem *item;
//    QStringList::const_iterator iter;
//    // for each item in the list
//    for (iter = iList.constBegin(); iter != iList.constEnd(); ++iter) {
//
//        qDebug() << "A";
//        item = new(std::nothrow) QTreeWidgetItem(iParentItem);
//        qDebug() << "B";
//        if (item) {
//            item->setFlags(flags);
//            item->setText(TreeWidget::NameCol, *iter);
//            item->setText(TreeWidget::IdCol, QString::number(iType));
//            item->setData(TreeWidget::NameCol, Qt::DisplayRole, *iter);
//        }
//    }
//}

}

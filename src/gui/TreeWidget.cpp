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
#include <QSqlDatabase>
#include <common/DbSchema.h>
#include <gui/TreeWidget.h>

static QString qDBCaption(const QSqlDatabase &);
static void setBold(QTreeWidgetItem *, bool);

/*!
 * Constructor
 */
TreeWidget::TreeWidget(/*QMenu *ipMenu, */QWidget *ipParent)
    : QTreeWidget(ipParent)//, mContextMenu(ipMenu)
{
    setHeaderLabel(QString(""));
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
 * @brief Set context menu
 *
 * @param[in] ipMenu - Context menu
 */
void
TreeWidget::setContextMenu(QMenu *ipMenu)
{
    mContextMenu = ipMenu;
}

/*!
 * @brief Read database and fill the tree
 */
void
TreeWidget::refresh()
{
    clear();

    QSqlDatabase db = QSqlDatabase::database("mainConnect");

    setHeaderLabel(qDBCaption(db));

    // construct the tree skeleton
    QTreeWidgetItem *rolesNode = new QTreeWidgetItem();
    rolesNode->setText(TreeWidget::NameCol, tr("Roles"));
    rolesNode->setText(TreeWidget::IdCol, QString::number(TreeWidget::RoleNode));
    setBold(rolesNode, true);
    addTopLevelItem(rolesNode);

    QTreeWidgetItem *schemasNode = new QTreeWidgetItem();
    schemasNode->setText(TreeWidget::NameCol, tr("Schemas"));
    schemasNode->setText(TreeWidget::IdCol, QString::number(TreeWidget::SchemaNode));
    setBold(schemasNode, true);
    addTopLevelItem(schemasNode);

    QTreeWidgetItem *indicesNode = new QTreeWidgetItem();
    indicesNode->setText(TreeWidget::NameCol, tr("Indices"));
    indicesNode->setText(TreeWidget::IdCol, QString::number(TreeWidget::IndexNode));
    setBold(indicesNode, true);
    addTopLevelItem(indicesNode);

    QTreeWidgetItem *langsNode = new QTreeWidgetItem();
    langsNode->setText(TreeWidget::NameCol, tr("Languages"));
    langsNode->setText(TreeWidget::IdCol, QString::number(TreeWidget::LanguageNode));
    setBold(langsNode, true);
    addTopLevelItem(langsNode);

    // interact with progressbar here

    // populate object tree
    // (better place rather than inside ctor)
    DbObjects::Common::Database *dbInst = DbObjects::Common::Database::instance();
    dbInst->resetData();
    dbInst->loadData();

    //// ROLES
    // read roles
    QStringList rolesList;
    dbInst->rolesList(rolesList);
    insertItems(rolesNode, &rolesList, TreeWidget::RoleItem);

    //// LANGS
    // get and sort langs list
    QStringList langsList;
    dbInst->languagesList(langsList);
    insertItems(langsNode, &langsList, TreeWidget::LanguageItem);

    //// SCHEMAS
    // get and sort schemas list
    QStringList schemasList;
    dbInst->schemasList(schemasList);

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

    //// INDICES

    // get and sort indices list
    QStringList indicesList;
    dbInst->indicesList(indicesList);

    insertItems(indicesNode, &indicesList, TreeWidget::IndexItem);
}

/*!
 * @brief Handler for context menu event
 *
 * @param[in] ipEvent - Context menu event
 */
void
TreeWidget::contextMenuEvent(QContextMenuEvent *ipEvent)
{
    mContextMenu->exec(ipEvent->globalPos());
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
 * @brief Create the database caption
 *
 * @param[in] ipDb - Database connection
 *
 * @return Database caption
 */
static QString
qDBCaption(const QSqlDatabase &ipDb)
{
    QString driverName = ipDb.driverName();
    driverName.append(QLatin1Char(':'));

    if (!ipDb.userName().isEmpty()) {
        driverName.append(ipDb.userName()).append(QLatin1Char('@'));
    }

    driverName.append(ipDb.databaseName());

    return driverName;
}

/*!
 * @brief Set font to bold
 *
 * @param[in] ipItem - Tree item we need to change the boldness
 * @param[in] ipBold - True for bold font or false for normal one
 */
static void
setBold(QTreeWidgetItem *ipItem, bool ipBold)
{
    QFont font = ipItem->font(0);
    font.setBold(ipBold);
    ipItem->setFont(0, font);
}

/*!
 * @brief Populate tree under parent-item with triggers
 *
 * @param[in] ipParentItem - Parent item we will populate
 * @param[in] ipList - The list of captions we need to fill with the parent item
 * @param[in] ipType - Item type
 */
void
TreeWidget::insertItems(QTreeWidgetItem *ipParentItem, QStringList *ipList, TreeWidget::Item ipType, bool /*ipDragEnabled*/)
{
    ipList->sort();

    QStringList::const_iterator iter;

    // for each item in the list
    for (iter = ipList->begin(); iter != ipList->end(); ++iter) {
        QString name = *iter;

        // create an item
        QTreeWidgetItem *item = new QTreeWidgetItem(ipParentItem);
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
        item->setText(TreeWidget::NameCol, name);
        item->setText(TreeWidget::IdCol, QString::number(ipType));
        item->setData(TreeWidget::NameCol, Qt::DisplayRole, name);
    }
}


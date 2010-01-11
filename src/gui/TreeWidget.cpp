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

/*
 * Constructor
 */
TreeWidget::TreeWidget(/*QMenu *ipMenu, */QWidget *ipParent)
    : QTreeWidget(ipParent)//, mContextMenu(ipMenu)
{
    setHeaderLabel(QString(""));
}

/*
 * Desctuctor
 */
TreeWidget::~TreeWidget()
{
}

/*
 * Set context menu
 */
void
TreeWidget::setContextMenu(QMenu *ipMenu)
{
    mContextMenu = ipMenu;
}

/*
 * Populate the tree with tables
 */
void
TreeWidget::refresh()
{
    clear();

    QSqlDatabase db = QSqlDatabase::database("mainConnect");

    setHeaderLabel(qDBCaption(db));

//    QFont font;
//    font.setBold(true);
//    QTreeWidgetItem *topSchemaItem = new QTreeWidgetItem;
//    topSchemaItem->setFont(0, font);
//    topSchemaItem->setText(TreeWidget::NameCol, "Schemas");
//    addTopLevelItem(topSchemaItem);

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
    Database *dbInst = Database::instance();

    //// ROLES
    // read roles
    dbInst->readRoles();

    QStringList rolesList;
    dbInst->rolesList(&rolesList);

    insertItems(rolesNode, &rolesList, TreeWidget::RoleItem);


    //// LANGS
    dbInst->readLanguages();

    // get and sort langs list
    QStringList langsList;
    dbInst->languagesList(&langsList);

    insertItems(langsNode, &langsList, TreeWidget::LanguageItem);

    //// SCHEMAS
    dbInst->readSchemas();

    // get and sort schemas list
    QStringList schemasList;
    dbInst->schemasList(&schemasList);

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

	DbSchema *schema = dbInst->findSchema(schemaName);

        // create tablesNode
        QTreeWidgetItem *tablesNode = new QTreeWidgetItem(schemaItem);
        tablesNode->setText(TreeWidget::NameCol, tr("Tables"));
        tablesNode->setText(TreeWidget::IdCol, QString::number(TreeWidget::TableNode));
	setBold(tablesNode, true);
        // create viewsNode
        QTreeWidgetItem *viewsNode = new QTreeWidgetItem(schemaItem);
        viewsNode->setText(TreeWidget::NameCol, tr("Views"));
        viewsNode->setText(TreeWidget::IdCol, QString::number(TreeWidget::ViewNode));
	setBold(viewsNode, true);
        // create procsNode
        QTreeWidgetItem *procsNode = new QTreeWidgetItem(schemaItem);
        procsNode->setText(TreeWidget::NameCol, tr("Procedures"));
        procsNode->setText(TreeWidget::IdCol, QString::number(TreeWidget::ProcedureNode));
        setBold(procsNode, true);
	// create procsNode
        QTreeWidgetItem *trigsNode = new QTreeWidgetItem(schemaItem);
        trigsNode->setText(TreeWidget::NameCol, tr("Triggers"));
        trigsNode->setText(TreeWidget::IdCol, QString::number(TreeWidget::TriggerNode));
        setBold(trigsNode, true);


        //// TABLES
        // get tables list for the given schema and sort
        QStringList tablesList;
        schema->tablesList(&tablesList);

        insertItems(tablesNode, &tablesList, TreeWidget::TableItem);


        //// VIEWS
        // get views list for the given schema and sort
        QStringList viewsList;
        schema->viewsList(&viewsList);

        insertItems(viewsNode, &viewsList, TreeWidget::ViewItem);


        //// PROCS
        // get procs list for the given schema and sort
        QStringList procsList;
        schema->proceduresList(&procsList);

        insertItems(procsNode, &procsList, TreeWidget::ProcedureItem);

	//// TRIGGERS
	QStringList trigsList;
	schema->triggersList(&trigsList);

	insertItems(trigsNode, &trigsList, TreeWidget::TriggerItem);

    }

    //// INDICES
    dbInst->readIndices();

    // get and sort indices list
    QStringList indicesList;
    dbInst->indicesList(&indicesList);

    insertItems(indicesNode, &indicesList, TreeWidget::IndexItem);



}

/*
 * Handler for context menu event
 */
void
TreeWidget::contextMenuEvent(QContextMenuEvent *event)
{
    mContextMenu->exec(event->globalPos());
}

/*
 * Create the database caption
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

/*
 * Set font to bold
 */
static void setBold(QTreeWidgetItem *item, bool bold)
{
    QFont font = item->font(0);
    font.setBold(bold);
    item->setFont(0, font);
}

/*
 * Populate tree under parent-item with triggers
 */
void
TreeWidget::insertItems(QTreeWidgetItem *ipParentItem, QStringList *ipList, TreeWidget::Item ipType)
{
    ipList->sort();

    QStringList::const_iterator iter;

    // for each item on the list
    for (iter = ipList->begin(); iter != ipList->end(); ++iter) {
        QString name = *iter;

		// create an item
		QTreeWidgetItem *item = new QTreeWidgetItem(ipParentItem);
		item->setText(TreeWidget::NameCol, name);
		item->setText(TreeWidget::IdCol, QString::number(ipType));
    }
}

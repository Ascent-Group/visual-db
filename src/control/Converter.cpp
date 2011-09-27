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
#include <control/Converter.h>
#include <gui/TreeWidget.h>

//#include <QMenu>

namespace Control {

Converter::Converter(DatabaseManager &iDbManager)
    : mDbManager(iDbManager)
{
}

Converter::~Converter()
{

}

/*!
 *
 */
bool
Converter::toTreeWidgetItems(const Objects &iObjects,
                             QList<Gui::TreeWidgetItem*> &oItems,
                             const ContextMenuManager *iMenuMgr)
{
    using namespace Gui;
    using namespace Control;

    TreeWidgetItem *item;
    QString parentName;
    ObjectType type;

    foreach(const QString &name, iObjects.keys()) {
        parentName = iObjects.value(name).first;
        type = iObjects.value(name).second;

        if ((item = new(std::nothrow) TreeWidgetItem(name, parentName, type))) {
            item->setContextMenu(iMenuMgr->menu(TreeItem | type));
            item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
            oItems.push_back(item);
        }
    }

    return true;
}

bool 
Converter::toTableItem(const Context &iContext, const DbObjects::Common::DbTablePtr &iDbTablePtr, 
        Gui::GraphicsItems::TableItem &oTableItem)
{
    using namespace DbObjects::Common;
    
    // create title item
    oTableItem.setTitleItem(new QGraphicsTextItem(iDbTablePtr.schemaName().toUpper() + "." + iDbTablePtr.name().toUpper()));

    // convert all fields
    for (int i = 0; i < iDbTablePtr->columnsCount(); ++i) {
        oTableItem.addFieldItem(new QGraphicsTextItem(iDbTablePtr->columnName(i) + ": " + iDbTablePtr->columnType(i)));
    }

    // convert all indices
    QVector<DbObjects::Common::DbIndexPtr> indices;
    mDbManager.indices(iContext, iDbTablePtr, indices);

    foreach (const DbIndexPtr &index, indices) {
        oTableItem.addIndexItem(new QGraphicsTextItem(index.name()));
    }

	return true;
}

bool 
Converter::toDbTable(DbObjects::Common::DbTablePtr &oDbTablePtr, 
	const Gui::GraphicsItems::TableItem &iTableItem)
{
	return true;
}

bool 
Converter::toViewItem(const DbObjects::Common::DbViewPtr &iDbViewPtr, 
        Gui::GraphicsItems::ViewItem &oViewItem)
{
    // create title item
    oViewItem.setTitleItem(new QGraphicsTextItem(iDbViewPtr.schemaName().toUpper() + "." + iDbViewPtr.name().toUpper()));

    // create field items
    foreach (QString fieldName, iDbViewPtr->columnsNames()) {
        oViewItem.addFieldItem(new QGraphicsTextItem(fieldName));
    }

	return true;
}

bool 
Converter::toDbView(DbObjects::Common::DbViewPtr &oDbViewPtr, 
	const Gui::GraphicsItems::ViewItem &iViewItem)
{
	return true;
}

bool 
Converter::toDescriptionWidget(const DbObjects::Common::DbIndexPtr &iDbIndexPtr, 
		Gui::DescriptionWidget &oDescriptionWidget)
{
	return true;
}

bool 
Converter::toDescriptionWidget(const DbObjects::Common::DbLanguagePtr &iDbLanguagePtr, 
		Gui::DescriptionWidget &oDescriptionWidget)
{
	return true;
}

bool 
Converter::toDescriptionWidget(const DbObjects::Common::DbProcedurePtr &iDbProcedurePtr, 
		Gui::DescriptionWidget &oDescriptionWidget)
{
	return true;
}

bool 
Converter::toDescriptionWidget(const DbObjects::Common::DbRolePtr &iDbRolePtr, 
		Gui::DescriptionWidget &oDescriptionWidget)
{
	return true;
}

bool 
Converter::toDescriptionWidget(const DbObjects::Common::DbSchemaPtr &iDbSchemaPtr, 
		Gui::DescriptionWidget &oDescriptionWidget)
{
	return true;
}

bool 
Converter::toDescriptionWidget(const DbObjects::Common::DbTablePtr &iDbTablePtr, 
		Gui::DescriptionWidget &oDescriptionWidget)
{
	return true;
}

bool 
Converter::toDescriptionWidget(const DbObjects::Common::DbTriggerPtr &iDbTriggerPtr, 
		Gui::DescriptionWidget &oDescriptionWidget)
{
	return true;
}

bool 
Converter::toDescriptionWidget(const DbObjects::Common::DbViewPtr &iDbTablePtr, 
		Gui::DescriptionWidget &oDescriptionWidget)
{
	return true;
}

}


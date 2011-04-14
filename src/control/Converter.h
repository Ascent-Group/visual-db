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

#ifndef CONTROL_CONVERTER_H
#define CONTROL_CONVERTER_H

#include <control/DatabaseManager.h>
#include <dbobjects/common/DbIndex.h>
#include <dbobjects/common/DbLanguage.h>
#include <dbobjects/common/DbProcedure.h>
#include <dbobjects/common/DbRole.h>
#include <dbobjects/common/DbSchema.h>
#include <dbobjects/common/DbTable.h>
#include <dbobjects/common/DbTrigger.h>
#include <dbobjects/common/DbView.h>
#include <gui/DescriptionWidget.h>
#include <gui/graphicsitems/TableItem.h>
#include <gui/graphicsitems/ViewItem.h>

namespace DbObjects
{

namespace Common
{

typedef DbObjectPtr<DbTable> DbTablePtr;

}

}

namespace Control {

/*!
 * \brief Auxiliary class that converts objects from one type to another.
 * \class Converter
 * \file control/converter.h
 */
class Converter {
    
    public:
        Converter(DatabaseManager &iDbManager);
        
        static bool toTableItem(const DbObjects::Common::DbTablePtr &iDbTablePtr, 
                Gui::GraphicsItems::TableItem &oTableItem);
        static bool toDbTable(DbObjects::Common::DbTablePtr &oDbTablePtr, 
                const Gui::GraphicsItems::TableItem &iTableItem);

        static bool toViewItem(const DbObjects::Common::DbViewPtr &iDbViewPtr, 
                Gui::GraphicsItems::ViewItem &oViewItem);
        static bool toDbView(DbObjects::Common::DbViewPtr &oDbViewPtr, 
                const Gui::GraphicsItems::ViewItem &iViewItem);

        static bool toDescriptionWidget(const DbObjects::Common::DbIndexPtr &iDbIndexPtr, 
                Gui::DescriptionWidget &oDescriptionWidget);
        static bool toDescriptionWidget(const DbObjects::Common::DbLanguagePtr &iDbLanguagePtr, 
                Gui::DescriptionWidget &oDescriptionWidget);
        static bool toDescriptionWidget(const DbObjects::Common::DbProcedurePtr &iDbProcedurePtr, 
                Gui::DescriptionWidget &oDescriptionWidget);
        static bool toDescriptionWidget(const DbObjects::Common::DbRolePtr &iDbRolePtr, 
                Gui::DescriptionWidget &oDescriptionWidget);
        static bool toDescriptionWidget(const DbObjects::Common::DbSchemaPtr &iDbSchemaPtr, 
                Gui::DescriptionWidget &oDescriptionWidget);
        static bool toDescriptionWidget(const DbObjects::Common::DbTablePtr &iDbTablePtr, 
                Gui::DescriptionWidget &oDescriptionWidget);
        static bool toDescriptionWidget(const DbObjects::Common::DbTriggerPtr &iDbTriggerPtr, 
                Gui::DescriptionWidget &oDescriptionWidget);
        static bool toDescriptionWidget(const DbObjects::Common::DbViewPtr &iDbTablePtr, 
                Gui::DescriptionWidget &oDescriptionWidget);

    private:
        Converter(const Converter &iConverter);
        Converter &operator=(const Converter &iConverter);
        ~Converter();

    private:
        DatabaseManager &mDbManager;
};

}

#endif // CONTROL_CONVERTER_H


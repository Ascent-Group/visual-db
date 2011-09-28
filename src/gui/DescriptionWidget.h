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

#ifndef DESCRIPTIONWIDGET_H
#define DESCRIPTIONWIDGET_H

#include <common/DbObjectPtr.h>
#include <gui/ui/ui_DescriptionWidget.h>
#include <QWidget>

namespace DbObjects
{

namespace Common
{

class DbIndex;
class DbProcedure;
class DbRole;
class DbSchema;
class DbTable;
class DbTrigger;
class DbView;

typedef DbObjectPtr<DbIndex> DbIndexPtr;
typedef DbObjectPtr<DbProcedure> DbProcedurePtr;
typedef DbObjectPtr<DbRole> DbRolePtr;
typedef DbObjectPtr<DbSchema> DbSchemaPtr;
typedef DbObjectPtr<DbTable> DbTablePtr;
typedef DbObjectPtr<DbTrigger> DbTriggerPtr;
typedef DbObjectPtr<DbView> DbViewPtr;

} // namespace Common

} //namespace DbObjects

namespace Gui {

/*!
 * \class DescriptionWidget
 * \headerfile gui/DescriptionWidget.h
 * \brief Description widget. Here the object's description is displayed.
 */
class DescriptionWidget : public QWidget
{
    Q_OBJECT

    public:
        enum DbTableCols {
            TableColumnNameCol = 0,
            TableTypeCol,
            TableNullCol,
            TableUniqueCol,
            TablePKCol,
            TableFKCol,
            TableForeignTableCol,
            DbTableColumnsCount
        };

        enum DbRoleCols {
            RoleNameCol = 0,
            RoleIdCol,
            RoleSuperUserCol,
            RoleInheritCol,
            RoleCreateRoleCol,
            RoleCreateDbCol,
            RoleUpdateSysCatCol,
            RoleLoginCol,
            RoleConnectionLimitCol,
            RoleExpiryDateCol,
            DbRoleColumnsCount
        };

        enum DbViewCols {
            ViewNameCol = 0,
            ViewSchemaCol,
            ViewOwnerCol,
            DbViewColumnsCount
        };

        enum DbIndexCols {
            IndexNameCol = 0,
            IndexTableCol,
            IndexColCountCol,
            IndexColNumsCol,
            IndexUniqueCol,
            IndexPrimaryCol,
            IndexClusteredCol,
            IndexValidCol,
            IndexChecksXMinCol,
            IndexReadyCol,
            DbIndexColumnsCount
        };

        enum DbTriggerCols {
            TriggerNameCol = 0,
            TriggerTableNameCol,
            TriggerProcedureNameCol,
            TriggerEnabledCol,
            TriggerIsConstraintCol,
            TriggerConstraintNameCol,
            TriggerReferencedTableNameCol,
            TriggerIsDeferrableCol,
            TriggerIsInitiallyDeferredCol,
            TriggerNumArgsCol,
            DbTriggerColumnsCount
        };

        enum DbSchemaCols {
            SchemaNameCol = 0,
            SchemaOwnerNameCol,
            SchemaDescriptionCol,
            DbSchemaColumnsCount
        };

        enum DbProcedureCols {
            ProcedureNameCol = 0,
            ProcedureSchemaCol,
            ProcedureOwnerCol,
            ProcedureLanguageCol,
            DbProcedureColumnsCount
        };

    public:
        DescriptionWidget(QWidget *iParent = 0);
        ~DescriptionWidget();

        QString objectName() const;
        QString schemaName() const;

        void describe(const DbObjects::Common::DbIndexPtr &iIndex);
        void describe(const DbObjects::Common::DbProcedurePtr &iProcedure);
        void describe(const DbObjects::Common::DbRolePtr &iRole);
        void describe(const DbObjects::Common::DbSchemaPtr &iSchema);
        void describe(const DbObjects::Common::DbTablePtr &iTable);
        void describe(const DbObjects::Common::DbTriggerPtr &iTrigger);
        void describe(const DbObjects::Common::DbViewPtr &iView);

    private:
        Ui::DescriptionWidget ui;

        QString mObjectName;
        QString mSchemaName;

        static const QString sCreateSchemaScript;
        static const QString sAddSchemaDecriptionScript;
};

}

#endif // DESCRIPTIONWIDGET_H


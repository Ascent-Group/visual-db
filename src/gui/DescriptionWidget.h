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

#include <gui/ui/ui_DescriptionWidget.h>
#include <QWidget>

class DbIndex;
class DbRole;
class DbSchema;
class DbTable;
class DbTrigger;
class DbView;

/*
 * Description widget. Here the object's description is displayed.
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

    public:
        DescriptionWidget(QWidget *ipParent = 0);
        ~DescriptionWidget();

        void describe(const DbSchema *ipSchema);
        void describe(const DbTable *ipTable);
        void describe(const DbRole *ipRole);
        void describe(const DbView *ipView);
        void describe(const DbIndex *ipIndex);
        void describe(const DbTrigger *ipTrigger);

    private:
	Ui::DescriptionWidget ui;

        static const QString sCreateSchemaScript;
        static const QString sAddSchemaDecriptionScript;
};

#endif // DESCRIPTIONWIDGET_H

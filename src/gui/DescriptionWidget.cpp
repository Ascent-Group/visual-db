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

#include <common/Database.h>
#include <common/DbIndex.h>
#include <common/DbRole.h>
#include <common/DbSchema.h>
#include <common/DbTable.h>
#include <common/DbTrigger.h>
#include <common/DbView.h>
#include <gui/DescriptionWidget.h>
#include <QMessageBox>
#include <QSplitter>
#include <QtDebug>

const QString DescriptionWidget::sCreateSchemaScript = "CREATE SCHEMA %1 \n"
            "AUTHORIZATION %2;\n";
const QString DescriptionWidget::sAddSchemaDecriptionScript = "\nCOMMENT ON SCHEMA public IS '%1';";

/*
 * Ctor
 */
DescriptionWidget::DescriptionWidget(QWidget *ipParent)
    : QWidget(ipParent)
{
    ui.setupUi(this);
}

/*
 * Dtor
 */
DescriptionWidget::~DescriptionWidget()
{
}

/*
 * Generates and represents schema's description
 */
void
DescriptionWidget::describe(const DbSchema *ipSchema)
{
    if (0 == ipSchema) {
        QMessageBox::critical(
                this,
                tr("Missing object"),
                tr("No such schema! \n\n Note: Try to reload data from DB."),
                tr("Ok"));
        return;
    }

    // get the Schema data
    QString name = ipSchema->name();
    QString ownerName = ipSchema->ownerName();
    QString description = ipSchema->description();

    ui.mTable->setRowCount(1);
    ui.mTable->setColumnCount(DescriptionWidget::DbSchemaColumnsCount);

    // set column labels
    QStringList labels;

    labels << tr("Name")
            << tr("Owner")
            << tr("Description");

    ui.mTable->setHorizontalHeaderLabels(labels);

    // fill table with Schema data
    QTableWidgetItem *nameItem = new QTableWidgetItem();
    nameItem->setText(name);
    ui.mTable->setItem(0, DescriptionWidget::SchemaNameCol, nameItem);

    QTableWidgetItem *ownerNameItem = new QTableWidgetItem();
    ownerNameItem->setText(ownerName);
    ui.mTable->setItem(0, DescriptionWidget::SchemaOwnerNameCol, ownerNameItem);

    QTableWidgetItem *descrItem = new QTableWidgetItem();
    descrItem->setText(description);
    ui.mTable->setItem(0, DescriptionWidget::SchemaDescriptionCol, descrItem);

    // auto resize cells
    ui.mTable->resizeColumnsToContents();

    QString body = sCreateSchemaScript.arg(name).arg(ownerName);

    if (description.length() > 0) {
        body += sAddSchemaDecriptionScript.arg(description);
    }

    ui.mBodyEdit->setText(body);
}

/*
 * Generates and represents table's description
 */
void
DescriptionWidget::describe(const DbTable *ipTable)
{
    if ( 0 == ipTable ) {
        QMessageBox::critical(
                this,
                tr("Missing object"),
                tr("No such table! \n\n Note: Try to reload data from DB."),
                tr("Ok"));
        return;
    }

    // get columns count
    quint16 columnsCount = ipTable->columnsCount();

    ui.mTable->setRowCount(columnsCount);
    ui.mTable->setColumnCount(DescriptionWidget::DbTableColumnsCount);

    // set column labels
    QStringList labels;

    labels << tr("Name")
            << tr("Type")
            << tr("Nullable")
            << tr("Unique")
            << tr("Primary key")
            << tr("Foreign key")
            << tr("Foreign table");

    ui.mTable->setHorizontalHeaderLabels(labels);

    // generate body text
    QString body = QString("CREATE TABLE %1.%2 (")
                   .arg(ipTable->schemaName())
                   .arg(ipTable->name());

    // for each db table's column fill the rows and generate body text
    for (quint16 i = 0; i < columnsCount; ++i) {
        // read attributes
        QString name = ipTable->columnName(i);
        QString type = ipTable->columnType(i);
        bool nullable = ipTable->isColumnNullable(i);
        bool unique = ipTable->isColumnUnique(i);
        bool primary = ipTable->isColumnPrimaryKey(i);
        bool foreign = ipTable->isColumnForeignKey(i);
        QString foreignSchemaName = ipTable->foreignSchemaName(i);
        QString foreignTableName = ipTable->foreignTableName(i);
        QString foreignFieldsNames = ipTable->foreignFields(i).join(", ");

        // fill column name
        QTableWidgetItem *nameItem = new QTableWidgetItem();
        nameItem->setText(name);
        ui.mTable->setItem(i, DescriptionWidget::TableColumnNameCol, nameItem);

        // fill column type
        QTableWidgetItem *typeItem = new QTableWidgetItem();
        typeItem->setText(type);
        ui.mTable->setItem(i, DescriptionWidget::TableTypeCol, typeItem);

        // fill nullable constraint
        QTableWidgetItem *nullableItem = new QTableWidgetItem();
        nullableItem->setTextAlignment(Qt::AlignHCenter);
        nullableItem->setText(nullable ? "Y" : "N");
        ui.mTable->setItem(i, DescriptionWidget::TableNullCol, nullableItem);

        // fill unique constraint
        QTableWidgetItem *uniqueItem = new QTableWidgetItem();
        uniqueItem->setTextAlignment(Qt::AlignHCenter);
        uniqueItem->setText(unique ? "Y" : "N");
        ui.mTable->setItem(i, DescriptionWidget::TableUniqueCol, uniqueItem);

        // fill PK constraint
        QTableWidgetItem *pkItem = new QTableWidgetItem();
        pkItem->setTextAlignment(Qt::AlignHCenter);
        pkItem->setText(primary ? "Y" : "N");
        ui.mTable->setItem(i, DescriptionWidget::TablePKCol, pkItem);

        // fill FK constraint
        QTableWidgetItem *fkItem = new QTableWidgetItem();
        fkItem->setTextAlignment(Qt::AlignHCenter);
        fkItem->setText(foreign ? "Y" : "N");
        ui.mTable->setItem(i, DescriptionWidget::TableFKCol, fkItem);

        // fill foreign table name
        QTableWidgetItem *foreignTableItem = new QTableWidgetItem();
        // if foreign key
        if (foreign) {
            // fill the cell
            foreignTableItem->setText(QString("%1.%2")
                                      .arg(foreignSchemaName)
                                      .arg(foreignTableName));
        }

        ui.mTable->setItem(i, DescriptionWidget::TableForeignTableCol, foreignTableItem);

        // generate body text
        body.append(QString(" \n\t%1\t%2").arg(name).arg(type));

        if (!nullable) {
            body.append(" NOT NULL");
        }

        if (unique) {
            body.append(" UNIQUE");
        }

        if (primary) {
            body.append(" PRIMARY KEY");
        }

        if (foreign) {
            body.append(QString(" REFERENCES %1.%2(%3)")
                        .arg(foreignSchemaName)
                        .arg(foreignTableName)
                        .arg(foreignFieldsNames));
        }
    
    if (i != columnsCount - 1) {
        body.append(",");
    }
    }

    // auto resize cells
    ui.mTable->resizeColumnsToContents();
    ui.mTable->resizeRowsToContents();

    // show generated body
    body.append("\n);");
    ui.mBodyEdit->setText(body);

}

/*
 * Generates and represents role's description
 */
void
DescriptionWidget::describe(const DbRole *ipRole)
{
    if ( 0 == ipRole ) {
        QMessageBox::critical(
                this,
                tr("Missing object"),
                tr("No such role! \n\n Note: Try to reload data from DB."),
                tr("Ok"));
        return;
    }

    ui.mTable->setRowCount(1);
    ui.mTable->setColumnCount(DescriptionWidget::DbRoleColumnsCount);

    // set column labels
    QStringList labels;

    labels << tr("Name")
            << tr("Id")
            << tr("Super user")
            << tr("Inherits\nprivileges")
            << tr("Create role")
            << tr("Create DB")
            << tr("Update\nsys catalogs")
            << tr("Login")
            << tr("Connection\nlimit")
            << tr("Expiration\ndate");


    ui.mTable->setHorizontalHeaderLabels(labels);

    // read attributes
    QString name = ipRole->name();
    quint64 id = ipRole->id();
    bool superUser = ipRole->isSuperUser();
    bool inherits = ipRole->inheritsPrivileges();
    bool createRole = ipRole->canCreateRole();
    bool createDb = ipRole->canCreateDb();
    bool updateSysCat = ipRole->canUpdateSysCat();
    bool login = ipRole->canLogin();
    quint32 connLimit = ipRole->connectionLimit();
    QDate expiryDate = ipRole->expiryDate();


    // fill role name
    QTableWidgetItem *nameItem = new QTableWidgetItem();
    nameItem->setText(name);
    ui.mTable->setItem(0, DescriptionWidget::RoleNameCol, nameItem);

    // fill role id
    QTableWidgetItem *idItem = new QTableWidgetItem();
    idItem->setText(QString::number(id));
    ui.mTable->setItem(0, DescriptionWidget::RoleIdCol, idItem);

    // fill super user flag
    QTableWidgetItem *superUserItem = new QTableWidgetItem();
    superUserItem->setTextAlignment(Qt::AlignHCenter);
    superUserItem->setText(superUser ? "Y" : "N");
    ui.mTable->setItem(0, DescriptionWidget::RoleSuperUserCol, superUserItem);

    // fill inherit priv flag
    QTableWidgetItem *inheritItem = new QTableWidgetItem();
    inheritItem->setTextAlignment(Qt::AlignHCenter);
    inheritItem->setText(inherits ? "Y" : "N");
    ui.mTable->setItem(0, DescriptionWidget::RoleInheritCol, inheritItem);

    // fill create role flag
    QTableWidgetItem *createRoleItem = new QTableWidgetItem();
    createRoleItem->setTextAlignment(Qt::AlignHCenter);
    createRoleItem->setText(createRole ? "Y" : "N");
    ui.mTable->setItem(0, DescriptionWidget::RoleCreateRoleCol, createRoleItem);

    // fill create db flag
    QTableWidgetItem *createDbItem = new QTableWidgetItem();
    createDbItem->setTextAlignment(Qt::AlignHCenter);
    createDbItem->setText(createDb ? "Y" : "N");
    ui.mTable->setItem(0, DescriptionWidget::RoleCreateDbCol, createDbItem);

    // fill update sys cat flag
    QTableWidgetItem *updSysCatItem = new QTableWidgetItem();
    updSysCatItem->setTextAlignment(Qt::AlignHCenter);
    updSysCatItem->setText(updateSysCat ? "Y" : "N");
    ui.mTable->setItem(0, DescriptionWidget::RoleUpdateSysCatCol, updSysCatItem);

    // fill login flag
    QTableWidgetItem *loginItem = new QTableWidgetItem();
    loginItem->setTextAlignment(Qt::AlignHCenter);
    loginItem->setText(login ? "Y" : "N");
    ui.mTable->setItem(0, DescriptionWidget::RoleLoginCol, loginItem);

    // fill connection limit
    QTableWidgetItem *connLimitItem = new QTableWidgetItem();
    connLimitItem->setTextAlignment(Qt::AlignHCenter);
    connLimitItem->setText(QString::number(connLimit));
    ui.mTable->setItem(0, DescriptionWidget::RoleConnectionLimitCol, connLimitItem);

    // fill expiry date
    QTableWidgetItem *expiryDateItem = new QTableWidgetItem();
    expiryDateItem->setTextAlignment(Qt::AlignHCenter);
    expiryDateItem->setText(expiryDate.toString("dd-MM-yyyy"));
    ui.mTable->setItem(0, DescriptionWidget::RoleExpiryDateCol, expiryDateItem);

    // auto resize cells
    ui.mTable->resizeColumnsToContents();
    ui.mTable->resizeRowsToContents();

    // generate body text
    QString body = QString("CREATE ROLE %1 WITH ").arg(name);

    body.append(QString("\n\t%1").arg( superUser ? "SUPERUSER" : "NOSUPERSUER"));
    body.append(QString("\n\t%1").arg( createDb ? "CREATEDB" : "NOCREATEDB"));
    body.append(QString("\n\t%1").arg( createRole ? "CREATEROLE" : "NOCREATEROLE"));
    body.append(QString("\n\t%1").arg( inherits ? "INHERIT" : "NOINHERIT"));
    body.append(QString("\n\t%1").arg( login ? "LOGIN" : "NOLOGIN"));
    body.append(QString("\n\tCONNECTION LIMIT %1").arg(QString::number(connLimit)));

    if (!expiryDate.isNull()) {
        body.append(QString("\n\tVALID UNTIL '%1'").arg(expiryDate.toString("dd-MM-yyyy")));
    }

    body.append(QString("\n\tSYSID %1").arg(QString::number(id)));

    // show generated body
    body.append("\n;");
    ui.mBodyEdit->setText(body);

}

/*
 * Generates and represents view's description
 */
void
DescriptionWidget::describe(const DbView *ipView)
{
    if ( 0 == ipView ) {
        QMessageBox::critical(
                this,
                tr("Missing object"),
                tr("No such view! \n\n Note: Try to reload data from DB."),
                tr("Ok"));
        return;
    }

    ui.mTable->setRowCount(1);
    ui.mTable->setColumnCount(DescriptionWidget::DbViewColumnsCount);

    // set column labels
    QStringList labels;

    labels << tr("Name")
            << tr("Schema")
            << tr("Owner");


    ui.mTable->setHorizontalHeaderLabels(labels);

    // read attributes
    QString name = ipView->name();
    QString schemaName = ipView->schema()->name();
    QString ownerName = ipView->owner()->name();
    QString def = ipView->definition();

    // fill view name
    QTableWidgetItem *nameItem = new QTableWidgetItem();
    nameItem->setText(name);
    ui.mTable->setItem(0, DescriptionWidget::ViewNameCol, nameItem);

    // fill schema name
    QTableWidgetItem *schemaNameItem = new QTableWidgetItem();
    schemaNameItem->setText(schemaName);
    ui.mTable->setItem(0, DescriptionWidget::ViewSchemaCol, schemaNameItem);

    // fill owner name
    QTableWidgetItem *ownerNameItem = new QTableWidgetItem();
    ownerNameItem->setText(ownerName);
    ui.mTable->setItem(0, DescriptionWidget::ViewOwnerCol, ownerNameItem);

    // auto resize cells
    ui.mTable->resizeColumnsToContents();
    ui.mTable->resizeRowsToContents();

    // format body
    def.replace(",", ",\n\t");
    def.replace("SELECT", "\nSELECT\n\t");
    def.replace("FROM", "\nFROM\n\t");
    def.replace("WHERE", "\nWHERE\n\t");

    ui.mBodyEdit->setText(def.trimmed());
}

/*
 * Generates and represents index's description
 */
void
DescriptionWidget::describe(const DbIndex *ipIndex)
{
    if ( 0 == ipIndex ) {
        QMessageBox::critical(
                this,
                tr("Missing object"),
                tr("No such index! \n\n Note: Try to reload data from DB."),
                tr("Ok"));
        return;
    }

    ui.mTable->setRowCount(1);
    ui.mTable->setColumnCount(DescriptionWidget::DbIndexColumnsCount);

    // set column labels
    QStringList labels;

    labels << tr("Name")
            << tr("Table")
            << tr("Columns\ncount")
            << tr("Columns")
            << tr("Unique")
            << tr("Primary")
            << tr("Clustered")
            << tr("Valid")
            << tr("ChecksXMin")
            << tr("Ready");

    ui.mTable->setHorizontalHeaderLabels(labels);

    // read attributes
    QString name = ipIndex->name();
    QString tableName = ipIndex->table()->name();
    QString schemaName = ipIndex->schema()->name();
    quint16 colsCount = ipIndex->columnsCount();
    QVector<qint16> colNums = ipIndex->columnsNumbers();
    bool unique = ipIndex->isUnique();
    bool primary = ipIndex->isPrimary();
    bool clustered = ipIndex->isClustered();
    bool valid = ipIndex->isValid();
    bool checkXMin = ipIndex->checksXMin();
    bool ready = ipIndex->isReady();

    // find table object
    DbSchema *schema = Database::instance()->findSchema(schemaName);
    DbTable *table;

    if (schema) {
        table = schema->findTable(tableName);
    }


    // fill view name
    QTableWidgetItem *nameItem = new QTableWidgetItem();
    nameItem->setText(name);
    ui.mTable->setItem(0, DescriptionWidget::IndexNameCol, nameItem);

    // fill table name
    QTableWidgetItem *tableItem = new QTableWidgetItem();
    tableItem->setText(QString("%1.%2").arg(schemaName).arg(tableName));
    ui.mTable->setItem(0, DescriptionWidget::IndexTableCol, tableItem);

    // fill cols count
    QTableWidgetItem *colsCountItem = new QTableWidgetItem();
    colsCountItem->setTextAlignment(Qt::AlignHCenter);
    colsCountItem->setText(QString::number(colsCount));
    ui.mTable->setItem(0, DescriptionWidget::IndexColCountCol, colsCountItem);

    // fill col nums
    QTableWidgetItem *colNumsItem = new QTableWidgetItem();
    colNumsItem->setTextAlignment(Qt::AlignHCenter);

    QVector<qint16>::const_iterator iter;
    QString colNames;

    for (iter = colNums.constBegin(); iter != colNums.constEnd(); ++iter) {
        qint16 id = *iter - 1;

        // if index is not for plain column but for an expr on a column
        if (0 < id) {
            colNames.append(QString("%1, ").arg(table->columnName(id)));
        // else an index if for a plain column
        } else {
            colNames.append(QString("expr(%1), ").arg(table->columnName(id)));
        }
    }

    // remove last comma and space
    colNames.chop(2);

    colNumsItem->setText(colNames);
    ui.mTable->setItem(0, DescriptionWidget::IndexColNumsCol, colNumsItem);

    // fill unique flag
    QTableWidgetItem *uniqueItem = new QTableWidgetItem();
    uniqueItem->setTextAlignment(Qt::AlignHCenter);
    uniqueItem->setText(unique ? "Y" : "N");
    ui.mTable->setItem(0, DescriptionWidget::IndexUniqueCol, uniqueItem);

    // fill primary flag
    QTableWidgetItem *primaryItem = new QTableWidgetItem();
    primaryItem->setTextAlignment(Qt::AlignHCenter);
    primaryItem->setText(primary ? "Y" : "N");
    ui.mTable->setItem(0, DescriptionWidget::IndexPrimaryCol, primaryItem);

    // fill clustered flag
    QTableWidgetItem *clusteredItem = new QTableWidgetItem();
    clusteredItem->setTextAlignment(Qt::AlignHCenter);
    clusteredItem->setText(clustered ? "Y" : "N");
    ui.mTable->setItem(0, DescriptionWidget::IndexClusteredCol, clusteredItem);

    // fill valid flag
    QTableWidgetItem *validItem = new QTableWidgetItem();
    validItem->setTextAlignment(Qt::AlignHCenter);
    validItem->setText(valid ? "Y" : "N");
    ui.mTable->setItem(0, DescriptionWidget::IndexValidCol, validItem);

    // fill checkXMin flag
    QTableWidgetItem *checkXMinItem = new QTableWidgetItem();
    checkXMinItem->setTextAlignment(Qt::AlignHCenter);
    checkXMinItem->setText(checkXMin ? "Y" : "N");
    ui.mTable->setItem(0, DescriptionWidget::IndexChecksXMinCol, checkXMinItem);

    // fill ready flag
    QTableWidgetItem *readyItem = new QTableWidgetItem();
    readyItem->setTextAlignment(Qt::AlignHCenter);
    readyItem->setText(ready ? "Y" : "N");
    ui.mTable->setItem(0, DescriptionWidget::IndexReadyCol, readyItem);

    // auto resize cells
    ui.mTable->resizeColumnsToContents();
    ui.mTable->resizeRowsToContents();

    // generate body
    QString body = QString("CREATE ");

    if (unique) {
        body.append("UNIQUE" );
    }

    body.append(QString(" INDEX %1 ON %2\n{ ")
                .arg(name)
                .arg(tableName));

    //colNames.replace(",", ",\n\t");

    body.append(colNames);
    body.append(" };");

    ui.mBodyEdit->setText(body);
}

/*
 * Generates and represents trig's description
 */
void
DescriptionWidget::describe(const DbTrigger *ipTrigger)
{
    if ( 0 == ipTrigger ) {
        QMessageBox::critical(
                this,
                tr("Missing object"),
                tr("No such trigger! \n\n Note: Try to reload data from DB."),
                tr("Ok"));
        return;
    }

    ui.mTable->setRowCount(1);
    ui.mTable->setColumnCount(DescriptionWidget::DbTriggerColumnsCount);

    // set column labels
    QStringList labels;

    labels << tr("Name")
            << tr("Table")
            << tr("Procedure")
            << tr("Enabled")
            << tr("Is constraint")
            << tr("Constraint name")
            << tr("Referenced table")
            << tr("Is deferrable")
            << tr("Is initially deferred")
            << tr("Number\nof\narguments");

    ui.mTable->setHorizontalHeaderLabels(labels);

    // read attributes
    QString name = ipTrigger->name();
    QString tableName = ipTrigger->table()->fullName();
    QString procName = ipTrigger->procedure()->fullName();
    QChar enabled = ipTrigger->enabled();
    bool isConstraint = ipTrigger->isConstraint();
    QString constraintName = ipTrigger->constraintName();
    QString refTableName = ipTrigger->referencedTable()->fullName();
    bool isDeferrable = ipTrigger->isDeferrable();
    bool isInitiallyDeferred = ipTrigger->isInitiallyDeferred();
    quint16 numArgs = ipTrigger->numArgs();

    // fill name
    QTableWidgetItem *nameItem = new QTableWidgetItem();
    nameItem->setText(name);
    ui.mTable->setItem(0, DescriptionWidget::TriggerNameCol, nameItem);

    // fill table
    QTableWidgetItem *tableItem = new QTableWidgetItem();
    tableItem->setText(tableName);
    ui.mTable->setItem(0, DescriptionWidget::TriggerTableNameCol, tableItem);

    // fill proc
    QTableWidgetItem *procItem = new QTableWidgetItem();
    procItem->setText(procName);
    ui.mTable->setItem(0, DescriptionWidget::TriggerProcedureNameCol, procItem);

    // fill enabled
    QTableWidgetItem *enabledItem = new QTableWidgetItem();
    enabledItem->setTextAlignment(Qt::AlignHCenter);
    enabledItem->setText(QString(enabled));
    ui.mTable->setItem(0, DescriptionWidget::TriggerEnabledCol, enabledItem);

    // fill isConstraint flag
    QTableWidgetItem *isConstraintItem = new QTableWidgetItem();
    isConstraintItem->setTextAlignment(Qt::AlignHCenter);
    isConstraintItem->setText(isConstraint ? "Y" : "N");
    ui.mTable->setItem(0, DescriptionWidget::TriggerIsConstraintCol, isConstraintItem);

    // fill constraint name
    QTableWidgetItem *constraintNameItem = new QTableWidgetItem();
    constraintNameItem->setText(constraintName);
    ui.mTable->setItem(0, DescriptionWidget::TriggerConstraintNameCol, constraintNameItem);

    // fill referenced table
    QTableWidgetItem *refTableItem = new QTableWidgetItem();
    refTableItem->setText(refTableName);
    ui.mTable->setItem(0, DescriptionWidget::TriggerReferencedTableNameCol, refTableItem);

    // fill isDeferrable flag
    QTableWidgetItem *isDeferrableItem = new QTableWidgetItem();
    isDeferrableItem->setTextAlignment(Qt::AlignHCenter);
    isDeferrableItem->setText(isDeferrable ? "Y" : "N");
    ui.mTable->setItem(0, DescriptionWidget::TriggerIsDeferrableCol, isDeferrableItem);

    // fill isInitiallyDeferred flag
    QTableWidgetItem *isInitiallyDeferredItem = new QTableWidgetItem();
    isInitiallyDeferredItem->setTextAlignment(Qt::AlignHCenter);
    isInitiallyDeferredItem->setText(isInitiallyDeferred ? "Y" : "N");
    ui.mTable->setItem(0, DescriptionWidget::TriggerIsInitiallyDeferredCol, isInitiallyDeferredItem);

    // fill num args
    QTableWidgetItem *numArgsItem = new QTableWidgetItem();
    numArgsItem->setTextAlignment(Qt::AlignHCenter);
    numArgsItem->setText(QString::number(numArgs));
    ui.mTable->setItem(0, DescriptionWidget::TriggerNumArgsCol, numArgsItem);

    QString body = QString("CREATE TRIGGER %1\nON %2 FOR EACH ROW\n")
                   .arg(name)
                   .arg(tableName);

    body.append(QString("EXECUTE PROCEDURE %1(...);").arg(procName));

    // auto resize cells
    ui.mTable->resizeColumnsToContents();
    ui.mTable->resizeRowsToContents();

}

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

#include <QCloseEvent>
#include <QColor>
#include <QDate>
#include <QDomDocument>
#include <QDomElement>
#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QPrinter>
#include <QProgressBar>
#include <QSettings>
#include <QSqlDatabase>
#include <QStatusBar>
#include <QTime>
#include <QTreeWidgetItem>
#include <QUndoCommand>
#include <QUndoStack>
#include <common/Database.h>
#include <common/DbSchema.h>
#include <connect/DbParameters.h>
#include <connect/ProxyParameters.h>
#include <consts.h>
#include <gui/ArrowItem.h>
#include <gui/DescriptionWidget.h>
#include <gui/GraphicsScene.h>
#include <gui/GraphicsView.h>
#include <gui/Legend.h>
#include <gui/MainWindow.h>
#include <gui/OptionsDialog.h>
#include <gui/SceneWidget.h>
#include <gui/SqlConnectionDialog.h>
#include <gui/SqlWidget.h>
#include <gui/TabWidget.h>
#include <gui/TableItem.h>
#include <gui/TableItemGroup.h>
#include <gui/TreeWidget.h>

#include <QtDebug>

/*
 * Constructor
 */
    MainWindow::MainWindow()
: QMainWindow()
{
    ui.setupUi(this);

    // create menus
    createMenus();

    /*DO NOT DELETE ME ALEX*/
    ui.mTree->setContextMenu(mTreeItemMenu);

    createStatusBar();

    mDbParameters = new DbParameters();
    mProxyParameters = new ProxyParameters();

    // set attributes
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowState(Qt::WindowMaximized);

    initSession();

    connect(ui.mNewConnectionAction, SIGNAL(triggered()), this, SLOT(showConnectionDialog()));
}

/*
 * Create actions
 */
void MainWindow::createActions()
{
    // undo stack
    mUndoStack = new QUndoStack();

    // undo action
    QAction *undoAction = mUndoStack->createUndoAction(this, tr("Undo"));
    undoAction->setIcon(QIcon(":/img/undo.png"));
    undoAction->setShortcut(QString("Ctrl+Z"));

    // redo action
    QAction *redoAction = mUndoStack->createRedoAction(this, tr("Redo"));
    redoAction->setIcon(QIcon(":/img/redo.png"));
    redoAction->setShortcut(QString("Ctrl+Y"));

    ui.mEditMenu->addAction(undoAction);
    ui.mEditMenu->addAction(redoAction);

    // align tables to the grid
    ui.mAlignToGridAction->setChecked(mSettings.value(Consts::VIEW_GRP + "/" + Consts::ALIGN_TO_GRID_SETTING, false).toBool());
}

/*
 * Create menus
 */
void
MainWindow::createMenus()
{
    // create actions before
    createActions();
    setEnableForActions(false);

    updateSessionMenu();

    // tree item menu
    mTreeItemMenu = new QMenu();
    mTreeItemMenu->addAction(ui.mAddTableAction);
    mTreeItemMenu->addAction(ui.mDescribeObjectAction);
    mTreeItemMenu->addAction(ui.mQueryDataAction);

    ui.mSceneWidget->setSchemeMenu(ui.mSchemeMenu);
    ui.mSceneWidget->setTableMenu(ui.mTableMenu);
}

/*
 * Update session menu
 */
void
MainWindow::updateSessionMenu()
{
    ui.mSessionMenu->clear();

    using namespace Consts;
    int countSavedSessions = mSettings.value(PREFS_GRP + "/" + COUNT_SAVED_SESSIONS_SETTING, 10).toInt();
    for (int i = 0; i < countSavedSessions; ++i) {
        // update session menu
        if (mSettings.contains(LAST_SESSION_GRP + "/" + SAVED_SESSION_SETTING + QString().setNum(i))) {
            // last session
            QAction *lastSessionAction = new QAction(mSettings.value(LAST_SESSION_GRP + "/" + SAVED_SESSION_SETTING + QString().setNum(i)).toString(), this);
            connect(lastSessionAction, SIGNAL(triggered()), this, SLOT(loadLastSession()));

            ui.mSessionMenu->addAction(lastSessionAction);
        } else {
            break;
        }
    }
    ui.mSessionMenu->addSeparator();
    ui.mSessionMenu->addAction(ui.mLoadSessionAction);
    ui.mSessionMenu->addAction(ui.mSaveSessionAction);
}

/*
 * Enable or disable some menu actions
 */
void
MainWindow::setEnableForActions(bool ipFlag)
{
    ui.mSaveToImgAction->setEnabled(ipFlag);
    ui.mDrawFullDBAction->setEnabled(ipFlag);

    ui.mSelectAllTablesAction->setEnabled(ipFlag);
    ui.mRemoveAllTablesAction->setEnabled(ipFlag);
    ui.mShowLegendAction->setEnabled(ipFlag);
    ui.mColorizeAccordingSchemasAction->setEnabled(ipFlag);

    ui.mDeleteTableAction->setEnabled(ipFlag);
    ui.mShowFieldsTypesAction->setEnabled(ipFlag);
    ui.mHideFieldsTypesAction->setEnabled(ipFlag);
    ui.mSetTableColorAction->setEnabled(ipFlag);
    ui.mAdjustTableSizeAction->setEnabled(ipFlag);
    ui.mGroupItemsAction->setEnabled(ipFlag);
    ui.mUngroupItemsAction->setEnabled(ipFlag);
    ui.mDescribeObjectAction->setEnabled(ipFlag);
    ui.mQueryDataAction->setEnabled(ipFlag);
    ui.mShowIndicesAction->setEnabled(ipFlag);
    ui.mHideIndicesAction->setEnabled(ipFlag);
    ui.mSelectAllTablesInSchemaAction->setEnabled(ipFlag);

    ui.mSaveSessionAction->setEnabled(ipFlag);

    ui.mReloadDataAction->setEnabled(ipFlag);
}

/*
 * Show connection dialog
 */
    int
MainWindow::showConnectionDialog(bool ipLoadSession)
{
    SqlConnectionDialog connDialog(mDbParameters, mProxyParameters, ipLoadSession);

    // nothing to do if canceled
    int code = connDialog.exec();
    if (code != QDialog::Accepted) {
        return code;
    }

    if (QSqlDatabase::database("mainConnect").open()) {
        ui.mSceneWidget->cleanTableSchemeScene();
        ui.mTree->refresh();
        ui.mSceneWidget->refreshLegend();
        setEnableForActions(true);
        return QDialog::Accepted;
    }

    return QDialog::Rejected;
}

/*
 * Show options dialog
 */
void
MainWindow::showOptionsDialog()
{
    OptionsDialog optionsDialog;

    // no need to check for accept/reject
    // this will be done by optionsDialog's accept method
    if (optionsDialog.exec() == QDialog::Accepted) {
        updateSessionMenu();
    }
}

/*
 * Show print preview dialog
 */
void
MainWindow::showPrintPreviewDialog()
{
    QPrinter printer(QPrinter::ScreenResolution);
    printer.setPaperSize(QPrinter::A4);
    QPrintPreviewDialog previewDialog(&printer, this);
    connect(&previewDialog, SIGNAL(paintRequested(QPrinter *)), this, SLOT(printPreview(QPrinter *)));
    previewDialog.exec();
}

/*
 * Show print dialog
 */
void
MainWindow::showPrintDialog()
{
#ifndef QT_NO_PRINTER
    QPrinter printer(QPrinter::ScreenResolution);
    QPrintDialog printDialog(&printer);
    if (printDialog.exec() == QDialog::Accepted) {
        ui.mSceneWidget->print(&printer);
    }
#endif
}

/*
 * Print the preview page
 */
void
MainWindow::printPreview(QPrinter *ipPrinter)
{
#ifdef QT_NO_PRINTER
    Q_UNUSED(ipPrinter);
#else
    ui.mSceneWidget->print(ipPrinter);
#endif
}

/*
 * Create status bar
 */
void
MainWindow::createStatusBar()
{
    QStatusBar *statusBar = new QStatusBar();

    statusBar->setFixedHeight(STATUS_BAR_DEFAULT_HEIGHT);
    statusBar->setFixedWidth(STATUS_BAR_DEFAULT_WIDTH);

    // create progressBar
    mProgressBar = new QProgressBar();

    statusBar->addWidget(mProgressBar);

    setStatusBar(statusBar);
}

/*
 * Make docked table list (in)visible
 */
void
MainWindow::setDockTableListVisible(bool ipFlag)
{
    if (ipFlag) {
        ui.mDockTableListWidget->show();
    } else {
        ui.mDockTableListWidget->hide();
    }
}

/*
 * Make docked log panel (in)visible
 */
void
MainWindow::setDockLogPanelVisible(bool ipFlag)
{
    if (ipFlag) {
        ui.mDockLogPanelWidget->show();
    } else {
        ui.mDockLogPanelWidget->hide();
    }
}
/*
 * Add table to scene
 */
void
MainWindow::addTableItem()
{
    QTreeWidgetItemIterator treeIter(ui.mTree);

    while (*treeIter) {
        if ((*treeIter)->isSelected()) {
            ui.mSceneWidget->showOnScene((*treeIter), TreeWidget::NameCol);
        }
        ++treeIter;
    }

    ui.mSceneWidget->updateLegend();
}

/*
 * Add table to scene
 */
void
MainWindow::addTableItem(QTreeWidgetItem *ipItem, int ipCol)
{
    //printMsg("Adding table '" + ipItem->text(ipCol) + "' to scene");

    ui.mSceneWidget->showOnScene(ipItem, ipCol);
    ui.mSceneWidget->updateLegend();
}

/*
 * Draw full db scheme
 */
void
MainWindow::drawFullDbScheme()
{
    int topLevelItemCount = ui.mTree->topLevelItemCount();

    for (int j = 0; j < topLevelItemCount; ++j) {
        QTreeWidgetItem *topLevelItem = ui.mTree->topLevelItem(j);

        int childrenCount = topLevelItem->childCount();

        // progress interaction
        //mProgressBar->setMaximum(topLevelItemsCount);
        //mProgressBar->setWindowModality(Qt::WindowModal);

        // go through children items
        for (int i = 0; i < childrenCount; ++i) {
            QTreeWidgetItem *childItem = topLevelItem->child(i);

            // if this is a schema item
            if (TreeWidget::SchemaItem == childItem->text(TreeWidget::IdCol).toInt()) {
                ui.mSceneWidget->showOnScene(childItem, TreeWidget::NameCol);
            }
            //mProgressBar->setValue(mProgressBar->value() + 1);
        }

        //mProgressBar->reset();
        statusBar()->showMessage(tr("Processing tales complete"), 3);
    }
}

/*
 * Do smth before mainwindows is closed
 */
void
MainWindow::closeEvent(QCloseEvent *ipEvent)
{
    /*! \todo If the connoection is not alive, then do not ask the confirmation
     *        For saving the session
     */

    /*! \todo Check if the connection is alive
     *        if Yes, do the rest, else - return
     */

    int result = QMessageBox::question(
            this,
            tr("Confirm exit"),
            tr("Do you want to save current session?"),
            QMessageBox::Yes,
            QMessageBox::No,
            QMessageBox::Cancel);

    //    int result = QMessageBox::question(
    //            this,
    //            tr("Confirm exit"),
    //            tr("Do you really want to exit?"),
    //            QMessageBox::Yes,
    //            QMessageBox::No);

    // if yes || no
    if (QMessageBox::Cancel != result) {

        if (QMessageBox::Yes == result) {
            // save parameters to xml file
            saveSession();
        }

        // get singleton instance and cleanup
        Database *dbInst = Database::instance();

        dbInst->resetData();

        DatabaseManager dbMngr;
        dbMngr.flush();

        ipEvent->accept();
    } else {
        ipEvent->ignore();
    }

}

/*
 * Describe selected database object
 */
void
MainWindow::describeObject()
{
    // get selected item
    QTreeWidgetItem *item = ui.mTree->currentItem();

    // if no item is selected
    if (0 == item) {
        return;
    }

    // get object id
    // !!!in fact it would be nice to use TreeWidget::Item/Node instead of int
    TreeWidget::Item objId = (TreeWidget::Item)item->text(TreeWidget::IdCol).toInt();

    // return if item is just a node
    if ( 0 == objId ) {
        return;
    }

    // get object name
    QString objName = item->text(TreeWidget::NameCol);
    QString tabTitle, schemaName;

    Database *dbInst = Database::instance();

    // create description widget
    DescriptionWidget *descWidget = new DescriptionWidget();

    DbSchema *schema;
    DbTable *table;
    DbRole *role;
    DbView *view;
    DbIndex *index;
    DbTrigger *trig;

    // find the correct object and describe
    switch ( objId ) {
        case TreeWidget::SchemaItem:
            schema = dbInst->findSchema(objName);

            // no check for null pointer, describe will handle it
            descWidget->describe(schema);
            tabTitle = tr("Schema: ");
            break;

        case TreeWidget::TableItem:
            // find table's schema
            schemaName = item->parent()->parent()->text(TreeWidget::NameCol);

            schema = dbInst->findSchema(schemaName);

            if ( schema ) {
                table = schema->findTable(objName);
                // no check for null pointer, describe will handle it
                descWidget->describe(table);
                tabTitle = tr("Table: ");
            }
            break;

        case TreeWidget::ViewItem:
            // find view's schema
            schemaName = item->parent()->parent()->text(TreeWidget::NameCol);

            schema = dbInst->findSchema(schemaName);

            if (schema) {
                view = schema->findView(objName);
                // no check for null pointer, describe will handle it
                descWidget->describe(view);
                tabTitle = tr("View: ");
            }
            break;

        case TreeWidget::RoleItem:
            role = dbInst->findRole(objName);

            // no check for null pointer, describe will handle it
            descWidget->describe(role);
            tabTitle = tr("Role: ");
            break;

        case TreeWidget::IndexItem:
            index = dbInst->findIndex(objName);

            descWidget->describe(index);
            tabTitle = tr("Index: ");
            break;

        case TreeWidget::TriggerItem:
            // find view's schema
            schemaName = item->parent()->parent()->text(TreeWidget::NameCol);

            schema = dbInst->findSchema(schemaName);

            if (schema) {
                trig = schema->findTrigger(objName);
                // no check for null pointer, describe will handle it

                descWidget->describe(trig);
                tabTitle = tr("Trigger: ");
            }
            break;
        default:
            qDebug() << "MainWindow::describeObject> Unknown object!";
            return;
            break;
    }

    // construct the whole tab title
    tabTitle.append(objName);
    // add description widget to tab widget
    ui.mTabWidget->addTab(descWidget, tabTitle);

    bool switchToNewTab = mSettings.value(Consts::PREFS_GRP + "/" + Consts::NEW_TAB_AUTO_SWITCH_SETTING).toBool();

    // if auto switch enabled
    if (switchToNewTab) {
        // activate last tab
        ui.mTabWidget->setCurrentIndex(ui.mTabWidget->count() - 1);
    }
}

/*
 *
 */
void
MainWindow::queryData()
{
    // get selected item
    QTreeWidgetItem *item = ui.mTree->currentItem();

    // if no item is selected
    if (0 == item) {
        return;
    }

    // get object id
    int objId = item->text(TreeWidget::IdCol).toInt();

    // return if item is just a node
    if (0 == objId) {
        return;
    }

    // get object name
    QString objName = item->text(TreeWidget::NameCol);
    QString tabTitle = QString("SQL: ");
    QString schemaName;

    Database *dbInst = Database::instance();

    // create sql widget
    SqlWidget *sqlWidget = new SqlWidget();

    DbSchema *schema;

    // find the correct object and query data
    switch ( objId ) {
        case TreeWidget::TableItem:
        case TreeWidget::ViewItem:
            // find schema
            schemaName = item->parent()->parent()->text(TreeWidget::NameCol);

            schema = dbInst->findSchema(schemaName);

            if ( schema ) {
                sqlWidget->setDefaultQuery(
                        QString("SELECT t.* FROM %1.%2 t;")
                        .arg(schemaName)
                        .arg(objName));
            }
            break;

        default:
            qDebug() << "MainWindow::queryData> Unknown object!";
            return;
            break;
    }

    // construct the whole tab title
    tabTitle.append(objName);
    // add description widget to tab widget
    ui.mTabWidget->addTab(sqlWidget, tabTitle);

    bool switchToNewTab = mSettings.value(Consts::PREFS_GRP + "/" + Consts::NEW_TAB_AUTO_SWITCH_SETTING, true).toBool();

    // if auto switch enabled
    if ( switchToNewTab ) {
        // activate last tab
        ui.mTabWidget->setCurrentIndex(ui.mTabWidget->count() - 1);
    }
}

/*
 * Save all parameters to xml file
 */
void
MainWindow::saveToXml(QString ipFileName)
{
    QDomDocument doc("VisualDB");
    QDomElement root = doc.createElement("visual-db");
    doc.appendChild(root);
    root.appendChild(mDbParameters->toXml(doc));
    root.appendChild(mProxyParameters->toXml(doc));
    root.appendChild(ui.mSceneWidget->toXml(doc, ui.mShowGridAction->isChecked(), ui.mDivideIntoPagesAction->isChecked(),
                ui.mShowLegendAction->isChecked(), ui.mShowControlWidgetAction->isChecked()));

    QFile file(ipFileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox messageBox;
        messageBox.setText("Can't open file");
        messageBox.exec();
    }

    QTextStream stream(&file);
    stream << doc.toString();

    file.close();
}

/*
 * Load parameters from xml file
 */
void
MainWindow::loadFromXml(QString ipFileName)
{
    QDomDocument doc("VisualDB");
    QFile file(ipFileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox messageBox;
        messageBox.setText("Can't open file");
        messageBox.exec();
        return;
    }
    if (!doc.setContent(&file)) {
        file.close();
        return;
    }
    file.close();

    QDomElement docElem = doc.documentElement();
    QDomNode child = docElem.firstChild();
    while (!child.isNull()) {
        QDomElement element = child.toElement(); // try to convert the node to an element.
        if (!element.isNull()) {
            if (element.tagName() == "database") {
                mDbParameters->fromXml(element);
            } else if (element.tagName() == "proxy") {
                mProxyParameters->fromXml(element);
            }
        }
        child = child.nextSibling();
    }

    // show connection dialog and check if we haven't pressed 'Cancel' button
    if (showConnectionDialog(true) == QDialog::Accepted) {
        // second loop to fill in schema from the xml (we need to connect to database first)
        docElem = doc.documentElement();
        child = docElem.firstChild();
        while (!child.isNull()) {
            QDomElement element = child.toElement();
            if (element.tagName() == "scene") {
                ui.mSceneWidget->fromXml(element);
                break;
            }
            child = child.nextSibling();
        }
    }
}

/*
 * Save session
 */
void
MainWindow::saveSession()
{
    using namespace Consts;
    QString defaultFileName = "session_" +
        /*! \todo Fix these hard coded strings approach */
        mSettings.value(LAST_SESSION_GRP + "/" + DB_NAME_SETTING, "undefined").toString() + "_" +
        mSettings.value(LAST_SESSION_GRP + "/" + DB_USER_SETTING, "undefined").toString() + "_" +
        QDate::currentDate().toString(Qt::DefaultLocaleShortDate) + "_" + QTime::currentTime().toString() + ".vdb";
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save session..."),
            mSettings.value(PREFS_GRP + "/" + SESSION_DIR_SETTING, "./").toString() + defaultFileName,
            tr("Xml files (*.vdb)"));
    // return if we don't select any file to save
    if (fileName == "") {
        return;
    }

    for (int i = 9; i > 0; --i) {
        if (mSettings.contains(LAST_SESSION_GRP + "/" + SAVED_SESSION_SETTING + QString().setNum(i - 1))) {
            mSettings.setValue(LAST_SESSION_GRP + "/" + SAVED_SESSION_SETTING + QString().setNum(i), mSettings.value(LAST_SESSION_GRP + "/" + SAVED_SESSION_SETTING + QString().setNum(i - 1)));
        }
    }

    mSettings.setValue(LAST_SESSION_GRP + "/" + SAVED_SESSION_SETTING + "0", fileName);
    updateSessionMenu();
    saveToXml(fileName);
}

/*
 * Load session
 */
void
MainWindow::loadSession()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open session..."),
            mSettings.value(Consts::PREFS_GRP + "/" + Consts::SESSION_DIR_SETTING, "./").toString(),
            tr("Xml files (*.vdb)"));
    if (!QFile::exists(fileName)) {
        QMessageBox messageBox;
        messageBox.setText("File doesn't exists");
        messageBox.exec();
        //  qDebug() << "File doesn't exists";
        return;
    }

    loadFromXml(fileName);
}

/*
 * Load last session
 */
void
MainWindow::loadLastSession()
{
    QAction *action = dynamic_cast<QAction *>(QObject::sender());
    loadFromXml(action->text());
}

/*
 * Show window with full screen or not
 */
void
MainWindow::setFullScreen(bool ipFlag)
{
    if (ipFlag) {
        setWindowState(Qt::WindowFullScreen);
    } else {
        setWindowState(Qt::WindowMaximized);
    }
}

/*
 * Reload data from DB
 */
void
MainWindow::reloadData()
{
    ui.mSceneWidget->cleanTableSchemeScene();
    Database::instance()->resetData();
    ui.mTree->refresh();
}

/*
 * Initialize session
 */
void
MainWindow::initSession()
{
    QString firstSavedSession = Consts::LAST_SESSION_GRP + "/" + Consts::SAVED_SESSION_SETTING + "0";
    // if checked flag in appearance page - load last session
    if (mSettings.value(Consts::PREFS_GRP + "/" + Consts::LOAD_LAST_SESSION_SETTING, false).toBool()) {
        if (mSettings.contains(firstSavedSession)) {
            loadFromXml(mSettings.value(firstSavedSession).toString());
        } else {
            qDebug() << "Last session not found";
        }
    }
}

/*
 * Move table slot
 */
void
MainWindow::addCommand(QUndoCommand *ipCommand)
{
    mUndoStack->push(ipCommand);
}

/*
 * [static]
 * Prints the message to the log panel
 */
void
MainWindow::printMsg(QString ipText) const
{
    qDebug() << ui.mLogPanel;
    ui.mLogPanel->print(ipText);
}


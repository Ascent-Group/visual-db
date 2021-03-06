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
#include <QFutureWatcher>
#include <QMessageBox>
#include <QPainter>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QPrinter>
#include <QProgressBar>
#include <QProgressDialog>
#include <QSqlDatabase>
#include <QStatusBar>
#include <QtConcurrentRun>
#include <QTime>
#include <QUndoCommand>
#include <QUndoStack>
#include <common/Database.h>
#include <connect/DbParameters.h>
#include <connect/ProxyParameters.h>
#include <consts.h>
#include <gui/DescriptionWidget.h>
#include <gui/Legend.h>
#include <gui/MainWindow.h>
#include <gui/OptionsDialog.h>
#include <gui/SceneWidget.h>
#include <gui/SqlConnectionDialog.h>
#include <gui/SqlWidget.h>
#include <gui/TabWidget.h>
#include <gui/TreeWidget.h>

#include <QtDebug>

using namespace QtConcurrent;

/*!
 * Constructor
 */
MainWindow::MainWindow()
    : QMainWindow(),
      mTreeItemMenu(0),
      mProgressBar(0),
      mDbParameters(0),
      mProxyParameters(0),
      mUndoStack(0)
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
    connect(ui.mLogPanelWidget, SIGNAL(closed()), this, SLOT(closeLogPanel()));
    connect(ui.mDatabaseTreeWidget, SIGNAL(closed()), this, SLOT(closeDatabaseTree()));
    connect(ui.mLogPanelWidget, SIGNAL(shown()), this, SLOT(showLogPanel()));
    connect(ui.mDatabaseTreeWidget, SIGNAL(shown()), this, SLOT(showDatabaseTree()));
}

/*!
 * Destructor
 */
MainWindow::~MainWindow()
{
    delete mTreeItemMenu;
    delete mDbParameters;
    delete mProxyParameters;
    delete mProgressBar;
    delete mUndoStack;
    // \note We have to delete it here because somehow the parent is not set for scene
    // widget in setupUi. If don't do this then graphics scene that is inside scene widget
    // won't get deleted.
    delete ui.mSceneWidget;
}

/*!
 * \brief Create actions
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

/*!
 * \brief Create menus
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

/*!
 * \brief Update session menu
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

/*!
 * \brief Enable or disable some menu actions
 *
 * \param[in] ipFlag - True if we want to enable actions, false otherwise
 */
void
MainWindow::setEnableForActions(bool ipFlag)
{
    ui.mSaveToImgAction->setEnabled(ipFlag);
    ui.mDrawFullDBAction->setEnabled(ipFlag);

    ui.mSelectAllTablesAction->setEnabled(ipFlag);
    ui.mRemoveAllTablesAction->setEnabled(ipFlag);
    ui.mShowLegendAction->setEnabled(ipFlag);
    ui.mAlignToGridAction->setEnabled(ipFlag);
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

/*!
 * \brief Import database
 *
 * \param[in] ui - UI of main window
 *
 * \return Result of dialog processing
 */
int
importDatabase(const Ui::MainWindow &ui)
{
    if (QSqlDatabase::database("mainConnect").open()) {
        return QDialog::Accepted;
    }

    return QDialog::Rejected;
}

/*!
 * \brief Show connection dialog
 *
 * \param[in] ipLoadSession - True if we want to load session parameters, false otherwise
 *
 * \return Result of dialog processing
 */
int
MainWindow::showConnectionDialog(bool ipLoadSession)
{
    SqlConnectionDialog connDialog(mDbParameters, mProxyParameters, ipLoadSession);

    while (connDialog.connectionFailed()) {
        // nothing to do if canceled
        if (QDialog::Rejected == connDialog.exec()) {
            return QDialog::Rejected;
        }
    }

    QProgressDialog progress("Importing database...", 0, 0, 0, this, Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    progress.setRange(0, 0);
    progress.setWindowModality(Qt::WindowModal);

    QFutureWatcher<int> futureWatcher;
    connect(&futureWatcher, SIGNAL(finished()), &progress, SLOT(reset()));
    connect(&progress, SIGNAL(canceled()), &futureWatcher, SLOT(cancel()));

    futureWatcher.setFuture(QtConcurrent::run(importDatabase, ui));

    progress.exec();

    futureWatcher.waitForFinished();

    if (futureWatcher.isCanceled()) {
        ui.mTree->clear();
        ui.mTree->setHeaderLabel(QString(""));
        return QDialog::Rejected;
    }

    if (futureWatcher.result() == QDialog::Accepted) {
        ui.mSceneWidget->cleanSchemeScene();
        Database::instance()->resetData();
        ui.mTree->refresh();
        ui.mSceneWidget->refreshLegend();
        setEnableForActions(true);
    }

    return futureWatcher.result();
}

/*!
 * \brief Show options dialog
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

/*!
 * \brief Show print preview dialog
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

/*!
 * \brief Show print dialog
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

/*!
 * \brief Print the preview page
 *
 * \param[in] ipPrinter - Printer
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

/*!
 * \brief Create status bar
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

/*!
 * \brief Make docked table list (in)visible
 *
 * \param[in] ipFlag - True if we want to show tree, false otherwise
 */
void
MainWindow::showDatabaseTree(bool ipFlag)
{
    if (ipFlag) {
        ui.mDatabaseTreeWidget->show();
    } else {
        ui.mDatabaseTreeWidget->hide();
    }
}

/*!
 * \brief Close database tree window
 */
void
MainWindow::closeDatabaseTree()
{
    ui.mShowTableListAction->setChecked(false);
}

/*!
 * \brief Show database tree view
 */
void
MainWindow::showDatabaseTree()
{
    ui.mShowTableListAction->setChecked(true);
}

/*!
 * \brief Make docked log panel (in)visible
 *
 * \param[in] ipFlag - True if we want to show log panel, false otherwise
 */
void
MainWindow::showLogPanel(bool ipFlag)
{
    if (ipFlag) {
        ui.mLogPanelWidget->show();
    } else {
        ui.mLogPanelWidget->hide();
    }
}

/*!
 * \brief Close log panel
 */
void
MainWindow::closeLogPanel()
{
    ui.mShowLogPanelAction->setChecked(false);
}

/*!
 * \brief Show log panel
 */
void
MainWindow::showLogPanel()
{
    ui.mShowLogPanelAction->setChecked(true);
}

/*!
 * \brief Add table to scene
 */
void
MainWindow::addItem()
{
    QTreeWidgetItemIterator treeIter(ui.mTree);

    while (*treeIter) {
        if ((*treeIter)->isSelected()) {
            ui.mSceneWidget->showOnScene((*treeIter), TreeWidget::NameCol, true);
        }
        ++treeIter;
    }

    ui.mSceneWidget->updateLegend();
}

/*!
 * \brief Show the item on the scene when we double click on it
 *
 * \param[in] ipItem - Item we clicked on
 * \param[in] ipColumn - Column of the item
 */
void
MainWindow::addItem(QTreeWidgetItem *ipItem, int ipColumn)
{
    ui.mSceneWidget->showOnScene(ipItem, ipColumn, true);
}


/*!
 * \brief Draw full db scheme
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
        statusBar()->showMessage(tr("Processing tables complete"), 3);
    }
}

/*!
 * \brief Do smth before mainwindows is closed
 *
 * \param[in] ipEvent - Close event
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

    do {
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
                if (!saveSession()) {
                    continue;
                }
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
        break;
    }
    while (true);
}

/*!
 * \brief Describe selected database object
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

    DbSchemaPtr schema;
    DbTablePtr table;
    DbRolePtr role;
    DbViewPtr view;
    DbIndexPtr index;
    DbTriggerPtr trig;
    DbProcedurePtr proc;

    // find the correct object and describe
    switch ( objId ) {
        case TreeWidget::SchemaItem:
            descWidget->describe(dbInst->findSchema(objName));
            tabTitle = tr("Schema: ");
            break;

        case TreeWidget::TableItem:
            // find table's schema
            schemaName = item->parent()->parent()->text(TreeWidget::NameCol);

            schema = dbInst->findSchema(schemaName);

            if (schema.valid()) {
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

            if (schema.valid()) {
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

            if (schema.valid()) {
                trig = schema->findTrigger(objName);

                // no check for null pointer, describe will handle it
                descWidget->describe(trig);
                tabTitle = tr("Trigger: ");
            }
            break;

        case TreeWidget::ProcedureItem:
            schemaName = item->parent()->parent()->text(TreeWidget::NameCol);
            schema = dbInst->findSchema(schemaName);
            if (schema.valid()) {
                proc = schema->findProcedure(objName);

                descWidget->describe(proc);
                tabTitle = tr("Procedure: ");
            }
            break;

        default:
            qDebug() << "MainWindow::describeObject> Unknown object!";
            delete descWidget;
            return;
            break;
    }

    // construct the whole tab title
    tabTitle.append(objName);
    // add description widget to tab widget
    ui.mTabWidget->addTab(descWidget, tabTitle);

    bool switchToNewTab = mSettings.value(Consts::PREFS_GRP + "/" + Consts::NEW_TAB_AUTO_SWITCH_SETTING, true).toBool();

    // if auto switch enabled
    if (switchToNewTab) {
        // activate last tab
        ui.mTabWidget->setCurrentIndex(ui.mTabWidget->count() - 1);
    }
}

/*!
 * \brief Show query dialog tab
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

    DbSchemaPtr schema;

    // find the correct object and query data
    switch (objId) {
        case TreeWidget::TableItem:
        case TreeWidget::ViewItem:
            // find schema
            schemaName = item->parent()->parent()->text(TreeWidget::NameCol);

            schema = dbInst->findSchema(schemaName);

            if (schema.get()) {
                sqlWidget->setDefaultQuery(
                        QString("SELECT t.* FROM %1.%2 t;")
                        .arg(schemaName)
                        .arg(objName));
            }
            break;

        default:
            qDebug() << "MainWindow::queryData> Unknown object!";
            delete sqlWidget;
            return;
            break;
    }

    // construct the whole tab title
    tabTitle.append(objName);
    // add description widget to tab widget
    ui.mTabWidget->addTab(sqlWidget, tabTitle);

    bool switchToNewTab = mSettings.value(Consts::PREFS_GRP + "/" + Consts::NEW_TAB_AUTO_SWITCH_SETTING, true).toBool();

    // if auto switch enabled
    if (switchToNewTab) {
        // activate last tab
        ui.mTabWidget->setCurrentIndex(ui.mTabWidget->count() - 1);
    }
}

/*!
 * \brief Save all parameters to xml file
 *
 * \param[in] ipFileName - Xml file name were we will store parameters to
 */
void
MainWindow::saveToXml(const QString &ipFileName)
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

/*!
 * \brief Load parameters from xml file
 *
 * \param[in] ipFileName - Xml file name were load parameter from
 */
void
MainWindow::loadFromXml(QString ipFileName)
{
    QFile file(ipFileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox messageBox;
        messageBox.setText("Can't open file");
        messageBox.exec();
        return;
    }

    QDomDocument doc("VisualDB");
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
    if (showConnectionDialog(true/*ipLoadSession*/) == QDialog::Accepted) {
        // second loop to fill in schema from the xml (we need to connect to database first)
        docElem = doc.documentElement();
        child = docElem.firstChild();
        while (!child.isNull()) {
            QDomElement element = child.toElement();
            if (element.tagName() == "scene") {
                ui.mSceneWidget->fromXml(element);
                ui.mShowLegendAction->setChecked(element.attribute("legend").toInt());
                ui.mShowGridAction->setChecked(element.attribute("grid").toInt());
                ui.mDivideIntoPagesAction->setChecked(element.attribute("divideIntoPages").toInt());
                ui.mShowControlWidgetAction->setChecked(element.attribute("controlWidget").toInt());
                break;
            }
            child = child.nextSibling();
        }
    }
}

/*!
 * \brief Save session
 *
 * \return True if we save was successfull, false otherwise
 */
bool
MainWindow::saveSession()
{
    using namespace Consts;
//    QString defaultFileName = "session_" +
//        mSettings.value(LAST_SESSION_GRP + "/" + DB_NAME_SETTING, "undefined").toString() + "_" +
//        mSettings.value(LAST_SESSION_GRP + "/" + DB_USER_SETTING, "undefined").toString() + "_" +
//        QDate::currentDate().toString(Qt::DefaultLocaleShortDate) + "_" + QTime::currentTime().toString() + ".vdb";
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save session..."), mSettings.value(PREFS_GRP + "/" + SESSION_DIR_SETTING, "./").toString(), tr("Session files (*.vdb)"));
//    QString fileName = QFileDialog::getSaveFileName(this, tr("Save session..."),
//            mSettings.value(PREFS_GRP + "/" + SESSION_DIR_SETTING, "./").toString() + defaultFileName,
//            tr("Xml files (*.vdb)"));
    // return if we don't select any file to save
    if (fileName == "") {
        return false;
    }

    for (int i = 9; i > 0; --i) {
        if (mSettings.contains(LAST_SESSION_GRP + "/" + SAVED_SESSION_SETTING + QString().setNum(i - 1))) {
            mSettings.setValue(LAST_SESSION_GRP + "/" + SAVED_SESSION_SETTING + QString().setNum(i), mSettings.value(LAST_SESSION_GRP + "/" + SAVED_SESSION_SETTING + QString().setNum(i - 1)));
        }
    }

    mSettings.setValue(LAST_SESSION_GRP + "/" + SAVED_SESSION_SETTING + "0", fileName);
    updateSessionMenu();
    saveToXml(fileName);

    return true;
}

/*!
 * \brief Load session.
 *
 * This function is called when we decide to load a session from a File... menu.
 */
void
MainWindow::loadSession()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open session..."),
            mSettings.value(Consts::PREFS_GRP + "/" + Consts::SESSION_DIR_SETTING, "./").toString(),
            tr("Session files (*.vdb)"));
    if (!QFile::exists(fileName)) {
        QMessageBox messageBox;
        messageBox.setText(tr("File doesn't exists"));
        messageBox.exec();
        //  qDebug() << "File doesn't exists";
        return;
    }

    loadFromXml(fileName);
}

/*!
 * \brief Load last session
 *
 * Called when we are loading recent session. The number of recent sessions is limited by
 * the parameter "Session limit:" in the options dialog.
 */
void
MainWindow::loadLastSession()
{
    QAction *action = dynamic_cast<QAction *>(QObject::sender());
    loadFromXml(action->text());
}

/*!
 * \brief Show window with full screen or not
 *
 * \param[in] ipFlag - True if we want to show in fullscreen, false otherwise
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

/*!
 * \brief Reload data from DB
 */
void
MainWindow::reloadData()
{
    ui.mSceneWidget->cleanSchemeScene();
    Database::instance()->resetData();
    ui.mTree->refresh();

    // flush scene cache
    mUndoStack->clear();
    ui.mSceneWidget->flushCache();
}

/*!
 * \brief Initialize session
 */
void
MainWindow::initSession()
{
    // if checked flag in appearance page - load last session
    if (mSettings.value(Consts::PREFS_GRP + "/" + Consts::LOAD_LAST_SESSION_SETTING, false).toBool()) {
        QString firstSavedSession = Consts::LAST_SESSION_GRP + "/" + Consts::SAVED_SESSION_SETTING + "0";
        if (mSettings.contains(firstSavedSession)) {
            loadFromXml(mSettings.value(firstSavedSession).toString());
        } else {
            qDebug() << "Last session not found";
        }
    }
}

/*!
 * \brief Add command to the stack
 */
void
MainWindow::addCommand(QUndoCommand *ipCommand)
{
    mUndoStack->push(ipCommand);
}

/*!
 * \brief Prints the message to the log panel
 *
 * \param[in] ipText - Message text
 */
void
MainWindow::printMsg(const QString &ipText) const
{
    qDebug() << ui.mLogPanel;
    ui.mLogPanel->print(ipText);
}


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
#include <QDockWidget>
#include <QDomDocument>
#include <QDomElement>
#include <QFileDialog>
#include <QMenu>
#include <QMenuBar>
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
#include <QToolBar>
#include <QTreeWidgetItem>
#include <QUndoCommand>
#include <QUndoStack>
#include <common/Database.h>
#include <common/DbSchema.h>
#include <connect/DbParameters.h>
#include <connect/ProxyParameters.h>
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
//    mScene = new GraphicsScene();
    mSceneWidget = new SceneWidget();

    // create menus
    createMenus();

    // create tree
    mTree = new TreeWidget(mTreeItemMenu);
    mTree->setExpandsOnDoubleClick(false);

    createToolBar();
    createStatusBar();
    createDockWindows();

    // create tab widget with graphics view on first tab
    mTabWidget = new TabWidget();

    mTabWidget->addTab(mSceneWidget, tr("DB scheme"));
    setCentralWidget(mTabWidget);

    setWindowTitle(tr("Visual PostgreSQL editor"));
    setMinimumSize(800, 600);
    setAttribute(Qt::WA_DeleteOnClose);

    mDbParameters = new DbParameters();
    mProxyParameters = new ProxyParameters();

    connect(mTree, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)),
	    this, SLOT(addTableItem(QTreeWidgetItem *, int)));

    setWindowState(Qt::WindowMaximized);

    connect(mSceneWidget, SIGNAL(tableActionDone(QUndoCommand *)),
	    this, SLOT(addCommand(QUndoCommand *)));

    initSession();
}

/*
 * Create actions
 */
void MainWindow::createActions()
{
    // exit
    mExitAction = new QAction(tr("Quit"), this);
    mExitAction->setShortcut(QString("Ctrl+Q"));
    mExitAction->setIcon(QIcon(":/img/exit.png"));
    mExitAction->setStatusTip(tr("Quit"));
    connect(mExitAction, SIGNAL(triggered()), this, SLOT(close()));

    // new connection
    mNewConnectionAction = new QAction(tr("New connection..."), this);
    mNewConnectionAction->setShortcut(QString("Ctrl+N"));
    mNewConnectionAction->setIcon(QIcon(":/img/connect_creating.png"));
    mNewConnectionAction->setStatusTip(tr("Establishing of new connection"));
    connect(mNewConnectionAction, SIGNAL(triggered()), this, SLOT(showConnectionDialog()));

    // show options dialog
    mShowOptionsDialogAction = new QAction(tr("Options..."), this);
    mShowOptionsDialogAction->setShortcut(QString("Ctrl+O"));
    mShowOptionsDialogAction->setIcon(QIcon(":/img/configure.png"));
    mShowOptionsDialogAction->setStatusTip(tr("Edit options"));
    connect(mShowOptionsDialogAction, SIGNAL(triggered()), this, SLOT(showOptionsDialog()));

    // show connection dialog
    mShowDockTablesListAction = new QAction(tr("Show tables list"), this);
    mShowDockTablesListAction->setCheckable(true);
    mShowDockTablesListAction->setChecked(true);
    connect(mShowDockTablesListAction, SIGNAL(toggled(bool)), this, SLOT(setDockTableListVisible(bool)));

    // draw full database (all tables by one click)
    mDrawFullDBAction = new QAction(tr("Draw full DB"), this);
    mDrawFullDBAction->setIcon(QIcon(":/img/pencil.png"));
    connect(mDrawFullDBAction, SIGNAL(triggered()), this, SLOT(drawFullDbScheme()));

    // show the preview page of the diagram
    mShowPrintPreviewDialogAction = new QAction(tr("Show preview..."), this);
    connect(mShowPrintPreviewDialogAction, SIGNAL(triggered()), this, SLOT(showPrintPreviewDialog()));

    // show the print dialog
    mShowPrintDialogAction = new QAction(tr("Print..."), this);
    connect(mShowPrintDialogAction, SIGNAL(triggered()), this, SLOT(showPrintDialog()));

    // undo stack
    mUndoStack = new QUndoStack();

    // undo action
    mUndoAction = mUndoStack->createUndoAction(this, tr("Undo"));
    mUndoAction->setIcon(QIcon(":/img/undo.png"));
    mUndoAction->setShortcut(QString("Ctrl+Z"));
    
    // redo action
    mRedoAction = mUndoStack->createRedoAction(this, tr("Redo"));
    mRedoAction->setIcon(QIcon(":/img/redo.png"));
    mRedoAction->setShortcut(QString("Ctrl+Y"));

    // add table action
    mAddTableAction = new QAction(tr("Add"), this);
    mAddTableAction->setStatusTip(tr("Add"));
    connect(mAddTableAction, SIGNAL(triggered()), this, SLOT(addTableItem()));

    // delete table action
    mDeleteTableAction = new QAction(tr("Delete"), this);
    mDeleteTableAction->setShortcut(QString("Delete"));
    mDeleteTableAction->setIcon(QIcon(":/img/remove.png"));
    mDeleteTableAction->setStatusTip(tr("Delete item from the scheme"));
    connect(mDeleteTableAction, SIGNAL(triggered()), mSceneWidget, SLOT(deleteTableItem()));

    // show fields' types
    mShowFieldsTypesAction = new QAction(tr("Show fields' types"), this);
    mShowFieldsTypesAction->setIcon(QIcon(":/img/show.png"));
    mShowFieldsTypesAction->setStatusTip(tr("Show fields' types"));
    connect(mShowFieldsTypesAction, SIGNAL(triggered()), mSceneWidget, SLOT(setFieldsTypesVisible()));

    // hide fields' types
    mHideFieldsTypesAction = new QAction(tr("Hide fields' types"), this);
    mHideFieldsTypesAction->setIcon(QIcon(":/img/hide.png"));
    mHideFieldsTypesAction->setStatusTip(tr("Hide fields' types"));
    connect(mHideFieldsTypesAction, SIGNAL(triggered()), mSceneWidget, SLOT(setFieldsTypesInvisible()));

    // show/hide foreign keys
    mShowIndicesAction = new QAction(tr("Show indices"), this);
    mShowIndicesAction->setStatusTip(tr("Show indices"));
    connect(mShowIndicesAction, SIGNAL(triggered()), mSceneWidget, SLOT(setIndicesVisible()));

    // show/hide foreign keys
    mHideIndicesAction = new QAction(tr("Hide indices"), this);
    mHideIndicesAction->setStatusTip(tr("Hide indices"));
    connect(mHideIndicesAction, SIGNAL(triggered()), mSceneWidget, SLOT(setIndicesInvisible()));

    // set table color
    mSetTableColorAction = new QAction(tr("Table color..."), this);
    mSetTableColorAction->setIcon(QIcon(":/img/colors.png"));
    mSetTableColorAction->setStatusTip(tr("Set table color"));
    connect(mSetTableColorAction, SIGNAL(triggered()), mSceneWidget, SLOT(setTableColor()));

    // select all tables
    mSelectAllTablesAction = new QAction(tr("Select all tables"), this);
    mSelectAllTablesAction->setShortcut(QString("Ctrl+A"));
    mSelectAllTablesAction->setStatusTip(tr("Select all tables"));
    connect(mSelectAllTablesAction, SIGNAL(triggered()), mSceneWidget, SLOT(selectAllTables()));

    // remove all tables
    mRemoveAllTablesAction = new QAction(tr("Remove all tables"), this);
    mRemoveAllTablesAction->setIcon(QIcon(":/img/eraser.png"));
    mRemoveAllTablesAction->setStatusTip(tr("Remove all tables from scheme"));
    connect(mRemoveAllTablesAction, SIGNAL(triggered()), mSceneWidget, SLOT(cleanTableSchemeScene()));

    // adjust all selected tables
    mAdjustTableSizeAction = new QAction(tr("Adjust"), this);
    mAdjustTableSizeAction->setShortcut(QString("Ctrl+J"));
    mAdjustTableSizeAction->setIcon(QIcon(":/img/adjustsize.png"));
    mAdjustTableSizeAction->setStatusTip(tr("Select all tables"));
    connect(mAdjustTableSizeAction, SIGNAL(triggered()), mSceneWidget, SLOT(adjustTables()));

    // save scheme to image
    mSaveToImgAction = new QAction(tr("Save to image..."), this);
    mSaveToImgAction->setIcon(QIcon(":/img/filesave.png"));
    mSaveToImgAction->setStatusTip(tr("Save scheme to image"));
    mSaveToImgAction->setShortcut(QString("Ctrl+S"));
    connect(mSaveToImgAction, SIGNAL(triggered()), mSceneWidget, SLOT(saveToImage()));

    // group items
    mGroupItemsAction = new QAction(tr("Group items"), this);
    mGroupItemsAction->setShortcut(QString("Ctrl+G"));
    mGroupItemsAction->setIcon(QIcon(":/img/group.png"));
    mGroupItemsAction->setStatusTip(tr("Group items"));
    connect(mGroupItemsAction, SIGNAL(triggered()), mSceneWidget, SLOT(groupItems()));

    // ungroup items
    mUngroupItemsAction = new QAction(tr("Ungroup items"), this);
    mUngroupItemsAction->setShortcut(QString("Ctrl+U"));
    mUngroupItemsAction->setIcon(QIcon(":/img/ungroup.png"));
    mUngroupItemsAction->setStatusTip(tr("Ungroup items"));
    connect(mUngroupItemsAction, SIGNAL(triggered()), mSceneWidget, SLOT(ungroupItems()));

    // anchor for tables
    mAnchorAction = new QAction(tr("Anchor"), this);
    mAnchorAction->setIcon(QIcon(":/img/anchor.png"));
    mAnchorAction->setStatusTip(tr("To anchor for selected tables"));
    connect(mAnchorAction, SIGNAL(triggered()), mSceneWidget, SLOT(anchorTables()));

    // weight anchor for tables
    mDisableAnchorAction = new QAction(tr("Disable anchor"), this);
    mDisableAnchorAction->setStatusTip("To weight anchor for selected tables");
    connect(mDisableAnchorAction, SIGNAL(triggered()), mSceneWidget, SLOT(weightAnchorTables()));

    // select all tables in schema
    mSelectAllTablesInSchemaAction = new QAction(tr("Select all tables in schema"), this);
    mSelectAllTablesInSchemaAction->setStatusTip(tr("Select all tables in schema"));
    connect(mSelectAllTablesInSchemaAction, SIGNAL(triggered()), mSceneWidget, SLOT(selectAllTablesInSchema()));

    // describe table in new tab
    mDescribeObjectAction = new QAction(tr("Describe"), this);
    mDescribeObjectAction->setStatusTip(tr("Get object description"));
    mDescribeObjectAction->setShortcut(QString("Alt+Enter"));
    connect(mDescribeObjectAction, SIGNAL(triggered()), this, SLOT(describeObject()));

    // query data in new tab
    mQueryDataAction = new QAction(tr("Query data"), this);
    mQueryDataAction->setStatusTip(tr("Run SQL command for data retrieving"));
    connect(mQueryDataAction, SIGNAL(triggered()), this, SLOT(queryData()));

    // colorize tables according schemas
    mColorizeAccordingSchemasAction = new QAction(tr("Colorize according schemas"), this);
    mColorizeAccordingSchemasAction->setStatusTip(tr("Colorize according schemas"));
    connect(mColorizeAccordingSchemasAction, SIGNAL(triggered()), mSceneWidget, SLOT(colorizeAccordingSchemas()));

    // show/hide grid
    mShowGridAction = new QAction(tr("Show grid"), this);
    mShowGridAction->setCheckable(true);
    mShowGridAction->setChecked(true);
    mShowGridAction->setStatusTip(tr("Show grid"));
    connect(mShowGridAction, SIGNAL(toggled(bool)), mSceneWidget, SLOT(showGrid(bool)));

    // attach tables to the grid
    mAttachToGridAction = new QAction(tr("Align to grid"), this);
    mAttachToGridAction->setCheckable(true);
    mAttachToGridAction->setChecked(mSettings.value("View/AttachToGrid", false).toBool());
    mAttachToGridAction->setStatusTip(tr("Align tables to the grid"));
    connect(mAttachToGridAction, SIGNAL(toggled(bool)), mSceneWidget, SLOT(attachToGrid(bool)));

    // show/hide grid
    mDivideOnPagesAction = new QAction(tr("Divide on pages"), this);
    mDivideOnPagesAction->setCheckable(true);
    mDivideOnPagesAction->setChecked(true);
    mDivideOnPagesAction->setStatusTip(tr("Divide on pages"));
    connect(mDivideOnPagesAction, SIGNAL(toggled(bool)), mSceneWidget, SLOT(divideOnPages(bool)));

    // show/hide control widget
    mShowControlWidgetAction = new QAction(tr("Show control widget"), this);
    mShowControlWidgetAction->setCheckable(true);
    mShowControlWidgetAction->setChecked(true);
    mShowControlWidgetAction->setStatusTip(tr("Show control widget"));
    connect(mShowControlWidgetAction, SIGNAL(toggled(bool)), mSceneWidget, SLOT(showControlWidget(bool)));

    // show/hide legend
    mShowLegendAction = new QAction(tr("Show legend"), this);
    mShowLegendAction->setCheckable(true);
    mShowLegendAction->setChecked(false);
    mShowLegendAction->setStatusTip(tr("Show legend"));
    connect(mShowLegendAction, SIGNAL(toggled(bool)), mSceneWidget, SLOT(showLegend(bool)));

    // save session
    mSaveSessionAction = new QAction(tr("Save session"), this);
    mSaveSessionAction->setStatusTip(tr("Save session"));
    connect(mSaveSessionAction, SIGNAL(triggered()), this, SLOT(saveSession()));

    // load session
    mLoadSessionAction = new QAction(tr("Load session"), this);
    mLoadSessionAction->setStatusTip(tr("Load session"));
    connect(mLoadSessionAction, SIGNAL(triggered()), this, SLOT(loadSession()));

    // set window to full screen
    mSetFullScreenAction = new QAction(tr("Full screen"), this);
    mSetFullScreenAction->setStatusTip(tr("Full screen"));
    mSetFullScreenAction->setShortcut(QString("Ctrl+F"));
    mSetFullScreenAction->setCheckable(true);
    mSetFullScreenAction->setChecked(false);
    connect(mSetFullScreenAction, SIGNAL(toggled(bool)), this, SLOT(setFullScreen(bool)));

    // reload data form db
    mReloadDataAction = new QAction(tr("Reload"), this);
    mReloadDataAction->setIcon(QIcon(":/img/reload.png"));
    mReloadDataAction->setStatusTip(tr("Reload"));
    mReloadDataAction->setShortcut(QString("Ctrl+R"));
    connect(mReloadDataAction, SIGNAL(triggered()), this, SLOT(reloadData()));
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

    // create menu bar
    mMenuBar = new QMenuBar();

    // session menu
    mSessionMenu = new QMenu(tr("Sessions"));
    updateSessionMenu();

    // file menu
    mFileMenu = new QMenu(tr("&File"));
    mFileMenu->addAction(mNewConnectionAction);
    mFileMenu->addSeparator();
    mFileMenu->addAction(mSaveToImgAction);
    mFileMenu->addSeparator();
    mFileMenu->addAction(mShowOptionsDialogAction);
    mFileMenu->addMenu(mSessionMenu);
    mFileMenu->addSeparator();
    mFileMenu->addAction(mShowPrintPreviewDialogAction);
    mFileMenu->addAction(mShowPrintDialogAction);
    mFileMenu->addSeparator();
    mFileMenu->addAction(mExitAction);
    mMenuBar->addMenu(mFileMenu);

    // edit menu
    mEditMenu = new QMenu(tr("Edit"));
    mEditMenu->addAction(mUndoAction);
    mEditMenu->addAction(mRedoAction);
    mMenuBar->addMenu(mEditMenu);

    // view menu
    mViewMenu = new QMenu(tr("&View"));
    mViewMenu->addAction(mSetFullScreenAction);
    mViewMenu->addAction(mShowDockTablesListAction);
    mMenuBar->addMenu(mViewMenu);

    // scheme menu
    mSchemeMenu = new QMenu(tr("&Scheme"));
    mSchemeMenu->addAction(mSelectAllTablesAction);
    mSchemeMenu->addAction(mRemoveAllTablesAction);
    mSchemeMenu->addAction(mShowGridAction);
    mSchemeMenu->addAction(mAttachToGridAction);
    mSchemeMenu->addAction(mDivideOnPagesAction);
    mSchemeMenu->addAction(mShowLegendAction);
    mSchemeMenu->addAction(mShowControlWidgetAction);
    mSchemeMenu->addAction(mColorizeAccordingSchemasAction);
    mMenuBar->addMenu(mSchemeMenu);

    // table menu
    mTableMenu = new QMenu(tr("&Table"));
    mTableMenu->addAction(mDeleteTableAction);
    mTableMenu->addAction(mShowFieldsTypesAction);
    mTableMenu->addAction(mHideFieldsTypesAction);
    mTableMenu->addAction(mShowIndicesAction);
    mTableMenu->addAction(mHideIndicesAction);
    mTableMenu->addAction(mSetTableColorAction);
    mTableMenu->addAction(mAdjustTableSizeAction);
    mTableMenu->addAction(mGroupItemsAction);
    mTableMenu->addAction(mUngroupItemsAction);
    mTableMenu->addAction(mAnchorAction);
    mTableMenu->addAction(mDisableAnchorAction);
    mTableMenu->addAction(mSelectAllTablesInSchemaAction);
    mMenuBar->addMenu(mTableMenu);

    // tree item menu
    mTreeItemMenu = new QMenu();
    mTreeItemMenu->addAction(mAddTableAction);
    mTreeItemMenu->addAction(mDescribeObjectAction);
    mTreeItemMenu->addAction(mQueryDataAction);

    setMenuBar(mMenuBar);

    mSceneWidget->setSchemeMenu(mSchemeMenu);
    mSceneWidget->setTableMenu(mTableMenu);
}

/*
 * Update session menu
 */
void
MainWindow::updateSessionMenu()
{
    mSessionMenu->clear();

    int countSavedSessions = mSettings.value("Preferences/CountSavedSessions", 10).toInt();
    for (int i = 0; i < countSavedSessions; ++i) {
	// update session menu
	if (mSettings.contains("LastSession/SavedSession" + QString().setNum(i))) {
	    // last session
	    mLastSessionAction = new QAction(mSettings.value("LastSession/SavedSession" + QString().setNum(i)).toString(), this);
	    connect(mLastSessionAction, SIGNAL(triggered()), this, SLOT(loadLastSession()));

	    mSessionMenu->addAction(mLastSessionAction);
	} else {
	    break;
	}
    }
    mSessionMenu->addSeparator();
    mSessionMenu->addAction(mLoadSessionAction);
    mSessionMenu->addAction(mSaveSessionAction);
}

/*
 * Enable or disable some menu actions
 */
void
MainWindow::setEnableForActions(bool ipFlag)
{
    mSaveToImgAction->setEnabled(ipFlag);
    mDrawFullDBAction->setEnabled(ipFlag);

    mSelectAllTablesAction->setEnabled(ipFlag);
    mRemoveAllTablesAction->setEnabled(ipFlag);
    mShowLegendAction->setEnabled(ipFlag);
    mColorizeAccordingSchemasAction->setEnabled(ipFlag);

    mDeleteTableAction->setEnabled(ipFlag);
    mShowFieldsTypesAction->setEnabled(ipFlag);
    mHideFieldsTypesAction->setEnabled(ipFlag);
    mSetTableColorAction->setEnabled(ipFlag);
    mAdjustTableSizeAction->setEnabled(ipFlag);
    mGroupItemsAction->setEnabled(ipFlag);
    mUngroupItemsAction->setEnabled(ipFlag);
    mDescribeObjectAction->setEnabled(ipFlag);
    mQueryDataAction->setEnabled(ipFlag);
    mShowIndicesAction->setEnabled(ipFlag);
    mHideIndicesAction->setEnabled(ipFlag);
    mSelectAllTablesInSchemaAction->setEnabled(ipFlag);

    mSaveSessionAction->setEnabled(ipFlag);

    mReloadDataAction->setEnabled(ipFlag);
}

/*
 * Create toolbar
 */
void
MainWindow::createToolBar()
{
    mToolBar = new QToolBar("Toolbar");

    mToolBar->addAction(mNewConnectionAction);
    mToolBar->addAction(mSaveToImgAction);
    mToolBar->addSeparator();

    mToolBar->addAction(mReloadDataAction);
    mToolBar->addAction(mDrawFullDBAction);
    mToolBar->addSeparator();

    mToolBar->addAction(mRemoveAllTablesAction);

    addToolBar(mToolBar);
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
	mSceneWidget->cleanTableSchemeScene();
	mTree->refresh();
	mSceneWidget->refreshLegend();
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
	mSceneWidget->print(&printer);
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
    mSceneWidget->print(ipPrinter);
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
 * Create docks
 */
void
MainWindow::createDockWindows()
{
    mDockTableListWidget = new QDockWidget(tr("Tables list"), this);
//    mDockTableListWidget->setAttribute(Qt::WA_DeleteOnClose);
    mDockTableListWidget->setFeatures(QDockWidget::DockWidgetMovable |
                                      QDockWidget::DockWidgetFloatable);
    mDockTableListWidget->setAllowedAreas(Qt::LeftDockWidgetArea |
	    Qt::RightDockWidgetArea);
    mDockTableListWidget->setWidget(mTree);
    addDockWidget(Qt::LeftDockWidgetArea, mDockTableListWidget);
}

/*
 * Make docked table list (in)visible
 */
void
MainWindow::setDockTableListVisible(bool ipFlag)
{
    if (ipFlag) {
	mDockTableListWidget->show();
    } else {
	mDockTableListWidget->hide();
    }
}

/*
 * Add table to scene
 */
void
MainWindow::addTableItem() 
{
    QTreeWidgetItemIterator treeIter(mTree);

    while (*treeIter) {
        if ((*treeIter)->isSelected()) {
            mSceneWidget->showOnScene((*treeIter), TreeWidget::NameCol);
        }
        ++treeIter;
    }

    mSceneWidget->updateLegend();
}

/*
 * Add table to scene
 */
void
MainWindow::addTableItem(QTreeWidgetItem *ipItem, int ipCol)
{
    mSceneWidget->showOnScene(ipItem, ipCol);
    mSceneWidget->updateLegend();
}

/*
 * Draw full db scheme
 */
void
MainWindow::drawFullDbScheme()
{
    int topLevelItemCount = mTree->topLevelItemCount();

    for (int j = 0; j < topLevelItemCount; ++j) {
        QTreeWidgetItem *topLevelItem = mTree->topLevelItem(j);

        int childrenCount = topLevelItem->childCount();

        // progress interaction
        //mProgressBar->setMaximum(topLevelItemsCount);
        //mProgressBar->setWindowModality(Qt::WindowModal);

        // go through children items
        for (int i = 0; i < childrenCount; ++i) {
            QTreeWidgetItem *childItem = topLevelItem->child(i);

            // if this is a schema item
            if (Database::SchemaObject == childItem->text(TreeWidget::IdCol).toInt()) {
                mSceneWidget->showOnScene(childItem, TreeWidget::NameCol);
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

        dbInst->cleanup();

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
    QTreeWidgetItem *item = mTree->currentItem();

    // if no item is selected
    if (0 == item) {
        return;
    }

    // get object id
    int objId = item->text(TreeWidget::IdCol).toInt();

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
        case Database::SchemaObject:
                    schema = dbInst->findSchema(objName);

                    // no check for null pointer, describe will handle it
                    descWidget->describe(schema);
                    tabTitle = tr("Schema: ");
                    break;

        case Database::TableObject:                   
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

        case Database::ViewObject:
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

        case Database::RoleObject:
                    role = dbInst->findRole(objName);

                    // no check for null pointer, describe will handle it
                    descWidget->describe(role);
                    tabTitle = tr("Role: ");
                    break;

        case Database::IndexObject:
                    index = dbInst->findIndex(objName);

                    descWidget->describe(index);
                    tabTitle = tr("Index: ");
                    break;

        case Database::TriggerObject:
                    trig = dbInst->findTrigger(objName);

                    descWidget->describe(trig);
                    tabTitle = tr("Trigger: ");
                    break;
        default:
                    qDebug() << "MainWindow::describeObject> Unknown object!";
                    return;
                    break;
    }

    // construct the whole tab title
    tabTitle.append(objName);
    // add description widget to tab widget
    mTabWidget->addTab(descWidget, tabTitle);

    bool switchToNewTab = mSettings.value("Preferences/NewTabAutoSwitch").toBool();

    // if auto switch enabled
    if (switchToNewTab) {
        // activate last tab
        mTabWidget->setCurrentIndex(mTabWidget->count() - 1);
    }
}

/*
 *
 */
void
MainWindow::queryData()
{
    // get selected item
    QTreeWidgetItem *item = mTree->currentItem();

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
        case Database::TableObject:
        case Database::ViewObject:
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
    mTabWidget->addTab(sqlWidget, tabTitle);

    bool switchToNewTab = mSettings.value("Preferences/NewTabAutoSwitch", true).toBool();

    // if auto switch enabled
    if ( switchToNewTab ) {
        // activate last tab
        mTabWidget->setCurrentIndex(mTabWidget->count() - 1);
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
    root.appendChild(mSceneWidget->toXml(doc, mShowGridAction->isChecked(), mDivideOnPagesAction->isChecked(), 
		mShowLegendAction->isChecked(), mShowControlWidgetAction->isChecked()));

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
		mSceneWidget->fromXml(element);
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
    QString defaultFileName = "session_" + 
	mSettings.value("LastSession/DbName", "undefined").toString() + "_" +
	mSettings.value("LastSession/DbUser", "undefined").toString() + "_" + 
	QDate::currentDate().toString(Qt::DefaultLocaleShortDate) + "_" + QTime::currentTime().toString() + ".vdb";
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save session..."),
	    mSettings.value("Preferences/SessionFolder", "./").toString() + defaultFileName,
	    tr("Xml files (*.vdb)"));
    // return if we don't select any file to save
    if (fileName == "") {
	return;
    }

    for (int i = 9; i > 0; --i) {
	if (mSettings.contains("LastSession/SavedSession" + QString().setNum(i - 1))) {
	    mSettings.setValue("LastSession/SavedSession" + QString().setNum(i), mSettings.value("LastSession/SavedSession" + QString().setNum(i - 1)));
	}
    }
    
    mSettings.setValue("LastSession/SavedSession0", fileName);
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
	    mSettings.value("Preferences/SessionFolder", "./").toString(),
	    tr("Xml files (*.vdb)"));
    if (!QFile::exists(fileName)) {
	QMessageBox messageBox;
	messageBox.setText("File doesn't exists");
	messageBox.exec();
//	qDebug() << "File doesn't exists";
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
    mSceneWidget->cleanTableSchemeScene();
    Database::instance()->cleanup();
    mTree->refresh();
}

/*
 * Initialize session
 */
void
MainWindow::initSession()
{
    // if checked flag in appearance page - load last session
    if (mSettings.value("Preferences/LoadLastSession", false).toBool()) {
        if (mSettings.contains("LastSession/SavedSession0")) {
            loadFromXml(mSettings.value("LastSession/SavedSession0").toString());
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

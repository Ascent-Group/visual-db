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
#include <QStatusBar>
#include <QTime>
#include <QUndoCommand>
#include <QUndoStack>
#include <connect/ConnectionInfo.h>
#include <control/Config.h>
#include <gui/DescriptionWidget.h>
#include <gui/graphicsitems/Legend.h>
#include <gui/MainWindow.h>
#include <gui/SceneWidget.h>
#include <gui/SqlConnectionDialog.h>
#include <gui/SqlWidget.h>
#include <gui/TabWidget.h>
#include <gui/TreeWidget.h>

#include <QtDebug>

namespace Gui {

//using namespace QtConcurrent;

/*!
 * Constructor
 */
MainWindow::MainWindow()
    : QMainWindow(),
      mProgressBar(0),
      mUndoStack(0)
{
    ui.setupUi(this);

    // create menus
    createMenus();

    /*DO NOT DELETE ME ALEX*/
    createStatusBar();

//    using namespace Connect;
//    DbHostInfo dbHostInfo;
//    ProxyHostInfo proxyHostInfo;

    // set attributes
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowState(Qt::WindowMaximized);

    initSession();

    connect(ui.mNewConnectionAction, SIGNAL(triggered()), this, SIGNAL(connectionDialogRequest()));
    connect(ui.mReloadDataAction, SIGNAL(triggered()), this, SIGNAL(reloadDataRequest()));
    connect(ui.mShowOptionsDialogAction, SIGNAL(triggered()), this, SIGNAL(optionsDialogRequest()));

    connect(ui.mLoadSessionAction, SIGNAL(triggered()), this, SIGNAL(loadSessionRequest()));
    connect(ui.mSaveSessionAction, SIGNAL(triggered()), this, SIGNAL(saveSessionRequest()));

    connect(ui.mLogPanelWidget, SIGNAL(closed()), this, SLOT(closeLogPanel()));
    connect(ui.mDatabaseTreeWidget, SIGNAL(closed()), this, SLOT(closeDatabaseTree()));
    connect(ui.mLogPanelWidget, SIGNAL(shown()), this, SLOT(showLogPanel()));
    connect(ui.mDatabaseTreeWidget, SIGNAL(shown()), this, SLOT(showDatabaseTree()));

    connect(ui.mTreeTabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(treeTabCloseRequested(int)));
    connect(ui.mTreeTabWidget, SIGNAL(currentChanged(int)), this, SLOT(treeTabChanged(int)));

    connect(ui.mTabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));
}

/*!
 * Destructor
 */
MainWindow::~MainWindow()
{
    // \todo remove progress bar
    delete mProgressBar;
    delete mUndoStack;
    // \note We have to delete it here because somehow the parent is not set for scene
    // widget in setupUi. If don't do this then graphics scene that is inside scene widget
    // won't get deleted.
//    delete ui.mSceneWidget;
}

/*!
 * Adds a tab with scene
 *
 * \note Widgets are passed by non const pointers because QTabWidget::addTab accepts
 * widgets in such a way.
 *
 * \param[in] iScene - Scene to put into this tab
 * \param[in] iTabTitle - Title of this tab
 */
void
MainWindow::addScene(Gui::SceneWidget *iScene, const QString &iTabTitle) const
{
    ui.mTabWidget->addTab(iScene, iTabTitle);
}

/*!
 * This function is used when we close the corresponding tree tab, i.e. disconnecting.
 *
 * \param[in] iScene - Scene whose tab we are going to close.
 */
void
MainWindow::removeScene(Gui::SceneWidget *iScene)
{
    ui.mTabWidget->removeTab(ui.mTabWidget->indexOf(iScene));
}

/*!
 * This function is used when a corresponding tree tab becomes active, i.e. activated by
 * user.
 *
 * \param[in] iScene - Scene to bring up to front
 */
void
MainWindow::activateScene(Gui::SceneWidget *iScene) const
{
    if (0 != iScene) {
        ui.mTabWidget->setCurrentIndex(ui.mTabWidget->indexOf(iScene));
    }
}

/*!
 * Adds a tab with tree
 *
 * \see note for MainWindow::addTree
 *
 * \param[in] iTree - Tree to put into this tab
 * \param[in] iTabTitle - Title of this tab
 */
void
MainWindow::addTree(Gui::TreeWidget *iTree, const QString &iTabTitle) const
{
    ui.mTreeTabWidget->addTab(iTree, iTabTitle);
}

/*!
 * This function is used when a corresponding scene tab becomes active, i.e. activated by
 * user.
 *
 * \param[in] iTree - Tree widget to bring up to front
 */
void
MainWindow::activateTree(Gui::TreeWidget *iTree) const
{
    if (0 != iTree) {
        ui.mTreeTabWidget->setCurrentIndex(ui.mTreeTabWidget->indexOf(iTree));
    }
}

/*!
 * Returns tree which is currently active. This function is used by director to determine
 * the current active context.
 *
 * \todo Check what will the function return if the tree widget is hidden!
 *
 * \return Pointer to tree which is currently on top.
 */
Gui::TreeWidget*
MainWindow::activeTree() const
{
    return dynamic_cast<Gui::TreeWidget*>(ui.mTreeTabWidget->currentWidget());
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
    Control::Config cfg;
    ui.mAlignToGridAction->setChecked(cfg.alignToGrid());
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

#if 0
    ui.mSceneWidget->setSchemeMenu(ui.mSchemeMenu);
    ui.mSceneWidget->setTableMenu(ui.mTableMenu);
#endif
}

/*!
 * \brief Update session menu
 *
 * \todo fix it to work with director
 */
void
MainWindow::updateSessionMenu()
{
    ui.mSessionMenu->clear();

    Control::Config cfg;

    int countSavedSessions = cfg.savedSessionsNumber();
    for (int i = 0; i < countSavedSessions; ++i) {
        // update session menu
        if (!cfg.savedSession(i).isEmpty()) {
            // last session
            QAction *lastSessionAction = new QAction(cfg.savedSession(i), this);
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
 * \param[in] iFlag - True if we want to enable actions, false otherwise
 */
void
MainWindow::setEnableForActions(bool iFlag)
{
    ui.mSaveToImgAction->setEnabled(iFlag);
    ui.mDrawFullDBAction->setEnabled(iFlag);

    ui.mSelectAllTablesAction->setEnabled(iFlag);
    ui.mRemoveAllTablesAction->setEnabled(iFlag);
    ui.mShowLegendAction->setEnabled(iFlag);
    ui.mAlignToGridAction->setEnabled(iFlag);
    ui.mColorizeAccordingSchemasAction->setEnabled(iFlag);

    ui.mDeleteTableAction->setEnabled(iFlag);
    ui.mShowFieldsTypesAction->setEnabled(iFlag);
    ui.mHideFieldsTypesAction->setEnabled(iFlag);
    ui.mSetTableColorAction->setEnabled(iFlag);
    ui.mAdjustTableSizeAction->setEnabled(iFlag);
    ui.mGroupItemsAction->setEnabled(iFlag);
    ui.mUngroupItemsAction->setEnabled(iFlag);
    ui.mDescribeObjectAction->setEnabled(iFlag);
    ui.mQueryDataAction->setEnabled(iFlag);
    ui.mShowIndicesAction->setEnabled(iFlag);
    ui.mHideIndicesAction->setEnabled(iFlag);
    ui.mSelectAllTablesInSchemaAction->setEnabled(iFlag);

    ui.mSaveSessionAction->setEnabled(iFlag);

    ui.mReloadDataAction->setEnabled(iFlag);
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
#if 0
#ifndef QT_NO_PRINTER
    QPrinter printer(QPrinter::ScreenResolution);
    QPrintDialog printDialog(&printer);
    if (printDialog.exec() == QDialog::Accepted) {
        ui.mSceneWidget->print(&printer);
    }
#endif
#endif
}

/*!
 * Slot for handling tree tab closing.
 *
 * \param[in] iIndex - Index of a tab that is about to be closed.
 */
void
MainWindow::treeTabCloseRequested(int iIndex)
{
    Gui::TreeWidget *tree = dynamic_cast<Gui::TreeWidget*>(ui.mTreeTabWidget->widget(iIndex));
    ui.mTreeTabWidget->removeTab(iIndex);
    emit treeTabClosed(tree);
}

/*!
 * Slot for handling tree tab switch.
 *
 * \param[in] iIndex - Index of a tab that became active
 */
void
MainWindow::treeTabChanged(int iIndex)
{
    Gui::TreeWidget *tree = dynamic_cast<Gui::TreeWidget*>(ui.mTreeTabWidget->widget(iIndex));
    emit treeTabChanged(tree);
}

/*!
 * \todo Comment
 */
void
MainWindow::tabChanged(int iIndex)
{
    Gui::SceneWidget *scene = dynamic_cast<Gui::SceneWidget*>(ui.mTabWidget->widget(iIndex));
    if (0 != scene) {
        emit tabChanged(scene);
    }
}

/*!
 * \brief Print the preview page
 *
 * \param[in] iPrinter - Printer
 */
void
MainWindow::printPreview(QPrinter *iPrinter)
{
#if 0
#ifdef QT_NO_PRINTER
    Q_UNUSED(iPrinter);
#else
    ui.mSceneWidget->print(iPrinter);
#endif
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
 * \param[in] iFlag - True if we want to show tree, false otherwise
 */
void
MainWindow::showDatabaseTree(bool iFlag)
{
    if (iFlag) {
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
 * \param[in] iFlag - True if we want to show log panel, false otherwise
 */
//void
//MainWindow::showLogPanel(bool iFlag)
//{
//    // \todo Maybe we can use a signal slot connection for this???
//    ui.mLogPanelWidget->setVisible(iFlag);
//}

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
 * \todo this function will be replaced with another one in Director
 */
void
MainWindow::addItem()
{
#if 0
    QTreeWidgetItemIterator treeIter(ui.mTree);

    while (*treeIter) {
        if ((*treeIter)->isSelected()) {
            ui.mSceneWidget->showOnScene((*treeIter), TreeWidget::NameCol, true);
        }
        ++treeIter;
    }

    ui.mSceneWidget->updateLegend();
#endif
}

/*!
 * \brief Show the item on the scene when we double click on it
 *
 * \param[in] iItem - Item we clicked on
 * \param[in] iColumn - Column of the item
 *
 * \todo move to director
 */
void
MainWindow::addItem(Gui::TreeWidgetItem *iItem, int iColumn)
{
#if 0
    ui.mSceneWidget->showOnScene(iItem, iColumn, true);
#endif
}


/*!
 * \brief Draw full db scheme
 *
 * \todo move to director
 */
void
MainWindow::drawFullDbScheme()
{
#if 0
    int topLevelItemCount = ui.mTree->topLevelItemCount();

    for (int j = 0; j < topLevelItemCount; ++j) {
        Gui::TreeWidgetItem *topLevelItem = ui.mTree->topLevelItem(j);

        int childrenCount = topLevelItem->childCount();

        // progress interaction
        //mProgressBar->setMaximum(topLevelItemsCount);
        //mProgressBar->setWindowModality(Qt::WindowModal);

        // go through children items
        for (int i = 0; i < childrenCount; ++i) {
            Gui::TreeWidgetItem *childItem = topLevelItem->child(i);

            // if this is a schema item
            if (TreeWidget::SchemaItem == childItem->text(TreeWidget::IdCol).toInt()) {
                ui.mSceneWidget->showOnScene(childItem, TreeWidget::NameCol);
            }
            //mProgressBar->setValue(mProgressBar->value() + 1);
        }

        //mProgressBar->reset();
        statusBar()->showMessage(tr("Processing tables complete"), 3);
    }
#endif
}

/*!
 * \brief Do smth before mainwindows is closed
 *
 * \param[in] iEvent - Close event
 */
void
MainWindow::closeEvent(QCloseEvent *iEvent)
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
                emit saveSessionRequest();
//                // save parameters to xml file
//                if (!saveSession()) {
//                    continue;
//                }
            }

            emit exitRequest();

            iEvent->accept();
        } else {
            iEvent->ignore();
        }
        break;
    }
    while (true);
}

/*!
 * \brief Describe selected database object
 * \todo move to director
 */
void
MainWindow::describeObject()
{
#if 0
    // get selected item
    Gui::TreeWidgetItem *item = ui.mTree->currentItem();

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

    bool switchToNewTab = Control::Config().newTabAutoSwitch();

    // if auto switch enabled
    if (switchToNewTab) {
        // activate last tab
        ui.mTabWidget->setCurrentIndex(ui.mTabWidget->count() - 1);
    }
#endif
}

/*!
 * \brief Show query dialog tab
 * \todo move to director
 */
void
MainWindow::queryData()
{
#if 0
    // get selected item
    Gui::TreeWidgetItem *item = ui.mTree->currentItem();

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

    bool switchToNewTab = Control::Config().newTabAutoSwitch();

    // if auto switch enabled
    if (switchToNewTab) {
        // activate last tab
        ui.mTabWidget->setCurrentIndex(ui.mTabWidget->count() - 1);
    }
#endif
}

/*!
 * \brief Save all parameters to xml file
 *
 * \param[in] iFileName - Xml file name were we will store parameters to
 *
 * \todo move to director
 */
void
MainWindow::saveToXml(const QString &iFileName)
{
#if 0
    QDomDocument doc("VisualDB");
    QDomElement root = doc.createElement("visual-db");
    doc.appendChild(root);
    mConnectionInfo.toXml(doc, root);
    root.appendChild(ui.mSceneWidget->toXml(doc, ui.mShowGridAction->isChecked(), ui.mDivideIntoPagesAction->isChecked(),
                ui.mShowLegendAction->isChecked(), ui.mShowControlWidgetAction->isChecked()));

    QFile file(iFileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox messageBox;
        messageBox.setText("Can't open file");
        messageBox.exec();
    }

    QTextStream stream(&file);
    stream << doc.toString();

    file.close();
#endif
}

/*!
 * \brief Load parameters from xml file
 *
 * \param[in] iFileName - Xml file name were load parameter from
 * \todo move to director
 */
void
MainWindow::loadFromXml(QString iFileName)
{
#if 0
    QFile file(iFileName);
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
            if (element.tagName() == "connection") {
                mConnectionInfo.fromXml(element);
            }
        }
        child = child.nextSibling();
    }

    // show connection dialog and check if we haven't pressed 'Cancel' button
    if (showConnectionDialog(true/*iLoadSession*/) == QDialog::Accepted) {
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
#endif
}

/*!
 * \brief Save session
 *
 * \return True if we save was successfull, false otherwise
 */
bool
MainWindow::saveSession()
{
    Control::Config cfg;
//    QString defaultFileName = "session_" +
//        mSettings.value(LAST_SESSION_GRP + "/" + DB_NAME_SETTING, "undefined").toString() + "_" +
//        mSettings.value(LAST_SESSION_GRP + "/" + DB_USER_SETTING, "undefined").toString() + "_" +
//        QDate::currentDate().toString(Qt::DefaultLocaleShortDate) + "_" + QTime::currentTime().toString() + ".vdb";
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save session..."), cfg.sessionDir(), tr("Session files (*.vdb)"));
//    QString fileName = QFileDialog::getSaveFileName(this, tr("Save session..."),
//            mSettings.value(PREFS_GRP + "/" + SESSION_DIR_SETTING, "./").toString() + defaultFileName,
//            tr("Xml files (*.vdb)"));
    // return if we don't select any file to save
    if (fileName == "") {
        return false;
    }

    for (int i = cfg.savedSessionsNumber(); i > 0; --i) {
        if (!cfg.savedSession(i - 1).isEmpty()) {
            cfg.setSavedSession(cfg.savedSession(i - 1), i);
        }
    }

    cfg.setSavedSession(fileName, 0);
    updateSessionMenu();
    saveToXml(fileName);

    return true;
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
    if (action) {
        loadFromXml(action->text());
    }
}

/*!
 * \brief Show window with full screen or not
 *
 * \param[in] iFlag - True if we want to show in fullscreen, false otherwise
 */
void
MainWindow::setFullScreen(bool iFlag)
{
    if (iFlag) {
        setWindowState(Qt::WindowFullScreen);
    } else {
        setWindowState(Qt::WindowMaximized);
    }
}

/*!
 * \brief Reload data from DB
 * \todo move to director
 */
//void
//MainWindow::reloadData()
//{
//    ui.mSceneWidget->cleanSchemeScene();
//    Database::instance()->resetData();
//    ui.mTree->refresh();
//
//    // flush scene cache
//    mUndoStack->clear();
//    ui.mSceneWidget->flushCache();
//}

/*!
 * \brief Initialize session
 */
void
MainWindow::initSession()
{
    Control::Config cfg;
    // if checked flag in appearance page - load last session
    if (cfg.loadLastSession()) {
        QString firstSavedSession = cfg.savedSession(0);
        if (!firstSavedSession.isEmpty()) {
            loadFromXml(firstSavedSession);
        } else {
            qDebug() << "Last session not found";
        }
    }
}

/*!
 * \brief Add command to the stack
 */
void
MainWindow::addCommand(QUndoCommand *iCommand)
{
    mUndoStack->push(iCommand);
}

/*!
 * \brief Prints the message to the log panel
 *
 * \param[in] iText - Message text
 */
void
MainWindow::printMsg(const QString &iText) const
{
    ui.mLogPanel->print(iText);
}

}


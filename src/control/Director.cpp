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

#include <common.h>
#include <consts.h>
#include <control/Config.h>
#include <control/Context.h>
#include <control/Director.h>
#include <control/Session.h>
#include <gui/MainWindow.h>
#include <gui/OptionsDialog.h>
#include <gui/SceneWidget.h>
#include <gui/SqlConnectionDialog.h>
#include <gui/SqlWidget.h>
#include <gui/TreeWidget.h>

#include <QFileDialog>
#include <QMessageBox>
//#include <QTimer>

#include <QtDebug>

namespace Control
{

/*!
 * Constructor
 */
Director::Director(QObject *iParent)
    : QObject(iParent),
//      mSplashScreen(QPixmap(":/img/splashscreen.png")),
      mFSM(),
      mInitialState(),
      mIdleState(),
      mBusyState(),
      mMainWindow(0),
      mRegistry(),
      mMenuMgr(this),
      mDbMgr(),
      mConverter(mDbMgr)
{
//    mSplashScreen.show();
//    QTimer::singleShot(2000, &mSplashScreen, SLOT(close()));
////    mSplashScreen.showMessage("Loading...");

    mFSM.addState(&mInitialState);
    mFSM.addState(&mIdleState);
    mFSM.addState(&mBusyState);

    // set transitions
    mInitialState.addTransition(this, SIGNAL(initializationComplete()), &mIdleState);
    mIdleState.addTransition(this, SIGNAL(requestReceived()), &mBusyState);
    mBusyState.addTransition(this, SIGNAL(requestProcessed()), &mIdleState);

    // connections for states
    connect(&mBusyState, SIGNAL(entered()), this, SLOT(busyStateEntered()));
    connect(&mBusyState, SIGNAL(exited()), this, SLOT(busyStateExited()));

    mFSM.setInitialState(&mInitialState);
    mFSM.start();
}

/*!
 * Destructor
 */
Director::~Director()
{
    qDebug() << "Director::~Director>";
    // clear the registry
    QList<Control::Context*> contexts = mRegistry.values();

    /**
     * We have to use a separate list for contexts because:
     * 1. we can't modify the registry
     * 2. registry values returns non unique value list.
     */
    Control::Context *ctx;
    while (!contexts.isEmpty()) {
        ctx = contexts.takeFirst();
        remove(ctx);
        contexts.removeAll(ctx);
    }
    mRegistry.clear();

    // \note we don't delete mainwindow since it has delete-on-close attribute  set
//    delete mMainWindow;
}

/*!
 * Command the director to start the execution of the application. When this function is
 * called the main window is being shown and a user can start exploring databases.
 */
void
Director::start()
{
    mMainWindow->show();
    emit logMessageRequest("Started...");
}

/*!
 * Performs initialization mostly of gui objects needed to start working with application.
 *
 * \return true - If the initialization succeeded.
 * \return false - If some error occured during the initialization.
 */
bool
Director::initialize()
{
    try {
        using namespace Gui;
        using namespace Connect;

        // main window + connections
        mMainWindow = new MainWindow();
        connect(mMainWindow, SIGNAL(connectionDialogRequest()), this, SLOT(connectionDialogRequested()));
        connect(mMainWindow, SIGNAL(reloadDataRequest()), this, SLOT(reloadDataRequested()));
//        connect(mMainWindow, SIGNAL(disconnectRequest()), this, SLOT(disconnectRequested()));
        connect(mMainWindow, SIGNAL(optionsDialogRequest()), this, SLOT(optionsDialogRequested()));
        connect(mMainWindow, SIGNAL(loadSessionRequest()), this, SLOT(loadSessionRequested()));
        connect(mMainWindow, SIGNAL(saveSessionRequest()), this, SLOT(saveSessionRequested()));
        connect(mMainWindow, SIGNAL(exitRequest()), this, SLOT(exitRequested()));

        connect(mMainWindow, SIGNAL(treeTabClosed(Gui::TreeWidget *)), this, SLOT(treeTabClosed(Gui::TreeWidget *)));
        connect(mMainWindow, SIGNAL(treeTabChanged(Gui::TreeWidget *)), this, SLOT(treeTabChanged(Gui::TreeWidget *)));

        connect(mMainWindow, SIGNAL(tabChanged(Gui::SceneWidget *)), this, SLOT(tabChanged(Gui::SceneWidget *)));

        connect(this, SIGNAL(logMessageRequest(const QString &)), mMainWindow, SLOT(printMsg(const QString &)));

        // \fixme Set more adequate text
//        mSplashScreen.showMessage("Loading something else...", Qt::AlignBottom | Qt::AlignLeft, Qt::cyan);
    } catch (...) {
        return false;
    }

    emit initializationComplete();

    // \todo check the flag for auto session restoring
    //restoreSession(lastSessionFileName);
    return true;
}

/*!
 * Adds widget-ctx pair to the registry. The most appropriate name for this function is
 * probably 'register' but since this is a keyword we ca'nt use it, so that's why 'add' is
 * used for the name.
 *
 * \param[in] iWidget - Widget we are registering.
 * \param[in] iContext - a context this widget belongs to.
 *
 * \return true - If the pair has been successfully added to the registry.
 * \return false - Otherwise, when this pair has been registered before.
 */
bool
Director::add(QWidget *iWidget, Control::Context *iContext)
{
    Q_ASSERT(0 != iWidget);
    Q_ASSERT(0 != iContext);

    if (!mRegistry.contains(iWidget)) {
        mRegistry.insert(iWidget, iContext);

        return true;
    }

    return false;
}

/*!
 * Removes a record for the given widget from a registry.
 *
 * \param[in] iWidget - Widget we are trying to unregister.
 *
 * \return true - If the widget has been removed from the registry.
 * \return false - If there were no such widget registered.
 */
bool
Director::remove(QWidget *iWidget)
{
    if (mRegistry.contains(iWidget)) {
        mRegistry.remove(iWidget);
        return true;
    }

#ifdef DEBUG_TRACE
    qDebug() << "Director::remove>(QWidget *)> Didn't find " << iWidget;

    foreach (QWidget *widget, mRegistry.keys()) {
        qDebug() << "Director::remove> Still here " << widget;
    }
#endif

    // We should not get here (in theory). If we got here, then it means that we are
    // trying to unregister a widget that has never been registered, i.e. we missed its
    // registration and need to check the code.
    Q_ASSERT(false);

    return false;
}

/*!
 * Removes all widgets for the specified context from the registry.
 *
 * \param[in] iContext - a context whose widgets we are trying to unregister.
 *
 * \return true - If all widgets for the given context have been successfully remove from
 * the registry.
 * \return false - Otherwise.
 */
bool
Director::remove(Control::Context *iContext)
{
    mDbMgr.remove(iContext);

    QList<QWidget *>::const_iterator iter = mRegistry.keys(iContext).begin();
#ifdef DEBUG_TRACE
    qDebug() << "Director::remove(Context*)> Context's widgets: " << mRegistry.keys(iContext).size();
#endif

    bool flag = true;
    foreach (QWidget *widget, mRegistry.keys(iContext)) {
        flag = remove(widget) && flag;
    }

    delete iContext;

    return flag;
}

/*!
 * Finds a context for the specified widget
 *
 * \param[in] iWidget - Widget whose context we are looking for.
 *
 * \return Context of the specified widget
 */
Control::Context*
Director::findContext(QWidget *iWidget) const
{
    return mRegistry.value(iWidget);
}

/*!
 * Finds all widgets for the given context
 *
 * \param[in] iContext - A context whose widgets we are looking for
 * \param[out] oWidgets - Vector with the found widgets.
 *
 * \return Number of widgets that we found.
 */
quint32
Director::findWidgets(Control::Context *iContext, QVector<QWidget*> &oWidgets) const
{
    quint32 count = 0;
    foreach (QWidget *widget, mRegistry.keys(iContext)) {
        oWidgets.push_back(widget);
        ++count;
    }

    return count;
}

/*!
 *
 */
Gui::TreeWidget*
Director::findTree(Control::Context *iCtx) const
{
    QVector<QWidget*> widgets;
    findWidgets(iCtx, widgets);

    int i = 0;
    Gui::TreeWidget *tree = 0;
    while (i < widgets.size() && 0 == (tree = dynamic_cast<Gui::TreeWidget*>(widgets.at(i)))) {
        ++i;
    }

    return tree;
}

/*!
 *
 */
Gui::SceneWidget*
Director::findScene(Control::Context *iCtx) const
{
    QVector<QWidget*> widgets;
    findWidgets(iCtx, widgets);

    int i = 0;
    Gui::SceneWidget *scene = 0;
    while (i < widgets.size() && 0 == (scene = dynamic_cast<Gui::SceneWidget*>(widgets.at(i)))) {
        ++i;
    }

//    qDebug() << "Director::findScene> scene = " << widgets.at(i);
    return scene;
}

/*!
 * Used to bring up a connection dialog for further connection establishing. This function
 * can be used either from slot Director::connectionDialogRequested() or from a function
 * that will restore sessions \sa Director::restoreSession
 *
 * \param[in] iLoadSession - Inidicates whether the connection dialog is needed for
 * session restoring or for establishing a new connection.
 */
void
Director::showConnectionDialog(bool iLoadSession)
{
    using namespace Gui;
    using namespace Connect;
    using namespace Control;

    // create sql connection dialog
    SqlConnectionDialog connDialog(iLoadSession);

    // set last used connection info
    QVector<ConnectionInfo> infos;

    QString sessionFile;
    Config cfg;
    ConnectionInfo recentConnInfo;
    for (int i = 0; i < cfg.savedSessionsNumber() ; ++i) {
        sessionFile = cfg.savedSession(i);
        if (!sessionFile.isEmpty()) {
            if (QFile::exists(sessionFile)) {
                Session session;
                session.load(sessionFile);
                for (int j = 0; j < session.countConnections(); ++j) {
                    session.loadConnectionInfo(recentConnInfo, j);
                    infos.push_back(recentConnInfo);
                }
            } else {
                emit logMessageRequest(QString("Session file '%1' doesn't exist!").arg(sessionFile));
            }
        }
    }

    connDialog.setConnectionInfos(infos);

    Context *ctx = 0;
    do {
        // nothing to do if canceled
        if (QDialog::Rejected == connDialog.exec()) {
            return;
        }

        QString errorMsg;
        ctx = mDbMgr.establishConnection(connDialog.connectionInfo(), errorMsg);

        if (!errorMsg.isEmpty()) {
            QMessageBox::critical(0, tr("Connection error"), errorMsg, QMessageBox::Ok);
        }
    } while (!ctx);

    // \todo save context's connection info

    // if we got here, then we have a valid ctx
    emit logMessageRequest(QString("Connected to '<b>%1</b> on behalf of <b>%2</b>'.")
            .arg(connectionName(ctx))
            .arg(ctx->connectionInfo().dbHostInfo().user()));

    QString tabTitle = QString("%1@%2 (%3)")
        .arg(ctx->connectionInfo().dbHostInfo().dbName())
        .arg(ctx->connectionInfo().dbHostInfo().address())
        .arg(ctx->connectionInfo().dbHostInfo().user());

    // create scene for it and register it
    SceneWidget *scene = new SceneWidget();
    // \todo uncomment next line when scene starts inheriting ContextMenuHolder
//    scene->setContextMenu(mMenuMgr.menu(ContextMenuManager::MENU_SCENE_WIDGET));
    connect(scene, SIGNAL(contextMenuRequest(QContextMenuEvent *)),
            &mMenuMgr, SLOT(contextMenuRequested(QContextMenuEvent *)));
    add(scene, ctx);
    mMainWindow->addScene(scene, QString("Scene: %1").arg(tabTitle));

    // create tree for it and register it
    TreeWidget *tree = new TreeWidget();
    tree->setContextMenu(mMenuMgr.menu(Tree));
    connect(tree, SIGNAL(contextMenuRequest(QContextMenuEvent *)),
            &mMenuMgr, SLOT(contextMenuRequested(QContextMenuEvent *)));
    add(tree, ctx);
    mMainWindow->addTree(tree, tabTitle);

    reloadDataRequested();
}

/*!
 * Slot. Executed when the director's state machine enters busy state.
 */
void
Director::busyStateEntered()
{
    // \todo show progress bar
}

/*!
 * Slot. Executed when the director's state machine enters idle state.
 */
void
Director::busyStateExited()
{
    // \todo hide and reset progress bar
}

/*!
 * Slot for handling new connection request. Executed when a 'New connection' button is
 * clicked. On execution shows the connection dialog.
 */
void
Director::connectionDialogRequested()
{
    showConnectionDialog(false);
}

/*!
 * Slot for handling options dialog request. Executed when a user chooses Options... in
 * file menu.
 */
void
Director::optionsDialogRequested()
{
    Gui::OptionsDialog optionsDialog;

    // this will be done by optionsDialog's accept method
    if (QDialog::Accepted == optionsDialog.exec()) {
        // \fixme where we should update session menu?
//        mMainWindow->updateSessionMenu();
    }

}

/*!
 * Slot for handling reload data request. Executed when a user clicks 'Reload' button.
 */
void
Director::reloadDataRequested()
{
    emit requestReceived();
    Gui::TreeWidget *tree = mMainWindow->activeTree();
    if (tree) {
        // find the active context
        Control::Context *ctx = findContext(tree);
        // do reload for this context
        if (!mDbMgr.reloadData(ctx)) {
            QString errorMsg = mDbMgr.lastError(ctx);
            emit logMessageRequest(errorMsg);

            QMessageBox::critical(0, tr("Update error"), errorMsg, QMessageBox::Ok);

            return;
        }

        // get new data and notify clients
        Objects objects;
//        tree->clear();
        mDbMgr.newObjects(ctx, objects);
        QList<Gui::TreeWidgetItem*> items;
        mConverter.toTreeWidgetItems(objects, items, &mMenuMgr);
        tree->display(items);

        mMainWindow->setEnableForActions(true);
    }

    emit requestProcessed();
}

/*!
 * Slot for handling disconnect request. Executed when a user clicks 'Disconnect button'.
 */
void
Director::disconnectRequested(Control::Context *iCtx)
{
    mMainWindow->removeScene(findScene(iCtx));
    remove(iCtx);

    emit logMessageRequest(QString("Disconnected from '<b>%1</b>'.")
            .arg(connectionName(iCtx)));

    if (mRegistry.isEmpty()) {
        mMainWindow->setEnableForActions(false);
    }
}

/*!
 * Slot for handling request to load session. Executed when user chooses to load session
 * from menu.
 */
void
Director::loadSessionRequested()
{
    QString fileName = QFileDialog::getOpenFileName(0,
            tr("Open session..."),
            Control::Config().sessionDir(),
            tr("Session files (*%1)").arg(Consts::SESSION_FILE_EXT));

    if (!QFile::exists(fileName)) {
        QMessageBox::critical(0, tr("Load session error"), tr("File doesn't exists"), QMessageBox::Ok);
        return;
    }

    restoreSession(fileName);
}

/*!
 * Slot for handling request to save session. Executed when a user closes main window and
 * confirm saving active sessions OR when user chooses to save session from the menu.
 */
void
Director::saveSessionRequested()
{
    qDebug() << "Director::saveSessionRequested>";
    using namespace Control;

    Config cfg;
    // check whether the default directory for saved sessions exists
    QString sessionDirPath(cfg.sessionDir());

    // create session dir if not it doesn't exist
    if (!QDir(sessionDirPath).exists()) {
        QDir().mkpath(sessionDirPath);
    }

    QString fileName = QFileDialog::getSaveFileName(mMainWindow,
            tr("Save session..."),
            sessionDirPath,
            tr("Session files (*%1)").arg(Consts::SESSION_FILE_EXT));

    if (!fileName.isEmpty()) {
        // Go through all contexts
        Session session;
        quint32 index = 0;
        QList<Context*> contexts = mRegistry.values();
        Context *ctx = 0;

        // \sa Director::~Director for reasons why we use this kind of loop
        while (!contexts.isEmpty()) {
            ctx = contexts.takeFirst();
            // save session for each context

            // \todo this includes saving connection infos, tab infos, widget sizes, etc.
            session.saveConnectionInfo(ctx->connectionInfo(), index++);

            contexts.removeAll(ctx);
        }

        if (!fileName.endsWith(Consts::SESSION_FILE_EXT)) {
            fileName.append(Consts::SESSION_FILE_EXT);
        }
        qDebug() << fileName;
        session.save(fileName);

        Control::Config cfg;
        cfg.setSavedSession(fileName);
    }
}

/*!
 * Restores session from a file.
 *
 * \param[in] iFileName - Path to the file that contains session info.
 */
void
Director::restoreSession(const QString &iFileName)
{
    qDebug() << "Director::restoreSession>";
    // \todo Implement
}

/*!
 * Slot for handling application exit. Executed when a user closes main window and
 * confirms its closing.
 */
void
Director::exitRequested()
{
    qDebug() << "Director::exitRequested>";
    // \todo disconnect
    // \todo Do cleanup
//    qApp->exit();
}

/*!
 * Handles closing of a tree widget.
 *
 * \param[in] iTree - Tree widget whose tab has been closed.
 */
void
Director::treeTabClosed(Gui::TreeWidget *iTree)
{
    disconnectRequested(findContext(iTree));
}

/*!
 * Handles the activation of a tree widget. When this happends the corresponding scene
 * should come to front.
 *
 * \param[in] iTree - Tree widget that came up to front in a tab tree widget.
 */
void
Director::treeTabChanged(Gui::TreeWidget *iTree)
{
    mMainWindow->activateScene(findScene(findContext(iTree)));
}

/*!
 * Handles activation of a scene. In this case we agreed to activate the corresponding
 * tree widget.
 *
 * \param[in] iScene - Scene widget whose tab has been activated.
 */
void
Director::tabChanged(Gui::SceneWidget *iScene)
{
    mMainWindow->activateTree(findTree(findContext(iScene)));
}

/*!
 * Slot. Handles expand all items action toggling.
 *
 * \param[in] iTree - Tree widget whose items are to be expanded/collapsed.
 * \param[in] iFlag - Indicates whether to expand(true) or collapse(flase) items.
 */
void
Director::expandAllTreeItems(Gui::TreeWidget *iTree, bool iFlag)
{
    if (iTree) {
        iTree->setExpandAll(iFlag);
    }
}

/*!
 * Slot. Handles adding of tree widget items to a graphics scene. Executed when an
 * addAction is triggered via context menu of a table items, or view items (or both).
 *
 * \todo Implement
 */
void
Director::addTreeItemsToScene()
{
    Gui::TreeWidget *tree = mMainWindow->activeTree();
    if (tree) {
        QList<QTreeWidgetItem*> selected = tree->selectedItems();
        foreach (const QTreeWidgetItem *item, selected) {
            // \todo add item to scene
        }
    }
}

/*!
 * Slot. Handles describing of table items and/or view items. Executed when a
 * describeAcition is triggered via context menu of table/view items.
 *
 * \todo Implement
 */
void
Director::describeItems()
{
    Gui::TreeWidget *tree = mMainWindow->activeTree();
    if (tree) {
        QList<QTreeWidgetItem*> selected = tree->selectedItems();
        foreach (const QTreeWidgetItem *item, selected) {
            // \todo create decription widget via converter
            // \todo pass the widget to main window
        }
    }
}

/*!
 * Slot. Handles running an SQL query for table/view items of tree widget. Executed when a
 * queryAction is triggered via context menu of table/view items.
 *
 * \todo Implement
 */
void
Director::queryItems()
{
    Gui::TreeWidget *tree = mMainWindow->activeTree();
    if (tree) {
        QString title;
        QList<QTreeWidgetItem*> selected = tree->selectedItems();
        foreach (const QTreeWidgetItem *item, selected) {
            // \todo Check if there is already query widget for the given item (and
            // context)
            // Ask MainWindow for a list of sql widgets that correspond the item's name
            // if there is an a sql widget registered for the context, then no need to
            // create a new one.
            // otherwise, create an sql widget, give it to main window and then
            // register add(sqlWidget, tree->context())
            //Gui::SqlWidget *widget = new Gui::SqlWidget();
            // \todo create sql query widget via converter
            //title = QString("Description: %1").arg(item->text(Gui::TreeWidget::NameCol));
            //mMainWindow->addTab(widget, title);
            // \note if we add the same widget two times, then the second time we add it,
            // it will replace the first tab and override the tab title. Proved by a
            // sample app.
        }
    }
}

/*!
 * \note Global helper function
 * Construct a connection name for the given context
 *
 * \param[in] iCtx - Context
 *
 * \return String with a connection name
 */
QString
connectionName(const Control::Context *iCtx)
{
    return QString("%1@%2")
        .arg(iCtx->connectionInfo().dbHostInfo().dbName())
        .arg(iCtx->connectionInfo().dbHostInfo().address());

}

} // namespace Control


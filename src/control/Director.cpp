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

#include <control/Config.h>
#include <control/Context.h>
#include <control/Director.h>
#include <control/Session.h>
#include <gui/MainWindow.h>
#include <gui/OptionsDialog.h>
#include <gui/SceneWidget.h>
#include <gui/SqlConnectionDialog.h>
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
      mDbMgr()
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
    foreach (Control::Context *ctx, mRegistry.values()) {
        mDbMgr.remove(ctx);
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
        connect(mMainWindow, SIGNAL(saveSessionRequest()), this, SLOT(saveSessionRequested()));
        connect(mMainWindow, SIGNAL(exitRequest()), this, SLOT(exitRequested()));

        connect(mMainWindow, SIGNAL(treeTabClosed(Gui::TreeWidget *)), this, SLOT(treeTabClosed(Gui::TreeWidget *)));
        connect(mMainWindow, SIGNAL(treeTabChanged(Gui::TreeWidget *)), this, SLOT(treeTabChanged(Gui::TreeWidget *)));

        connect(mMainWindow, SIGNAL(tabChanged(Gui::SceneWidget *)), this, SLOT(tabChanged(Gui::SceneWidget *)));

        connect(this, SIGNAL(logMessageRequest(const QString &)),
                mMainWindow, SLOT(printMsg(const QString &)));

        // \fixme Set more adequate text
//        mSplashScreen.showMessage("Loading something else...", Qt::AlignBottom | Qt::AlignLeft, Qt::cyan);
    } catch (...) {
        return false;
    }

    emit initializationComplete();
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

    // debug info
    qDebug() << "Director::add> Registry contains " << mRegistry.values().size() << " contexts";
    qDebug() << "Director::add> Registry contains " << mRegistry.keys().size() << " widgets";

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
    QList<QWidget *>::const_iterator iter = mRegistry.keys(iContext).begin();

    bool flag = true;
    for (; iter != mRegistry.keys(iContext).end(); ++iter) {
        flag = (flag && remove(*iter));
    }

    mDbMgr.remove(iContext);

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
 * that will restore sessions \todo name the mentioned fucntion
 *
 * \param[in] iLoadSession - Inidicates whether the connection dialog is needed for
 * session restoring or for establishing a new connection.
 *
 * \todo Implement
 */
void
Director::showConnectionDialog(bool iLoadSession)
{
    using namespace Gui;
    using namespace Connect;

    // \todo we should get the last connetion info somehow
    // \note I Suggest we have a constructor with only bool parameter.

    // create sql connection dialog
    SqlConnectionDialog connDialog(iLoadSession);

    // \todo set last used connection info
    QVector<ConnectionInfo> infos;
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
    emit logMessageRequest(QString("Connected to '<b>%1@%2</b> on behalf of <b>%3</b>'")
            .arg(ctx->connectionInfo().dbHostInfo().dbName())
            .arg(ctx->connectionInfo().dbHostInfo().address())
            .arg(ctx->connectionInfo().dbHostInfo().user()));

    QString tabTitle = QString("%1@%2 (%3)")
        .arg(ctx->connectionInfo().dbHostInfo().dbName())
        .arg(ctx->connectionInfo().dbHostInfo().address())
        .arg(ctx->connectionInfo().dbHostInfo().user());

    // create scene for it and register it
    SceneWidget *scene = new SceneWidget();
    add(scene, ctx);
    mMainWindow->addScene(scene, "Scene: " + tabTitle);

    // create tree for it and register it
    TreeWidget *tree = new TreeWidget();
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
            // \todo Error message should be taken from dbMgr, something like lastError().
            QString errorMsg("Unable to reload data.");
            emit logMessageRequest(errorMsg);

            QMessageBox::critical(0, tr("Update error"), errorMsg, QMessageBox::Ok);

            return;
        }

        // \todo activate buttons on toolbar
        mMainWindow->setEnableForActions(true);
        // \todo get updated data and notify clients
    }

    emit requestProcessed();
}

/*!
 * Slot for handling disconnect request. Executed when a user clicks 'Disconnect button'.
 */
void
Director::disconnectRequested(Control::Context *iCtx)
{
    mDbMgr.remove(iCtx);

    mMainWindow->removeScene(findScene(iCtx));

    // \todo log message

    if (mRegistry.isEmpty()) {
        mMainWindow->setEnableForActions(false);
    }
}

/*!
 * Slot for handling request to save session. Executed when a user closes main window and
 * confirm saving active sessions.
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

    QString fileName = QFileDialog::getSaveFileName(mMainWindow, tr("Save session..."), sessionDirPath, tr("Session files (*.vdb)"));
    if (!fileName.isEmpty()) {
        // Go through all contexts
        foreach (Context *ctx, mRegistry.values()) {
            // save session for each context
            Session session;
            session.setFile(fileName);

            // \todo this includes saving connection infos, tab infos, widget sizes, etc.
            session.setConnectionInfo(ctx->connectionInfo());
        }
    }
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
 * \todo comment
 */
void
Director::treeTabClosed(Gui::TreeWidget *iTree)
{
    disconnectRequested(findContext(iTree));
}

/*!
 * \todo comment
 */
void
Director::treeTabChanged(Gui::TreeWidget *iTree)
{
    mMainWindow->activateScene(findScene(findContext(iTree)));
}

/*!
 *
 */
void
Director::tabChanged(Gui::SceneWidget *iScene)
{
    mMainWindow->activateTree(findTree(findContext(iScene)));
}

} // namespace Control


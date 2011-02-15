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

#include <control/Context.h>
#include <control/Director.h>
#include <gui/MainWindow.h>
#include <gui/SceneWidget.h>
#include <gui/SqlConnectionDialog.h>
#include <gui/TreeWidget.h>

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
//    qDeleteAll(mRegistry);
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
        connect(mMainWindow, SIGNAL(disconnectRequest()), this, SLOT(disconnectRequested()));
        connect(mMainWindow, SIGNAL(optionsDialogRequest()), this, SLOT(optionsDialogRequested()));
        connect(mMainWindow, SIGNAL(saveSessionRequest()), this, SLOT(saveSessionRequested()));
        connect(mMainWindow, SIGNAL(exitRequest()), this, SLOT(exitRequested()));

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
    QList<QWidget *>::const_iterator iter = mRegistry.keys(iContext).begin();

    quint32 count = 0;
    for (; iter != mRegistry.keys(iContext).end(); ++iter) {
        oWidgets.push_back(*iter);
        ++count;
    }

    return count;
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
    mMainWindow->addScene(scene, "Scene: " + tabTitle);
    add(scene, ctx);

    // create tree for it and register it
    TreeWidget *tree = new TreeWidget();
    mMainWindow->addTree(tree, tabTitle);
    add(tree, ctx);

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
 * Slot for handling options dialog request. Executed when a user chooses options... in
 * file dialog.
 */
void
Director::optionsDialogRequested()
{
    // \todo Implement
}

/*!
 * Slot for handling reload data request. Executed when a user clicks 'Reload' button.
 */
void
Director::reloadDataRequested()
{
    // \todo Find the active context
    // \todo Do reload for this context
}

/*!
 * Slot for handling disconnect request. Executed when a user clicks 'Disconnect button'.
 */
void
Director::disconnectRequested()
{
    Context *ctx/* \todo get active context */;
    mDbMgr.remove(ctx);
}

/*!
 * Slot for handling request to save session. Executed when a user closes main window and
 * confirm saving active sessions.
 */
void
Director::saveSessionRequested()
{
    qDebug() << "Director::saveSessionRequested>";
    // \todo Implement
    // \todo Go through all contexts
    // \todo save session for each context
    // this includes saving connection infos, tab infos, widget sizes, etc.
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

} // namespace Control


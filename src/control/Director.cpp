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
#include <gui/SqlConnectionDialog.h>

#include <QMessageBox>

#include <QtDebug>

namespace Control
{

/*!
 * Constructor
 */
Director::Director(QObject *iParent)
    : QObject(iParent),
      mSplashScreen(QPixmap(":/img/splashscreen.png")),
      mFSM(),
      mInitialState(),
      mIdleState(),
      mBusyState(),
      mMainWindow(0),
      mRegistry(),
      mDbMgr()
{
    mSplashScreen.show();
//    mSplashScreen.showMessage("Loading...");

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

void
Director::start()
{
    mMainWindow->show();
    emit logMessageRequest("Started...");
}

/*!
 *
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
 *
 */
void
Director::busyStateEntered()
{
    // \todo show progress bar
}

/*!
 *
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
 *
 */
void
Director::saveSessionRequested()
{
    qDebug() << "Director::saveSessionRequested>";
    // \todo Implement
    // \todo Go through all contexts
    // \todo save session for each context
}

/*!
 * Slot for handling application exit. Executed when a user closes main window and
 * confirms its closing.
 */
void
Director::exitRequested()
{
    qDebug() << "Director::exitRequested>";
    // \todo Do cleanup
//    qApp->exit();
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
//    connDialog.setConnectionInfos(QVector);

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
        // \todo if error occurs, we need to show message dialog
    } while (!ctx);

    // if we got here, then we have a valid ctx
    emit logMessageRequest(QString("Connected to '%1'").arg(ctx->connectionInfo().dbHostInfo().dbName()));
    // \todo create scene and tree for it
    // \todo register them
}

} // namespace Control


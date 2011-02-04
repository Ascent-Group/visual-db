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

#include <control/Director.h>
#include <gui/MainWindow.h>


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
      mMainWindow(0)
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

    // do the initialization
    if (!initialize()) {
        qDebug() << "[[31mERROR[0m] Director initialization failed!";
        // terminate the app
        // we cannot use qApp->quit or qApp->exit() here because the main loop has not
        // started yet
        throw -1;
    }
}

/*!
 * Destructor
 */
Director::~Director()
{
}

void
Director::start()
{
    mMainWindow->show();
}

/*!
 *
 */
bool
Director::initialize()
{
    using namespace Gui;

    try {
        // main window
        mMainWindow = new MainWindow();
        mSplashScreen.finish(mMainWindow);
    } catch (...) {
        return false;
    }

    return true;
}

/*!
 *
 */
void
Director::busyStateEntered()
{

}

/*!
 *
 */
void
Director::busyStateExited()
{

}

} // namespace Control


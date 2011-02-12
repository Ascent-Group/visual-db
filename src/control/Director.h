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

#ifndef CONTROL_DIRECTOR_H
#define CONTROL_DIRECTOR_H

#include <connect/ConnectionInfo.h>
#include <control/DatabaseManager.h>
#include <QObject>
#include <QSplashScreen>
#include <QStateMachine>

#include <QtDebug>

namespace Gui
{
class MainWindow;
}

namespace Control
{

class Context;

/*!
 * \class Director
 * \headerfile control/Director.h
 * \brief Class that represents the environment of current execution.
 */
class Director : public QObject
{
    Q_OBJECT

    public:
        Director(QObject *iParent = 0);
        ~Director();

        void start();
        bool initialize();

    private:
        QSplashScreen mSplashScreen;
        QStateMachine mFSM;

        QState mInitialState;
        QState mIdleState;
        QState mBusyState;

        Gui::MainWindow *mMainWindow;

        QMap<QWidget*, Control::Context*> mRegistry;
        Control::DatabaseManager mDbMgr;

        void showConnectionDialog(bool iLoadSession);

    private slots:
        // FSM slots
        void busyStateEntered();
        void busyStateExited();

        // slots for processing requests from ui
        void connectionDialogRequested();
        void reloadDataRequested();
        void optionsDialogRequested();

        void saveSessionRequested();
        void exitRequested();

    signals:
        // signals for internal use
        void initializationComplete();
        void requestReceived();
        void requestProcessed();

        void logMessageRequest(const QString &);

};

} // namespace Control

#endif // CONTROL_DIRECTOR_H


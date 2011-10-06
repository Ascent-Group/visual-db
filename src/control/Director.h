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
#include <control/ContextMenuManager.h>
#include <control/Converter.h>
#include <control/DatabaseManager.h>
#include <QObject>
//#include <QSplashScreen>
#include <QStateMachine>

#include <QtDebug>

#if DEBUG
class DirectorTest;
#endif

namespace Gui
{
class MainWindow;
class SceneWidget;
class TreeWidget;
}

namespace Control
{

class Context;

/*!
 * \class Director
 * \headerfile control/Director.h
 * \brief Main entity that controls the whole workflow of the application.
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
        // \note maybe this should go to main()
//        QSplashScreen mSplashScreen;
        QStateMachine mFSM;

        QState mInitialState;
        QState mIdleState;
        QState mBusyState;

        Gui::MainWindow *mMainWindow;

        /*!
         * Many-to-one registry. Used for correct request handling taking into
         * consideration the fact that several multiple connections are supported.
         * We are probably allowed to have a map of non-consts here because director is
         * the one who is responsible for their modifications. As for the function
         * parameters of other modules, then they will definitely accpet const pointers.
         */
        QMap<QWidget *, Control::Context *> mRegistry;
        Control::ContextMenuManager mMenuMgr;
        Control::DatabaseManager mDbMgr;
        Control::Converter mConverter;

        bool add(QWidget *, Control::Context *);
        bool remove(QWidget *);
        bool remove(Control::Context *);

        Control::Context* findContext(QWidget *) const;
        quint32 findWidgets(Control::Context *, QVector<QWidget*> &oWidgets) const;

        Gui::TreeWidget* findTree(Control::Context *) const;
        Gui::SceneWidget* findScene(Control::Context *) const;

        void showConnectionDialog(bool iLoadSession);
        void disconnectRequested(Control::Context *);

    private slots:
        // FSM slots
        void busyStateEntered();
        void busyStateExited();

        // slots for processing requests from ui
        void connectionDialogRequested();
        void reloadDataRequested();
        void optionsDialogRequested();

        void loadSessionRequested();
        void saveSessionRequested();
        void restoreSession(const QString &iFileName);
        void exitRequested();

        void treeTabClosed(Gui::TreeWidget *iTree);
        void treeTabChanged(Gui::TreeWidget *iTree);

        void tabChanged(Gui::SceneWidget *iScene);
        void tabClosed(QWidget *iWidget);

        /*! Slots for handling context menu actions */
        void expandAllTreeItems(Gui::TreeWidget *, bool);
        void addTreeItemsToScene();
        void describeItems();
        void queryItems();

    signals:
        // signals for internal use
        /*! Used to signal the state machine that we can enter idle state. */
        void initializationComplete();
        /*! Used to signal the state machine that we should enter busy state. */
        void requestReceived();
        /*! Used to signal the state machine that the request has been processed and we
         * may leave busy state.*/
        void requestProcessed();
        /*! Used to print messages to log panel */
        void logMessageRequest(const QString &);

    private:
#if DEBUG
        friend class ::DirectorTest;
#endif
};

QString connectionName(const Control::Context *iCtx);

} // namespace Control

#endif // CONTROL_DIRECTOR_H


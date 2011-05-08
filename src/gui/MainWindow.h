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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <connect/ConnectionInfo.h>
#include <gui/ui/ui_MainWindow.h>
#include <QMainWindow>
#include <QPointF>

class QColorDialog;
class QFileDialog;
class QGraphicsItem;
class QPrinter;
class QProgressBar;
class QTreeWidgetItem;
class QUndoCommand;
class QUndoStack;

namespace Gui {

class GraphicsScene;
class GraphicsView;
class SceneWidget;
class TreeWidget;

/*!
 * \class MainWindow
 * \headerfile gui/MainWindow.h
 * \brief Main window. Container for all widgets of the application.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow();
        ~MainWindow();

        void addScene(Gui::SceneWidget *, const QString &) const;
        void removeScene(Gui::SceneWidget *);
        void activateScene(Gui::SceneWidget *) const;

        void addTree(Gui::TreeWidget *, const QString &) const;
        void activateTree(Gui::TreeWidget *) const;

        Gui::TreeWidget* activeTree() const;



        void setEnableForActions(bool iFlag);

    public slots:
        void printMsg(const QString &) const;

    private:
        Ui::MainWindow ui;

        // These actions are undo stack actions
        // Designer does NOT let us create them
        // => declared here, not in designer
        //QAction *mUndoAction;
        //QAction *mRedoAction;

        // These menus initially are not attached
        // to any gui element
//        QMenu *mGroupMenu;

        QProgressBar *mProgressBar;

        QUndoStack *mUndoStack;

        static const int STATUS_BAR_DEFAULT_WIDTH = 700;
        static const int STATUS_BAR_DEFAULT_HEIGHT = 20;

    private:
        void createActions();
        void createMenus();
        void updateSessionMenu();
        void createStatusBar();

        void closeEvent(QCloseEvent *iEvent);

        void saveToXml(const QString &);
        void loadFromXml(QString);

        void initSession();

    private slots:
        void addCommand(QUndoCommand*);
        bool saveSession();
        void loadLastSession();
        void setFullScreen(bool);
        void queryData();
        void describeObject();
        void drawFullDbScheme();
        void addItem();
        void addItem(QTreeWidgetItem *, int);
        void showDatabaseTree(bool);
        void closeDatabaseTree();
        void showDatabaseTree();
        void closeLogPanel();
        void showLogPanel();
        void printPreview(QPrinter *);
        void showPrintPreviewDialog();
        void showPrintDialog();

        void treeTabCloseRequested(int);
        void treeTabChanged(int);

        void tabChanged(int);

    signals:
        void connectionDialogRequest();
        void reloadDataRequest();
//        void disconnectRequest();
        void optionsDialogRequest();

        void loadSessionRequest();
        void saveSessionRequest();
        void exitRequest();

        void treeTabClosed(Gui::TreeWidget *iTree);
        void treeTabChanged(Gui::TreeWidget *iTree);

        void tabChanged(Gui::SceneWidget *iScene);
};

}

#endif // MAINWINDOW_H


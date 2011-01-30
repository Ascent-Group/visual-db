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
#include <QSettings>
#include <QPointF>

class GraphicsScene;
class GraphicsView;
class QColorDialog;
class QFileDialog;
class QGraphicsItem;
class QMenu;
class QPrinter;
class QProgressBar;
class QTreeWidgetItem;
class QUndoCommand;
class QUndoStack;

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
        QMenu *mTreeItemMenu;
//        QMenu *mGroupMenu;

        QProgressBar *mProgressBar;

        QSettings mSettings;
        Connect::ConnectionInfo mConnectionInfo;

        QUndoStack *mUndoStack;

        static const int STATUS_BAR_DEFAULT_WIDTH = 700;
        static const int STATUS_BAR_DEFAULT_HEIGHT = 20;

    private:
        void createActions();
        void setEnableForActions(bool iFlag);
        //  void showConnectionDialog(DbParameters *, ProxyParameters *, bool);
        void createMenus();
        void updateSessionMenu();
        void createStatusBar();

        void closeEvent(QCloseEvent *iEvent);

        void saveToXml(const QString &);
        void loadFromXml(QString);

        void initSession();

    private slots:
        // this slot is not standard that's why it is declared here
        int showConnectionDialog(bool iLoadSession = false);
        void addCommand(QUndoCommand*);
        void reloadData();
        bool saveSession();
        void loadSession();
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
        void showLogPanel(bool);
        void closeLogPanel();
        void showLogPanel();
        void printPreview(QPrinter*);
        void showOptionsDialog();
        void showPrintPreviewDialog();
        void showPrintDialog();

};

#endif // MAINWINDOW_H


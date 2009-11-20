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

#include <QMainWindow>
#include <QSettings>
#include <QPointF>

class AddTableCommand;
class DbParameters;
class GraphicsScene;
class GraphicsView;
class ProxyParameters;
class QAction;
class QColorDialog;
class QFileDialog;
class QGraphicsItem;
class QMenu;
class QMenuBar;
class QPrinter;
class QProgressBar;
class QTreeWidgetItem;
class QUndoStack;
class MoveTableCommand;
class SceneWidget;
class TableItem;
class TableItemGroup;
class TabWidget;
class TreeWidget;

/*
 * Main window. Container for all widgets of the application.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
	MainWindow();

    private:
        TreeWidget*mTree;

        QMenu *mFileMenu;
	QMenu *mEditMenu;
        QMenu *mViewMenu;
        QMenu *mSchemeMenu;
        QMenu *mTableMenu;
        QMenu *mTreeItemMenu;
        QMenu *mGroupMenu;
        QMenu *mSessionMenu;

        QAction	*mExitAction;
        QAction	*mNewConnectionAction;
        QAction *mShowOptionsDialogAction;
        QAction	*mShowDockTablesListAction;
        QAction	*mShowPrintPreviewDialogAction;
        QAction	*mShowPrintDialogAction;
	QAction	*mUndoAction;
	QAction	*mRedoAction;
        QAction	*mDeleteTableAction;
        QAction	*mAddTableAction;
        QAction	*mShowFieldsTypesAction;
        QAction	*mHideFieldsTypesAction;
        QAction	*mShowIndicesAction;
        QAction	*mHideIndicesAction;
        QAction	*mSetTableColorAction;
        QAction	*mSelectAllTablesAction;
        QAction	*mRemoveAllTablesAction;
        QAction	*mAdjustTableSizeAction;
        QAction	*mSaveToImgAction;
        QAction	*mDrawFullDBAction;
        QAction	*mGroupItemsAction;
        QAction	*mUngroupItemsAction;
        QAction	*mColorizeAccordingSchemasAction;
        QAction	*mShowGridAction;
	QAction *mAttachToGridAction;
	QAction *mAnchorAction;
	QAction *mWeightAnchorAction;
        QAction	*mDivideOnPagesAction;
        QAction	*mShowControlWidgetAction;
        QAction	*mShowLegendAction;
        QAction	*mSelectAllTablesInSchemaAction;
        QAction *mDescribeObjectAction;
        QAction *mQueryDataAction;
        QAction *mSaveSessionAction;
        QAction *mLoadSessionAction;
        QAction *mLastSessionAction;
        QAction	*mSetFullScreenAction;
        QAction *mReloadDataAction;
        QMenuBar *mMenuBar;
        QToolBar *mToolBar;

        QProgressBar *mProgressBar;

        SceneWidget *mSceneWidget;

        TabWidget *mTabWidget;

        QDockWidget *mDockTableListWidget;

        QSettings mSettings;
        DbParameters *mDbParameters;
	ProxyParameters *mProxyParameters;

	QUndoStack *mUndoStack;

        static const int STATUS_BAR_DEFAULT_WIDTH = 700;
        static const int STATUS_BAR_DEFAULT_HEIGHT = 20;

    private:
	void createActions();
	void setEnableForActions(bool ipFlag);
//	void showConnectionDialog(DbParameters *, ProxyParameters *, bool);
	void createMenus();
	void updateSessionMenu();
        void createToolBar();
	void createStatusBar();
	void createDockWindows();

        void closeEvent(QCloseEvent *ipEvent);
	
	void saveToXml(QString);
	void loadFromXml(QString);

        void initSession();

    private slots:
	int showConnectionDialog(bool ipLoadSession = false);
        void drawFullDbScheme();
        void showOptionsDialog();
	void showPrintPreviewDialog();
	void showPrintDialog();
	void setDockTableListVisible(bool);
	void addTableItem();
	void addTableItem(QTreeWidgetItem *, int);
        void describeObject();
        void queryData();
	void saveSession();
	void loadSession();
	void loadLastSession();
	void setFullScreen(bool);
	void printPreview(QPrinter *);
	void reloadData();
	void addCommand(MoveTableCommand *);
	void addCommand(AddTableCommand *);
};

#endif // MAINWINDOW_H

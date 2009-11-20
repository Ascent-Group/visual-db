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

#ifndef SQLWIDGET_H
#define SQLWIDGET_H

#include <QWidget>

class QAction;
class QTableWidget;
class QTextEdit;
class QToolBar;
class QToolButton;

/*
 * Provide the execution of different types of queries in this widget.
 */ 
class SqlWidget : public QWidget
{
    Q_OBJECT

    public:
        SqlWidget(QWidget *ipParent = 0);
        ~SqlWidget();

        void setDefaultQuery(QString ipQueryText);

    private:
        enum Portions {
            PreviousPortion = 0,
            NextPortion
        };

        enum Position {
            InterimPage = 0,
            LastPage,
            EndPage
        };

        QToolButton *mSafeQueryButton;
        QTextEdit *mQueryEdit;
        QTableWidget *mTable;
        QToolBar *mToolBar;

        QAction *mRunAction;
        QAction *mPrevPortionAction;
        QAction *mNextPortionAction;
        QAction *mFullResultAction;

        unsigned int mPortionSize;
        int mOffset;
        Position mPos;

    private:
        void createActions();
        void readPortion(Portions ipDirection);

    private slots:
        void runQuery();
        void readPrevPortion();
        void readNextPortion();
        void readFullResult();
};

bool isQuerySafe(const QString &ipQueryText);

#endif // SQLWIDGET_H

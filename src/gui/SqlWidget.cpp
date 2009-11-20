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

#include <gui/MainWindow.h>
#include <gui/SqlWidget.h>
#include <QAction>
#include <QGridLayout>
#include <QMessageBox>
#include <QSplitter>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QStatusBar>
#include <QTableWidget>
#include <QTextEdit>
#include <QToolBar>
#include <QToolButton>

#include <QtDebug>

/*
 * Ctor
 */
SqlWidget::SqlWidget(QWidget *ipParent)
    : QWidget(ipParent), mPortionSize(30), mOffset(-1), mPos(SqlWidget::InterimPage)
{
    // create tool bar
    mToolBar = new QToolBar();

    createActions();

    // populate toolbar with action
    mToolBar->addAction(mRunAction);
    mToolBar->addAction(mPrevPortionAction);
    mToolBar->addAction(mNextPortionAction);
    mToolBar->addAction(mFullResultAction);

    // create checkbox
    mSafeQueryButton = new QToolButton();
    mSafeQueryButton->setIcon(QIcon(":/img/safequery.png"));
    mSafeQueryButton->setToolTip(tr("Safe query"));
    mSafeQueryButton->setCheckable(true);
    mSafeQueryButton->setChecked(true);
    // lyuts: temporary disable button
    mSafeQueryButton->setEnabled(false);
    mToolBar->addWidget(mSafeQueryButton);

    // create horizontal layout
    QHBoxLayout *hlayout = new QHBoxLayout();
    //hlayout->addWidget(mSafeQueryBox);
    hlayout->addWidget(mToolBar);

    // create edit for query
    mQueryEdit = new QTextEdit();

    // create a table for sql result
    mTable = new QTableWidget();
    mTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mTable->setAlternatingRowColors(true);

    // create spliiter
    QSplitter *splitter = new QSplitter(Qt::Vertical);
    splitter->addWidget(mQueryEdit);
    splitter->addWidget(mTable);

    // create layout
    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->addLayout(hlayout, 0, 0, Qt:: AlignLeft);
    mainLayout->addWidget(splitter, 1, 0);

}

/*
 * Dtor
 */
SqlWidget::~SqlWidget()
{

}

/*
 * Sets default query for db object
 */
void
SqlWidget::setDefaultQuery(QString ipQueryText)
{
    mQueryEdit->setText(ipQueryText);
}

/*
 * Run button click slot
 */
void
SqlWidget::runQuery()
{
    QString qstr = mQueryEdit->toPlainText();

    // if safe query if checked
    if (mSafeQueryButton->isChecked()) {
        if ( ! isQuerySafe(qstr) ) {

            QMessageBox::information(
                    this,
                    tr("Safe query guard"),
                    tr("The query you are running is not safe!"),
                    tr("Ok"));
            return;
        }
    }

    mOffset = -1;
    readPortion(SqlWidget::NextPortion);

    MainWindow *mainWindow = dynamic_cast<MainWindow*>(parentWidget()->parentWidget()->parentWidget());
    mainWindow->statusBar()->showMessage(tr("Query succeeded"));
}

/*
 * Reads the previous portion of results
 */
void
SqlWidget::readPrevPortion()
{
    readPortion(SqlWidget::PreviousPortion);
}

/*
 * Reads the next portion of results
 */
void
SqlWidget::readNextPortion()
{
    readPortion(SqlWidget::NextPortion);
}

/*
 * Reads full result
 */
void
SqlWidget::readFullResult()
{
    // save orignial size of portion
    int origPortionSize = mPortionSize;

    // set maximum portion size
    mPortionSize = static_cast<unsigned int>(-1);
    mOffset = -1;
    mPos = SqlWidget::InterimPage;

    // read full result
    readPortion(SqlWidget::NextPortion);

    // restore portion
    mPortionSize = origPortionSize;

    // disable buttons
    mPrevPortionAction->setEnabled(false);
    mNextPortionAction->setEnabled(false);
}

/*
 * Creates actions
 */
void
SqlWidget::createActions()
{
    // create action for running queries
    mRunAction = new QAction(tr("Run"), this);
    mRunAction->setShortcut(Qt::Key_F8);
    mRunAction->setIcon(QIcon(":/img/run.png"));
    mRunAction->setStatusTip(tr("Execute query (F8)"));
    connect(mRunAction, SIGNAL(triggered()), this, SLOT(runQuery()));

    // create action for browsing previous portion of records
    mPrevPortionAction = new QAction(tr("Previous portion"), this);
    mPrevPortionAction->setIcon(QIcon(":/img/uparrow.png"));
    mPrevPortionAction->setStatusTip(tr("Show previous portion of results"));
    connect(mPrevPortionAction, SIGNAL(triggered()), this, SLOT(readPrevPortion()));

    // create action for browsing next portion of records
    mNextPortionAction = new QAction(tr("Next portion"), this);
    mNextPortionAction->setIcon(QIcon(":/img/downarrow.png"));
    mNextPortionAction->setStatusTip(tr("Show next portion of results"));
    connect(mNextPortionAction, SIGNAL(triggered()), this, SLOT(readNextPortion()));

    // create action for browsing full result
    mFullResultAction = new QAction(tr("Full result"), this);
    mFullResultAction->setIcon(QIcon(":/img/fulldownarrow.png"));
    mFullResultAction->setStatusTip(tr("Show full result"));
    connect(mFullResultAction, SIGNAL(triggered()), this, SLOT(readFullResult()));
}

/*
 * Checks the query for safety
 * lyuts: later will allow users to define and use their own rules
 */
bool
isQuerySafe(const QString &ipQueryText)
{
    if (ipQueryText.contains("DELETE", Qt::CaseInsensitive)
        || ipQueryText.contains("DROP", Qt::CaseInsensitive)
        || ipQueryText.contains("INSERT", Qt::CaseInsensitive)
        || ipQueryText.contains("UPDATE", Qt::CaseInsensitive)
        ) {

        return false;
    }

    return true;
}

/*
 * Reads a portion of records
 */
void
SqlWidget::readPortion(Portions ipDirection)
{
    QSqlDatabase db = QSqlDatabase::database("mainConnect");
    QSqlQuery query(db);
    QString qstr = mQueryEdit->toPlainText().trimmed();

    switch (ipDirection) {
        case SqlWidget::PreviousPortion:
                            if (0 < mOffset && (SqlWidget::EndPage != mPos)) {
                                --mOffset;
                            }
                            break;
        case SqlWidget::NextPortion:
                            //if (0 <= mOffset || -1 == mOffset) {
                                ++mOffset;
                            //}
                            break;
    }

    // if ends with colon
    if (qstr.endsWith(";")) {
        //remove it
        qstr.chop(1);
    }

    qstr.append(QString(" LIMIT %1 OFFSET %2;")
                .arg(mPortionSize)
                .arg((mOffset)*mPortionSize));

    qDebug() << qstr;

    // renew table
    mTable->clear();
    mTable->setColumnCount(0);
    mTable->setRowCount(0);

    // if query execution failed
    if (!query.exec(qstr)) {
        QMessageBox::critical(
                this,
                tr("Query failed"),
                query.lastError().text(),
                tr("Ok"));

    }

    // get columns count
    QSqlRecord record = query.record();

    int columnsCount = record.count();

    mTable->setColumnCount(columnsCount);

    // set column labels
    QStringList labels;
    for (int i = 0; i < columnsCount; ++i) {
        labels << record.fieldName(i);
    }

    mTable->setHorizontalHeaderLabels(labels);

    // if some data has been read
    if (query.first()) {

        int row = 0;
        // for each row
        do {
            // add empty row
            mTable->insertRow(row);

            // populate the new row
            for (int i = 0; i < columnsCount; ++i) {
                QTableWidgetItem *item = new QTableWidgetItem();

                item->setText(query.value(i).toString());

                mTable->setItem(row, i, item);
            }

            ++row;
        } while (query.next());

        // if we reached end
        if (mPortionSize > (unsigned int)row) {
            mPos = SqlWidget::LastPage;
        } else {
            mPos = SqlWidget::InterimPage;
        }

    } else {

        // nothing found, restore the offset
        if (0 < mOffset/* && NextPortion == ipDirection*/) {
            --mOffset;
            mPos = SqlWidget::EndPage;
        }
    }

    bool disableNext;
    // update button states
    if (SqlWidget::LastPage == mPos || SqlWidget::EndPage == mPos) {
        disableNext = true;
    } else {
        disableNext = false;
    }

    mNextPortionAction->setDisabled(disableNext);
    mPrevPortionAction->setDisabled( (0 == mOffset) ? true : false);

    query.clear();

    mTable->resizeColumnsToContents();
    mTable->resizeRowsToContents();

}

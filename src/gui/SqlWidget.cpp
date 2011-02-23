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
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QStatusBar>

#include <QtDebug>

namespace Gui {

/*!
 * Ctor
 */
SqlWidget::SqlWidget(QWidget *iParent)
    : QWidget(iParent), mPortionSize(30), mOffset(-1), mPos(SqlWidget::InterimPage)
{
    ui.setupUi(this);

    // \note: we are using QFrame instead of QToolBar
    // reason: QtDesigner doesn't support it for non-MainWindow gui

}

/*!
 * Dtor
 */
SqlWidget::~SqlWidget()
{

}

/*!
 * \brief Sets default query for db object
 *
 * \param[in] iQueryText - Default query text
 */
void
SqlWidget::setDefaultQuery(const QString &iQueryText)
{
    ui.mQueryEdit->setText(iQueryText);
}

/*!
 * \brief Run button click slot
 */
void
SqlWidget::runQuery()
{
    QString qstr = ui.mQueryEdit->toPlainText();

    // if safe query if checked
    if (ui.mSafeQueryButton->isChecked()) {
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

    MainWindow *mainWindow = dynamic_cast<MainWindow*>(parentWidget()->parentWidget()->parentWidget()->parentWidget());

    mainWindow->statusBar()->showMessage(tr("Query succeeded"));
    mainWindow->printMsg("Query successfully executed.");
}

/*!
 * \brief Reads the previous portion of results
 */
void
SqlWidget::readPrevPortion()
{
    readPortion(SqlWidget::PreviousPortion);
}

/*!
 * \brief Reads the next portion of results
 */
void
SqlWidget::readNextPortion()
{
    readPortion(SqlWidget::NextPortion);
}

/*!
 * \brief Reads full result
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
    ui.mPrevPortionButton->setEnabled(false);
    ui.mNextPortionButton->setEnabled(false);
}

/*!
 * \brief Checks the query for safety
 * lyuts: later will allow users to define and use their own rules
 *
 * \param[in] iQueryText - Query text we will check for safety
 *
 * \return True if query is safe, false otherwise
 */
bool
isQuerySafe(const QString &iQueryText)
{
    if (iQueryText.contains("DELETE", Qt::CaseInsensitive)
        || iQueryText.contains("DROP", Qt::CaseInsensitive)
        || iQueryText.contains("INSERT", Qt::CaseInsensitive)
        || iQueryText.contains("UPDATE", Qt::CaseInsensitive)
        ) {

        return false;
    }

    return true;
}

/*!
 * \brief Reads a portion of records
 *
 * \param[in] iDirection - Direction we will read in (next or previous)
 */
void
SqlWidget::readPortion(Portions iDirection)
{
    // todo reimplement according to new design
    QSqlDatabase db = QSqlDatabase::database("mainConnect");
    QSqlQuery query(db);
    QString qstr = ui.mQueryEdit->toPlainText().trimmed();

    switch (iDirection) {
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
    ui.mTable->clear();
    ui.mTable->setColumnCount(0);
    ui.mTable->setRowCount(0);

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

    ui.mTable->setColumnCount(columnsCount);

    // set column labels
    QStringList labels;
    for (int i = 0; i < columnsCount; ++i) {
        labels << record.fieldName(i);
    }

    ui.mTable->setHorizontalHeaderLabels(labels);

    // if some data has been read
    if (query.first()) {

        int row = 0;
        // for each row
        do {
            // add empty row
            ui.mTable->insertRow(row);

            // populate the new row
            for (int i = 0; i < columnsCount; ++i) {
                QTableWidgetItem *item = new QTableWidgetItem();

                item->setText(query.value(i).toString());

                ui.mTable->setItem(row, i, item);
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
        if (0 < mOffset/* && NextPortion == iDirection*/) {
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

    ui.mNextPortionButton->setDisabled(disableNext);
    ui.mPrevPortionButton->setDisabled( (0 == mOffset) ? true : false);

    query.clear();

    ui.mTable->resizeColumnsToContents();
    ui.mTable->resizeRowsToContents();

}

}

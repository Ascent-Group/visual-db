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

#include <gui/LogPanel.h>
#include <QFileDialog>
#include <QMessageBox>

LogPanel* LogPanel::mInstance = 0;

/*
 * Ctor
 */
LogPanel::LogPanel(QWidget *ipParent)
    : QWidget(ipParent)
{
    ui.setupUi(this);

    connect(ui.mSaveButton, SIGNAL(clicked()), this, SLOT(saveToFile()));
}

/*
 * Dtor
 */
LogPanel::~LogPanel()
{
}

/*
 * Saves current log to a file
 */
void
LogPanel::saveToFile()
{
    QString text = ui.mOutputEdit->toPlainText();

    if (0 < text.trimmed().length()) {
	QString fileName = QFileDialog::getSaveFileName(this);
	QFile file(fileName);

	if (!file.open(QIODevice::Text)) {
	    QMessageBox::critical(this, tr("Error"), tr("Unable to open file!"), tr("Ok"));
	    return;
	}

	file.write(text.toLocal8Bit());

	file.close();
    }
}

/*
 *
 */
LogPanel*
LogPanel::instance(QWidget *ipParent)
{
    // if not yet created => create
    if (0 == mInstance) {
	mInstance = new LogPanel(ipParent);
    }
    
    return mInstance;
}

/*
 *
 */
void
LogPanel::print(QString ipText)
{
    instance()->ui.mOutputEdit->append(ipText);
}

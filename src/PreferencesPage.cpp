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

#include <QCheckBox>
#include <QFileDialog>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>

#include <PreferencesPage.h>

/*
 * Constructor
 */
PreferencesPage::PreferencesPage(QWidget *ipParent)
    : QWidget(ipParent)
{
    // create checkbox for hide/show indices
    mShowIndicesBox = new QCheckBox();
    mShowIndicesBox->setText(tr("Show indices"));
    // set this flag to the one from settings
    mShowIndicesBox->setChecked(mSettings.value("Preferences/ShowIndices", false).toBool());

    // create spin box to select count of saved sessions
    mCountSessionsSpin = new QSpinBox();
    mCountSessionsSpin->setValue(mSettings.value("Preferences/CountSavedSessions", 10).toInt());
    mCountSessionsSpin->setMaximumWidth(50);

    // choose directory where sessions will be saved
    mSessionDirectoryEdit = new QLineEdit(mSettings.value("Preferences/SessionFolder", "./").toString());
    mSessionDirectoryEdit->setReadOnly(true);
    mSessionDirectoryButton = new QPushButton();
    mSessionDirectoryButton->setIcon(QIcon(":img/folder.png"));
    connect(mSessionDirectoryButton, SIGNAL(clicked()), this, SLOT(folder()));

    // create check box for auto switch to a new tab
    mNewTabAutoSwitchBox = new QCheckBox();
    mNewTabAutoSwitchBox->setText(tr("Automatically switch to a new tab"));
    // set this flag to the one from settings
    mNewTabAutoSwitchBox->setChecked(mSettings.value("Preferences/NewTabAutoSwitch", true).toBool());

    // load or not last saved session
    mLoadLastSessionBox = new QCheckBox();
    mLoadLastSessionBox->setText(tr("Automatically load last session when open application"));
    // set this flag to the one from settings
    mLoadLastSessionBox->setChecked(mSettings.value("Preferences/LoadLastSession", false).toBool());

    // create mainLayout
    QGridLayout *mainLayout = new QGridLayout(this);

    // populate main layout
    mainLayout->addWidget(mShowIndicesBox, 0, 0);
    mainLayout->addWidget(new QLabel(tr("Sessions limit")), 1, 0);
    mainLayout->addWidget(mCountSessionsSpin, 1, 1);
    mainLayout->addWidget(new QLabel(tr("Session directory:")), 2, 0);
    mainLayout->addWidget(mSessionDirectoryEdit, 2, 1);
    mainLayout->addWidget(mSessionDirectoryButton, 2, 2);
    mainLayout->addWidget(mNewTabAutoSwitchBox, 3, 0, 1, 2);
    mainLayout->addWidget(mLoadLastSessionBox, 4, 0, 1, 2);

//    mainLayout->addWidget(new QLabel(tr("Under constrution... Sorry...")));
}

/*
 * Destructor
 */
PreferencesPage::~PreferencesPage()
{
}

/*
 * Get flag - show or not indices
 */
bool
PreferencesPage::showIndices() const
{
    return mShowIndicesBox->isChecked();
}

/*
 * Get session folder
 */
QString
PreferencesPage::sessionFolder() const
{
    return mSessionDirectoryEdit->text();
}

/*
 *
 */
bool
PreferencesPage::newTabAutoSwitch() const
{
    return mNewTabAutoSwitchBox->isChecked();
}

/*
 * Get the folder where the sessions are saved
 */
void
PreferencesPage::folder()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::DirectoryOnly);
    QStringList fileNames;
    if (dialog.exec()) {
	fileNames = dialog.selectedFiles();
	mSessionDirectoryEdit->setText(fileNames.first() + "/");
    }
}

/*
 * Get count of saved sessions
 */
int
PreferencesPage::countSavedSession() const
{
    return mCountSessionsSpin->value();
}

/*
 * Get the flag - load or not last session when application is opened
 */
bool
PreferencesPage::loadLastSession() const
{
    return mLoadLastSessionBox->isChecked();
}

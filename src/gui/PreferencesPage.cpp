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

#include <gui/PreferencesPage.h>
#include <QFileDialog>

/*
 * Constructor
 */
PreferencesPage::PreferencesPage(QWidget *ipParent)
    : QWidget(ipParent)
{
    ui.setupUi(this);

    // set this flag to the one from settings
    ui.mShowIndicesBox->setChecked(mSettings.value("Preferences/ShowIndices", false).toBool());

    // create spin box to select count of saved sessions
    ui.mCountSessionsSpin->setValue(mSettings.value("Preferences/CountSavedSessions", 10).toInt());

    // choose directory where sessions will be saved
    ui.mSessionDirectoryEdit->setText(mSettings.value("Preferences/SessionFolder", "./").toString());

    // set this flag to the one from settings
    ui.mNewTabAutoSwitchBox->setChecked(mSettings.value("Preferences/NewTabAutoSwitch", true).toBool());

    // set this flag to the one from settings
    ui.mLoadLastSessionBox->setChecked(mSettings.value("Preferences/LoadLastSession", false).toBool());

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
    return ui.mShowIndicesBox->isChecked();
}

/*
 * Get session folder
 */
QString
PreferencesPage::sessionFolder() const
{
    return ui.mSessionDirectoryEdit->text();
}

/*
 *
 */
bool
PreferencesPage::newTabAutoSwitch() const
{
    return ui.mNewTabAutoSwitchBox->isChecked();
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
    ui.mSessionDirectoryEdit->setText(fileNames.first() + "/");
    }
}

/*
 * Get count of saved sessions
 */
int
PreferencesPage::countSavedSession() const
{
    return ui.mCountSessionsSpin->value();
}

/*
 * Get the flag - load or not last session when application is opened
 */
bool
PreferencesPage::loadLastSession() const
{
    return ui.mLoadLastSessionBox->isChecked();
}

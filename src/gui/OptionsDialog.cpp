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

#include <gui/AppearancePage.h>
#include <gui/ColorsPage.h>
#include <gui/OptionsDialog.h>
#include <gui/PreferencesPage.h>

#include <consts.h>

#include <QtDebug>

/*!
 * Constructor
 */
OptionsDialog::OptionsDialog(QWidget *ipParent)
    : QDialog(ipParent)
{
    ui.setupUi(this);
}

/*!
 * Destructor
 */
OptionsDialog::~OptionsDialog()
{
}

/*!
 * \brief Save settings
 */
void
OptionsDialog::writeSettings()
{
    using namespace Consts;
    mSettings.setValue(APPEARANCE_GRP + "/" + LANG_SETTING, ui.mAppearancePage->language());
    mSettings.setValue(PREFS_GRP + "/" + SESSION_DIR_SETTING, ui.mPreferencesPage->sessionFolder());
    mSettings.setValue(PREFS_GRP + "/" + NEW_TAB_AUTO_SWITCH_SETTING, ui.mPreferencesPage->newTabAutoSwitch());
    mSettings.setValue(PREFS_GRP + "/" + LOAD_LAST_SESSION_SETTING, ui.mPreferencesPage->loadLastSession());
    mSettings.setValue(PREFS_GRP + "/" + COUNT_SAVED_SESSIONS_SETTING, ui.mPreferencesPage->countSavedSession());
    mSettings.setValue(PREFS_GRP + "/" + SHOW_INDICES_SETTING, ui.mPreferencesPage->showIndices());
    mSettings.setValue(COLOR_GRP + "/" + BACKGROUND_SETTING, ui.mColorsPage->backgroundColor());
    mSettings.setValue(COLOR_GRP + "/" + TABLE_SETTING, ui.mColorsPage->tableColor());
//    mSettings.setValue(COLOR_GRP + "/" + FONT_SETTING, ui.mColorsPage->fontColor());
    mSettings.setValue(COLOR_GRP + "/" + ARROW_SETTING, ui.mColorsPage->arrowColor());
    mSettings.setValue(COLOR_GRP + "/" + BORDER_SETTING, ui.mColorsPage->borderColor());
}

/*!
 * \brief Overriden accept() method
 */
void
OptionsDialog::accept()
{
    writeSettings();
    QDialog::accept();
}

/*!
 * \brief Handle page change
 *
 * \param[in] ipCurrent - Pointer to the current item
 * \param[in] ipPrevious - Pointer to the previous item
 */
void
OptionsDialog::changePage(QListWidgetItem *ipCurrent, QListWidgetItem *ipPrevious)
{
    if (!ipCurrent) {
        ipCurrent = ipPrevious;
    }

    // show appropriate page
    ui.mOptionsPages->setCurrentIndex(ui.mOptionsList->row(ipCurrent));
}


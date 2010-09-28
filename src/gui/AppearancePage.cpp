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

#include <consts.h>
#include <gui/AppearancePage.h>
#include <QLocale>


/*
 * Constructor
 */
AppearancePage::AppearancePage(QWidget *ipParent)
    : QWidget(ipParent)
{
    ui.setupUi(this);

    // create combo box to select the appropriate language
    ui.mLanguageCombo->addItem(QLocale::languageToString(QLocale::English), QLocale::English);
    ui.mLanguageCombo->addItem(QLocale::languageToString(QLocale::Russian), QLocale::Russian);

    // set language to the one from settings
    using namespace Consts;
    int currentIndex = ui.mLanguageCombo->findData(mSettings.value(APPEARANCE_GRP + "/" + LANG_SETTING, QLocale::English));
    ui.mLanguageCombo->setCurrentIndex(currentIndex);

}

/*
 * Destructor
 */
AppearancePage::~AppearancePage()
{
}

/*
 * Get the desired language
 */
int
AppearancePage::language() const
{
    return ui.mLanguageCombo->itemData(ui.mLanguageCombo->currentIndex()).toInt();
}


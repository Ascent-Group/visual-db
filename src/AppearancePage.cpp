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

#include <QComboBox>
#include <QGridLayout>
#include <QLabel>
#include <QLocale>

#include <AppearancePage.h>

/*
 * Constructor
 */
AppearancePage::AppearancePage(QWidget *ipParent)
    : QWidget(ipParent)
{
    // create combo box to select the appropriate language 
    mLanguageCombo = new QComboBox();
    mLanguageCombo->addItem(tr("English"), QLocale::English);
    mLanguageCombo->addItem(tr("Russian"), QLocale::Russian);
    
    // set language to the one from settings
    int currentIndex = mLanguageCombo->findData(mSettings.value("Appearance/Language", QLocale::English));
    mLanguageCombo->setCurrentIndex(currentIndex);

    // create main layout
    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->setAlignment(Qt::AlignTop);
    mainLayout->addWidget(new QLabel(tr("Language:")), 0, 0);
    mainLayout->addWidget(mLanguageCombo, 0, 1);

    mainLayout->addWidget(new QLabel(tr("*In order to change the language "
		    "you need to restart the application!")), 5, 0, 1, 2, Qt::AlignBottom);
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
    return mLanguageCombo->itemData(mLanguageCombo->currentIndex()).toInt();
}


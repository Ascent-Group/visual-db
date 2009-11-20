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
#include <QGridLayout>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPushButton>
#include <QSpacerItem>
#include <QStackedWidget>
#include <QtDebug>


/*
 * Constructor
 */
OptionsDialog::OptionsDialog(QWidget *ipParent)
    : QDialog(ipParent)
{
    setWindowTitle(tr("Options"));
    
    // create list of options
    mOptionsList = new QListWidget();
	    
    // create appearence item
    QListWidgetItem *appearanceItem = new QListWidgetItem(mOptionsList);
    appearanceItem->setIcon(QIcon(":/img/window_new.png"));
    appearanceItem->setText(tr("Appearance"));
    appearanceItem->setTextAlignment(Qt::AlignLeft);
    appearanceItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    
    // create preferences item
    QListWidgetItem *preferencesItem = new QListWidgetItem(mOptionsList);
    preferencesItem->setIcon(QIcon(":/img/configure.png"));
    preferencesItem->setText(tr("Preferences"));
    preferencesItem->setTextAlignment(Qt::AlignLeft);
    preferencesItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
     
    // create color item
    QListWidgetItem *colorsItem = new QListWidgetItem(mOptionsList);
    colorsItem->setIcon(QIcon(":/img/colors.png"));
    colorsItem->setText(tr("Colors"));
    colorsItem->setTextAlignment(Qt::AlignLeft);
    colorsItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
   
    // create each page
    mOptionsPages = new QStackedWidget();
    mAppearancePage = new AppearancePage();
    mPreferencesPage = new PreferencesPage();
    mColorsPage = new ColorsPage();
    
    // add pages to the dialog
    mOptionsPages->addWidget(mAppearancePage);
    mOptionsPages->addWidget(mPreferencesPage);
    mOptionsPages->addWidget(mColorsPage);
    
    // create apply button
    mApplyButton = new QPushButton();
    mApplyButton->setText(tr("Apply"));
    
    // create cansel button
    mCancelButton = new QPushButton();
    mCancelButton->setText(tr("Cancel"));
    
    // create horizontal layout
    QHBoxLayout *hlayout = new QHBoxLayout();
    // add spacer and buttons to horizontal layout
    QSpacerItem *spacerItem = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
    hlayout->addItem(spacerItem);
    hlayout->addWidget(mApplyButton);
    hlayout->addWidget(mCancelButton);
    
    // create main layout
    QGridLayout	*mainLayout = new QGridLayout(this);
    // set column stretch factors
    mainLayout->setColumnStretch(0, 1);
    mainLayout->setColumnStretch(1, 2);
    // add options list, options pages and horizontal layout to the main layout
    mainLayout->addWidget(mOptionsList, 0, 0);
    mainLayout->addWidget(mOptionsPages, 0, 1);
    mainLayout->addLayout(hlayout, 1, 1);
    
    // connect signals and slots
    connect(mApplyButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(mCancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    connect(mOptionsList, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)),
	    this, SLOT(changePage(QListWidgetItem *, QListWidgetItem*)));
}

/*
 * Destructor
 */
OptionsDialog::~OptionsDialog()
{
}

/*
 * Save settings
 */
void
OptionsDialog::writeSettings()
{
    mSettings.setValue("Appearance/Language", mAppearancePage->language());
    mSettings.setValue("Preferences/SessionFolder", mPreferencesPage->sessionFolder());
    mSettings.setValue("Preferences/NewTabAutoSwitch", mPreferencesPage->newTabAutoSwitch());
    mSettings.setValue("Preferences/LoadLastSession", mPreferencesPage->loadLastSession());
    mSettings.setValue("Preferences/CountSavedSessions", mPreferencesPage->countSavedSession());
    mSettings.setValue("Preferences/ShowIndices", mPreferencesPage->showIndices());
    mSettings.setValue("Color/Background", mColorsPage->backgroundColor());
    mSettings.setValue("Color/Table", mColorsPage->tableColor());
//    mSettings.setValue("Color/Font", mColorsPage->fontColor());
    mSettings.setValue("Color/ArrowItem", mColorsPage->arrowColor());
    mSettings.setValue("Color/Border", mColorsPage->borderColor());
}

/*
 * Overriden accept() method
 */
void
OptionsDialog::accept()
{
    writeSettings();
    QDialog::accept();
}

/*
 * Handle page change
 */
void
OptionsDialog::changePage(QListWidgetItem *ipCurrent, QListWidgetItem *ipPrevious)
{
    if (!ipCurrent) {
        ipCurrent = ipPrevious;
    }
    
    // show appropriate page
    mOptionsPages->setCurrentIndex(mOptionsList->row(ipCurrent));
}

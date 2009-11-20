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

#include <QToolButton>
#include <gui/TabWidget.h>

/*
 * Ctor
 */
TabWidget::TabWidget(QWidget *ipParent)
    : QTabWidget(ipParent)
{
#if QT_VERSION >= 0x040500
    // at this point forbid moving tabs
    setMovable(false);
#endif

    // create button for closing tabs
    mCloseTabButton = new QToolButton();
    mCloseTabButton->setIcon(QIcon(":img/closetab.png"));
    mCloseTabButton->setShortcut(QString("Ctrl+W"));

    setCornerWidget(mCloseTabButton);

    connect(mCloseTabButton, SIGNAL(clicked()), this, SLOT(closeTabButtonClicked()));
}

/*
 * Dtor
 */
TabWidget::~TabWidget()
{

}

/*
 * Makes the tab with the given id active
 */
void
TabWidget::setActiveTab(const int ipTabId)
{
    setCurrentIndex(ipTabId);
}

/*
 * Closes the active tab
 */
void
TabWidget::closeTabButtonClicked()
{
    // get active tab id
    int activeTabId = currentIndex();

    // lyuts: remember last tab index so we can switch to it back!!

    // if id is greater than 0 = we are closing any tab except grachics scheme
    if ( 0 < activeTabId ) {
        removeTab(activeTabId);
    }
}

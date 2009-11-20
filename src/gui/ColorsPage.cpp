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

#include <QColor>
#include <QDebug>
#include <QGridLayout>

#include "ColorsPage.h"
#include "SelectColorWidget.h"

/*
 * Constructor
 */
ColorsPage::ColorsPage(QWidget *ipParent)
    : QWidget(ipParent)
{
    // create items to select given colors
    mBackgroundWidget = new SelectColorWidget("Background", Qt::white);
    mTableWidget = new SelectColorWidget("Table", Qt::white);
//    mFontWidget = new SelectColorWidget("Font", Qt::black);
    mArrowItemWidget = new SelectColorWidget("ArrowItem", Qt::black);
    mBorderWidget = new SelectColorWidget("Border", Qt::black);

    // create mainLayout
    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->setAlignment(Qt::AlignTop);

    // place items on the widget
    mainLayout->addWidget(mBackgroundWidget, 0, 0);
    mainLayout->addWidget(mTableWidget, 1, 0);
//    mainLayout->addWidget(mFontWidget, 2, 0);
    mainLayout->addWidget(mArrowItemWidget, 2, 0);
    mainLayout->addWidget(mBorderWidget, 3, 0);
}

/*
 * Destructor
 */
ColorsPage::~ColorsPage()
{
}

/*
 * Get default background color
 */
QColor
ColorsPage::backgroundColor() const
{
    return mBackgroundWidget->color();
}

/*
 * Get default table color
 */
QColor
ColorsPage::tableColor() const
{
    return mTableWidget->color();
}

/*
 * Get default font color
 */
//QColor
//ColorsPage::fontColor() const
//{
//    return mFontWidget->color();
//}

/*
 * Get default arrow color
 */
QColor
ColorsPage::arrowColor() const
{
    return mArrowItemWidget->color();
}

/*
 * Get default border color
 */
QColor
ColorsPage::borderColor() const
{
    return mBorderWidget->color();
}

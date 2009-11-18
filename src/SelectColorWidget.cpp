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
#include <QColorDialog>
#include <QComboBox>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGridLayout>
#include <QLabel>
#include <QPalette>
#include <QPushButton>

#include <SelectColorWidget.h>

SelectColorWidget::SelectColorWidget(const QString &ipParam, const QColor &ipDefaultColor, QWidget *ipParent)
    : QWidget(ipParent), mDefaultColor(ipDefaultColor)
{
    // get the default colors from the settings
    mColor = mSettings.value("Color/" + ipParam, Qt::white).value<QColor>();

    // select default color
    mCombo = new QComboBox();
    mCombo->addItem(tr("Default"));
    mCombo->addItem(tr("Another color"));
    if (ipDefaultColor != mColor) {
	mCombo->setCurrentIndex(1);
    }

    mColorButton = new QPushButton();
    mColorButton->setMinimumSize(40, 20);
    mColorButton->setMaximumSize(40, 20);
    mColorButton->setPalette(QPalette(mColor));

    // create mainLayout
    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->setAlignment(Qt::AlignTop);

    mainLayout->addWidget(new QLabel(ipParam + " color:"), 0, 0);
    mainLayout->addWidget(mCombo, 0, 1);
    mainLayout->addWidget(mColorButton, 0, 2);

    connect(mCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(colorSelectSlot(int)));
    connect(mColorButton, SIGNAL(clicked()), this, SLOT(buttonClickSlot()));
}

/*
 * Process color selection
 */
void
SelectColorWidget::colorSelectSlot(int ipIndex)
{
    switch (ipIndex) {
	case 1:
	    getColorFromDialog();
	    break;
	
	case 0:
	default:
	    mColor = mDefaultColor;
	    break;
    }

    mColorButton->setPalette(QPalette(mColor));
}

/*
 * Return the selected color
 */
QColor
SelectColorWidget::color() const
{
    return mColor;
}

/*
 * Handle the button press event
 */
void
SelectColorWidget::buttonClickSlot()
{
    // if 'another color' is selected - make button ckickable
    if (mCombo->currentIndex() == 1) {
	getColorFromDialog();
	mColorButton->setPalette(QPalette(mColor));
    }
}

/*
 * Show the 'color dialog' and get choosed color
 */
void
SelectColorWidget::getColorFromDialog()
{
    QColor color = QColorDialog::getColor(mColor, this);
    if (color.isValid()) {
	mColor = color;
    }
}

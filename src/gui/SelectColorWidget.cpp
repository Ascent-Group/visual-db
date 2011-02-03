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
#include <gui/SelectColorWidget.h>
#include <QColor>
#include <QColorDialog>
#include <QDebug>
#include <QPalette>

namespace Gui {

/*!
 * Ctor
 */
SelectColorWidget::SelectColorWidget(QWidget *iParent)
    : QWidget(iParent)
{
    ui.setupUi(this);

    ui.mLabel->setText("");

    init();

    // these connects, should be at the end !!!
    // we are connecting them manually, because if setupUi does this
    // the previous code will end up with slots execution
    connect(ui.mCombo, SIGNAL(activated(int)), this, SLOT(colorSelect(int)));
    connect(ui.mColorButton, SIGNAL(clicked()), this, SLOT(buttonClick()));
}

/*!
 * Dtor
 */
SelectColorWidget::~SelectColorWidget()
{
}

/*!
 * \brief Process color selection - open the dialog
 *
 * \param[in] iIndex - input color for default value of color picker in the dialog
 */
void
SelectColorWidget::colorSelect(int iIndex)
{
    switch (iIndex) {
        case 1:
            getColorFromDialog();
            break;

        case 0:
        default:
            mColor = mDefaultColor;
            break;
    }

    ui.mColorButton->setPalette(QPalette(mColor));
}

/*!
 * \brief Get the selected color
 *
 * \return Selected color
 */
QColor
SelectColorWidget::color() const
{
    return mColor;
}

/*!
 * \brief Handle the button press event
 */
void
SelectColorWidget::buttonClick()
{
    // if 'another color' is selected - make button clickable
    if (ui.mCombo->currentIndex() == 1) {
        getColorFromDialog();
        ui.mColorButton->setPalette(QPalette(mColor));
    }
}

/*!
 * \brief Show the 'color dialog' and get choosed color
 */
void
SelectColorWidget::getColorFromDialog()
{
    QColor color = QColorDialog::getColor(mColor, this);
    if (color.isValid()) {
        mColor = color;
    }
}

/*!
 * \brief Get the label text
 *
 * \return Label text
 */
QString
SelectColorWidget::labelText() const
{
    return ui.mLabel->text();
}

/*!
 * \brief Set the label text
 *
 * \param[in] iText - Label text
 */
void
SelectColorWidget::setLabelText(const QString &iText)
{
    ui.mLabel->setText(iText);

    init();
}

/*!
 * \brief Get default color
 *
 * \return Default color
 */
QColor
SelectColorWidget::defaultColor() const
{
    return mDefaultColor;
}

/*!
 * \brief Set default color
 *
 * \param[in] iColor - Default color
 */
void
SelectColorWidget::setDefaultColor(const QColor &iColor)
{
    mDefaultColor = iColor;

    if (mDefaultColor != mColor) {
        ui.mCombo->setCurrentIndex(1);
    }

    ui.mColorButton->setPalette(QPalette(mColor));
}

/*!
 * \brief Init the dialog
 */
void
SelectColorWidget::init()
{
    // get the default colors from the settings
    mColor = mSettings.value(Consts::COLOR_GRP + "/" + ui.mLabel->text(), mDefaultColor).value<QColor>();

    qDebug() << mDefaultColor << ":" << mColor;
    // select default color
    if (mDefaultColor == mColor) {
        ui.mCombo->setCurrentIndex(0);
    } else {
        ui.mCombo->setCurrentIndex(1);
    }

    ui.mColorButton->setPalette(QPalette(mColor));
}

}

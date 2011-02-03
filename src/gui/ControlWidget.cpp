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

#include <gui/ControlWidget.h>

#include <QtDebug>

namespace Gui {

/*!
 * Constructor
 */
ControlWidget::ControlWidget(QWidget *iParent)
    : QWidget(iParent)
{
    ui.setupUi(this);

    setMinZoom(0);
    setMaxZoom(10);
    setZoom(5);
}

/*!
 * Destructor
 */
ControlWidget::~ControlWidget()
{
}

/*!
 * \brief Process value changed signal (just emit the signal)
 *
 * \param[in] iFactor -
 */
void
ControlWidget::changeValue(int iFactor)
{
    emit valueChanged(iFactor);
}

/*!
 * \brief Increment value
 */
void
ControlWidget::increaseValue()
{
    ui.mScaleSlider->setValue(ui.mScaleSlider->value() + 1);
}

/*!
 * Decrement value
 */
void
ControlWidget::decreaseValue()
{
    ui.mScaleSlider->setValue(ui.mScaleSlider->value() - 1);
}

/*!
 * \brief Move the scene up
 */
void
ControlWidget::moveUp()
{
    emit movedUp();
}

/*!
 * \brief Move the scene down
 */
void
ControlWidget::moveDown()
{
    emit movedDown();
}

/*!
 * \brief Move the scene left
 */
void
ControlWidget::moveLeft()
{
    emit movedLeft();
}

/*!
 * \breif Move the scene right
 */
void
ControlWidget::moveRight()
{
    emit movedRight();
}

/*!
 * \brief Set move mode/drag&drop mode
 */
void
ControlWidget::setMoveMode()
{
    emit moveModeSet(ui.mMoveButton->isChecked());
}

/*!
 * \brief Get the minimum scaling factor
 *
 * \return Minimum scaling factor
 */
int
ControlWidget::minZoom() const
{
    return ui.mScaleSlider->minimum();
}

/*!
 * \brief Set the minimum scaling factor
 *
 * \param[in] iVal - Minimum scaling factor
 */
void
ControlWidget::setMinZoom(int iVal)
{
    ui.mScaleSlider->setMinimum(iVal);
}

/*!
 * \brief Get the maximum scaling factor
 *
 * \return Maximum scaling factor
 */
int
ControlWidget::maxZoom() const
{
    return ui.mScaleSlider->maximum();
}

/*!
 * \brief Set the maximum scaling factor
 *
 * \param[in] iVal - Maximum scaling factor
 */
void
ControlWidget::setMaxZoom(int iVal)
{
    ui.mScaleSlider->setMaximum(iVal);
}

/*!
 * \brief Get current scaling factor
 *
 * \return Current scaling factor
 */
int
ControlWidget::zoom() const
{
    return ui.mScaleSlider->value();
}

/*!
 * \brief Set current scaling factor
 *
 * \param[in] iVal - Current scaling factor
 */
void
ControlWidget::setZoom(int iVal)
{
    ui.mScaleSlider->setValue(iVal);
}

}

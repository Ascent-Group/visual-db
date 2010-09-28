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

/*
 * Constructor
 */
ControlWidget::ControlWidget(/*int ipMinValue, int ipMaxValue, */QWidget *ipParent)
    : QWidget(ipParent)
{
    ui.setupUi(this);

    setMinZoom(0);
    setMaxZoom(10);
    setZoom(5);
}

/*
 * Destructor
 */
ControlWidget::~ControlWidget()
{
}

/*
 * Process value changed signal
 */
void
ControlWidget::changeValue(int ipFactor)
{
    emit valueChanged(ipFactor);
}

/*
 * Increase value
 */
void
ControlWidget::increaseValue()
{
    ui.mScaleSlider->setValue(ui.mScaleSlider->value() + 1);
}

/*
 * Decrease value
 */
void
ControlWidget::decreaseValue()
{
    ui.mScaleSlider->setValue(ui.mScaleSlider->value() - 1);
}

/*
 * Move the scene up
 */
void
ControlWidget::moveUp()
{
    emit movedUp();
}

/*
 * Move the scene down
 */
void
ControlWidget::moveDown()
{
    emit movedDown();
}

/*
 * Move the scene left
 */
void
ControlWidget::moveLeft()
{
    emit movedLeft();
}

/*
 * Move the scene right
 */
void
ControlWidget::moveRight()
{
    emit movedRight();
}

/*
 * Set move mode
 */
void
ControlWidget::setMoveMode()
{
    emit moveModeSet(ui.mMoveButton->isChecked());
}

/*
 *
 */
int
ControlWidget::minZoom() const
{
    return ui.mScaleSlider->minimum();
}

/*
 *
 */
void
ControlWidget::setMinZoom(int ipVal)
{
    ui.mScaleSlider->setMinimum(ipVal);
}

/*
 *
 */
int
ControlWidget::maxZoom() const
{
    return ui.mScaleSlider->maximum();
}

/*
 *
 */
void
ControlWidget::setMaxZoom(int ipVal)
{
    ui.mScaleSlider->setMaximum(ipVal);
}

/*
 *
 */
int
ControlWidget::zoom() const
{
    return ui.mScaleSlider->value();
}

/*
 *
 */
void
ControlWidget::setZoom(int ipVal)
{
    ui.mScaleSlider->setValue(ipVal);
}

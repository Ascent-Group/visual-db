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
#include <QDebug>
#include <QGridLayout>
#include <QPushButton>
#include <QSlider>
#include <QToolButton>

/*
 * Constructor
 */
ControlWidget::ControlWidget(int ipMinValue, int ipMaxValue, QWidget *ipParent)
    : QWidget(ipParent)
{
    mScaleSlider = new QSlider();
    mScaleSlider->setMinimum(ipMinValue);
    mScaleSlider->setMaximum(ipMaxValue);
    mScaleSlider->setValue((mScaleSlider->maximum() - mScaleSlider->minimum()) / 2);
    mScaleSlider->setPageStep(1);
    connect(mScaleSlider, SIGNAL(valueChanged(int)), this, SLOT(changeValue(int)));

    mPlusButton = new QPushButton("+");
    mPlusButton->setMinimumSize(25, 25);
    mPlusButton->setMaximumSize(25, 25);
    connect(mPlusButton, SIGNAL(clicked()), this, SLOT(increaseValue()));

    mMinusButton = new QPushButton("-");
    mMinusButton->setMinimumSize(25, 25);
    mMinusButton->setMaximumSize(25, 25);
    connect(mMinusButton, SIGNAL(clicked()), this, SLOT(decreaseValue()));

    mMoveButton = new QPushButton();
    mMoveButton->setIcon(QIcon(":/img/cursor-openhand.png"));
    mMoveButton->setMinimumSize(25, 25);
    mMoveButton->setMaximumSize(25, 25);
    mMoveButton->setCheckable(true);
    connect(mMoveButton, SIGNAL(clicked()), this, SLOT(setMoveMode()));

    mUpButton = new QPushButton();
    mUpButton->setIcon(QIcon(":/img/up.png"));
    mUpButton->setMinimumSize(25, 25);
    mUpButton->setMaximumSize(25, 25);
    connect(mUpButton, SIGNAL(clicked()), this, SLOT(moveUp()));

    mDownButton = new QPushButton();
    mDownButton->setIcon(QIcon(":/img/down.png"));
    mDownButton->setMinimumSize(25, 25);
    mDownButton->setMaximumSize(25, 25);
    connect(mDownButton, SIGNAL(clicked()), this, SLOT(moveDown()));

    mRightButton = new QPushButton();
    mRightButton->setIcon(QIcon(":/img/right.png"));
    mRightButton->setMinimumSize(25, 25);
    mRightButton->setMaximumSize(25, 25);
    connect(mRightButton, SIGNAL(clicked()), this, SLOT(moveRight()));

    mLeftButton = new QPushButton();
    mLeftButton->setIcon(QIcon(":/img/left.png"));
    mLeftButton->setMinimumSize(25, 25);
    mLeftButton->setMaximumSize(25, 25);
    connect(mLeftButton, SIGNAL(clicked()), this, SLOT(moveLeft()));

    // create mainLayout
    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);

    mainLayout->addWidget(mUpButton, 0, 1);
    mainLayout->addWidget(mLeftButton, 1, 0);
    mainLayout->addWidget(mMoveButton, 1, 1);
    mainLayout->addWidget(mRightButton, 1, 2);
    mainLayout->addWidget(mDownButton, 2, 1);
    mainLayout->addWidget(mPlusButton, 3, 1);
    mainLayout->addWidget(mScaleSlider, 4, 1);
    mainLayout->addWidget(mMinusButton, 5, 1);
    mainLayout->setSpacing(5);

    setLayout(mainLayout);
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
    mScaleSlider->setValue(mScaleSlider->value() + 1);
}

/*
 * Decrease value
 */
void
ControlWidget::decreaseValue()
{
    mScaleSlider->setValue(mScaleSlider->value() - 1);
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
    emit settedMoveMode(mMoveButton->isChecked());
}

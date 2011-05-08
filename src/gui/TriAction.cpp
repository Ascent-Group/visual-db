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

#include <gui/TriAction.h>
#include <QCheckBox>
#include <QEvent>

#include <QtDebug>

namespace Gui
{

/*!
 * Constructor
 */
TriAction::TriAction(const QString &iText, QObject *iParent)
    : QWidgetAction(iParent),
      mCheckBox(0)
{
    mCheckBox = new QCheckBox(iText, dynamic_cast<QWidget*>(this));
    mCheckBox->setTristate(true);

    setDefaultWidget(mCheckBox);

    connect(mCheckBox, SIGNAL(stateChanged(int)), this, SLOT(actionStateChanged(int)));
}

/*!
 * Constructor
 */
TriAction::TriAction(const QIcon &iIcon, const QString &iText, QObject *iParent)
    : QWidgetAction(iParent),
      mCheckBox(0)
{
    mCheckBox = new QCheckBox(iText, dynamic_cast<QWidget*>(this));
    mCheckBox->setTristate(true);
    mCheckBox->setIcon(iIcon);

    setDefaultWidget(mCheckBox);

    connect(mCheckBox, SIGNAL(stateChanged(int)), this, SLOT(actionStateChanged(int)));
}

/*!
 * \param[in] iState - State of tri action
 */
void
TriAction::setState(State iState)
{
    mCheckBox->setCheckState(static_cast<Qt::CheckState>(iState));
}

/*!
 * \return Current action state
 */
TriAction::State
TriAction::state() const
{
    return static_cast<TriAction::State>(mCheckBox->checkState());
}

/*!
 * Slot for handling tri action's state change. Emits triggered signal with state
 * identifier.
 *
 * \param[in] iState - State that has been entered.
 */
void
TriAction::actionStateChanged(int iState)
{
    qDebug() << "TriAction::actionTriggered> " << iState;

//    QSize iconSize;
    QIcon::Mode mode;
//    QIcon::State state;

    TriAction::State actionState = static_cast<TriAction::State>(iState);

//    iconSize = icon.actualSize(iconSize, mode, state);
//    qDebug() << iconSize;
//    switch (actionState) {
//        case Qt::PartiallyChecked:
//                // \todo gray out the icon
//                mode = QIcon::Disabled;
//                break;
//
//        case Qt::Unchecked:
//        case Qt::Checked:
//        default:
//                mode = QIcon::Normal;
//                break;
//    }

//    QIcon icon = mCheckBox->icon();

//    if (!icon.isNull) {
//        mCheckBox->setIcon(icon.addPixmap(icon.pixmap(1, mode, QIcon::On)));
//    }

    emit triggered(actionState);
}

} // namespace Gui


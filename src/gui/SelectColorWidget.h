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

#ifndef SELECTCOLORWIDGET_H
#define SELECTCOLORWIDGET_H

#include <gui/ui/ui_SelectColorWidget.h>
#include <QDynamicPropertyChangeEvent>
#include <QSettings>
#include <QWidget>

class QColor;

/*
 * Widget to provide the color selection.
 */
class SelectColorWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString labelText READ labelText WRITE setLabelText)
    Q_PROPERTY(QColor defaultColor READ defaultColor WRITE setDefaultColor)

    public:
        SelectColorWidget(QWidget *ipParent = 0);
        ~SelectColorWidget();

        QColor color() const;

        QString labelText() const;
        void setLabelText(QString ipText);

        QColor defaultColor() const;
        void setDefaultColor(QColor ipColor);

    private:
        Ui::SelectColorWidget ui;
        QSettings mSettings;

        QColor mColor;
        QColor mDefaultColor;

    private:
        void init();
        void getColorFromDialog();

    private slots:
        void colorSelect(int);
        void buttonClick();
};

#endif // SELECTCOLORWIDGET_H


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

#ifndef CONTROLWIDGET_H
#define CONTROLWIDGET_H

#include <gui/ui/ui_ControlWidget.h>
#include <QWidget>


/*
 * Widget for contorlling the scene - move it, 
 * resize and change the drag-and-drop meaning
 */
class ControlWidget : public QWidget
{
    Q_OBJECT
        Q_PROPERTY(int minZoom READ minZoom WRITE setMinZoom)
        Q_PROPERTY(int maxZoom READ maxZoom WRITE setMaxZoom)
        Q_PROPERTY(int zoom READ zoom WRITE setZoom)

    public:
        ControlWidget(/*int, int, */QWidget *ipParent = 0);
        ~ControlWidget();

        int minZoom() const;
        void setMinZoom(int);

        int maxZoom() const;
        void setMaxZoom(int);

        int zoom() const;
        void setZoom(int);

    signals:
        void valueChanged(int ipFactor);
        void movedUp();
        void movedDown();
        void movedRight();
        void movedLeft();
        void moveModeSet(bool);

    public slots:
        void increaseValue();
        void decreaseValue();

    private:
        Ui::ControlWidget ui;

    private slots:
        void changeValue(int);
        void moveUp();
        void moveDown();
        void moveLeft();
        void moveRight();
        void setMoveMode();
};

#endif // CONTROLWIDGET_H

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

#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>

class QAction;
class QPointF;

/*
 * View of the scene.
 */
class GraphicsView : public QGraphicsView
{
    Q_OBJECT

    public:
        GraphicsView();
        GraphicsView(QGraphicsScene *, QWidget *);
        ~GraphicsView();

    public slots:
        void scaleView(int);

    public:
        static const int MINIMUM_FACTOR = 0;
        static const int MAXIMUM_FACTOR = 10;

    signals:
        void valueDecreased();
        void valueIncreased();

    protected:
        virtual void keyPressEvent(QKeyEvent *ipEvent);
        virtual void wheelEvent(QWheelEvent *ipEvent);
        virtual void scrollContentsBy(int, int);
        virtual void mousePressEvent(QMouseEvent *);
        virtual void mouseMoveEvent(QMouseEvent *);
        virtual void mouseReleaseEvent(QMouseEvent *);

    private:
        bool mMovingNow;
        QPointF mBeginPoint;
        QAction *mDeleteTableAction;
        int mPrevFactor;
        static const int MOVE_STEP = 10;

    private slots:
        void moveUp();
        void moveDown();
        void moveLeft();
        void moveRight();
        void setMoveMode(bool);
};

#endif // GRAPHICSVIEW_H


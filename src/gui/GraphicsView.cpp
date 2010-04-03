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

#include <QAction>
#include <QColor>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QPointF>
#include <QScrollBar>
#include <QWheelEvent>
#include <math.h>

#include "GraphicsScene.h"
#include "GraphicsView.h"
#include "TableItem.h"
#include "TableItemGroup.h"

#include <QDebug>

/*
 * Constructor
 */
    GraphicsView::GraphicsView()
: QGraphicsView()
{
    mPrevFactor = (MAXIMUM_FACTOR - MINIMUM_FACTOR) / 2;
}

/*
 * Constructor
 */
    GraphicsView::GraphicsView(QGraphicsScene *scene, QWidget *parent = 0)
: QGraphicsView(scene, parent), mMovingNow(false)
{
    centerOn(GraphicsScene::DEFAULT_WIDTH / 2, GraphicsScene::DEFAULT_HEIGHT / 2);
}

/*
 * Destructor
 */
GraphicsView::~GraphicsView()
{
}

/*
 * Handler for key press ipEvent
 * + - maximize the table scheme view
 * - - minimize the table scheme view
 * arrow keys - move items to the appropriate sides
 */
void 
GraphicsView::keyPressEvent(QKeyEvent *ipEvent)
{
    switch (ipEvent->key()) {
        case Qt::Key_Plus:
            //      scaleView(mPrevFactor + 1);
            break;
        case Qt::Key_Minus:
            //      scaleView(mPrevFactor - 1);
            break;
        case Qt::Key_Left:
            foreach (QGraphicsItem *item, scene()->selectedItems()) {
                if ((item->type() == TableItem::Type
                            || item->type() == TableItemGroup::Type)
                        && item->flags() & QGraphicsItem::ItemIsMovable) {
                    item->moveBy(-MOVE_STEP, 0);
                }
            }
            break;
        case Qt::Key_Right:
            foreach (QGraphicsItem *item, scene()->selectedItems()) {
                if ((item->type() == TableItem::Type
                            || item->type() == TableItemGroup::Type)
                        && item->flags() & QGraphicsItem::ItemIsMovable) {
                    item->moveBy(MOVE_STEP, 0);
                }
            }
            break;
        case Qt::Key_Up:
            foreach (QGraphicsItem *item, scene()->selectedItems()) {
                if ((item->type() == TableItem::Type
                            || item->type() == TableItemGroup::Type)
                        && item->flags() & QGraphicsItem::ItemIsMovable) {
                    item->moveBy(0, -MOVE_STEP);
                }
            }
            break;
        case Qt::Key_Down:
            foreach (QGraphicsItem *item, scene()->selectedItems()) {
                if ((item->type() == TableItem::Type
                            || item->type() == TableItemGroup::Type)
                        && item->flags() & QGraphicsItem::ItemIsMovable) {
                    item->moveBy(0, MOVE_STEP);
                }
            }
            break;
    }
}

/*
 * Handler for scrolling event
 */
void 
GraphicsView::wheelEvent(QWheelEvent *ipEvent)
{
    // if we scroll with control button pressed
    if (ipEvent->modifiers() == Qt::CTRL) {
        // scale the view
        if (ipEvent->delta() < 0) {
            emit valueDecreased();
        } else {
            emit valueIncreased();
        }
    } else {
        // else simply scroll
        QGraphicsView::wheelEvent(ipEvent);
    }
}

/*
 * Scaling the table scheme
 */
void 
GraphicsView::scaleView(int ipScaleFactor)
{
    static int prevFactor = (MAXIMUM_FACTOR - MINIMUM_FACTOR) / 2;

    // by default we that scale factor increases
    qreal scaleFactor = pow(1.2, fabs(ipScaleFactor - prevFactor));
    // but here we check if it decreases
    if (prevFactor > ipScaleFactor) {
        scaleFactor = 1 / pow(1.2, fabs(ipScaleFactor - prevFactor))/* 1 / scaleFactor */;
    }

    qreal factor = matrix().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < MINIMUM_FACTOR || factor > MAXIMUM_FACTOR) {
        return;
    }

    // remember previous factor
    prevFactor = ipScaleFactor;

    qDebug() << scaleFactor;
    scale(scaleFactor, scaleFactor);
    if (factor < 1) {
        scene()->setSceneRect(0, 0, GraphicsScene::DEFAULT_WIDTH / factor, GraphicsScene::DEFAULT_HEIGHT / factor);
    }
}

/*
 * Handle scrolling event
 */
void
GraphicsView::scrollContentsBy(int ipDx, int ipDy)
{
    dynamic_cast<GraphicsScene *>(scene())->moveLegend(-ipDx, -ipDy);
    QGraphicsView::scrollContentsBy(ipDx, ipDy);
}

/*
 * Move the widget up
 */
void
GraphicsView::moveUp()
{
    verticalScrollBar()->setValue(verticalScrollBar()->value() - MOVE_STEP);
}

/*
 * Move the widget down
 */
void 
GraphicsView::moveDown()
{
    verticalScrollBar()->setValue(verticalScrollBar()->value() + MOVE_STEP);
}

/*
 * Move the widget left
 */
void
GraphicsView::moveLeft()
{
    horizontalScrollBar()->setValue(horizontalScrollBar()->value() - MOVE_STEP);
}

/*
 * Move the widget right
 */
void
GraphicsView::moveRight()
{
    horizontalScrollBar()->setValue(horizontalScrollBar()->value() + MOVE_STEP);
}

void
GraphicsView::mousePressEvent(QMouseEvent *ipEvent)
{
    if (dynamic_cast<GraphicsScene *>(scene())->moveMode()) {
        if (!mMovingNow) {
            mMovingNow = true;
            mBeginPoint = ipEvent->pos();
        }
        QWidget::setCursor(Qt::ClosedHandCursor);
    }
    QGraphicsView::mousePressEvent(ipEvent);
}

void
GraphicsView::mouseMoveEvent(QMouseEvent *ipEvent)
{
    if (dynamic_cast<GraphicsScene *>(scene())->moveMode()) {
        if (mMovingNow) {
            horizontalScrollBar()->setValue(horizontalScrollBar()->value() - (int)(ipEvent->pos().x() - mBeginPoint.x()));
            verticalScrollBar()->setValue(verticalScrollBar()->value() - (int)(ipEvent->pos().y() - mBeginPoint.y()));
            mBeginPoint = ipEvent->pos();
        }
    }
    QGraphicsView::mouseMoveEvent(ipEvent);
}

void
GraphicsView::mouseReleaseEvent(QMouseEvent *ipEvent)
{
    if (dynamic_cast<GraphicsScene *>(scene())->moveMode()) {
        mMovingNow = false;
        QWidget::setCursor(Qt::OpenHandCursor);
    }
    QGraphicsView::mouseReleaseEvent(ipEvent);
}

void
GraphicsView::setMoveMode(bool ipFlag)
{
    dynamic_cast<GraphicsScene *>(scene())->setMoveMode(ipFlag);
    if (ipFlag) {
        setCursor(Qt::OpenHandCursor);
    } else {
        setCursor(Qt::ArrowCursor);
    }
}

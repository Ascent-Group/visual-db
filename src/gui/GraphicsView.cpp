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
#include <QPointF>
#include <QScrollBar>
#include <QWheelEvent>
#include <math.h>

#include <gui/GraphicsScene.h>
#include <gui/GraphicsView.h>
#include <gui/TreeWidget.h>
#include <gui/behaviour/AddItemCommand.h>

#include <QDebug>

/*!
 * Constructor
 */
GraphicsView::GraphicsView()
    : QGraphicsView()
{
    mPrevFactor = (MAXIMUM_FACTOR - MINIMUM_FACTOR) / 2;
    setAcceptDrops(true);
}

/*!
 * Constructor
 */
    GraphicsView::GraphicsView(QGraphicsScene *scene, QWidget *parent = 0)
: QGraphicsView(scene, parent)
{
    centerOn(GraphicsScene::DEFAULT_WIDTH / 2, GraphicsScene::DEFAULT_HEIGHT / 2);
}

/*!
 * Destructor
 */
GraphicsView::~GraphicsView()
{
}

/*!
 * \brief Handler for scrolling event
 *
 * \param[in] ipEvent - Wheel event
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

/*!
 * \brief Scaling the scheme
 *
 * \param[in] ipScaleFactor - Scale factor
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

/*!
 * \brief Drag enter event
 *
 * \param[in] ipEvent - Drag enter event
 */
void 
GraphicsView::dragEnterEvent(QDragEnterEvent *ipEvent)
{
    if (ipEvent->mimeData()->hasFormat("table/x-table")) {
        ipEvent->accept();
    }
}

/*!
 * \brief Drag move event
 *
 * \param[in] ipEvent - Drag move event
 */
void 
GraphicsView::dragMoveEvent(QDragMoveEvent *ipEvent)
{
    if (!ipEvent) {
        qDebug() << "[E][" << __func__ << "][" << __LINE__ << 
            "]Can't handle drag move event because input event is NULL";   
    }

    if (ipEvent->mimeData()->hasFormat("table/x-table")) {
        ipEvent->setDropAction(Qt::CopyAction);
        ipEvent->accept();
    }
}

/*!
 * \brief Drop event
 *
 * \param[in] ipEvent - Drop event
 */
void
GraphicsView::dropEvent(QDropEvent *ipEvent)
{
    if (!ipEvent) {
        qDebug() << "[E][" << __func__ << "][" << __LINE__ << 
            "]Can't handle drop event because input event is NULL";   
    }

    if (ipEvent->mimeData()->hasFormat("table/x-table")) {
        GraphicsScene *graphicsScene = dynamic_cast<GraphicsScene *>(scene());
        QTreeWidget *itemTree = dynamic_cast<QTreeWidget *>(ipEvent->source());
        if (graphicsScene && itemTree) {
            int i = 0;
            foreach (QTreeWidgetItem *treeItem, itemTree->selectedItems()) {
                QList<QGraphicsItem *> itemList = graphicsScene->showOnScene(treeItem, TreeWidget::NameCol, ipEvent->pos() + QPoint(i * SEEK_STEP, i * SEEK_STEP));
                emit itemActionDone(new AddItemCommand(graphicsScene, itemList));
                ++i;
            }
        } else {
            qDebug() << "[E][" << __func__ << "][" << __LINE__ << 
                "]Can't handle drop event because graphics scene or item tree doesn't have appropriate type";   
        }
        ipEvent->setDropAction(Qt::CopyAction);
        ipEvent->accept();    
    } else {
        qDebug() << "[W][" << __func__ << "][" << __LINE__ << 
            "]Can't handle drop event because event mime type doesn't have appropriate type";   
    }
}

/*!
 * \brief Handle scrolling event
 *
 * \param[in] ipDx - Gorizontal distance
 * \param[in] ipDy - Vertical distance
 */
void
GraphicsView::scrollContentsBy(int ipDx, int ipDy)
{
    GraphicsScene *graphicsScene = dynamic_cast<GraphicsScene *>(scene());
    if (graphicsScene) {
        graphicsScene->moveLegend(-ipDx, -ipDy);
        QGraphicsView::scrollContentsBy(ipDx, ipDy);
    } else {
        qDebug() << "[E][" << __func__ << "][" << __LINE__ << 
            "]Can't handle scroll event because graphics scene doesn't have appropriate type";   
    }
}

/*!
 * \brief Move the widget up
 */
void
GraphicsView::moveUp()
{
    verticalScrollBar()->setValue(verticalScrollBar()->value() - MOVE_STEP);
}

/*!
 * \brief Move the widget down
 */
void
GraphicsView::moveDown()
{
    verticalScrollBar()->setValue(verticalScrollBar()->value() + MOVE_STEP);
}

/*!
 * \brief Move the widget left
 */
void
GraphicsView::moveLeft()
{
    horizontalScrollBar()->setValue(horizontalScrollBar()->value() - MOVE_STEP);
}

/*!
 * \brief Move the widget right
 */
void
GraphicsView::moveRight()
{
    horizontalScrollBar()->setValue(horizontalScrollBar()->value() + MOVE_STEP);
}

/*!
 * \brief Set move mode for the scene (when you drag and drop scene itself with mouse)
 *
 * \param[in] ipFlag - True if move mode is enabled, false otherwise
 */
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

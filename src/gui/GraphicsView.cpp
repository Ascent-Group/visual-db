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
#include <gui/graphicsitems/DbObjectItem.h>
#include <gui/TreeWidget.h>
#include <gui/behaviour/AddItemCommand.h>

#include <QDebug>

/*!
 * Constructor
 */
GraphicsView::GraphicsView()
    : QGraphicsView(), mMoveMode(false), mScaleFactor(1.0)
{
    mPrevFactor = (MAXIMUM_FACTOR - MINIMUM_FACTOR) / 2;
    setAcceptDrops(true);
}

/*!
 * Constructor
 */
GraphicsView::GraphicsView(QGraphicsScene *scene, QWidget *parent = 0)
    : QGraphicsView(scene, parent), mMoveMode(false), mScaleFactor(1.0)
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
 * \param[in] iEvent - Wheel event
 */
void
GraphicsView::wheelEvent(QWheelEvent *iEvent)
{
    // if we scroll with control button pressed
    if (iEvent->modifiers() == Qt::CTRL) {
        // scale the view
        if (iEvent->delta() < 0) {
            emit valueDecreased();
        } else {
            emit valueIncreased();
        }
    } else {
        // else simply scroll
        QGraphicsView::wheelEvent(iEvent);
    }
}

/*!
 * \brief Scaling the scheme
 *
 * \param[in] iScaleFactor - Scale factor
 */
void
GraphicsView::scaleView(int iScaleFactor)
{
    static int prevFactor = (MAXIMUM_FACTOR - MINIMUM_FACTOR) / 2;

    // by default we that scale factor increases
    qreal factor = pow(1.2, fabs(iScaleFactor - prevFactor));
    // but here we check if it decreases
    if (prevFactor > iScaleFactor) {
        factor = 1 / pow(1.2, fabs(iScaleFactor - prevFactor))/* 1 / factor */;
    }

    mScaleFactor = matrix().scale(factor, factor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (mScaleFactor < MINIMUM_FACTOR || mScaleFactor > MAXIMUM_FACTOR) {
        return;
    }

    // remember previous factor
    prevFactor = iScaleFactor;

    scale(factor, factor);
    if (mScaleFactor < 1) {
        scene()->setSceneRect(0, 0, GraphicsScene::DEFAULT_WIDTH / mScaleFactor, GraphicsScene::DEFAULT_HEIGHT / mScaleFactor);
    }
}

/*!
 * \brief Drag enter event
 *
 * \param[in] iEvent - Drag enter event
 */
void
GraphicsView::dragEnterEvent(QDragEnterEvent *iEvent)
{
    if (iEvent->mimeData()->hasFormat("table/x-table")) {
        iEvent->accept();
    }
}

/*!
 * \brief Drag move event
 *
 * \param[in] iEvent - Drag move event
 */
void
GraphicsView::dragMoveEvent(QDragMoveEvent *iEvent)
{
    if (!iEvent) {
        qDebug() << "[E][" << __func__ << "][" << __LINE__ <<
            "]Can't handle drag move event because input event is NULL";
    }

    if (iEvent->mimeData()->hasFormat("table/x-table")) {
        iEvent->setDropAction(Qt::CopyAction);
        iEvent->accept();
    }
}

/*!
 * \brief Drop event
 *
 * \param[in] iEvent - Drop event
 */
void
GraphicsView::dropEvent(QDropEvent *iEvent)
{
    if (!iEvent) {
        qDebug() << "[E][" << __func__ << "][" << __LINE__ <<
            "]Can't handle drop event because input event is NULL";
    }

    if (iEvent->mimeData()->hasFormat("table/x-table")) {
        GraphicsScene *graphicsScene = dynamic_cast<GraphicsScene *>(scene());
        QTreeWidget *itemTree = dynamic_cast<QTreeWidget *>(iEvent->source());
        if (graphicsScene && itemTree) {
            int i = 0;
            foreach (QTreeWidgetItem *treeItem, itemTree->selectedItems()) {
                QList<QGraphicsItem *> itemList = graphicsScene->showOnScene(treeItem, TreeWidget::NameCol,
                        mapToScene(iEvent->pos() + QPoint(i * SEEK_STEP, i * SEEK_STEP)).toPoint());

                foreach (QGraphicsItem *item, itemList) {
                    using namespace Gui::GraphicsItems;

                    if (toDbObject(item) && !graphicsScene->findItem(toDbObject(item)->schemaName(), toDbObject(item)->name())) {
                        // HACK: we have our own inner x:y coordinates, so we should compensate Qt shifting
                        item->moveBy(-item->x(), -item->y());
                    }
                }

                emit itemActionDone(new AddItemCommand(graphicsScene, itemList));
                ++i;
            }
        } else {
            qDebug() << "[E][" << __func__ << "][" << __LINE__ <<
                "]Can't handle drop event because graphics scene or item tree doesn't have appropriate type";
        }
        iEvent->setDropAction(Qt::CopyAction);
        iEvent->accept();
    } else {
        qDebug() << "[W][" << __func__ << "][" << __LINE__ <<
            "]Can't handle drop event because event mime type doesn't have appropriate type";
    }
}

/*!
 * \brief Handle scrolling event
 *
 * \param[in] iDx - Gorizontal distance
 * \param[in] iDy - Vertical distance
 */
void
GraphicsView::scrollContentsBy(int iDx, int iDy)
{
    GraphicsScene *graphicsScene = dynamic_cast<GraphicsScene *>(scene());
    if (graphicsScene) {
        graphicsScene->moveLegend(-iDx / mScaleFactor, -iDy / mScaleFactor);
        QGraphicsView::scrollContentsBy(iDx, iDy);
    } else {
        qDebug() << "[E][" << __func__ << "][" << __LINE__ <<
            "]Can't handle scroll event because graphics scene doesn't have appropriate type";
    }
}

/*!
 * \brief Handle mouse press event
 */
void
GraphicsView::mousePressEvent(QMouseEvent *iEvent)
{
    if (mMoveMode) {
        mStartPos = iEvent->pos();
    } else {
        QGraphicsView::mousePressEvent(iEvent);
    }
}

/*!
 * \brief Handle mouse move event
 */
void
GraphicsView::mouseMoveEvent(QMouseEvent *iEvent)
{
    if (mMoveMode && (iEvent->buttons() & Qt::LeftButton)) {
        verticalScrollBar()->setValue(verticalScrollBar()->value() - (iEvent->pos().y() - mStartPos.y()));
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - (iEvent->pos().x() - mStartPos.x()));
        mStartPos = iEvent->pos();
    } else {
        QGraphicsView::mouseMoveEvent(iEvent);
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
 * \param[in] iFlag - True if move mode is enabled, false otherwise
 */
void
GraphicsView::setMoveMode(bool iFlag)
{
    mMoveMode = iFlag;
    dynamic_cast<GraphicsScene *>(scene())->setMoveMode(iFlag);
    if (iFlag) {
        setCursor(Qt::OpenHandCursor);
    } else {
        setCursor(Qt::ArrowCursor);
    }
}

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

#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QGraphicsScene>
#include <QSet>
#include <QSettings>
#include <QTimer>

class QGraphicsItem;
class QGraphicsPathItem;
class QGraphicsSceneMouseEvent;
class QMenu;
class QKeyEvent;
class QPoint;
class QSlider;
class QTreeWidgetItem;

namespace Gui {

namespace GraphicsItems {

class ArrowItem;
class ControlWidget;
class DbObjectItem;
class GraphicsItem;
class ItemGroup;
class Legend;
class TableItem;
class ViewItem;

}

}

/*!
 * \class GraphicsScene
 * \headerfile gui/GraphicsScene.h
 * \brief Scene for db items. Allow browsing db items, relations between them and move them.
 */
class GraphicsScene : public QGraphicsScene {

    Q_OBJECT

    public:
        static const int LOW_GRID_DX = 10;
        static const int LOW_GRID_DY = 10;
        static const int DEFAULT_WIDTH = 2000;
        static const int DEFAULT_HEIGHT = 1500;

    public:
        GraphicsScene(QObject *iParent = 0);
        ~GraphicsScene();

        void addItems(const QList<QGraphicsItem *> &);
        Gui::GraphicsItems::DbObjectItem *findItem(const QString &, const QString &);
        void setSchemeMenu(QMenu *);
        void setTableMenu(QMenu *);
        Gui::GraphicsItems::ItemGroup *createItemGroup(const QList<QGraphicsItem *> &);
        void deleteItems(QList<QGraphicsItem *> &iTabliList);
        void setAcceptsHoverEvents(bool);
        void refreshLegend();
        void updateLegend();
        void drawRelations();
        void createRelations(Gui::GraphicsItems::TableItem *);
        Gui::GraphicsItems::TableItem *newTableItem(const QString &, const QString &, QMenu *, const QPoint &);
        Gui::GraphicsItems::ViewItem *newViewItem(const QString &, const QString &, QMenu *, const QPoint &);
        void setItemColor(QList<QGraphicsItem *>, const QColor &);
        bool moveMode() const;

        void flushCache();

    signals:
        void itemMoved(QList <QGraphicsItem *>, int, int);
        void itemResized(Gui::GraphicsItems::GraphicsItem *, const QRectF &, const QRectF &);
        void itemColorChanged(Gui::GraphicsItems::GraphicsItem *, const QColor &, const QColor &);

    public slots:
        QList<QGraphicsItem *> showOnScene(QTreeWidgetItem *, int, const QPoint &, bool iCenterOn = false);
        void setFieldsTypesVisible();
        void setFieldsTypesInvisible();
        void setIndicesVisible(bool);
        void setItemColor();
        void setItemColor(Gui::GraphicsItems::GraphicsItem*, const QColor &);
        void selectAllItems();
        void adjustItems();
        void groupItems();
        void ungroupItems();
        void colorizeAccordingSchemas();
        void setAnchor(bool);
        void showGrid(bool);
        void alignToGrid(bool);
        void divideIntoPages(bool);
        void showLegend(bool);
        void moveLegend(int, int);
        void selectAllItemsInSchema();
        void setMoveMode(bool);
        QDomElement toXml(QDomDocument &, bool, bool, bool, bool) const;
        QList<QGraphicsItem *> fromXml(const QDomNode &);


    protected:
        virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *);
        virtual void mousePressEvent(QGraphicsSceneMouseEvent *);
        virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *);
        virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *);
        virtual void keyPressEvent(QKeyEvent *iEvent);
        virtual void drawBackground(QPainter *, const QRectF &);

    private:
        void setFieldsTypesVisible(QList<QGraphicsItem *>, bool);
        void setIndicesVisible(QList<QGraphicsItem *>, bool);
        void adjustItems(QList<QGraphicsItem *>);
        void setAnchor(QList<QGraphicsItem *>, bool);
        void createRelation(Gui::GraphicsItems::TableItem *, Gui::GraphicsItems::TableItem *, Gui::GraphicsItems::ArrowItem *);
        void groupItems(QList<QGraphicsItem *>);
        void ungroupItems(QList<QGraphicsItem *>);
        void drawGrid(QPainter *);
        void divideIntoPages(QPainter *);

    private slots:
        void resize(int);
        void movingTimerExpired();

    private:
        QSettings mSettings;

        QPointF mStartSelect;
        QPointF mEndSelect;
        QGraphicsPathItem *mSelectionPath;

        bool mMoveMode;
        Gui::GraphicsItems::Legend *mLegend;

        QMenu *mSchemeMenu;
        QMenu *mTableMenu;
        QSet<Gui::GraphicsItems::DbObjectItem *> mDbItems;
        QSet<Gui::GraphicsItems::ArrowItem *> mArrows;

        QPointF mOldPos;
        QRectF mOldRect;

        int mDiffX;
        int mDiffY;
        QTimer mStartMovingTimer;

        static const int MOVE_STEP = 10;
        static const int MOVE_INTERVAL = 600;
        static const int SEEK_STEP = 20;
};

#endif // GRAPHICSSCENE_H


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

class ControlWidget;
class DbObjectItem;
class GraphicsItem;
class ItemGroup;
class Legend;
class TableItem;
class ViewItem;

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
        GraphicsScene();
        ~GraphicsScene();

        void addItems(const QList<QGraphicsItem *> &);
        void setSchemeMenu(QMenu *);
        void setTableMenu(QMenu *);
        ItemGroup *createItemGroup(const QList<QGraphicsItem *> &);
        void deleteItems(QList<QGraphicsItem *> &ipTabliList);
        void setAcceptsHoverEvents(bool);
        void refreshLegend();
        void updateLegend();
        void drawRelations();
        void createRelations(TableItem *);
        TableItem *newTableItem(const QString &, const QString &, QMenu *, const QPoint &);
        ViewItem *newViewItem(const QString &, const QString &, QMenu *, const QPoint &);
        void setItemColor(QList<QGraphicsItem *>, QColor);
        bool moveMode() const;

    signals:
        void itemMoved(QList <QGraphicsItem *>, int, int);
        void itemResized(GraphicsItem *, const QRectF &, const QRectF &);
        void itemColorChanged(GraphicsItem *, const QColor &, const QColor &);

    public slots:
        QList<QGraphicsItem *> showOnScene(QTreeWidgetItem *, int, const QPoint &, bool ipCenterOn = false);
        void setFieldsTypesVisible();
        void setFieldsTypesInvisible();
        void setIndicesVisible(bool);
        void setItemColor();
        void setItemColor(GraphicsItem*, QColor);
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
        virtual void keyPressEvent(QKeyEvent *ipEvent);
        virtual void drawBackground(QPainter *, const QRectF &);

    private:
        DbObjectItem *findItem(const QString &, const QString &);
        void setFieldsTypesVisible(QList<QGraphicsItem *>, bool);
        void setIndicesVisible(QList<QGraphicsItem *>, bool);
        void adjustItems(QList<QGraphicsItem *>);
        void setAnchor(QList<QGraphicsItem *>, bool);
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
        Legend *mLegend;

        QMenu *mSchemeMenu;
        QMenu *mTableMenu;
        QVector<DbObjectItem *> mDbItems;

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


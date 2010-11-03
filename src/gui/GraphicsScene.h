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
class DbObjectsItem;
class Legend;
class TableItem;
class TableItemGroup;
class ViewItem;

/*
 * Scene for tables. Allow browsing the tables, relations between them and move tables on it.
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

        void addTableItems(const QList<QGraphicsItem *> &);
        void setSchemeMenu(QMenu *);
        void setTableMenu(QMenu *);
        TableItemGroup *createItemGroup(const QList<QGraphicsItem *> &);
        void deleteTableItems(QList<QGraphicsItem *> &ipTabliList);
        void setAcceptsHoverEvents(bool);
        void refreshLegend();
        void updateLegend();
        void drawRelations();
        void createRelations(DbObjectsItem *);
        TableItem *newTableItem(QString, QString, QMenu *, const QPoint &);
        ViewItem *newViewItem(QString, QString, QMenu *, const QPoint &);
        void setTableColor(DbObjectsItem*, QColor);
        bool moveMode() const;

    signals:
        void tableMoved(QList <QGraphicsItem *>, int, int);

    public slots:
        QList<QGraphicsItem *> showOnScene(QTreeWidgetItem *, int, const QPoint &);
        void setFieldsTypesVisible();
        void setFieldsTypesInvisible();
        void setIndicesVisible(bool);
        void setTableColor();
        void selectAllTables();
        void adjustTables();
        void groupItems();
        void ungroupItems();
        void colorizeAccordingSchemas();
        void showGrid(bool);
        void alignToGrid(bool);
        void divideIntoPages(bool);
        void showLegend(bool);
        void moveLegend(int, int);
        void selectAllTablesInSchema();
        void setMoveMode(bool);

    protected:
        virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *);
        virtual void mousePressEvent(QGraphicsSceneMouseEvent *);
        virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *);
        virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *);
        virtual void keyPressEvent(QKeyEvent *ipEvent);
        virtual void drawBackground(QPainter *, const QRectF &);

    private:
        DbObjectsItem *findItem(const QString &, const QString &);
        void setFieldsTypesVisible(QList<QGraphicsItem *>, bool);
        void setIndicesVisible(QList<QGraphicsItem *>, bool);
        void setTableColor(QList<QGraphicsItem *>, QColor);
        void adjustTables(QList<QGraphicsItem *>);
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
        QVector<DbObjectsItem *> mDbItems;

        QPointF mOldPos;

        int mDiffX;
        int mDiffY;
        QTimer mStartMovingTimer;

        static const int MOVE_STEP = 10;
        static const int MOVE_INTERVAL = 600;
        static const int SEEK_STEP = 20;
};

#endif // GRAPHICSSCENE_H


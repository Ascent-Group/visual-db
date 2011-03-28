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

#ifndef SCENEWIDGET_H
#define SCENEWIDGET_H

#include <QWidget>

class ControlWidget;
class GraphicsScene;
class GraphicsView;
class QDomDocument;
class QDomElement;
class QGraphicsItem;
class QGridLayout;
class QMenu;
class QPrinter;
class QTreeWidgetItem;
class QUndoCommand;

namespace Gui {

namespace GraphicsItems {

class TableItem;
class ItemGroup;
class GraphicsItem;

}

/*!
 * \class SceneWidget
 * \headerfile gui/SceneWidget.h
 * \brief This is widget to provide the visualization.
 *        It contains graphics scene, graphics view, control widget and menus.
 */
class SceneWidget : public QWidget
{
    Q_OBJECT

    public:
        SceneWidget(QWidget *iParent = 0, Qt::WindowFlags iFlags = 0);
        ~SceneWidget();

        void toXml(QDomDocument &, QDomElement &, bool, bool, bool, bool);
        QList<QGraphicsItem *> items () const;
        Gui::GraphicsItems::ItemGroup *createItemGroup(const QList<QGraphicsItem *> &);
        void fromXml(QDomElement &);
        void print(QPrinter *);
        void refreshLegend();
        void setSchemeMenu(QMenu *);
        void setTableMenu(QMenu *);
        void updateLegend();
        void flushCache();

    signals:
        void movedUp();
        void movedDown();
        void movedLeft();
        void movedRight();
        void itemActionDone(QUndoCommand *);

    public slots:
        void adjustItems();
        void anchorItems();
        void alignToGrid(bool);
        void colorizeAccordingSchemas();
        void deleteItem();
        void divideIntoPages(bool);
        void groupItems();
        void saveToImage();
        void selectAllItems();
        void selectAllItemsInSchema();
        void setFieldsTypesInvisible();
        void setFieldsTypesVisible();
        void setIndicesInvisible();
        void setIndicesVisible();
        void showControlWidget(bool);
        void showGrid(bool);
        void showLegend(bool);
        void showOnScene(QTreeWidgetItem *, int, bool iCenterOn = false);
        void ungroupItems();
        void disableAnchorItems();
        void cleanSchemeScene();
        void setItemColor();
        void sendItemActionDone(QUndoCommand *);

    private:
        void setAnchor(QList<QGraphicsItem *>, bool);

    private slots:
        void sendItemMoved(QList <QGraphicsItem *>, int, int);
        void sendItemColorChanged(Gui::GraphicsItems::GraphicsItem *, const QColor &, const QColor &);
        void sendItemResized(Gui::GraphicsItems::GraphicsItem *, const QRectF &, const QRectF &);

    private:
        GraphicsScene *mScene;
        GraphicsView *mView;
        ControlWidget *mControlWidget;
        QGridLayout *mainLayout;
        QMenu *mTableMenu;
        QMenu *mSchemeMenu;
};

}

#endif // SCENEWIDGET_H


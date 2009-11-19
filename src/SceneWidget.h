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

class QDomDocument;
class QDomElement;
class QGraphicsItem;
class QGridLayout;
class QMenu;
class QPrinter;
class QTreeWidgetItem;

class ControlWidget;
class GraphicsScene;
class GraphicsView;
class TableItem;
class TableItemGroup;

/*
 * This is widget to provide the vizualization.
 * It contains graphics scene, graphics view, control widget and menus.
 */
class SceneWidget : public QWidget
{
    Q_OBJECT

    public:
	SceneWidget(QWidget *ipParent = 0, Qt::WindowFlags ipFlags = 0);
	~SceneWidget();
	
	void setSchemeMenu(QMenu *);
	void setTableMenu(QMenu *);
	void refreshLegend();
	void updateLegend();
	QList<QGraphicsItem *> items () const;
	TableItem *addTableItem(QString, QString, QMenu *);
	TableItemGroup *createItemGroup(const QList<QGraphicsItem *> &);
	QDomElement toXml(QDomDocument &, bool, bool, bool, bool);
	void fromXml(QDomElement &);
	TableItem *tableFromXml(QDomElement &);
	TableItemGroup *tableGroupFromXml(QDomElement &);
	void print(QPrinter *);

    public slots:
	void showOnScene(QTreeWidgetItem *, int);
	void deleteTableItem();
        void cleanTableSchemeScene();
	void setFieldsTypesVisible();
	void setFieldsTypesInvisible();
	void setIndicesVisible();
	void setIndicesInvisible();
        void setTableColor();
	void selectAllTables();
	void adjustTables();
	void groupItems();
	void ungroupItems();
	void anchorTables();
	void weightAnchorTables();
	void colorizeAccordingSchemas();
	void showGrid(bool);
	void attachToGrid(bool);
	void divideOnPages(bool);
	void showLegend(bool);
	void selectAllTablesInSchema();
	void saveToImage();
	void showControlWidget(bool);

    private:
	void setAnchor(QList<QGraphicsItem *>, bool);

    private:
	GraphicsScene *mScene;
	GraphicsView *mView;
	ControlWidget *mControlWidget;
	QGridLayout *mainLayout;
	QMenu *mTableMenu;
	QMenu *mSchemeMenu;
};

#endif // SCENEWIDGET_H

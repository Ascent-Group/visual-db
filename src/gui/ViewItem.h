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

#ifndef VIEWEITEM_H
#define VIEWEITEM_H

#include <gui/GraphicsItem.h>
#include <QFont>
#include <QSettings>

class QAction;
class QColor;
class QCursor;
class QDomDocument;
class QDomElement;
class QGraphicsSceneContextMenuEvent;
class QGraphicsTextItem;
class QImage;
class QMenu;

class ArrowItem;

namespace DbObjects
{
    namespace Common
    {
        class DbIndex;
        class DbView;
    } // namespace Common
} // namespace DbObjects

/*
 * Graphics item, implements the database view. Support moving, resizing, changing of the color etc.
 */
class ViewItem : public GraphicsItem
{
    public:
        ViewItem(const QString &, const QString &, QMenu *, const QPoint &);
        ~ViewItem();
        virtual int type() const;

        void addIndexItem(QGraphicsTextItem *);
        void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *widget = 0);
        void addArrowItem(ArrowItem *);
        void removeArrowItem(ArrowItem *);
        void removeArrowItems();
        void setFieldsTypesVisible(bool);
        void setIndicesVisible(bool);

        QList<ArrowItem *> arrows() const;
        QString viewName() const;
        QString schemaName() const;
        DbObjects::Common::DbView *viewModel() const;
        QDomElement toXml(QDomDocument &);

        static void setSeek(int);

    public:
        enum { Type = UserType + 8 };

    protected:
        void contextMenuEvent(QGraphicsSceneContextMenuEvent *);
        QVariant itemChange(GraphicsItemChange, const QVariant &);
        void mouseMoveEvent(QGraphicsSceneMouseEvent *);
        void mousePressEvent(QGraphicsSceneMouseEvent *);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *);
        void hoverMoveEvent(QGraphicsSceneHoverEvent *);
        void hoverLeaveEvent(QGraphicsSceneHoverEvent *);

    private:
        // mode for view (you can resize view from different positions or move it)
        enum Mode {
            RIGHT_BOTTOM_CORNER_RESIZE,
            LEFT_BOTTOM_CORNER_RESIZE,
            LEFT_TOP_CORNER_RESIZE,
            RIGHT_TOP_CORNER_RESIZE,

            LEFT_VERTICAL_RESIZE,
            RIGHT_VERTICAL_RESIZE,
            TOP_HORIZONTAL_RESIZE,
            BOTTOM_HORIZONTAL_RESIZE,

            MOVE
        };

        static int mSeek;

        QSettings mSettings;
        DbObjects::Common::DbView *mViewModel;
        QVector<DbObjects::Common::DbIndex *> mIndices;

        QMenu *mContextMenu;

        QList<ArrowItem *> mArrowItems;

        QList<QGraphicsTextItem *> mIndexItems;

        Mode mMode;
        bool mFieldsTypesVisible;
        bool mIndicesVisible;
        QFont mFont;

        QImage *mViewImage;
        QImage *mKeyImage;
        QImage *mForeignKeyImage;
        QImage *mFieldImage;
        QImage *mAnchorImage;

        static const int SEEK_STEP = 20;
};

bool isView(QGraphicsItem *);

#endif // VIEWEITEM_H


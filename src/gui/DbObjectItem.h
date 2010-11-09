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

#ifndef DBOBJECTSITEM_H
#define DBOBJECTSITEM_H

#include <gui/GraphicsItem.h>
#include <QDomElement>
#include <QFont>
#include <QSettings>

class ArrowItem;
class QColor;
class QGraphicsSceneContextMenuEvent;
class QGraphicsTextItem;
class QGraphicsTextItem;
class QImage;

namespace DbObjects
{

namespace Common
{

class DbTable;

} // namespace Common

} // namespace DbObjects

/*!
 * \class DbObjectItem
 * \headerfile gui/DbObjectItem.h
 * \brief Parent class for graphics items that implements real database objects on the scene (like table or view)
 */
class DbObjectItem : public GraphicsItem 
{
    public:
        enum { Type = UserType + 10 };

    public:
        explicit DbObjectItem(QMenu *ipMenu = 0);
        virtual ~DbObjectItem();

        virtual void setFieldsTypesVisible(bool);
        
        virtual void addArrowItem(ArrowItem *);
        virtual void removeArrowItem(ArrowItem *);
        virtual void removeArrowItems();
        virtual QList<ArrowItem *> arrows() const;
        
        virtual QString name() const = 0;
        virtual QString schemaName() const = 0;

        virtual QDomElement toXml(QDomDocument &) const = 0;

        static void setSeek(int);
        static int seek();

    protected:
        void contextMenuEvent(QGraphicsSceneContextMenuEvent *);
        QVariant itemChange(GraphicsItemChange, const QVariant &);
        void mouseMoveEvent(QGraphicsSceneMouseEvent *);
        void mousePressEvent(QGraphicsSceneMouseEvent *);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *);
        void hoverMoveEvent(QGraphicsSceneHoverEvent *);
        void hoverLeaveEvent(QGraphicsSceneHoverEvent *);

        virtual void paintFieldImage(QPainter *, int) { };
        virtual void paintAdditionalInfo(QPainter *) { };

        virtual QDomElement toXml(QDomDocument &, const QString &) const;

        static const int SEEK_STEP = 20;

    private:
        QSettings mSettings;
        
        static int mSeek;

        bool mFieldsTypesVisible;

        QList<ArrowItem *> mArrowItems;

        QMenu *mContextMenu;

        // mode for table (you can resize table from different positions or move it)
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
        Mode mMode;
};

DbObjectItem * toDbObject(QGraphicsItem *);

#endif // DBOBJECTSITEM_H


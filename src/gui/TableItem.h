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

#ifndef TABLEITEM_H
#define TABLEITEM_H

#include <gui/DbObjectsItem.h>
#include <QSettings>

class QAction;
class QDomDocument;
class QDomElement;
class QMenu;

namespace DbObjects
{

namespace Common
{

class DbIndex;
class DbTable;

} // namespace Common

} // namespace DbObjects

/*
 * Graphics item, implements the database table. Support moving, resizing, changing of the color etc.
 */
class TableItem : public DbObjectsItem 
{
    public:
        enum { Type = UserType + 5 };

    public:
        TableItem(const QString &, const QString &, QMenu *, const QPoint &);
        ~TableItem();
        virtual int type() const;

        void addIndexItem(QGraphicsTextItem *);
        void setIndicesVisible(bool);

        QString foreignSchemaName(int) const;
        QString foreignTableName(int) const;
        bool isColumnForeignKey(int) const;
        int columnsCount() const;

        QString name() const;
        QString schemaName() const;

        QDomElement toXml(QDomDocument &) const;

    protected:
        void paintFieldImage(QPainter *, int);
        void paintIndeces(QPainter *);

    private:

        QSettings mSettings;

        QVector<DbObjects::Common::DbIndex *> mIndices;
        QList<QGraphicsTextItem *> mIndexItems;

        bool mIndicesVisible;
        
        QImage *mKeyImage;
        QImage *mForeignKeyImage;

        DbObjects::Common::DbTable *mModel;
};

bool isTable(QGraphicsItem *);

#endif // TABLEITEM_H


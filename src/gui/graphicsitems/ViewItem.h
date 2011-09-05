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

#include <dbobjects/common/DbObjectPtr.h>
#include <gui/graphicsitems/DbObjectItem.h>

class QAction;
class QDomDocument;
class QDomElement;
class QMenu;

namespace DbObjects
{
namespace Common
{
class DbView;

typedef DbObjectPtr<DbView> DbViewPtr;
}
}

namespace Gui {

namespace GraphicsItems {

class ArrowItem;

/*!
 * \class ViewItem
 * \headerfile gui/graphicsitems/ViewItem.h
 * \brief Graphics item, implements the database view. Support moving, resizing, changing of the color etc.
 */
class ViewItem : public DbObjectItem
{
    public:
        ViewItem(const QString &, const QString &, QMenu *, const QPoint &);
        ~ViewItem();
        virtual int type() const;

        QString name() const;
        QString schemaName() const;

        QDomElement toXml(QDomDocument &) const;
        // \todo add fromXml(...)

        void setFieldsTypesVisible(bool);
//        void setIndicesVisible(bool);

    public:
        enum { Type = UserType + 8 };

    private:
        DbObjects::Common::DbViewPtr mModel;
};

ViewItem *toView(QGraphicsItem *);

}

}

#endif // VIEWEITEM_H


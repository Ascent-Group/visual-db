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

#include <QDebug>
#include <QDomDocument>
#include <QDomElement>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <gui/TableItem.h>
#include <gui/ItemGroup.h>

/*!
 * Constructor
 */
ItemGroup::ItemGroup(QGraphicsItem *parent)
    : QGraphicsItemGroup(parent), mContextMenu(0)
{
    // allow selecting and moving of the group
    setAcceptsHoverEvents(true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
}

/*!
 * Destructor
 */
ItemGroup::~ItemGroup()
{
}

/*!
 * \brief Get the item's type
 *
 * \return Item's type
 */
int
ItemGroup::type() const
{
    return Type;
}

/*!
 * \brief Set context menu
 *
 * \param[in] ipContextMenu - Context menu for group
 */
void
ItemGroup::setContextMenu(QMenu *ipContextMenu)
{
    mContextMenu = ipContextMenu;
}

/*!
 * \brief Handler of the right mouse button click
 *
 * param[in] ipEvent - Context menu event
 */
void
ItemGroup::contextMenuEvent(QGraphicsSceneContextMenuEvent *ipEvent)
{
    if (mContextMenu) {
        setSelected(true);
        // show context menu
        mContextMenu->exec(ipEvent->screenPos());
    }
}

/*!
 * \brief Convert table group to xml
 *
 * \param[in] ipDoc - Dom document where the xml representation will be created
 * 
 * \return Xml representation of the items group 
 */
QDomElement
ItemGroup::toXml(QDomDocument &ipDoc)
{
    QDomElement element = ipDoc.createElement("tableGroup");
    foreach (QGraphicsItem *item, childItems()) {
        if (qgraphicsitem_cast<ItemGroup *>(item)) {
            element.appendChild(qgraphicsitem_cast<ItemGroup *>(item)->toXml(ipDoc));
        } else if (qgraphicsitem_cast<TableItem *>(item)) {
            element.appendChild(qgraphicsitem_cast<TableItem *>(item)->toXml(ipDoc));
        }
    }
    return element;
}

/*
 * Load table group from the xml file
 */
QList<QGraphicsItem *>
ItemGroup::fromXml(const QDomElement &ipElement, GraphicsScene *ipScene, QMenu *ipMenu)
{
    QList<QGraphicsItem *> tableList;
    // loop for all childs of this group
    QDomNode child = ipElement.firstChild();
    while (!child.isNull()) {
        QDomElement element = child.toElement();
        if (!element.isNull()) {
            if (element.tagName() == "table") {
                tableList << TableItem::fromXml(element, ipScene, ipMenu);
            } else if (element.tagName() == "tableGroup") {
                tableList << ItemGroup::fromXml(element, ipScene, ipMenu);
            }
        }
        child = child.nextSibling();
    }

    ItemGroup *group = ipScene->createItemGroup(tableList);
    group->setContextMenu(ipMenu);
    tableList << group;
    return tableList;
}

/*!
 * \brief Check if input graphics item is group item
 *
 * \param[in] ipItem - Graphics item
 *
 * \return True if given item is a group item, false if not
 */
ItemGroup *
toGroup(QGraphicsItem *ipItem)
{
    return qgraphicsitem_cast<ItemGroup *>(ipItem);
}

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
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QPen>
#include <common/Database.h>
#include <gui/graphicsitems/Legend.h>
#include <gui/graphicsitems/TableItem.h>

namespace Gui {

namespace GraphicsItems {

/*!
 * Constructor
 */
Legend::Legend()
    : GraphicsItem()
{
    setTitleItem(new QGraphicsTextItem("Legend"));

    setX(DEFAULT_X);
    setY(DEFAULT_Y);
    setBrush(Qt::white);
}

/*!
 * Destructor
 */
Legend::~Legend()
{
    clearFields();
}

/*!
 * \brief Return the item's type
 *
 * \return The type of this element
 */
int
Legend::type() const
{
    return Type;
}

/*!
 * \brief Refresh the list of schemas
 */
void
Legend::refresh()
{
    DbObjects::Common::Database *dbInst = DbObjects::Common::Database::instance();

    QStringList schemasNames;
    dbInst->schemasList(schemasNames);

    clearFields();
    for (int i = 0; i < schemasNames.count(); ++i) {
        addFieldItem(new QGraphicsTextItem(schemasNames.at(i)));
    }

    adjustSize();
}

///*
// *
// */
//void
//Legend::paintFieldImage(QPainter * /*iPainter*/, int /*iIdx*/)
//{
//}
//
///*
// *
// */
//void
//Legend::paintIndices(QPainter * /*iPainter*/)
//{
//}

/*!
 * \param[in] iPainter - Painter that will be used to draw field image.
 * \param[in] iIdx - Index of a field whose image is going to be drawn.
 *
 * \see GraphicsItem::paintFieldImage
 */
void
Legend::paintFieldImage(QPainter *iPainter, int iIdx)
{
    QColor color = QColor();
    bool initializedColor = false;
    bool hasAllSameColor = true;
    foreach (QGraphicsItem *item, scene()->items()) {
        // if it's a table
        TableItem *tableItem = toTable(item);
        if (tableItem && tableItem->schemaName() == fieldText(iIdx)) {
            if (!initializedColor) {
                initializedColor = true;
                color = tableItem->itemColor();
                // and if this table color is another than all privious table colors in this scene - break
            } else if (tableItem->itemColor() != color) {
                hasAllSameColor = false;
                break;
            }
        }
    }

    if (hasAllSameColor) {
        iPainter->fillRect((int)x() + INTERVAL, (int)y() + (FIELD_HEIGHT + INTERVAL) * (iIdx + 1) + INTERVAL * 2,
                IMG_WIDTH + INTERVAL, IMG_HEIGHT + INTERVAL - 1, color);
    } else {
        iPainter->drawText((int)x() + INTERVAL, (int)y() + (FIELD_HEIGHT + INTERVAL) * (iIdx + 1) + INTERVAL * 2,
                IMG_WIDTH + INTERVAL, IMG_HEIGHT + INTERVAL - 1, Qt::AlignCenter, QString("N"));
    }
}

/*!
 * \see GraphicsItem::paintAnchor
 */
void
Legend::paintAnchor(QPainter *)
{

}

/*!
 * \see GraphicsItem::paintAdditionalInfo
 */
void
Legend::paintAdditionalInfo(QPainter*)
{
}

/*!
 * \param[in] iItem - Graphics item to convert to Legend.
 *
 * \return Graphics item casted to Legend.
 */
Legend *
toLegend(QGraphicsItem *iItem)
{
    return qgraphicsitem_cast<Legend *>(iItem);
}

}

}

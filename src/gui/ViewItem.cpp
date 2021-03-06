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

#include <QDomDocument>
#include <QDomElement>
#include <QGraphicsScene>
#include <QMenu>
#include <QtDebug>
#include <common/Database.h>
#include <consts.h>
#include <gui/ViewItem.h>

/*!
 * Constructor
 */
ViewItem::ViewItem(const QString &ipSchemaName, const QString &ipViewName, QMenu *ipMenu, const QPoint &ipPos)
    : DbObjectItem(ipMenu)
{
    using namespace DbObjects::Common;
    Database *dbInst = Database::instance();

    // find schema
    DbSchemaPtr schema = dbInst->findSchema(ipSchemaName);

    // if found
    if (schema.get()) {
        // find view in this schema
        DbViewPtr view = schema->findView(ipViewName);

        // if found
        if (view.get()) {
            mModel = view;
        } else {
            qDebug() << "Cann't find this view: " << ipViewName;
            return;
        }
        // lyuts: maybe else we should do this
        // mViewModel = new PsqlView(ipSchemaName, ipViewName);
    } else {
        qDebug() << "Cann't find this schema: " << ipSchemaName;
        return;
    }

    // create title item
    setTitleItem(new QGraphicsTextItem(ipSchemaName.toUpper() + "." + ipViewName.toUpper()));

    // create field items
    foreach (QString fieldName, mModel->columnsNames()) {
        addFieldItem(new QGraphicsTextItem(fieldName));
    }
    
//    for (int i = 0; i < mModel->columnsCount(); ++i) {
//        addFieldItem(new QGraphicsTextItem(mModel->columnName(i) + ": " + mModel->columnType(i)));
//    }

    // set left top point coordinates
    if (ipPos.x() == 0 && ipPos.y() == 0) {
        setX(ipPos.x() + seek());
        setY(ipPos.y() + seek());

        setSeek(seek() + SEEK_STEP);
    } else {
        setX(ipPos.x());
        setY(ipPos.y());
    }

    // set width and height
    setWidth(DEFAULT_WIDTH);
    setHeight((mModel->columnsNames().size() + 1) * (FIELD_HEIGHT + INTERVAL) + INTERVAL * 3);

    updatePolygon();

    // set the color of the view
    setBrush(itemColor());
    setPen(borderColor());

    setZValue(0);

    // allow selecting and moving of the view
    setAcceptsHoverEvents(true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
}

/*!
 * Destructor
 */
ViewItem::~ViewItem()
{
}

/*!
 * \brief Return the type identifier
 *
 * \return Type identifier
 */
int
ViewItem::type() const
{
    return Type;
}

/*!
 * \brief Get the name of the view
 *
 * \return Name of the view
 */
QString
ViewItem::name() const
{
    return mModel.name();
}

/*!
 * \brief Get the schema name
 *
 * \return Schema name
 */
QString
ViewItem::schemaName() const
{
    return mModel.schemaName();
}

/*!
 * \brief Create the xml represantation for the view
 *
 * \param[in] ipDoc - Parent xml dom document
 *
 * \return Filled with schema info xml dom element connected to input document
 */
QDomElement
ViewItem::toXml(QDomDocument &ipDoc) const
{
    return DbObjectItem::toXml(ipDoc, "view");
}

/*!
 * \brief According to the given flag show or hide fields' types
 *
 * \param[in] ipFlag - True if we want to show field types, false otherwise
 */
void
ViewItem::setFieldsTypesVisible(bool ipFlag)
{
    // \todo Here will be view specific code when we will read view column definitions
    DbObjectItem::setFieldsTypesVisible(ipFlag);
}

/*!
 * \brief Check if input graphics item is view item
 *
 * \param[in] ipItem - any graphics item
 *
 * \return Converted to ViewItem element or 0 otherwise
 */
ViewItem *
toView(QGraphicsItem *ipItem)
{
    return qgraphicsitem_cast<ViewItem *>(ipItem);
}

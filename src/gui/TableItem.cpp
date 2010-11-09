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
#include <QGraphicsTextItem>
#include <QMenu>
#include <QPainter>
#include <QtDebug>
#include <common/Database.h>
#include <common/DbIndex.h>
#include <common/DbSchema.h>
#include <common/DbTable.h>
#include <consts.h>
#include <gui/TableItem.h>
#include <math.h>

/*
 * Constructor
 */
TableItem::TableItem(const QString &ipSchemaName, const QString &ipTableName, QMenu *ipMenu, const QPoint &ipPos)
    : DbObjectItem(ipMenu), mIndicesVisible(true)
{
    using namespace DbObjects::Common;
    Database *dbInst = Database::instance();

    // find schema
    DbSchema *schema = dbInst->findSchema(ipSchemaName);

    // if foung
    if (schema) {
        // find table in this schema
        DbTable *table = schema->findTable(ipTableName);

        // if found
        if (table) {
            mModel = table;
        } else {
            qDebug() << "Cann't find this table: " << ipTableName;
            return;
        }
        // lyuts: maybe else we should do this
        // mTableModel = new PsqlTable(ipSchemaName, ipTableName);
    } else {
        qDebug() << "Cann't find this schema: " << ipSchemaName;
        return;
    }

    // create title item
    setTitleItem(new QGraphicsTextItem(ipSchemaName.toUpper() + "." + ipTableName.toUpper()));

    // create field items
    for (int i = 0; i < mModel->columnsCount(); ++i) {
        addFieldItem(new QGraphicsTextItem(mModel->columnName(i) + ": " + mModel->columnType(i)));
    }

    dbInst->findTableIndices(mModel, mIndices);
    foreach (DbIndex *index, mIndices) {
        addIndexItem(new QGraphicsTextItem(index->name()));
    }

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

    mIndicesVisible = mSettings.value(Consts::PREFS_GRP + "/" + Consts::SHOW_INDICES_SETTING, false).toBool();
    if (mIndicesVisible) {
        setHeight((mModel->columnsCount() + mIndices.count() + 1) * (FIELD_HEIGHT + INTERVAL) + INTERVAL * 3);
    } else {
        setHeight((mModel->columnsCount() + 1) * (FIELD_HEIGHT + INTERVAL) + INTERVAL * 3);
    }

    updatePolygon();

    // set the color of the table
    setBrush(itemColor());
    setPen(borderColor());

    setZValue(0);

    // preload images
    mKeyImage = new QImage(":/img/key.png");
    mForeignKeyImage = new QImage(":/img/foreignkey.png");
    
    // allow selecting and moving of the table
    setAcceptsHoverEvents(true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
}

/*
 * Destructor
 */
TableItem::~TableItem()
{
}

/*
 * Return the type
 */
int
TableItem::type() const
{
    return Type;
}

/*
 *
 */
void
TableItem::addIndexItem(QGraphicsTextItem *ipIndexItem)
{
    mIndexItems << ipIndexItem;
}

/*
 *
 */
void
TableItem::paintFieldImage(QPainter *ipPainter, int ipIdx)
{
    QImage *image = 0;
    // draw image for primary key field with margins = INTERVAL for top, bottom, left and right sizes
    if (mModel->isColumnPrimaryKey(ipIdx)) {
        image = mKeyImage;
    } else if (mModel->isColumnForeignKey(ipIdx)) {
        image = mForeignKeyImage;
    }
    if (image) {
        QRectF target((int)x() + INTERVAL, (int)y() + (FIELD_HEIGHT + INTERVAL) * (ipIdx + 1) + INTERVAL,
                IMG_WIDTH + INTERVAL, IMG_HEIGHT + INTERVAL);
        QRectF source(0.0, 0.0, image->width(), image->height());
        ipPainter->drawImage(target, *image, source);
    }
}

/*
 *
 */
void
TableItem::paintIndeces(QPainter *ipPainter)
{
    // if we need to show indeces
    if (mIndicesVisible) {
        for (int i = 0; i < mIndexItems.size(); ++i) {
            // break drawing if we have reached the board
            if (height() < (FIELD_HEIGHT + INTERVAL) * (countFields() + i + 2) + INTERVAL) {
                break;
            }
            ipPainter->drawText((int)x() + IMG_WIDTH + 2 * INTERVAL,
                    (int)y() + (FIELD_HEIGHT + INTERVAL) * (countFields() + i + 1) + INTERVAL,
                    (int)width() - IMG_WIDTH - INTERVAL * 3,
                    FIELD_HEIGHT + INTERVAL * 2, Qt::AlignLeft,
                    mIndexItems.at(i)->toPlainText());
        }
    }
}

/*
 * Show or hide indices
 */
void
TableItem::setIndicesVisible(bool ipFlag)
{
    mIndicesVisible = ipFlag;
    update(x(), y(), width(), height());
}

/*
 * Get the foreign schema name
 */
QString
TableItem::foreignSchemaName(int ipIdx) const
{
    return mModel->foreignSchemaName(ipIdx);
}

/*
 * Get the foreign table name
 */
QString
TableItem::foreignTableName(int ipIdx) const
{
    return mModel->foreignTableName(ipIdx);
}

/*
 * Get the flag for field - is it foreign key or not
 */
bool
TableItem::isColumnForeignKey(int ipIdx) const
{
    return mModel->isColumnForeignKey(ipIdx);
}

/*
 * Get columns count
 */
int
TableItem::columnsCount() const
{
    return mModel->columnsCount();
}

/*
 * Get the name of the view item
 */
QString
TableItem::name() const
{
    return mModel->name();
}

/*
 * Get the name of the schema
 */
QString
TableItem::schemaName() const
{
    return mModel->schema()->name();
}

/*
 * Create the xml represantation for the table
 */
QDomElement
TableItem::toXml(QDomDocument &ipDoc) const
{
    return DbObjectItem::toXml(ipDoc, "table");
}

/*
 * Load table from the xml file
 */
TableItem *
TableItem::fromXml(const QDomElement &ipElement, GraphicsScene *ipScene, QMenu *ipMenu)
{
    // get table's coordinates
    TableItem *newTable = ipScene->newTableItem(ipElement.attribute("schema"),
            ipElement.attribute("name"), ipMenu, QPoint(ipElement.attribute("x").toInt(),
            ipElement.attribute("y").toInt()));

    // get table's size
    newTable->setWidth(ipElement.attribute("width").toInt());
    newTable->setHeight(ipElement.attribute("height").toInt());
    newTable->updatePolygon();

    // get table's color
    newTable->setItemColor(QColor(ipElement.attribute("red").toInt(),
                ipElement.attribute("green").toInt(), ipElement.attribute("blue").toInt()));

    return newTable;
}

/*
 * Check if input graphics item is table item
 */
TableItem *
toTable(QGraphicsItem *ipItem)
{
    return qgraphicsitem_cast<TableItem *>(ipItem);
}

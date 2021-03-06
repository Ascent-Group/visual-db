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
#include <QSettings>
#include <common/Database.h>
#include <consts.h>
#include <gui/GraphicsScene.h>
#include <gui/TableItem.h>

#include <QtDebug>

/*!
 * Constructor
 */
TableItem::TableItem(const QString &ipSchemaName, const QString &ipTableName, QMenu *ipMenu, const QPoint &ipPos)
    : DbObjectItem(ipMenu),
      mIndexItems(),
      mIndicesVisible(true)
{
    using namespace DbObjects::Common;
    Database *dbInst = Database::instance();

    // find schema
    DbSchemaPtr schema = dbInst->findSchema(ipSchemaName);

    // if foung
    if (schema.get()) {
        // find table in this schema
        DbTablePtr table = schema->findTable(ipTableName);

        // if found
        if (table.get()) {
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
    foreach (const DbIndexPtr &index, mIndices) {
        addIndexItem(new QGraphicsTextItem(index.name()));
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

    QSettings settings;
    mIndicesVisible = settings.value(Consts::PREFS_GRP + "/" + Consts::SHOW_INDICES_SETTING, false).toBool();
    setIndicesVisible(mIndicesVisible);
//    if (mIndicesVisible) {
//        setHeight((mModel->columnsCount() + mIndices.count() + 1) * (FIELD_HEIGHT + INTERVAL) + INTERVAL * 3);
//    } else {
//        setHeight((mModel->columnsCount() + 1) * (FIELD_HEIGHT + INTERVAL) + INTERVAL * 3);
//    }

    updatePolygon();

    // set the color of the table
    setBrush(itemColor());
    setPen(borderColor());

    setZValue(0);

    // preload images
    mTableImage = new QImage(":/img/table.png");
    mKeyImage = new QImage(":/img/key.png");
    mForeignKeyImage = new QImage(":/img/foreignkey.png");
    mPrimaryAndForeignKeyImage = new QImage(":/img/primary_and_foreign.png");
    mIndexImage = new QImage(":/img/index.png");

    // allow selecting and moving of the table
    setAcceptsHoverEvents(true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
}

/*!
 * Constructor
 */
TableItem::FullName::FullName(const QString &ipSchemaName, const QString &ipTableName)
    : mSchemaName(ipSchemaName), mTableName(ipTableName)
{
}

/*!
 * Destructor
 */
TableItem::~TableItem()
{
    qDeleteAll(mIndexItems);
    delete mTableImage;
    delete mKeyImage;
    delete mForeignKeyImage;
    delete mPrimaryAndForeignKeyImage;
}

/*!
 * \brief Return the type
 *
 * \return Type identifier
 */
int
TableItem::type() const
{
    return Type;
}

/*!
 * \brief Adds input index item to the list of indices for this table
 *
 * \param[in] ipIndexItem - Index item we will add
 */
void
TableItem::addIndexItem(QGraphicsTextItem *ipIndexItem)
{
    mIndexItems << ipIndexItem;
}

/*!
 * \brief Draws an image in the title bar of the item
 *
 * \param[in] ipPainter - Painter
 */
void
TableItem::paintTitleImage(QPainter *ipPainter)
{
    // draw image for table
    QRectF target((int)x() + INTERVAL, (int)y() + INTERVAL,
            IMG_HEIGHT + INTERVAL, IMG_HEIGHT + INTERVAL);
    QRectF source(0.0, 0.0, mTableImage->width(), mTableImage->height());
    ipPainter->drawImage(target, *mTableImage, source);
}

/*!
 * \brief Paints field image
 *
 * \param[in] ipPainter - Painter
 * \param[in] ipIdx - Index of field we will draw image for
 */
void
TableItem::paintFieldImage(QPainter *ipPainter, int ipIdx)
{
    QImage *image = 0;
    // draw image for primary key field with margins = INTERVAL for top, bottom, left and right sizes
    if (mModel->isColumnPrimaryKey(ipIdx)) {
        if (mModel->isColumnForeignKey(ipIdx)) {
            image = mPrimaryAndForeignKeyImage;
        } else {
            image = mKeyImage;
        }
    } else if (mModel->isColumnForeignKey(ipIdx)) {
        image = mForeignKeyImage;
    }
    if (image) {
        QRectF target((int)x() + INTERVAL, (int)y() + (FIELD_HEIGHT + INTERVAL) * (ipIdx + 1) + INTERVAL * 2,
                IMG_WIDTH + INTERVAL, IMG_HEIGHT + INTERVAL);
        QRectF source(0.0, 0.0, image->width(), image->height());
        ipPainter->drawImage(target, *image, source);
    }
}

/*!
 * \brief Paints indices
 *
 * \param[in] ipPainter - Painter
 */
void
TableItem::paintAdditionalInfo(QPainter *ipPainter)
{
    // if we need to show indices
    if (mIndicesVisible) {
        for (int i = 0; i < mIndexItems.size(); ++i) {
            // break drawing if we have reached the board
            if (height() < (FIELD_HEIGHT + INTERVAL) * (countFields() + i + 2) + INTERVAL) {
                break;
            }
            
            QRectF target((int)x() + INTERVAL, 
                    (int)y() + (FIELD_HEIGHT + INTERVAL) * (countFields() + i + 1) + INTERVAL * 2,
                    IMG_WIDTH + INTERVAL, 
                    IMG_HEIGHT + INTERVAL);
            QRectF source(0.0, 0.0, mIndexImage->width(), mIndexImage->height());
            ipPainter->drawImage(target, *mIndexImage, source);
           
            ipPainter->drawText((int)x() + IMG_WIDTH + 3 * INTERVAL,
                    (int)y() + (FIELD_HEIGHT + INTERVAL) * (countFields() + i + 1) + INTERVAL * 2,
                    (int)width() - IMG_WIDTH - INTERVAL * 4,
                    FIELD_HEIGHT + INTERVAL * 2, Qt::AlignLeft,
                    mIndexItems.at(i)->toPlainText());
        }
    }
}

/*!
 * \brief Show or hide indices
 *
 * \param[in] ipFlag - True if we need to show indices, false if we don't
 */
void
TableItem::setIndicesVisible(bool ipFlag)
{
    mIndicesVisible = ipFlag;
    if (ipFlag) {
        setHeight((mModel->columnsCount() + mIndices.count() + 1) * (FIELD_HEIGHT + INTERVAL) + INTERVAL * 4);
    } else {
        setHeight((mModel->columnsCount() + 1) * (FIELD_HEIGHT + INTERVAL) + INTERVAL * 3);
    }

    updatePolygon();
}

/*!
 * \brief Get the foreign schema name for field
 *
 * \param[in] ipIdx - Field index for which we want to get info
 *
 * \return Foreign schema name
 */
QString
TableItem::foreignSchemaName(int ipIdx) const
{
    return mModel->foreignSchemaName(ipIdx);
}

/*!
 * \brief Get the foreign table name
 *
 * \param[in] ipIdx - Field index for which we want to get info
 *
 * \return Foreign table name
 */
QString
TableItem::foreignTableName(int ipIdx) const
{
    return mModel->foreignTableName(ipIdx);
}

/*!
 * \brief Get the flag for field - is it foreign key or not
 *
 * \param[in] ipIdx - Field index for which we want to get info
 *
 * \return True if the column is foreign key, false if it isn't
 */
bool
TableItem::isColumnForeignKey(int ipIdx) const
{
    return mModel->isColumnForeignKey(ipIdx);
}

/*!
 * \brief Get columns count
 *
 * \return Columns count
 */
int
TableItem::columnsCount() const
{
    return mModel->columnsCount();
}

/*!
 * \brief Get the name of the table item
 *
 * \return Table item name
 */
QString
TableItem::name() const
{
    return mModel.name();
}

/*!
 * \brief Get the name of the schema
 *
 * \return Schema name
 */
QString
TableItem::schemaName() const
{
    return mModel.schemaName();
}

/*!
 * \brief Return list of full names of parents
 *
 * \return List of full names of parents
 */
QList<TableItem::FullName>
TableItem::parents() const
{
    QVector<DbObjects::Common::DbTablePtr> parentsPtr;
    QList<TableItem::FullName> parentsFullNames;

    mModel->parentTables(parentsPtr);
    foreach (DbObjects::Common::DbTablePtr parent, parentsPtr) {
        parentsFullNames.push_back(TableItem::FullName(parent.schemaName(), parent.name()));
    }

    return parentsFullNames;
}

/*!
 * \brief Create the xml represantation for the table
 *
 * \param[in] ipDoc - Xml dom document
 *
 * \return Filled with table info xml dom element
 */
QDomElement
TableItem::toXml(QDomDocument &ipDoc) const
{
    return DbObjectItem::toXml(ipDoc, "table");
}

/*!
 * \brief Load table from the xml file
 *
 * \param[in] ipElement - Xml dom element we will read the table info from
 * \param[in] ipScene - Scene where we will create new table item for
 * \param[in] ipMenu - Context menu for new table item
 *
 * \return Created table item
 */
TableItem *
TableItem::fromXml(const QDomElement &ipElement, GraphicsScene *ipScene, QMenu *ipMenu)
{
    // get table's coordinates
    TableItem *newTable = ipScene->newTableItem(ipElement.attribute("schema"),
            ipElement.attribute("name"), ipMenu, QPoint(ipElement.attribute("x").toInt(),
            ipElement.attribute("y").toInt()));

        // FIXME: we should check the newTable if it isn't null

    // get table's size
    newTable->setWidth(ipElement.attribute("width").toInt());
    newTable->setHeight(ipElement.attribute("height").toInt());
    newTable->updatePolygon();

    // get table's color
    newTable->setItemColor(QColor(ipElement.attribute("red").toInt(),
                ipElement.attribute("green").toInt(), ipElement.attribute("blue").toInt()));

    return newTable;
}

/*!
 * \brief According to the given flag show or hide fields' types
 *
 * \param[in] ipFlag - True if we want to show field types, false otherwise
 */
void
TableItem::setFieldsTypesVisible(bool ipFlag)
{
    // TODO: implement it according new class structure
    for (int i = 0; i < countFields(); ++i) {
        if (ipFlag) {
            setFieldText(i, mModel->columnName(i) + ": " + mModel->columnType(i));
        } else {
            setFieldText(i, mModel->columnName(i));
        }
    }

    DbObjectItem::setFieldsTypesVisible(ipFlag);
}

/*!
 * \brief Check if input graphics item is table item
 *
 * \param[in] ipItem - Any graphics item
 *
 * \return Converted to TableItem item or 0
 */
TableItem *
toTable(QGraphicsItem *ipItem)
{
    return qgraphicsitem_cast<TableItem *>(ipItem);
}


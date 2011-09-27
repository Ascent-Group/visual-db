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

#include <common/Database.h>
#include <common/DbSchema.h>
#include <common/DbView.h>

#include <QtDebug>

namespace DbObjects
{

namespace Common
{

/*!
 * Constructor
 *
 * \param[in] iName - Name of a view
 * \param[in] iSchema - Handle to schema that contains the given view
 */
DbView::DbView(QString iName, const DbSchemaPtr &iSchema)
    : DbObject(iName),
      mSchema(iSchema),
      mOwner(),
      mDefinition("")
{
    if (!mSchema.get()) qDebug() << "DbView::DbView> mSchema is NULL!";
}

/*!
 * Dtor
 */
DbView::~DbView()
{

}

/*!
 * \return View's full name in a "Schema.View" format
 */
QString
DbView::fullName() const
{
    return QString("%1.%2").arg(mSchema->name()).arg(mName);
}

/*!
 * \return Schema containing this view
 */
DbSchemaPtr
DbView::schema() const
{
    return mSchema;
}

/*!
 * \param[in] iSchema - Parent schema for the view
 */
void
DbView::setSchema(const DbSchemaPtr &iSchema)
{
    mSchema = iSchema;
}

/*!
 * \return Owner of this view
 */
DbRolePtr
DbView::owner() const
{
    return mOwner;
}

/*!
 * \param[in] iOwner - Owner for this view
 */
void
DbView::setOwner(const DbRolePtr &iOwner)
{
    mOwner = iOwner;
}

/*!
 * \return The definition (reconstructed select)
 */
QString
DbView::definition() const
{
    return mDefinition;
}

/*!
 * Sets the definition for this view
 *
 * \param[in] iDef - View's definition
 */
void
DbView::setDefinition(const QString &iDef)
{
    mDefinition = iDef;
}

/*!
 * \brief Get columns names of the given view.
 * \note No need to pass stringlist by reference because this will give no benefit.
 *
 * \return List of columns names.
 */
QStringList
DbView::columnsNames() const
{
    return mColumnsNames;
}

/*!
 * \brief Resets view description read from database
 */
void
DbView::resetData()
{
    mSchema = DbSchemaPtr();
    mOwner = DbRolePtr();
    mDefinition = "";
    mColumnsNames.clear();

    DbObject::resetData();
}

/*!
 * \return Database object type identifier
 */
ObjectType
DbView::type() const
{
    return ViewObject;
}

} // namespace Common

} // namespace DbObjects


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
#include <common/DbView.h>

#include <QtDebug>

namespace DbObjects
{

namespace Common
{

/*!
 * Ctor
 * \param[in] ipSchemaName - Schema name
 * \param[in] ipName - View name
 */
DbView::DbView(QString ipSchemaName, QString ipName)
    : DbObject(ipName), mSchemaName(ipSchemaName)
{
    setSchema(Database::instance()->findSchema(mSchemaName));
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
    return QString("%1.%2").arg(mSchemaName).arg(mName);
}

/*!
 * \return Schema containing this view
 */
DbSchema*
DbView::schema() const
{
    return mSchema;
}

/*!
 * \param[in] ipSchema - Parent schema for the view
 */
void
DbView::setSchema(DbSchema *ipSchema)
{
    mSchema = ipSchema;
}

/*!
 * \return Owner of this view
 */
DbRole*
DbView::owner() const
{
    return mOwner;
}

/*!
 * \param[in] ipOwner - Owner for this view
 */
void
DbView::setOwner(DbRole *ipOwner)
{
    mOwner = ipOwner;
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
 * \param[in] ipDef - View's definition
 */
void
DbView::setDefinition(QString ipDef)
{
    mDefinition = ipDef;
}

/*!
 * \brief Resets view description read from database
 */
void
DbView::resetData()
{
    /*! \todo Implement */
    mSchemaName = "";
    mSchema = 0;
    mOwner = 0;
    mDefinition = "";

    DbObject::resetData();
}

/*!
 * \return Database object type identifier
 */
DbObject::Type
DbView::type() const
{
    return DbObject::ViewObject;
}

} // namespace Common

} // namespace DbObjects


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

#include <common/DbObject.h>
#include <QtDebug>

namespace DbObjects
{

namespace Common
{

/*!
 * Ctor
 */
DbObject::DbObject(QString iName)
    : mName(iName),
      mIsLoaded(false)
{

}

/*!
 * Dtor
 */
DbObject::~DbObject()
{

}

/*!
 * \return Name of the object
 */
QString
DbObject::name() const
{
    return mName;
}

/*!
 * \brief Sets the name of the object
 *
 * \param[in] iName - Name of the object
 */
void
DbObject::setName(const QString &iName)
{
    mName = iName;
}

/*!
 * \return true If the object description has been already loaded from db
 * \return false Otherwise
 */
bool
DbObject::isLoaded() const
{
    return mIsLoaded;
}

/*!
 *
 */
void
DbObject::setDatabase(const DbObjects::Common::Database *iDatabase)
{
    mDatabase = iDatabase;
}

/*!
 * \brief Loads object informaton from datase
 *
 * \return true If the information has been successfully read
 * \return false Otherwise
 */
bool
DbObject::loadData()
{
    qDebug() << "DbObject::loadData> for " << mName;
    mIsLoaded = true;
    return true;
}

/*!
 * \brief Resets object information read from database
 */
void
DbObject::resetData()
{
    mName = "";
    mIsLoaded = false;
}

} // namespace Common

} // namespace DbObjects


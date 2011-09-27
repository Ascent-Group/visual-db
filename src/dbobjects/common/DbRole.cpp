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
#include <common/DbRole.h>

namespace DbObjects
{

namespace Common
{

/*!
 * Ctor
 */
DbRole::DbRole(QString iName)
    : DbObject(iName),
      mIsSuperUser(false),
      mInheritsPrivileges(false),
      mCanCreateRole(false),
      mCanCreateDb(false),
      mCanUpdateSysCat(false),
      mCanLogin(false),
      mConnectionLimit(false),
      mExpiryDate(),
      mId(0)
{

}

/*!
 * Dtor
 */
DbRole::~DbRole()
{

}

/*!
 * \return true - If the role has superuser privileges
 * \return false - Otherwise
 */
bool
DbRole::isSuperUser() const
{
    return mIsSuperUser;
}

/*!
 * \brief Sets the rolsuper flag
 *
 * \param[in] iFlag
 */
void
DbRole::setSuperUser(bool iFlag)
{
    mIsSuperUser = iFlag;
}

/*!
 * \return true - If role inherits its parent's privileges
 * \return false - Otherwise
 */
bool
DbRole::inheritsPrivileges() const
{
    return mInheritsPrivileges;
}

/*!
 * \brief Sets the rolinherit flag
 *
 * \param[in] iFlag
 */
void
DbRole::setInheritsPrivileges(bool iFlag)
{
    mInheritsPrivileges = iFlag;
}

/*!
 * \return true - If role is allowed to create roles
 * \return false - Otherwise
 */
bool
DbRole::canCreateRole() const
{
    return mCanCreateRole;
}

/*!
 * \brief Sets the rolcreaterole flag
 *
 * \param[in] iFlag
 */
void
DbRole::setCanCreateRole(bool iFlag)
{
    mCanCreateRole = iFlag;
}

/*!
 * \return true - If role is allowed to create db
 * \return false - Otherwise
 */
bool
DbRole::canCreateDb() const
{
    return mCanCreateDb;
}

/*!
 * \brief Sets the rolcreatedb flag
 *
 * \param[in] iFlag
 */
void
DbRole::setCanCreateDb(bool iFlag)
{
    mCanCreateDb = iFlag;
}

/*!
 * \return true - If role is allowed to update system catalogs
 * \return false - Otherwise
 */
bool
DbRole::canUpdateSysCat() const
{
    return mCanUpdateSysCat;
}

/*!
 * \brief Sets the rolcatupdate flag
 *
 * \param[in] iFlag
 */
void
DbRole::setCanUpdateSysCat(bool iFlag)
{
    mCanUpdateSysCat = iFlag;
}

/*!
 * \return true - If role is allowed to login
 * \return false - Otherwise
 */
bool
DbRole::canLogin() const
{
    return mCanLogin;
}

/*!
 * \brief Sets the rolcanlogin flag
 *
 * \param[in] iFlag
 */
void
DbRole::setCanLogin(bool iFlag)
{
    mCanLogin = iFlag;
}

/*!
 * \return The connection limit for the given role
 */
quint32
DbRole::connectionLimit() const
{
    return mConnectionLimit;
}

/*!
 * \brief Sets the connection limit for the role
 *
 * \param[in] iLimit - Connection limit for the given role
 */
void
DbRole::setConnectionLimit(quint32 iLimit)
{
    mConnectionLimit = iLimit;
}

/*!
 * \return Role's expiration date
 */
QDate
DbRole::expiryDate() const
{
    return mExpiryDate;
}

/*!
 * \brief Sets the expiration date of the role
 *
 * \param[in] iDate - Expiration date
 */
void
DbRole::setExpiryDate(const QDate &iDate)
{
    mExpiryDate = iDate;
}

/*!
 * \return The id of the role
 */
quint64
DbRole::id() const
{
    return mId;
}

/*!
 * \brief Sets the id for the role
 *
 * \param[in] iId - Role id
 */
void
DbRole::setId(quint64 iId)
{
    mId = iId;
}

/*!
 * \brief Resets role description read from database
 */
void
DbRole::resetData()
{
    mIsSuperUser = false;
    mInheritsPrivileges = false;
    mCanCreateRole = false;
    mCanCreateDb = false;
    mCanUpdateSysCat = false;
    mCanLogin = false;
    mConnectionLimit = 0;
    mExpiryDate = QDate();
    mId = 0;

    DbObject::resetData();
}

/*!
 * \return Database object type
 *
 * \see ObjectType
 */
ObjectType
DbRole::type() const
{
    return RoleObject;
}

} // namespace Common

} // namespace DbObjects


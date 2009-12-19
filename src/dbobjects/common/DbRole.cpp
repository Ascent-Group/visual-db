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

/*
 * Ctor
 */
DbRole::DbRole(QString ipName)
    : DbObject(ipName), mId(0)
{

}

/*
 * Dtor
 */
DbRole::~DbRole()
{

}

/*
 * Returns true if the role has superuser privileges
 */
bool
DbRole::isSuperUser() const
{
    return mIsSuperUser;
}

/*
 * Sets the rolsuper flag
 */
void
DbRole::setSuperUser(const bool ipFlag)
{
    mIsSuperUser = ipFlag;
}

/*
 * Returns true if role inherits its parent's privileges
 */
bool
DbRole::inheritsPrivileges() const
{
    return mInheritsPrivileges;
}

/*
 * Sets the rolinherit flag
 */
void
DbRole::setInheritsPriviligese(const bool ipFlag)
{
    mInheritsPrivileges = ipFlag;
}

/*
 * Returns true if role is allowed to create roles
 */
bool
DbRole::canCreateRole() const
{
    return mCanCreateRole;
}

/*
 * Sets the rolcreaterole flag
 */
void
DbRole::setCanCreateRole(const bool ipFlag)
{
    mCanCreateRole = ipFlag;
}

/*
 * Returns true if role is allowed to create db
 */
bool
DbRole::canCreateDb() const
{
    return mCanCreateDb;
}

/*
 * Sets the rolcreatedb flag
 */
void
DbRole::setCanCreateDb(const bool ipFlag)
{
    mCanCreateDb = ipFlag;
}

/*
 * Returns true if role is allowed to update system catalogs
 */
bool
DbRole::canUpdateSysCat() const
{
    return mCanUpdateSysCat;
}

/*
 * Sets the rolcatupdate flag
 */
void
DbRole::setCanUpdateSysCat(const bool ipFlag)
{
    mCanUpdateSysCat = ipFlag;
}

/*
 * Returns true if role is allowed to login
 */
bool
DbRole::canLogin() const
{
    return mCanLogin;
}

/*
 * Sets the rolcanlogin flag
 */
void
DbRole::setCanLogin(const bool ipFlag)
{
    mCanLogin = ipFlag;
}

/*
 * Returns the connection limit for the given role
 */
int
DbRole::connectionLimit() const
{
    return mConnectionLimit;
}

/*
 * Sets the connection limit for the role
 */
void
DbRole::setConnectionLimit(const int ipLimit)
{
    mConnectionLimit = ipLimit;
}

/*
 * Returns role's expiration date
 */
QDate
DbRole::expiryDate() const
{
    return mExpiryDate;
}

/*
 * Sets the expiration date of the role
 */
void
DbRole::setExpiryDate(const QDate ipDate)
{
    mExpiryDate = ipDate;
}

/*
 * Returns the id of the role
 */
int
DbRole::id() const
{
    return mId;
}

/*
 * Sets the id for the role
 */
void
DbRole::setId(const int ipId)
{
    mId = ipId;
}

/*
 * Returns the id of database object type
 */
DbObject::Type
DbRole::type()
{
    return DbObject::RoleObject;
}

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

#ifndef DBOBJECTS_COMMON_DBROLE_H
#define DBOBJECTS_COMMON_DBROLE_H

#include <common/DbObject.h>
#include <common/DbObjectPtr.h>
#include <QDate>

namespace DbObjects
{

namespace Common
{

/*!
 * \class DbRole
 * \brief Defines database role object
 * \headerfile common/DbRole.h
 */
class DbRole : public DbObject
{
    public:
        virtual ~DbRole();

        bool isSuperUser() const;
        void setSuperUser(bool ipFlag);

        bool inheritsPrivileges() const;
        void setInheritsPrivileges(bool ipFlag);

        bool canCreateRole() const;
        void setCanCreateRole(bool ipFlag);

        bool canCreateDb() const;
        void setCanCreateDb(bool ipFlag);

        bool canUpdateSysCat() const;
        void setCanUpdateSysCat(bool ipFlag);

        bool canLogin() const;
        void setCanLogin(bool ipFlag);

        quint32 connectionLimit() const;
        void setConnectionLimit(quint32 ipLimit);

        QDate expiryDate() const;
        void setExpiryDate(const QDate &ipDate);

        quint64 id() const;
        void setId(quint64 ipId);

        virtual void resetData();
        // virtual void save() = 0;

        virtual DbObject::Type type() const;

    protected:
        /*! Does role have superuser privileges? */
        bool mIsSuperUser;
        /*! Role automatically inherits privileges of roles it is a member of */
        bool mInheritsPrivileges; // true if the role inherits its parent's privileges
        /*! Can role create other roles */
        bool mCanCreateRole;
        /*! Can role create databases */
        bool mCanCreateDb;
        /*! Can role update system catalogs directly. (Even a superuser cannot do this unless this column is true.) */
        bool mCanUpdateSysCat;
        /*! Role can log in. That is, this role can be given as the initial session auth id */
        bool mCanLogin;
        // I even doubt that any server can handle such number of connections
        /*! Connection limit for the given role */
        quint32 mConnectionLimit;
        /*! Date when the role expires */
        QDate mExpiryDate;
        /*! Db role id */
        quint64 mId;

    protected:
        DbRole(QString ipName = 0);
};

} // namespace Common

} // namespace DbObjects

#endif // DBOBJECTS_COMMON_DBROLE_H


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

#ifndef DBROLE_H
#define DBROLE_H

#include <QDate>
#include <common/DbObject.h>

class DbRole : public DbObject
{
    public:
        virtual ~DbRole();

        bool isSuperUser() const;
        void setSuperUser(const bool ipFlag);

        bool inheritsPrivileges() const;
        void setInheritsPriviligese(const bool ipFlag);

        bool canCreateRole() const;
        void setCanCreateRole(const bool ipFlag);

        bool canCreateDb() const;
        void setCanCreateDb(const bool ipFlag);

        bool canUpdateSysCat() const;
        void setCanUpdateSysCat(const bool ipFlag);

        bool canLogin() const;
        void setCanLogin(const bool ipFlag);

        int connectionLimit() const;
        void setConnectionLimit(const int ipLimit);

        QDate expiryDate() const;
        void setExpiryDate(const QDate ipDate);

        int id() const;
        void setId(const int ipId);

        virtual void loadData() = 0;
        // lyuts: for future use
        // virtual void save() = 0;

        virtual DbObject::Type type();

    protected:
        bool mIsSuperUser;
        bool mInheritsPrivileges; // true if the role inherits its parent's privileges
        bool mCanCreateRole;
        bool mCanCreateDb;
        bool mCanUpdateSysCat;
        bool mCanLogin;
        int mConnectionLimit;
        QDate mExpiryDate;
        int mId;

    protected:
        DbRole(QString ipName = 0);
};

#endif // DBROLE_H

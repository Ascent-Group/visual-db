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

#ifndef DBOBJECTS_FACTORY_TRIGGER_H
#define DBOBJECTS_FACTORY_TRIGGER_H

#include <QtCore/qglobal.h>

namespace DbObjects
{

namespace Common
{
class DbTrigger;
}

namespace Psql
{
class Trigger;
}

namespace Factory
{

/*!
 * \class Trigger
 * \headerfile <factory/Trigger.h>
 * \brief Defines database trigger factory
 */
class Trigger
{
    public:
        static Common::DbTrigger* createTrigger(const QString &ipSchemaName, const QString &ipName);

    protected:
        static Psql::Trigger* createPsqlTrigger(const QString &ipSchemaName, const QString &ipName);
//        static Mysql::Trigger* createMysqlTrigger(const QString &ipName);

    private:
        Q_DISABLE_COPY(Trigger);
};

} // namespace Factory

} // namespace DbObjects

#endif // DBOBJECTS_FACTORY_TRIGGER_H


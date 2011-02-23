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

#ifndef DBOBJECTS_COMMON_PROCEDUREFACTORY_H
#define DBOBJECTS_COMMON_PROCEDUREFACTORY_H

#include <QString>
#include <qglobal.h>

namespace DbObjects
{
namespace Common
{
template<typename T> class DbObjectPtr;
class DbSchema;
typedef DbObjectPtr<DbSchema> DbSchemaPtr;

class DbProcedure;

/*!
 * \class ProcedureFactory
 * \headerfile common/ProcedureFactory.h
 * \brief Defines an abstract factory for procedures
 */
class ProcedureFactory
{
    public:
        virtual ~ProcedureFactory();
        virtual DbProcedure* create(const QString &iName, const DbSchemaPtr &iSchema) const = 0;

    protected:
        ProcedureFactory();

        Q_DISABLE_COPY(ProcedureFactory);
};

} // namespace Common
} // namespace DbObjects

#endif // DBOBJECTS_COMMON_PROCEDUREFACTORY_H


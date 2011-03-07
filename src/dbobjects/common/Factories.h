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

#ifndef DBOBJECTS_COMMON_FACTORIES_H
#define DBOBJECTS_COMMON_FACTORIES_H

#include <qglobal.h>

namespace DbObjects
{
namespace Common
{

class IndexFactory;
class LanguageFactory;
class ProcedureFactory;
class RoleFactory;
class SchemaFactory;
class TableFactory;
class TriggerFactory;
class ViewFactory;

/*!
 * \class Factories
 * \headerfile common/Factories.h
 * \brief Defines a collection of factories with factory methods for all database objects
 */
class Factories
{
    public:
        virtual ~Factories();

        DbObjects::Common::IndexFactory* indexFactory() const;
        DbObjects::Common::LanguageFactory* languageFactory() const;
        DbObjects::Common::ProcedureFactory* procedureFactory() const;
        DbObjects::Common::RoleFactory* roleFactory() const;
        DbObjects::Common::SchemaFactory* schemaFactory() const;
        DbObjects::Common::TableFactory* tableFactory() const;
        DbObjects::Common::TriggerFactory* triggerFactory() const;
        DbObjects::Common::ViewFactory* viewFactory() const;

    protected:
        Factories();

        Q_DISABLE_COPY(Factories);

        DbObjects::Common::IndexFactory *mIndexFactory;
        DbObjects::Common::LanguageFactory *mLanguageFactory;
        DbObjects::Common::ProcedureFactory *mProcedureFactory;
        DbObjects::Common::RoleFactory *mRoleFactory;
        DbObjects::Common::SchemaFactory *mSchemaFactory;
        DbObjects::Common::TableFactory *mTableFactory;
        DbObjects::Common::TriggerFactory *mTriggerFactory;
        DbObjects::Common::ViewFactory *mViewFactory;

};

} // namespace Common
} // namespace DbObjects

#endif // DBOBJECTS_COMMON_FACTORIES_H


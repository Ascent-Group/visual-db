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

#include <common/Factories.h>

#include <common/IndexFactory.h>
#include <common/LanguageFactory.h>
#include <common/ProcedureFactory.h>
#include <common/RoleFactory.h>
#include <common/SchemaFactory.h>
#include <common/TableFactory.h>
#include <common/TriggerFactory.h>
#include <common/ViewFactory.h>

namespace DbObjects
{
namespace Common
{

/*!
 * Constructor
 */
Factories::Factories()
    : mIndexFactory(0),
      mLanguageFactory(0),
      mProcedureFactory(0),
      mRoleFactory(0),
      mSchemaFactory(0),
      mTableFactory(0),
      mTriggerFactory(0),
      mViewFactory(0)
{

}

/*!
 * Destructor
 */
Factories::~Factories()
{
    delete mIndexFactory;
    delete mLanguageFactory;
    delete mProcedureFactory;
    delete mRoleFactory;
    delete mSchemaFactory;
    delete mTableFactory;
    delete mTriggerFactory;
    delete mViewFactory;
}

/*!
 * \return Factory for indices
 */
DbObjects::Common::IndexFactory*
Factories::indexFactory() const
{
    return mIndexFactory;
}

/*!
 * \return Factory for languages
 */
DbObjects::Common::LanguageFactory*
Factories::languageFactory() const
{
    return mLanguageFactory;
}

/*!
 * \return Factory for procedures
 */
DbObjects::Common::ProcedureFactory*
Factories::procedureFactory() const
{
    return mProcedureFactory;
}

/*!
 * \return Factory for roles
 */
DbObjects::Common::RoleFactory*
Factories::roleFactory() const
{
    return mRoleFactory;
}

/*!
 * \return Factory for schemas
 */
DbObjects::Common::SchemaFactory*
Factories::schemaFactory() const
{
    return mSchemaFactory;
}

/*!
 * \return Factory for tables
 */
DbObjects::Common::TableFactory*
Factories::tableFactory() const
{
    return mTableFactory;
}

/*!
 * \return Factory for triggers
 */
DbObjects::Common::TriggerFactory*
Factories::triggerFactory() const
{
    return mTriggerFactory;
}

/*!
 * \return Factory for views
 */
DbObjects::Common::ViewFactory*
Factories::viewFactory() const
{
    return mViewFactory;
}

} // namespace Common
} // namespace DbObjects


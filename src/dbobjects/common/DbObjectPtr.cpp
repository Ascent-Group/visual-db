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

#ifndef DBOBJECTS_COMMON_DBOBJECTPTRIMPL_H
#define DBOBJECTS_COMMON_DBOBJECTPTRIMPL_H

#include <common/DbObjectPtr.h>

#include <common/DbIndex.h>
#include <common/DbLanguage.h>
#include <common/DbProcedure.h>
#include <common/DbRole.h>
#include <common/DbSchema.h>
#include <common/DbTable.h>
#include <common/DbTrigger.h>
#include <common/DbView.h>

#include <factory/Index.h>
#include <factory/Language.h>
#include <factory/Procedure.h>
#include <factory/Role.h>
#include <factory/Schema.h>
#include <factory/Table.h>
#include <factory/Trigger.h>
#include <factory/View.h>

#include <QtDebug>

namespace DbObjects
{

namespace Common
{

// initialization of map
// \todo
template<typename T>
/*typename*/ QMap<T*, quint32> DbObjectPtr<T>::mRefs;


//template<typename T> class DbObjectPtr;
class DbIndex;
class DbLanguage;
class DbProcedure;
class DbRole;
class DbSchema;
class DbTable;
class DbTrigger;
class DbView;

/*!
 * Specialization for index
 */
template<> inline
DbIndex* DbObjectPtr<DbIndex>::initialize()
{
    mPointee = Factory::Index::createIndex(mName);
    return mPointee;
}

/*!
 * Specialization for language
 */
template<> inline
DbLanguage* DbObjectPtr<DbLanguage>::initialize()
{
    mPointee = Factory::Language::createLanguage(mName);
    return mPointee;
}

/*!
 * Specialization for procedure
 */
template<> inline
DbProcedure* DbObjectPtr<DbProcedure>::initialize()
{
    mPointee = Factory::Procedure::createProcedure(mName, mSchemaName);
    return mPointee;
}

/*!
 * Specialization for role
 */
template<> inline
DbRole* DbObjectPtr<DbRole>::initialize()
{
    mPointee = Factory::Role::createRole(mName);
    return mPointee;
}

/*!
 * Specialization for schema
 */
template<> inline
DbSchema* DbObjectPtr<DbSchema>::initialize()
{
//    qDebug () << mName;
    mPointee = Factory::Schema::createSchema(mName);
    return mPointee;
}

/*!
 * Specialization for table
 */
template<> inline
DbTable* DbObjectPtr<DbTable>::initialize()
{
    mPointee = Factory::Table::createTable(mName, mSchemaName);
    return mPointee;
}

/*!
 * Specialization for trigger
 */
template<> inline
DbTrigger* DbObjectPtr<DbTrigger>::initialize()
{
    mPointee = Factory::Trigger::createTrigger(mName, mSchemaName);
//    qDebug() << "DbObjectPtr<DbTrigger>::initialize> " << mPointee << " " << mName;
    return mPointee;
}

/*!
 * Specialization for view
 */
template<> inline
DbView* DbObjectPtr<DbView>::initialize()
{
    mPointee = Factory::View::createView(mName, mSchemaName);
    return mPointee;
}

// explicit instantiation
template class DbObjectPtr<DbIndex>;
template class DbObjectPtr<DbLanguage>;
template class DbObjectPtr<DbProcedure>;
template class DbObjectPtr<DbRole>;
template class DbObjectPtr<DbSchema>;
template class DbObjectPtr<DbTable>;
template class DbObjectPtr<DbTrigger>;
template class DbObjectPtr<DbView>;

} // namespace Common
} // namespace DbObjects

//typedef DbObjects::Common::DbObjectPtr<DbObjects::Common::DbIndex> DbIndexPtr;
//typedef DbObjects::Common::DbObjectPtr<DbObjects::Common::DbLanguage> DbLanguagePtr;
//typedef DbObjects::Common::DbObjectPtr<DbObjects::Common::DbProcedure> DbProcedurePtr;
//typedef DbObjects::Common::DbObjectPtr<DbObjects::Common::DbRole> DbRolePtr;
//typedef DbObjects::Common::DbObjectPtr<DbObjects::Common::DbSchema> DbSchemaPtr;
//typedef DbObjects::Common::DbObjectPtr<DbObjects::Common::DbTable> DbTablePtr;
//typedef DbObjects::Common::DbObjectPtr<DbObjects::Common::DbTrigger> DbTriggerPtr;
//typedef DbObjects::Common::DbObjectPtr<DbObjects::Common::DbView> DbViewPtr;

#endif // DBOBJECTS_COMMON_DBOBJECTPTRIMPL_H


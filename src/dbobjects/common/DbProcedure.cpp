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

#include <common/DbProcedure.h>
#include <common/DbSchema.h>

#include <QtDebug>

namespace DbObjects
{

namespace Common
{

/*!
 * Ctor
 *
 * On creation finds the parent schema by its name ipSchemaName and sets it as
 * parent schema object with setSchema() function.
 */
DbProcedure::DbProcedure(QString ipName, DbSchema *ipSchema)
    : DbObject(ipName),
      mSchema(ipSchema),
      mOwner(0),
      mLanguage(0),
      mSourceCode("")
{
    if (!mSchema) qDebug() << "DbProcedure::DbProcedure> mSchema is NULL!";
}

/*!
 * Dtor
 */
DbProcedure::~DbProcedure()
{

}

/*!
 * \return Database object type
 */
DbObject::Type
DbProcedure::type() const
{
    return DbObject::ProcedureObject;
}

/*!
 * \return Procedure's full name that has a look of
 *         "schemaName.procedureName"
 */
QString
DbProcedure::fullName() const
{
    return QString("%1.%2").arg(mSchema->name()).arg(mName);
}

/*!
 * \return Parent schema object
 */
DbSchema*
DbProcedure::schema() const
{
    return mSchema;
}

/*!
 * \brief Sets parent schema
 *
 * \param[in] ipSchema - Parent schema object
 */
void
DbProcedure::setSchema(DbSchema *ipSchema)
{
    mSchema = ipSchema;
}

/*!
 * \return The owner of the given procedure
 */
DbRole*
DbProcedure::owner() const
{
    return mOwner;
}

/*!
 * \brief Sets the owner
 *
 * \param[in] ipRole - Owner of the procedure
 */
void
DbProcedure::setOwner(DbRole *ipRole)
{
    mOwner = ipRole;
}

/*!
 * \return The language
 */
DbLanguage*
DbProcedure::language() const
{
    return mLanguage;
}

/*!
 * \brief Sets the language
 *
 * \param[in] ipLang
 */
void
DbProcedure::setLanguage(DbLanguage *ipLang)
{
    mLanguage = ipLang;
}

/*!
 * \return The proc's source code text
 */
QString
DbProcedure::sourceCode() const
{
    return mSourceCode;
}

/*!
 * \brief Sets the proc's source code
 *
 * \param[in] ipText - Source code text
 */
void
DbProcedure::setSourceCode(const QString &ipText)
{
    mSourceCode = ipText;
}

/*!
 * \brief Resets procedure description read from database
 */
void
DbProcedure::resetData()
{
    /*! \todo Implement */
    mSchema = 0;
    mOwner = 0;
    mLanguage = 0;
    mSourceCode = "";

    DbObject::resetData();
}

} // namespace Common

} // namespace DbObjects


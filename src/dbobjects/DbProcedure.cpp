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

#include <DbProcedure.h>

/*
 * Ctor
 */
DbProcedure::DbProcedure(QString ipSchemaName, QString ipName)
    : DbObject(ipName), mSchemaName(ipSchemaName)
{
    setSchema(Database::instance()->findSchema(mSchemaName));
}

/*
 * Dtor
 */
DbProcedure::~DbProcedure()
{

}

/*
 * Returns the id of database object
 */
int DbProcedure::objectId()
{
    return Database::ProcedureObject;
}

/*
 * Returns full name
 */
QString
DbProcedure::fullName() const
{
    return QString("%1.%2").arg(mSchemaName).arg(mName);
}

/*
 * Returns schema
 */
DbSchema*
DbProcedure::schema() const
{
    return mSchema;
}

/*
 * Sets schema
 */
void
DbProcedure::setSchema(DbSchema *ipSchema)
{
    mSchema = ipSchema;
}

/*
 * Returns the owner
 */
DbRole*
DbProcedure::owner() const
{
    return mOwner;
}

/*
 * Sets the owner
 */
void
DbProcedure::setOwner(DbRole *ipRole)
{
    mOwner = ipRole;
}

/*
 * Returns the language
 */
DbLanguage*
DbProcedure::language() const
{
    return mLanguage;
}

/*
 * Sets the language
 */
void
DbProcedure::setLanguage(DbLanguage *ipLang)
{
    mLanguage = ipLang;
}

/*
 * Returns the proc's source code
 */
QString
DbProcedure::sourceCode() const
{
    return mSourceCode;
}

/*
 * Sets the proc's source code
 */
void
DbProcedure::setSourceCode(const QString &ipText)
{
    mSourceCode = ipText;
}

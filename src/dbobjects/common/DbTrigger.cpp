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

#include <common/DbSchema.h>
#include <common/DbTrigger.h>

/*
 * Ctor
 */
DbTrigger::DbTrigger(QString ipSchemaName, QString ipName)
    : DbObject(ipName), mSchemaName(ipSchemaName)
{
    setSchema(Database::instance()->findSchema(mSchemaName));
}

/*
 * Dtor
 */
DbTrigger::~DbTrigger()
{

}

/*
 *
 */
DbSchema*
DbTrigger::schema() const
{
    return mSchema;
}

/*
 *
 */
void
DbTrigger::setSchema(DbSchema *ipSchema)
{
    mSchema = ipSchema;
}

/*
 * Returns the table this trigger is on
 */
DbTable*
DbTrigger::table() const
{
    return mTable;
}

/*
 * Sets the table
 */
void
DbTrigger::setTable(DbTable *ipTable)
{
    mTable = ipTable;
}

/*
 * Returns the procedure that runs when trigger fires
 */
DbProcedure*
DbTrigger::procedure() const
{
    return mProcedure;
}

/*
 * Sets the procedure
 */
void
DbTrigger::setProcedure(DbProcedure *ipProcedure)
{
    mProcedure = ipProcedure;
}

/*
 * Returns enabled flag
 */
QChar
DbTrigger::enabled() const
{
    return mEnabled;
}

/*
 *
 */
QString
DbTrigger::schemaName() const
{
    if (mSchema) {
    return mSchema->name();
    }

    return mSchemaName;
}

/*
 *
 */
QString
DbTrigger::fullName() const
{
    return QString("%1.%2").arg(mSchemaName).arg(mName);
}

/*
 * Sets the enabled flag
 */
void
DbTrigger::setEnabled(const QChar ipFlag)
{
    mEnabled = ipFlag;
}

/*
 * Returns true is trigger is a constraint
 */
bool
DbTrigger::isConstraint() const
{
    return mIsConstraint;
}

/*
 * Sets the constraint flag
 */
void
DbTrigger::setConstraint(const bool ipFlag)
{
    mIsConstraint = ipFlag;
}

/*
 * Returns the constraint name if the trigger is a constraint
 */
QString
DbTrigger::constraintName() const
{
    if (!mIsConstraint) {
        return QString("");
    }

    return mConstraintName;
}

/*
 * Sets the constraint name for a constraint trigger
 */
void
DbTrigger::setConstraintName(const QString &ipName)
{
    mConstraintName = ipName;
}

/*
 * Returns the referenced table
 */
DbTable*
DbTrigger::referencedTable() const
{
    return mReferencedTable;
}

/*
 * Sets the table referenced by this trigger
 */
void
DbTrigger::setReferencedTable(DbTable *ipTable)
{
    mReferencedTable = ipTable;
}

/*
 * Returns true if the trigger if deferrable
 */
bool
DbTrigger::isDeferrable() const
{
    return mIsDeferrable;
}

/*
 * Sets the deferrrable flag
 */
void
DbTrigger::setDeferrable(const bool ipFlag)
{
    mIsDeferrable = ipFlag;
}

/*
 * Returns true if the trigger is initially deferred
 */
bool
DbTrigger::isInitiallyDeferred() const
{
    return mIsInitiallyDeferred;
}

/*
 * Sets the initially deferred flag
 */
void
DbTrigger::setInitiallyDeferred(const bool ipFlag)
{
    mIsInitiallyDeferred = ipFlag;
}

/*
 * Returns the number of args passed to the trigger
 */
quint16
DbTrigger::numArgs() const
{
    return mNumArgs;
}

/*
 * Sets the number of arguments passed to the trigger
 */
void
DbTrigger::setNumArgs(const quint16 &ipNum)
{
    mNumArgs = ipNum;
}

/*
 * Returns the database object type
 */
DbObject::Type
DbTrigger::type() const
{
    return DbObject::TriggerObject;
}


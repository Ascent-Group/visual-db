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

#include <QtDebug>

namespace DbObjects
{

namespace Common
{

/*!
 * Constructor
 *
 * \param[in] ipName - Name of a trigger
 * \param[in] ipSchema - Handle to schema that contains the given trigger
 */
DbTrigger::DbTrigger(QString ipName, const DbSchemaPtr &ipSchema)
    : DbObject(ipName),
      mTable(),
      mSchema(ipSchema),
      mProcedure(),
      mEnabled(),
      mIsConstraint(false),
      mConstraintName(""),
      mReferencedTable(),
      mIsDeferrable(false),
      mIsInitiallyDeferred(false),
      mNumArgs(0)
{
    if (!mSchema.get()) qDebug() << "DbTrigger::DbTrigger> mSchema is NULL!";
}

/*!
 * Destructor
 */
DbTrigger::~DbTrigger()
{

}

/*!
 * \return Handle to parent schema
 */
DbSchemaPtr
DbTrigger::schema() const
{
    return mSchema;
}

/*!
 * \param[in] ipSchema - Parent schema handle
 */
void
DbTrigger::setSchema(const DbSchemaPtr &ipSchema)
{
    mSchema = ipSchema;
}

/*!
 * \return Handle for the table this trigger is on
 */
DbTablePtr
DbTrigger::table() const
{
    return mTable;
}

/*!
 * Sets the table
 * \param[in] ipTable - Table handle
 */
void
DbTrigger::setTable(const DbTablePtr &ipTable)
{
    mTable = ipTable;
}

/*!
 * \return The procedure that runs when trigger fires
 */
DbProcedurePtr
DbTrigger::procedure() const
{
    return mProcedure;
}

/*!
 * Sets the procedure
 *
 * \param[in] ipProcedure - Procedure handle
 */
void
DbTrigger::setProcedure(const DbProcedurePtr &ipProcedure)
{
    mProcedure = ipProcedure;
}

/*!
 * \return 'Enabled' flag
 */
QChar
DbTrigger::enabled() const
{
    return mEnabled;
}

/*!
 * \return Trigger's fullname in "Schema.Trigger" format
 */
QString
DbTrigger::fullName() const
{
    return QString("%1.%2").arg(mSchema->name()).arg(mName);
}

/*!
 * Sets the enabled flag
 * \param[in] ipFlag - Indicates whether the trigger is enabled
 */
void
DbTrigger::setEnabled(const QChar &ipFlag)
{
    mEnabled = ipFlag;
}

/*!
 * \return true - If trigger is a constraint
 * \return false - Otherwise
 */
bool
DbTrigger::isConstraint() const
{
    return mIsConstraint;
}

/*!
 * Sets the constraint flag
 *
 */
void
DbTrigger::setConstraint(bool ipFlag)
{
    mIsConstraint = ipFlag;
}

/*!
 * \return The constraint name if the trigger is a constraint
 */
QString
DbTrigger::constraintName() const
{
    if (!mIsConstraint) {
        return QString("");
    }

    return mConstraintName;
}

/*!
 * Sets the constraint name for a constraint trigger
 *
 * \param[in] ipName - Constraint name
 */
void
DbTrigger::setConstraintName(const QString &ipName)
{
    mConstraintName = ipName;
}

/*!
 * \return The referenced table handle
 */
DbTablePtr
DbTrigger::referencedTable() const
{
    return mReferencedTable;
}

/*!
 * Sets the table referenced by this trigger
 *
 * \param[in] ipTable
 */
void
DbTrigger::setReferencedTable(const DbTablePtr &ipTable)
{
    mReferencedTable = ipTable;
}

/*!
 * \return true - If the trigger if deferrable
 * \return false - Otherwise
 */
bool
DbTrigger::isDeferrable() const
{
    return mIsDeferrable;
}

/*!
 * Sets the deferrrable flag
 *
 * \param[in] ipFlag - Inidicates whether the trigger is deferrable
 */
void
DbTrigger::setDeferrable(bool ipFlag)
{
    mIsDeferrable = ipFlag;
}

/*!
 * \return true - If the trigger is initially deferred
 * \return false - Otherwise
 */
bool
DbTrigger::isInitiallyDeferred() const
{
    return mIsInitiallyDeferred;
}

/*!
 * Sets the initially deferred flag
 *
 * \param[in] ipFlag - Indicates, whether the trigger is initially deferred
 */
void
DbTrigger::setInitiallyDeferred(bool ipFlag)
{
    mIsInitiallyDeferred = ipFlag;
}

/*!
 * \return The number of args passed to the trigger
 */
quint16
DbTrigger::numArgs() const
{
    return mNumArgs;
}

/*!
 * Sets the number of arguments passed to the trigger
 *
 * \param[in] ipNum - Number of args
 */
void
DbTrigger::setNumArgs(const quint16 &ipNum)
{
    mNumArgs = ipNum;
}

/*!
 * \return The database object type identifier
 */
DbObject::Type
DbTrigger::type() const
{
    return DbObject::TriggerObject;
}

/*!
 * \brief Resets data read from database
 */
void
DbTrigger::resetData()
{
    mTable = DbTablePtr();
    mSchema = DbSchemaPtr();
    mProcedure = DbProcedurePtr();
    mEnabled = 0;
    mIsConstraint = false;
    mConstraintName = "";
    mReferencedTable = DbTablePtr();
    mIsDeferrable = false;
    mIsInitiallyDeferred = false;
    mNumArgs = 0;

    DbObject::resetData();
}

} // namespace Common

} // namespace DbObjects


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

#include <control/Context.h>

namespace Control
{

/*!
 * Constructor
 */
Context::Context()
    : mConnectionInfo(),
      mDbHandle()
{

}

/*!
 * Constructor
 *
 * \param[in] iConnectionInfo - Connection info.
 * \param[in] iDbHandle - database connection handle.
 */
Context::Context(const Connect::ConnectionInfo &iConnectionInfo, const QSqlDatabase &iDbHandle)
    : mConnectionInfo(iConnectionInfo),
      mDbHandle(iDbHandle)
{
}

/*!
 *
 */
Context::~Context()
{
}

/*!
 * Copy constructor
 * \note FORBIDDEN
 */
Context::Context(const Context &iContext)
{
    Q_UNUSED(iContext);
}

/*!
 * Assignment operator
 * \note FORBIDDEN
 */
const Context&
Context::operator=(const Context &iRhs)
{
    Q_UNUSED(iRhs);
    return *this;
}

/*!
 * \return Connection info
 */
Connect::ConnectionInfo
Context::connectionInfo() const
{
    return mConnectionInfo;
}

/*!
 * Set the connection info for the given context.
 *
 * \param[in] iInfo - Connection info
 */
void
Context::setConnectionInfo(const Connect::ConnectionInfo &iInfo)
{
    mConnectionInfo = iInfo;
}

/*!
 * \return Database connection handle
 */
QSqlDatabase
Context::dbHandle() const
{
    return mDbHandle;
}

/*!
 * \param[in] iDbHandle - Database connection handle
 */
void
Context::setDbHandle(const QSqlDatabase &iDbHandle)
{
    mDbHandle = iDbHandle;
}

/*!
 * \param[in] iInst - Another instance of context that we are comparing our context to
 *
 * \return true - If contexts are different
 * \return false - Otherwise
 */
bool
Context::operator!=(const Context &iInst)
{
    return !(*this == iInst);
}

/*!
 * \param[in] iInst - Another instance of context that we are comparing our context to
 * \return true - If contexts are equal
 * \return false - Otherwise
 */
bool
Context::operator==(const Context &iInst)
{
    // \note QSqlDatabase has no public == operator defined
    return (mConnectionInfo == iInst.mConnectionInfo
            && mDbHandle.databaseName() == iInst.mDbHandle.databaseName()
            && mDbHandle.driverName() == iInst.mDbHandle.driverName()
            && mDbHandle.hostName() == iInst.mDbHandle.hostName()
            && mDbHandle.port() == iInst.mDbHandle.port()
            && mDbHandle.userName() == iInst.mDbHandle.userName());
}

} // namespace Control


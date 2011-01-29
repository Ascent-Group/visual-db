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

#ifndef CONTROL_CONTEXT_H
#define CONTROL_CONTEXT_H

#include <connect/ConnectionInfo.h>

#include <QSqlDatabase>


namespace Control
{

/*!
 * \class Context
 * \headerfile control/Context.h
 * \brief Class that represents the environment of current execution.
 */
class Context
{
    public:
        Context();
        Context(const Connect::ConnectionInfo &, const QSqlDatabase &);
        ~Context();

        Connect::ConnectionInfo connectionInfo() const;
        void setConnectionInfo(const Connect::ConnectionInfo &);

        QSqlDatabase dbHandle() const;
        void setDbHandle(const QSqlDatabase &);

        bool operator!=(const Context &);
        bool operator==(const Context &);

    private:
        /*! Connection parameters */
        Connect::ConnectionInfo mConnectionInfo;
        /*! Db connection descriptor */
        QSqlDatabase mDbHandle;

        /*!
         * \note Having 2 identical contexts violates the idea of having unique execution
         * environments
         */
        Context(const Context &);
        const Context& operator=(const Context &);
};

} // namespace Control

#endif // CONTROL_CONTEXT_H


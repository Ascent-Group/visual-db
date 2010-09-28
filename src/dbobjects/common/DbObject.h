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

#ifndef DBOBJECTS_COMMON_DBOBJECT_H
#define DBOBJECTS_COMMON_DBOBJECT_H

#include <QString>

namespace DbObjects
{

namespace Common
{

/*!
 * \class DbObject
 * \headerfile DbObject.h <common/DbObject.h>
 * \brief Defines a base class for all database objects
 */
class DbObject
{
    public:

        /*!
         * \enum Type
         * Database object indentifiers
         */
        enum Type {
            UnkObject = 0,      /*!< Unknown object */
            SchemaObject,       /*!< Schema */
            TableObject,        /*!< Table */
            ViewObject,         /*!< View */
            RoleObject,         /*!< Role */
            TriggerObject,      /*!< Trigger */
            LanguageObject,     /*!< Language */
            IndexObject,        /*!< Index */
            ProcedureObject,    /*!< Procedure */
        };

        QString name() const;
        void setName(const QString &ipName);

        /*! \see Descendants' implementation */
        virtual DbObject::Type type() const = 0;
        virtual bool loadData() = 0;
        virtual void resetData() = 0;

    protected:
        /*! Name of db object */
        QString mName;

    protected:
        DbObject(QString ipName = 0);
        virtual ~DbObject();
};

} // namespace Common

} // namespace DbObjects

#endif // DBOBJECTS_COMMON_DBOBJECT_H


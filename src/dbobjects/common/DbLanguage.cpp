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

#include <common/Database.h>
#include <common/DbLanguage.h>

namespace DbObjects
{

namespace Common
{

/*!
 * Ctor
 *
 * \param[in] iName - Name of the language
 */
DbLanguage::DbLanguage(QString iName)
    : DbObject(iName),
      mIsTrusted(false)
{

}

/*!
 * Dtor
 */
DbLanguage::~DbLanguage()
{

}

/*!
 * \return true - If the given language is trusted
 * \return false - Otherwise
 */
bool
DbLanguage::isTrusted() const
{
    return mIsTrusted;
}

/*!
 * \brief Sets the trusted flag
 *
 * \param[in] iFlag
 */
void
DbLanguage::setTrusted(bool iFlag)
{
    mIsTrusted = iFlag;
}

/*!
 * \brief Resets language data read from database
 */
void
DbLanguage::resetData()
{
    mIsTrusted = false;

    DbObject::resetData();
}

/*!
 * \return Database object type
 */
ObjectType
DbLanguage::type() const
{
    return LanguageObject;
}

} // namespace Common

} // namespace DbObjects


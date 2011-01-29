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

#ifndef NAMESPACES_H
#define NAMESPACES_H

/*!
 * \namespace Consts
 * \brief Holds various constants
 */
namespace Consts { }

/*!
 * \namespace Gui
 * \brief Holds visual elements and entities that control their displaying.
 */
namespace Gui
{

    /*!
     * \namespace Behaviour
     * \brief Holds all commands of the undo stack
     */
    namespace Behaviour { }

    /*!
     * \namespace GraphicsItems
     * \brief Holds items that are displayed by the scene
     */
    namespace GraphicsItems { }

    /*!
     * \namespace Strategy
     * \brief Holds strategies that implement pretty items arrangement on the scene.
     */
    namespace Startegy { }
}

/*!
 * \namespace Connect
 * \brief Holds entities responsible for connection
 */
namespace Connect { }

/*!
 * \namespace Control
 * \brief Holds entities that coordinate the cooperation of modules
 */
namespace Control { }

/*!
 * \namespace DbObjects
 * \brief Top level namespace for all database objects
 */
namespace DbObjects
{
    /*!
     * \namespace Common
     * \brief Holds abstract database objects
     */
    namespace Common { }

    /*!
     * \namespace Factory
     * \brief Holds factories of database objects
     */
    namespace Factory { }

    /*!
     * \namespace Psql
     * \brief Holds PostgreSQL specific implementations
     */
    namespace Psql { }

    /*!
     * \namespace Mysql
     * \brief Holds MySQL specific implementations
     */
    namespace Mysql { }

} // namespace DbObjects

#endif // NAMESPACES_H


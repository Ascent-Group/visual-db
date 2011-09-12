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

#include <gui/ContextMenuHolder.h>
#include <QMenu>

namespace Gui {

//const QMenu* ContextMenuHolder::mMenu = 0;

/*!
 * \param[in] iObject - Pointer to the object who implents this interface
 *
 * \note Maybe iObject parameter will be used later.
 */
ContextMenuHolder::ContextMenuHolder(QObject *iObject)
    : mMenu(0)
{

}

/*!
 * Destructor
 *
 * Context menu holder doesn't own the context menu but the context menu manager does and
 * context menu manager will handle its destruction.
 */
ContextMenuHolder::~ContextMenuHolder()
{

}

/*!
 * Set the context menu for the object.
 *
 * \param[in] iMenu - Context menu instance.
 *
 */
void
ContextMenuHolder::setContextMenu(const QMenu *iMenu)
{
    mMenu = iMenu;
}

/*!
 *
 * \return Context menu associated with the object.
 */
const QMenu*
ContextMenuHolder::contextMenu() const
{
    return mMenu;
}

} // namespace Gui

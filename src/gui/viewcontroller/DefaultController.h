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

#ifndef DEFAULTCONTROLLER_H
#define DEFAULTCONTROLLER_H

#include <gui/viewcontroller/TreeViewController.h>

namespace Gui
{
namespace ViewController
{

/*!
 * \class DefaultController
 * \headerfile gui/viewcontroller/DefaultController.h
 * \brief Default tree view controller
 *
 * Responsible for building a tree of the following type:
 *
 * <pre>
 * +
 * |_Roles
 * |_Languages
 * |_Schemas
 *   |_&lt;schema 1&gt;
 *     |_Tables
 *       |_&lt;table 1&gt;
 *       |_...
 *       |_&lt;table N&gt;
 *     |_Triggers
 *     |_Procedures
 *     |_Views
 *   |_&lt;schema 2&gt;
 *   |_...
 *   |_&lt;schema N&gt;
 * |_Indices
 * </pre>
 *
 * \todo comment
 */
class DefaultController : public TreeViewController
{
    public:
        DefaultController();
        virtual ~DefaultController();

        virtual void buildTree(Gui::TreeWidget *iTree, const QList<Gui::TreeWidgetItem*> &iItems);
};

} // namespace ViewController
} // namespace Gui

#endif // DEFAULTCONTROLLER_H


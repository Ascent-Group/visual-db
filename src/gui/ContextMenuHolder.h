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

#ifndef GUI_CONTEXTMENUHOLDER_H
#define GUI_CONTEXTMENUHOLDER_H

//#include <QObject>

class QContextMenuEvent;
class QMenu;
class QObject;

namespace Gui {

/*!
 * \class ContextMenuHolder
 * \brief Defines base class for entities that are supposed to have context menu.
 * \headerfile gui/ContextMenuHolder.h
 *
 * This class should be used as following, due to limitation of moc system in qt, which
 * prohibits multiple inheritance from QObject
 *
 * <code>
 * class MyClass : public QFirstSuperClass, public Gui::ContextMenuHolder
 * {
 *     ...
 *
 *     signals:
 *         void contextMenuRequest(QContextMenuEvent *);
 * };
 * </code>
 */
class ContextMenuHolder/* : public QObject*/
{
//    Q_OBJECT

    public:
        ContextMenuHolder(QObject *iObject);
        virtual ~ContextMenuHolder();

        void setContextMenu(const QMenu *iMenu);
        const QMenu* contextMenu() const;

        /*! Subclass should declare this signal in their own header */
        virtual void contextMenuRequest(QContextMenuEvent *) = 0;

    private:
        /*static */const QMenu *mMenu;

};

} // namespace Gui

#endif // GUI_CONTEXTMENUHOLDER_H


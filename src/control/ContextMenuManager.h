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

#ifndef CONTROL_CONTEXT_MENU_MANAGER_H
#define CONTROL_CONTEXT_MENU_MANAGER_H

#include <QMap>
#include <QObject>

class QContextMenuEvent;
class QMenu;

namespace Control
{

/*!
 * \class ContextMenuManager
 * \headerfile control/ContextMenuManager.h
 * \brief Class that represents the entity that manages context menus and their actions.
 */
class ContextMenuManager : public QObject
{
    Q_OBJECT

    public:
        enum MenuType
        {
            MENU_UNKNOWN = 0,
            MENU_TREE_TABLE_ITEM
        };

        ContextMenuManager();
        ~ContextMenuManager();

    public slots:
        void contextMenuRequested(QContextMenuEvent *);

    private:
        QMap<MenuType, QMenu *> mMenus;

    private:
        void createMenus();
        ContextMenuManager::MenuType treeItemType2MenuType(quint32 iType) const;
};

} // namespace Control

#endif // CONTROL_CONTEXT_MENU_MANAGER_H


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

#ifndef COMMON_H
#define COMMON_H

#include <QMultiHash>


/*!
 * \enum DbObjectType
 *
 * Database object indentifiers.
 * \fixme Come up with proper names for enum and its elements.
 */
enum ObjectType {
    UnkObject = 0,      /*!< Unknown object */
    SchemaObject,       /*!< Schema */
    TableObject,        /*!< Table */
    ViewObject,         /*!< View */
    RoleObject,         /*!< Role */
    TriggerObject,      /*!< Trigger */
    LanguageObject,     /*!< Language */
    IndexObject,        /*!< Index */
    ProcedureObject     /*!< Procedure */
};

/*!
 * \enum WidgetModifier
 *
 * Modifier based on a widget that displays db objects describing elements.
 * \fixme Come up with proper names for enum and its elements.
 */
enum WidgetModifier {
    Tree  = 0x010000,
    Scene = 0x020000
};

/*!
 * \enum ElementModifier
 *
 * Modifier based on a type of element. For example, TreeWidgetItem describing a table
 * object will have a type of table object OR'd with Item modifier.
 *
 * \note This enum may be extended to contain modifiers for graphics items and/or groups
 * of them, in case scene becomes capable of displaying different types of elements.
 * \fixme Come up with proper names for enum and its elements.
 */
enum ElementModifier {
    Item = 0x0100,
    Node = 0x0200
};

/*!
 * \enum ObjectTypeModifier
 *
 * Defined for convenience.
 * \fixme Come up with proper names for enum and its elements.
 */
enum ObjectTypeModifier {
    TreeItem = Tree | Item,
    TreeNode = Tree | Node,
    SceneItem = Scene | Item
};


// < ObjName, <SchemaName, ObjType> >
typedef QMultiHash<QString, QPair<QString, ObjectType> > Objects;

#endif // COMMON_H


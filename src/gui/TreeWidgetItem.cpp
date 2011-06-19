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

#include <gui/TreeWidget.h>
#include <gui/TreeWidgetItem.h>
#include <QStringList>
#include <QTreeWidget>

namespace Gui
{

TreeWidgetItem::TreeWidgetItem(int iType)
    : QTreeWidgetItem(iType),
      Gui::ContextMenuHolder(dynamic_cast<QObject*>(this))
{
}

TreeWidgetItem::TreeWidgetItem(const QStringList &iStrings, int iType)
    : QTreeWidgetItem(iStrings, iType),
      Gui::ContextMenuHolder(dynamic_cast<QObject*>(this))
{
}


TreeWidgetItem::TreeWidgetItem(QTreeWidget *iParent, int iType)
    : QTreeWidgetItem(iParent, iType),
      Gui::ContextMenuHolder(dynamic_cast<QObject*>(this))
{
}


TreeWidgetItem::TreeWidgetItem(QTreeWidget *iParent, const QStringList &iStrings, int iType)
    : QTreeWidgetItem(iParent, iStrings, iType),
      Gui::ContextMenuHolder(dynamic_cast<QObject*>(this))
{
}


TreeWidgetItem::TreeWidgetItem(QTreeWidget *iParent, QTreeWidgetItem *iPreceding, int iType)
    : QTreeWidgetItem(iParent, iPreceding, iType),
      Gui::ContextMenuHolder(dynamic_cast<QObject*>(this))
{
}


TreeWidgetItem::TreeWidgetItem(QTreeWidgetItem *iParent, int iType)
    : QTreeWidgetItem(iParent, iType),
      Gui::ContextMenuHolder(dynamic_cast<QObject*>(this))
{
}


TreeWidgetItem::TreeWidgetItem(QTreeWidgetItem *iParent, const QStringList &iStrings, int iType)
    : QTreeWidgetItem(iParent, iStrings, iType),
      Gui::ContextMenuHolder(dynamic_cast<QObject*>(this))
{
}


TreeWidgetItem::TreeWidgetItem(QTreeWidgetItem *iParent, QTreeWidgetItem *iPreceding, int iType)
    : QTreeWidgetItem(iParent, iPreceding, iType),
      Gui::ContextMenuHolder(dynamic_cast<QObject*>(this))
{
}


TreeWidgetItem::TreeWidgetItem(const QTreeWidgetItem &iOther)
    : QTreeWidgetItem(iOther),
      Gui::ContextMenuHolder(dynamic_cast<QObject*>(this))
{
}

/*!
 * Custom constructor
 */
TreeWidgetItem::TreeWidgetItem(const QString &iName, const QString &iSchemaName, int iType)
    : QTreeWidgetItem(/*iType*/),
      Gui::ContextMenuHolder(dynamic_cast<QObject*>(this))
{
    setText(Gui::TreeWidget::TypeCol, QString::number(iType));
    setText(Gui::TreeWidget::NameCol, iName);
    setText(Gui::TreeWidget::SchemaCol, iSchemaName);
}

TreeWidgetItem::~TreeWidgetItem()
{

}

} // namespace Gui


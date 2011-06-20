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

#include <gui/viewcontroller/TreeViewController.h>

namespace Gui
{
namespace ViewController
{

/*!
 * \brief Set font to bold
 *
 * \param[in] iItem - Tree item we need to change the boldness
 * \param[in] iBold - True for bold font or false for normal one
 */
static void
setBold(Gui::TreeWidgetItem *iItem, bool iBold)
{
    QFont font = iItem->font(TreeWidget::NameCol);
    font.setBold(iBold);
    iItem->setFont(TreeWidget::NameCol, font);
}



/*!
 * Constructor
 */
TreeViewController::TreeViewController()
{
}

/*!
 * Destructor
 */
TreeViewController::~TreeViewController()
{
}

/*!
 * Creates a node for storing objects of specified type.
 *
 * \param[in] iParent - Parent item who will hold this node as a child.
 * \param[in] iName - Name of the node.
 * \param[in] iType - Type of a node that is going to be created.
 *
 * \return Newly created tree widget node if its creation succeeded.
 * \return 0 - Otherwise.
 */
Gui::TreeWidgetItem*
TreeViewController::createNode(Gui::TreeWidgetItem *iParent, const QString &iName, TreeWidget::Node iType)
{
    Gui::TreeWidgetItem *node = new(std::nothrow) Gui::TreeWidgetItem(iParent);

    if (node) {
        node->setText(TreeWidget::NameCol, iName);
        node->setText(TreeWidget::TypeCol, QString::number(iType));
        setBold(node, true);
    }

    return node;
}

/*!
 * \brief Insert a child item for the given parent
 *
 * \param[in] iParentNode - Parent item we will populate.
 * \param[in] iText - Text to display by the new item.
 * \param[in] iType - Item type.
 * \param[in] iDrabEnabled - Indicates whether these items should be draggable or not.
 *
 * \return Tree widget item that was created
 */
Gui::TreeWidgetItem*
TreeViewController::insertItem(Gui::TreeWidgetItem *iParentNode, const QString &iText, TreeWidget::Item iType, bool iDragEnabled)
{
    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    if (iDragEnabled) {
        flags |= Qt::ItemIsDragEnabled;
    }

    Gui::TreeWidgetItem *item = new(std::nothrow) Gui::TreeWidgetItem(iParentNode);

    if (item) {
        item->setFlags(flags);
        item->setText(TreeWidget::NameCol, iText);
        item->setText(TreeWidget::TypeCol, QString::number(iType));
        item->setData(TreeWidget::NameCol, Qt::DisplayRole, iText);
    }

    return item;
}

} // namespace ViewController
} // namespace Gui


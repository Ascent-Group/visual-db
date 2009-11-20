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

#include <AddTableCommand.h>
#include <GraphicsScene.h>
#include <QUndoCommand>
#include <TableItem.h>
#include <TableItemGroup.h>

/*
 * Ctor
 */
AddTableCommand::AddTableCommand(GraphicsScene *ipScene, TableItem *ipTable, QUndoCommand *ipParent)
    : QUndoCommand(ipParent)
{
    mScene = ipScene;
    mTable = ipTable;
    mInitialPosition = ipTable->scenePos();
    setText(QObject::tr("Add table"));
}

/*
 * Dtor
 */
AddTableCommand::~AddTableCommand()
{
}

/*
 * Undo add node
 */
void 
AddTableCommand::undo()
{
    QList<QGraphicsItem *> list;
    list << mTable;
    mScene->deleteTableItem(list);
    mScene->update();
}

/*
 * Redo add node
 */
void 
AddTableCommand::redo()
{
    mScene->addItem(mTable);
    mTable->setPos(mInitialPosition);

    // draw all relations between new table and already added ones
    foreach (QGraphicsItem *item, mScene->items()) {
	if (qgraphicsitem_cast<TableItem *>(item)) {
	    mScene->createRelations(qgraphicsitem_cast<TableItem *>(item));
	}
    }  
    mScene->clearSelection();
    mScene->update();
}


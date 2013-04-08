/******************************************************************************
** Copyright (C) 2011 Martin Hoppe martin@2x2hoppe.de
**
** This file is part of the QDiagram Toolkit (qdiagramlib)
**
** qdiagramlib is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as
** published by the Free Software Foundation, either version 3 of the
** License, or (at your option) any later version.
**
** qdiagramlib is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Leser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with qdialgramlib.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/
#include "stdafx.h"
#include "qdiagrammoveshapecommand.h"

#include "qdiagramsheet.h"

QDiagramMoveShapeCommand::QDiagramMoveShapeCommand(QAbstractDiagram* diagram, QAbstractDiagramShape* item, const QPointF & oldPos, const QPointF & newPos, QUndoCommand* parent ) :
    QDiagramUndoCommand(diagram, item->uuid(), "default", parent)
{
    cNewPos = newPos;
    cOldPos = oldPos;
    setText(QObject::tr("Move %1 to %2,%3").arg(uuid()).arg(cNewPos.x()).arg(cNewPos.y()));
}

bool QDiagramMoveShapeCommand::mergeWith(const QUndoCommand* command)
{
    const QDiagramMoveShapeCommand* mMoveCommand = static_cast<const QDiagramMoveShapeCommand *>(command);
    if (mMoveCommand->uuid() != uuid()){
        return false;
    }
    QAbstractDiagramGraphicsItem* mItem = diagram()->findItemByUuid(uuid());
    if (mItem == 0){
        return false;
    }
    cNewPos = mItem->pos();
    setText(QObject::tr("Move %1 to %2,%3").arg(shape()).arg(cNewPos.x()).arg(cNewPos.y()));

    return true;
}

void QDiagramMoveShapeCommand::undo()
{
    QAbstractDiagramGraphicsItem* mItem = diagram()->findItemByUuid(uuid());
    if (mItem){
        mItem->setPos(cOldPos);
        diagram()->currentSheet()->update();
    }
}

void QDiagramMoveShapeCommand::redo()
{
    QAbstractDiagramGraphicsItem* mItem = diagram()->findItemByUuid(uuid());
    if (mItem){
        mItem->setPos(cNewPos);
    }
}

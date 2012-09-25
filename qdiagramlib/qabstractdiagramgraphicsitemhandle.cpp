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
#include "qabstractdiagramgraphicsitemhandle.h"

#include "qabstractdiagramscene.h"

QAbstractDiagramGraphicsItemHandle::QAbstractDiagramGraphicsItemHandle(QGraphicsItem *parent) :
    QGraphicsRectItem(parent)
{
}

QPointF QAbstractDiagramGraphicsItemHandle::alignToGrid( const QPointF & pos ) const
{
    QPointF mPos(pos);
    QAbstractDiagramScene* mScene = qobject_cast<QAbstractDiagramScene*>(scene());
    if (mScene && mScene->isSnapToGridEnabled()){
        mPos.setX(mScene->gridSize().width() * (int)(mPos.x() / mScene->gridSize().width()));
        mPos.setY(mScene->gridSize().height() * (int)(mPos.y() / mScene->gridSize().height()));
    }
    return mPos;
}

QPointF QAbstractDiagramGraphicsItemHandle::alignToGrid(qreal x, qreal y) const
{
    return alignToGrid(QPointF(x, y));
}

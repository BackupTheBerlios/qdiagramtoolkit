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
#include "qabstractdiagramscene.h"

#include "qabstractdiagram.h"
#include "qabstractdiagramgraphicsitem.h"

QAbstractDiagramScene::QAbstractDiagramScene(QAbstractDiagram* diagram) :
    QGraphicsScene(diagram)
{
    m_movingItem = 0;
    m_snapToGrid = true;
    m_gridSize = QSize(13, 13);
}

//void QAbstractDiagramScene::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
//{
//    diagram()->contextMenuEventHandler(event);
//}

QAbstractDiagram* QAbstractDiagramScene::diagram() const
{
    return qobject_cast<QAbstractDiagram*>(parent());
}

void QAbstractDiagramScene::dragEnterEvent( QGraphicsSceneDragDropEvent* event )
{
    diagram()->dragEnterEventHandler(event);
}

void QAbstractDiagramScene::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
    diagram()->dragLeaveEventHandler(event);
}

void QAbstractDiagramScene::dragMoveEvent(QGraphicsSceneDragDropEvent* event)
{
    diagram()->dragMoveEventHandler(event);
}

void QAbstractDiagramScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    diagram()->dropEventHandler(event);
}

QSize QAbstractDiagramScene::gridSize() const
{
    return m_gridSize;
}

bool QAbstractDiagramScene::isSnapToGridEnabled() const
{
    return m_snapToGrid;
}

QAbstractDiagramGraphicsItem* QAbstractDiagramScene::item( const QString & uuid ) const
{
    Q_FOREACH(QGraphicsItem* mGraphicsItem, items()){
        QAbstractDiagramGraphicsItem* mDiagramItem = dynamic_cast<QAbstractDiagramGraphicsItem*>(mGraphicsItem);
        if (mDiagramItem && mDiagramItem->uuid() == uuid){
            return mDiagramItem;
        }
    }
    return 0;
}

void QAbstractDiagramScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    m_movingItem = itemAt(event->buttonDownScenePos(Qt::LeftButton));

    if (m_movingItem != 0 && event->button() == Qt::LeftButton) {
        if (m_movingItem->parentItem()){
            m_oldPos = m_movingItem->parentItem()->pos();
            m_movingItem = m_movingItem->parentItem();
        } else {
            m_oldPos = m_movingItem->pos();
        }
    }
    QGraphicsScene::mousePressEvent(event);
}

void QAbstractDiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_movingItem != 0 && event->button() == Qt::LeftButton) {
        if (m_oldPos != m_movingItem->pos()){
            emit itemMoved(m_movingItem, m_oldPos, m_movingItem->pos());
        }
        m_movingItem = 0;
    }
    QGraphicsScene::mouseReleaseEvent(event);
}

QColor QAbstractDiagramScene::selectionColor() const
{
    return m_selectionColor;
}

void QAbstractDiagramScene::setGridSize( const QSize & size )
{
    m_gridSize = size;
}

void QAbstractDiagramScene::setSelectionColor(const QColor & color)
{
    m_selectionColor = color;
}

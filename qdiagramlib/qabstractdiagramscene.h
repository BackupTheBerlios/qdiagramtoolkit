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

#ifndef QABSTRACTDIAGRAMSCENE_H
#define QABSTRACTDIAGRAMSCENE_H

#include "qdiagramlib_global.h"

#include <QGraphicsScene>

class QAbstractDiagram;
class QAbstractDiagramGraphicsItem;
class QAbstractDiagramShape;

class QDIAGRAMLIBSHARED_EXPORT QAbstractDiagramScene : public QGraphicsScene
{
    Q_OBJECT
    Q_PROPERTY(QSize gridSize READ gridSize WRITE setGridSize)
    Q_PROPERTY(QColor selectionColor READ selectionColor WRITE setSelectionColor)
public:
    explicit QAbstractDiagramScene(QAbstractDiagram* diagram = 0);
    /**
      * Returns the diagram associated with the scene.
      */
    QAbstractDiagram* diagram() const;
    /**
      * Returns the diagram's grid size.
      */
    QSize gridSize() const;

    QColor selectionColor() const;
    /**
      * Returns true if snap to grid is enabled. Otherwise false.
      */
    bool isSnapToGridEnabled() const;
    /**
      * Returns the item speicified by the given @p uuid or 0 if @p uuid cannot be retrieved.
      */
    QAbstractDiagramGraphicsItem* item( const QString & uuid ) const;
    /**
      * Sets the grid @p size.
      */
    void setGridSize( const QSize & size );
    /**
      * Sets the selection @p color.
      */
    void setSelectionColor(const QColor & color);
signals:
    void handleConnectItemsEvent(QAbstractDiagramShape* from, QAbstractDiagramShape* to, const QMap<QString,QVariant> & properties);
    void handleDragEnterEvent(QGraphicsSceneDragDropEvent *event);
    void handleDragLeaveEvent(QGraphicsSceneDragDropEvent *event);
    void handleDragMoveEvent(QGraphicsSceneDragDropEvent *event);
    void handleDropEvent(QGraphicsSceneDragDropEvent *event);

    void itemMoved(QGraphicsItem* item, const QPointF & oldPos, const QPointF & newPos);
public slots:

protected:
//    void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);
    void dragEnterEvent(QGraphicsSceneDragDropEvent* event);
    void dragLeaveEvent(QGraphicsSceneDragDropEvent *event);
    void dragMoveEvent(QGraphicsSceneDragDropEvent* event);
    void dropEvent(QGraphicsSceneDragDropEvent* event);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
private:
    QGraphicsItem* m_movingItem;
    QPointF m_oldPos;
    QColor m_selectionColor;
    bool m_snapToGrid;
    QSize m_gridSize;
};

#endif // QABSTRACTDIAGRAMSCENE_H

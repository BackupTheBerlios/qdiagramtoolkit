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
#ifndef QDIAGRAMSHAPESIZEGRIP_H
#define QDIAGRAMSHAPESIZEGRIP_H

#include <QGraphicsRectItem>

#include <qdiagramlib_global.h>

class QDIAGRAMLIBSHARED_EXPORT QDiagramShapeSizeGripHandle : public QGraphicsRectItem
{
public:
    enum HandlePosition {
        Bottom = 0x0001,
        BottomRight = 0x0002,
        BottomLeft = 0x0004,
        Left = 0x0008,
        Right = 0x0010,
        Top = 0x0020,
        TopLeft = 0x0040,
        TopRight = 0x0080
    };
    Q_DECLARE_FLAGS(HandlePositions, HandlePosition)

    QDiagramShapeSizeGripHandle(QDiagramShapeSizeGripHandle::HandlePosition position, QGraphicsItem* parent = 0);
    ~QDiagramShapeSizeGripHandle();

    QDiagramShapeSizeGripHandle::HandlePosition handlePosition() const;

    void updatePosition();
protected:
    qreal alignHorizontal(qreal value);
    qreal alignVertical(qreal value);

    void hoverEnterEvent( QGraphicsSceneHoverEvent* event );
    void hoverLeaveEvent( QGraphicsSceneHoverEvent* event );

    void mouseMoveEvent( QGraphicsSceneMouseEvent * event );
    void mousePressEvent( QGraphicsSceneMouseEvent * event );
    void mouseReleaseEvent( QGraphicsSceneMouseEvent * event );


private:
    QPointF restrictPosition(const QPointF& newPos);
private:
    QPointF m_mousePos;
    bool m_active;
    QDiagramShapeSizeGripHandle::HandlePosition m_posFlag;
    QSizeF m_last;
    QRectF m_rect;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QDiagramShapeSizeGripHandle::HandlePositions)

#endif // QDIAGRAMSHAPESIZEGRIPHANDLE_H

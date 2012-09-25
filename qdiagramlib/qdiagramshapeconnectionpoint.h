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
#ifndef QDIAGRAMSHAPECONNECTIONPOINT_H
#define QDIAGRAMSHAPECONNECTIONPOINT_H
#include "qdiagramlib_global.h"

#include <QList>
#include <QRectF>

#include <QExplicitlySharedDataPointer>

class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;

#include "qabstractdiagramshapeconnectionpoint.h"

class QAbstractDiagramShapeConnector;
class QAbstractDiagramShape;

class sdDiagramGraphicsItemTiePoint;

typedef struct {
    int direction;
    QAbstractDiagramShapeConnector* connector;
} QDiagramGraphicsItemTiePointConnection;


class QDIAGRAMLIBSHARED_EXPORT QDiagramShapeConnectionPoint : public QAbstractDiagramShapeConnectionPoint
{
public:
    enum Position {
        North = 0x0001,
        NorthNorthEast = 0x0002,
        NorthEast = 0x0004,
        EastNorthEast = 0x0008,
        East = 0x0010,
        EastSouthEast = 0x0020,
        SouthEast = 0x0040,
        SouthSouthEast = 0x0080,
        South = 0x0100,
        SouthSouthWest = 0x0200,
        SouthWest = 0x0400,
        WestSouthWest = 0x0800,
        West = 0x1000,
        WestNorthWest = 0x2000,
        NorthWest = 0x4000,
        NorthNorthWest = 0x8000
    };

    QDiagramShapeConnectionPoint(QAbstractDiagramShape* parentShape, QDiagramShapeConnectionPoint::Position position, QAbstractDiagramShapeConnectionPoint::Orientation orientation);
    ~QDiagramShapeConnectionPoint();

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void updatePosition();
private:
    QPointF calcConnectionPointPosition(qreal x, qreal y);
    QString toString(Position pos) const;

    QDiagramShapeConnectionPoint::Position m_pos;
    QSizeF m_size;
};

#endif // QDIAGRAMSHAPECONNECTIONPOINT_H

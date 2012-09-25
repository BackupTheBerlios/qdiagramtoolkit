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
#ifndef QDIAGRAMGRAPHICSITEM_H
#define QDIAGRAMGRAPHICSITEM_H

#include "qdiagramlib_global.h"
#include <qabstractdiagramshape.h>

#include <QUuid>

#include "qdiagramshapeconnectionpoint.h"

class QDiagram;
class QDiagramShapeConnector;

class QDIAGRAMLIBSHARED_EXPORT QDiagramShape : public QAbstractDiagramShape
{
public:
    explicit QDiagramShape(QGraphicsItem* parent = 0);

//    explicit QDiagramShape(const QString & uuid, const QString & diagramShape, QGraphicsItem* parent = 0);

    virtual ~QDiagramShape();

    void initConnectionPoint(QDiagramShapeConnectionPoint::Position position, QAbstractDiagramShapeConnectionPoint::Orientation orientation);

    QRectF boundingRect() const;

    virtual QPainterPath shape() const;
protected:

    void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent* event);

    virtual void paintTiePoints(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void setBoundingRect( qreal x, qreal y, qreal width, qreal height );
    void setBoundingRect( const QRectF & rect );
private:
    QRectF m_boundingRect;
};

#endif // QDIAGRAMGRAPHICSITEM_H

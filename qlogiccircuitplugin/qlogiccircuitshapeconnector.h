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
#ifndef QLOGICCIRCUITSHAPECONNECTOR_H
#define QLOGICCIRCUITSHAPECONNECTOR_H

#include "qlogiccircuitplugin_global.h"

#include <qabstractdiagramshapeconnector.h>

//! The QLogicCircuitShapeConnector class provides a connector to connect QLogicCircuitShapes.
class QLogicCircuitShapeConnector : public QAbstractDiagramShapeConnector
{
public:
	QDIAGRAM_DECLARE_CONNECTOR(QLogicCircuitShapeConnector)

	QLogicCircuitShapeConnector(const QVariantMap & properties);
    /**
      * Constructs a QLogicCircuitShapeConnector with the given @p uuid and @p style.
      */
    //explicit QLogicCircuitShapeConnector(const QString & uuid, const QString & style);
    /**
      * Returns the outer bounds of the connector as a rectangle.
      */
    QRectF boundingRect() const;

    bool canConnect(QAbstractDiagramShapeConnectionPoint* start, QAbstractDiagramShapeConnectionPoint* end) const;

    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);

    void updatePosition();
private:
    QList<QPointF> breakPoints() const;

    void calcBoundingRect();

    QList<QPointF> defaultConnector() const;

    void paintBreakPoints(QPainter * painter, const QList<QPointF> & points);

    QRectF m_boundingRect;
    QList<QPointF> m_breakPoints;
    QLineF m_line;
    QPointF m_tempEndPos;
    QDiagramToolkit::ConnectionPointOrientation m_tempOrientationAtEnd;
    QRectF m_tempToRect;
};

#endif // QLOGICCIRCUITSHAPECONNECTOR_H

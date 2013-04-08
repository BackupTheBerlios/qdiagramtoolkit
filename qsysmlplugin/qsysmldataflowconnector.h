/******************************************************************************
** Copyright (C) 2013 Martin Hoppe martin@2x2hoppe.de
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
#ifndef QSYSMLDATAFLOWCONNECTOR_H
#define QSYSMLDATAFLOWCONNECTOR_H

#include "qsysmlflowconnector.h"

class QSysMLDataFlowConnector : public QSysMLFlowConnector
{
public:
	QDIAGRAM_DECLARE_CONNECTOR(QSysMLDataFlowConnector)

	QSysMLDataFlowConnector(const QVariantMap & properties);
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

	void paintEndOfLine(QPainter* painter, const QPointF & pos, double angle);

    QRectF m_boundingRect;
    QList<QPointF> m_breakPoints;
    QLineF m_line;
    QPointF m_tempEndPos;
    QDiagramToolkit::ConnectionPointOrientation m_tempOrientationAtEnd;
    QRectF m_tempToRect;
};

#endif // QSYSMLDATAFLOWCONNECTOR_H

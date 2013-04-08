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
#include "stdafx.h"
#include "qsysmldataflowconnector.h"
#include "qsysmlplugin.h"

#include <qdiagramstylesheet.h>

#define PI 3.14159265

QSysMLDataFlowConnector::QSysMLDataFlowConnector(const QVariantMap & properties) :
    QSysMLFlowConnector("flow.data", properties)
{
	restoreProperties(properties);
}

QRectF QSysMLDataFlowConnector::boundingRect() const
{
    return m_boundingRect.adjusted(-3, -3, 3, 3);
}

QList<QPointF> QSysMLDataFlowConnector::breakPoints() const
{
    return defaultConnector();
}

void QSysMLDataFlowConnector::calcBoundingRect()
{
    QPainterPath p;
    if (!m_breakPoints.isEmpty()){
        p.moveTo(m_breakPoints.first());
        for (int iPoints = 1; iPoints < m_breakPoints.size(); iPoints++){
            p.lineTo(m_breakPoints.at(iPoints));
        }
    }
    m_boundingRect = p.boundingRect();
}

bool QSysMLDataFlowConnector::canConnect(QAbstractDiagramShapeConnectionPoint* startPoint, QAbstractDiagramShapeConnectionPoint* endPoint) const
{
    if (startPoint == 0 || endPoint == 0){
        return false;
    }
    if (startPoint == endPoint){
        return false;
    }
    //
	if (startPoint->id().left(7) == "dataOut" && endPoint->id().left(6) == "dataIn"){
		return true;
	}
    return false;
}

QList<QPointF> QSysMLDataFlowConnector::defaultConnector() const
{
    QList<QPointF> points;
    QList<QPointF> sections;
    QLineF line(startPos(), endPos());
    QLineF lineEnd;
    QLineF lineStart;

	if (connectionPointAtStart()){
		points << QPointF(startPos().x() + connectionPointAtStart()->rect().width() / 2, startPos().y());
	} else {
		points << startPos();
	}
	if (connectionPointAtEnd()){
	    points << QPointF(endPos().x() - connectionPointAtEnd()->rect().width(), startPos().y());
	} else {
		points << endPos();
	}
	return points;
}

void QSysMLDataFlowConnector::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

	painter->save();
	painter->setPen(diagram()->styleSheet()->lineStyle("DataFlowLineStyle").pen());
    drawPolyline(painter, m_breakPoints);

	QLineF l(startPos(), endPos());
	paintEndOfLine(painter, endPos(), l.angle() * 2 * PI / 360);

    if (isSelected()){
        paintBreakPoints(painter, m_breakPoints);
    }
	painter->restore();

	if (isSelected()){
		paintFocus(painter, option, widget);
    }

}

void QSysMLDataFlowConnector::paintBreakPoints(QPainter * painter, const QList<QPointF> & points)
{
    if (points.size() < 3){
        return;
    }
    painter->save();
    painter->setBrush(QBrush(Qt::red));
    for (int iPoints = 1; iPoints < points.size() - 1; iPoints++){
        painter->drawRect(points.at(iPoints).x() - 2, points.at(iPoints).y() - 2, 4, 4);
    }
    painter->restore();
}


void QSysMLDataFlowConnector::paintEndOfLine(QPainter* painter, const QPointF & pos, double angle)
{
    double arrowSize = 50;
    QPointF arrowP1 = pos - QPointF(sin(angle + PI / 3) * arrowSize,
                                           cos(angle + PI / 3) * arrowSize);
    QPointF arrowP2 = pos - QPointF(sin(angle + PI - PI / 3) * arrowSize,
                                           cos(angle + PI - PI / 3) * arrowSize);

    QPolygonF arrowHead;
	arrowHead.clear();
	arrowHead << pos << arrowP1 << arrowP2;
	painter->drawLine(pos, arrowP1);
	painter->drawLine(pos, arrowP2);
}

void QSysMLDataFlowConnector::updatePosition()
{
    prepareGeometryChange();
    m_breakPoints = breakPoints();
    calcBoundingRect();
    update();
}

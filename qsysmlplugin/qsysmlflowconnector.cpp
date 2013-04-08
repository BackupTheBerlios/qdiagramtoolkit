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
#include "qsysmlflowconnector.h"
#include "qsysmlplugin.h"

#include <qdiagramtoolkit.h>

QSysMLFlowConnector::QSysMLFlowConnector(const QString & itemClass, const QVariantMap & properties) :
    QAbstractDiagramShapeConnector(QSysMLPlugin::staticName(), itemClass, properties)
{
    addProperty("textColor", QDiagramToolkit::Color, false, properties.value("textColor", QColor("black")));
    addProperty("textFont", QDiagramToolkit::Font, false, properties.value("textFont"));
}

QSysMLFlowConnector::~QSysMLFlowConnector()
{
}

void QSysMLFlowConnector::drawPolyline(QPainter* painter, const QList<QPointF> & points)
{
	if (points.size() < 2){
		return;
	}
	for (int i = 1; i < points.size(); i++){
		if (points.at(i - 1).x() == points.at(i).x()){
			painter->drawLine(points.at(i - 1), points.at(i));
		}
	}
}

QRectF QSysMLFlowConnector::focusRect() const
{
	return boundingRect();
}

QLineF QSysMLFlowConnector::line() const
{
	QLineF l;
	if (connectionPointAtStart() && connectionPointAtStart()->orientation() == QDiagramToolkit::West){
		l.setP1(QPointF(startPos().x()- connectionPointAtStart()->boundingRect().center().x(), startPos().y()));
	} else if (connectionPointAtStart() && connectionPointAtStart()->orientation() == QDiagramToolkit::South){
		l.setP1(QPointF(startPos().x(), startPos().y() + connectionPointAtStart()->boundingRect().height()));
	} else {
		l.setP1(startPos());
	}
	if (connectionPointAtEnd() && connectionPointAtEnd()->orientation() == QDiagramToolkit::North){
		l.setP2(QPointF(endPos().x(), endPos().y() - connectionPointAtEnd()->boundingRect().height()));
	} else {
		l.setP2(endPos());
	}
	return l;
}

void QSysMLFlowConnector::paintArrow(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	if (connectionPointAtStart() && connectionPointAtStart()->id().left(9) == "controlIn"){
	}
}

void QSysMLFlowConnector::paintFocus(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->save();
    if (isSelected()){
		painter->setPen(QPen(QBrush(QColor("darkred")), 5));
		painter->setBrush(QBrush(QColor("firebrick")));

		QList<QPointF> b = breakPoints();

		QPainterPath ps;
		ps.moveTo(0, 0);
		ps.lineTo(30, 0);
		ps.lineTo(30, 30);
		ps.lineTo(0, 30);
		ps.closeSubpath();
		ps = ps.translated(-15, -15);

		painter->drawPath(ps.translated(b.first()));

		QPainterPath p;
		p.moveTo(0, 15);
		p.lineTo(15, 0);
		p.lineTo(30, 15);
		p.lineTo(15, 30);
		p.closeSubpath();
		p = p.translated(-15, -15);
		for (int i = 1; i < b.size() - 1; i++){
			painter->drawPath(p.translated(b.at(i)));
		}
		painter->setBrush(QBrush(Qt::white));
		painter->drawPath(ps.translated(b.last()));
    }
    painter->restore();
}

qreal QSysMLFlowConnector::radius() const
{
	int r = 100;
	if (abs(line().dy()) < r || abs(line().dx()) < r){
		if (abs(line().dy()) < abs(line().dx())){
			r = abs(line().dy());
		} else {
			r = abs(line().dx());
		}
	}
	return r;
}

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
#include "qlogiccircuitoutputshape.h"
#include "qlogiccircuitoutputshape_p.h"

#include "qlogiccircuitplugin.h"

#define GATE_BASE_SIZE 50.0
#define GATE_CP_SIZE 40.0

QLogicCircuitOutputShapeConnectionPoint::QLogicCircuitOutputShapeConnectionPoint(QAbstractDiagramShape* shape) :
    QAbstractDiagramShapeConnectionPoint(shape, "in", QDiagramToolkit::West, 1)
{
    setDirection(QAbstractDiagramShapeConnectionPoint::In);
    updatePosition();
}

QLogicCircuitOutputShapeConnectionPoint::~QLogicCircuitOutputShapeConnectionPoint()
{

}

void QLogicCircuitOutputShapeConnectionPoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->save();
    painter->drawRect(rect());
    painter->restore();
}

void QLogicCircuitOutputShapeConnectionPoint::updatePosition()
{
	QRectF r;
	r.setLeft(0);
	r.setTop(parentShape()->geometry().height() / 2 - GATE_CP_SIZE / 2);
	r.setWidth(GATE_CP_SIZE);
	r.setHeight(GATE_CP_SIZE);
    setRect(r);
}

QLogicCircuitOutputShape::QLogicCircuitOutputShape(QGraphicsItem* parent) :
    QAbstractDiagramShape(parent)
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    setAcceptHoverEvents(true);

    addConnectionPoint(new QLogicCircuitOutputShapeConnectionPoint(this));
}

QLogicCircuitOutputShape::QLogicCircuitOutputShape(const QMap<QString, QVariant> & properties, QGraphicsItem* parent) :
QAbstractDiagramShape(QLogicCircuitPlugin::staticName(), QLogicCircuitOutputShape::staticItemClass(), properties, parent)
{
	initGeometry(900, 100);
    addProperty("name", QDiagramToolkit::String, false, properties.value("name", "<output>").toString());
    addProperty("signalType", QDiagramToolkit::String, true, properties.value("signalType").toString());
	addProperty("textColor", QDiagramToolkit::Font, true, properties.value("textColor"));
	addProperty("textFont", QDiagramToolkit::Font, true, properties.value("textFont"));
	addProperty("lineStyle", QDiagramToolkit::Pen, true, properties.value("lineStyle"));

	restoreProperties(properties);

    setAcceptHoverEvents(true);

    addConnectionPoint(new QLogicCircuitOutputShapeConnectionPoint(this));
}

QRectF QLogicCircuitOutputShape::boundingRect() const
{
    QRectF r = geometry();
    r.moveTo(0, 0);
    return r;
}

QVariantMap QLogicCircuitOutputShape::defaultProperties(const QString & id)
{
	QVariantMap p;

	p["textColor"] = "black";
	QFont f("Arial");
	f.setPointSizeF(4);
	p["textFont"] = QDiagramProperty::toMap(f);

	QPen pen(Qt::black);
	pen.setWidthF(5);
	p["lineStyle"] = QDiagramProperty::toMap(pen);

	if (id == "output.analog"){
        p["signalType"] = "analog";
    } else if (id == "output.digital"){
        p["signalType"] = "digital";
	}
	return p;
}

QPointF QLogicCircuitOutputShape::hotSpot(const QString & id)
{
	return QPointF(0, -50);
}

void QLogicCircuitOutputShape::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

	QPen pen = qdiagramproperty_cast<QPen>(property("lineStyle"));
	if (hasProperty("ghost") && property("ghost").toBool()){
		pen.setColor(QColor("firebrick"));
	}
	painter->setPen(pen);
    painter->setBrush(QBrush(Qt::white, Qt::SolidPattern));
    painter->drawPath(shape());
    if (property("signalType").toString() == "analog"){
        int offset = geometry().height() + geometry().height() / 4;
        painter->save();
        QPainterPath p;
		p.moveTo(10 + offset, 20);
		p.lineTo(10 + offset, 80);
		p.moveTo(10 + offset, 50);
		p.lineTo(100 + offset, 50);

		p.moveTo(10 + offset, 50);
		p.lineTo(30 + offset, 30);
		p.lineTo(60 + offset, 70);
		p.lineTo(90 + offset, 20);

        painter->drawPath(p);
        painter->restore();
    } else if (property("signalType").toString() == "digital"){
        int offset = geometry().height() + geometry().height() / 4;
        painter->save();
        QPainterPath p;
        p.moveTo(10 + offset, 80);
        p.lineTo(30 + offset, 80);
        p.lineTo(30 + offset, 20);
        p.lineTo(80 + offset, 20);
        p.lineTo(80 + offset, 80);
        p.lineTo(100 + offset, 80);

		painter->drawPath(p);
        painter->restore();

		QFont f = pointToPixel(QFont("Arial", 3));
        painter->setFont(f);
        QRectF r(boundingRect());
        r.setWidth(150 + offset);
		r.adjust(0, 7, 0, 0);
        painter->drawText(r, Qt::AlignRight, "1\n0");
    }

	painter->setFont(pointToPixel(qdiagramproperty_cast<QFont>(property("textFont"))));

	painter->setPen(qdiagramproperty_cast<QPen>(property("textColor")));
	
    QRectF r(boundingRect().adjusted(0, 0, -GATE_BASE_SIZE / 2, 0));
    painter->drawText(r, Qt::AlignRight | Qt::AlignVCenter, property("name").toString());
    paintConnectionPoints(painter, option, widget);

    paintFocus(painter, option, widget);
}

QPainterPath QLogicCircuitOutputShape::shape() const
{
    QPainterPath p;
    p.addRoundedRect(geometry().height(), 0, geometry().width() - geometry().height(), geometry().height(), geometry().height() / 4, geometry().height() / 4);
    p.moveTo(0, geometry().height() / 2);
    p.lineTo(geometry().height(), geometry().height() / 2);
    return p;
}

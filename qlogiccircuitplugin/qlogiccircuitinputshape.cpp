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
** You should have received a copy o	f the GNU Lesser General Public License
** along with qdialgramlib.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/
#include "stdafx.h"
#include "qlogiccircuitinputshape.h"
#include "qlogiccircuitinputshape_p.h"

#include "qlogiccircuitplugin.h"

#define GATE_BASE_SIZE 50.0
#define GATE_CP_SIZE 40.0

QLogicCircuitInputShapeConnectionPoint::QLogicCircuitInputShapeConnectionPoint(QAbstractDiagramShape* shape) :
    QAbstractDiagramShapeConnectionPoint(shape, "in", QDiagramToolkit::East)
{
    updatePosition();
}

QLogicCircuitInputShapeConnectionPoint::~QLogicCircuitInputShapeConnectionPoint()
{

}

void QLogicCircuitInputShapeConnectionPoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->save();
    painter->drawRect(rect());
    painter->restore();
}

void QLogicCircuitInputShapeConnectionPoint::updatePosition()
{
    QRectF r;
	r.setLeft(parentShape()->geometry().width() - GATE_CP_SIZE);
	r.setTop(parentShape()->geometry().height() / 2 - GATE_CP_SIZE / 2);
	r.setWidth(GATE_CP_SIZE);
	r.setHeight(GATE_CP_SIZE);
    setRect(r);
}

QLogicCircuitInputShape::QLogicCircuitInputShape(QGraphicsItem* parent) :
    QAbstractDiagramShape(parent)
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    setAcceptHoverEvents(true);

    addConnectionPoint(new QLogicCircuitInputShapeConnectionPoint(this));
}

QLogicCircuitInputShape::QLogicCircuitInputShape(const QMap<QString, QVariant> & properties, QGraphicsItem* parent) :
QAbstractDiagramShape(QLogicCircuitPlugin::staticName(), QLogicCircuitInputShape::staticItemClass(), properties, parent)
{
	initGeometry(900, 100);
    addProperty("name", QDiagramToolkit::String, false, properties.value("name", "<input>"));
    addProperty("signalType", QDiagramToolkit::String, true, properties.value("signalType").toString());
	addProperty("textColor", QDiagramToolkit::Color, false, properties.value("textColor"));
	addProperty("textFont", QDiagramToolkit::Font, true, properties.value("textFont"));
	addProperty("lineStyle", QDiagramToolkit::Pen, true, properties.value("lineStyle"));
    addProperty("state", QDiagramToolkit::Bool, false, properties.value("state", false).toBool());

	restoreProperties(properties);

	setAcceptHoverEvents(true);

    addConnectionPoint(new QLogicCircuitInputShapeConnectionPoint(this));
}

QRectF QLogicCircuitInputShape::boundingRect() const
{
//    return QRectF(0,0, 8 * GATE_BASE_SIZE, 2 * GATE_BASE_SIZE);
    QRectF r = geometry();
    r.moveTo(0, 0);
    return r;
}

QVariantMap QLogicCircuitInputShape::defaultProperties(const QString & id)
{
	QVariantMap p;

	p["textColor"] = QDiagramProperty::toMap(QColor("black"));
	QFont f("Arial");
	f.setPointSizeF(4);
	p["textFont"] = QDiagramProperty::toMap(f);

	QPen pen(Qt::black);
	pen.setWidthF(5);
	p["lineStyle"] = QDiagramProperty::toMap(pen);

	if (id == "input.analog"){
        p["signalType"] = "analog";
    } else if (id == "input.digital"){
        p["signalType"] = "digital";
    } else if (id == "input.flag"){
        p["signalType"] = "flag";
		p["name"] = "<flag>";
	}
	return p;
}

QPointF QLogicCircuitInputShape::hotSpot(const QString & id)
{
	return QPointF(0, -50);
}

void QLogicCircuitInputShape::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

	painter->setPen(qdiagramproperty_cast<QPen>(property("lineStyle")));
	if (hasProperty("ghost")){
		if (property("ghost").toBool()){
			QPen p = painter->pen();
			p.setColor(QColor("firebrick"));
			painter->setPen(p);
		}
	}
    painter->setBrush(QBrush(Qt::white, Qt::SolidPattern));
    painter->drawPath(shape());
    if (property("signalType").toString() == "analog"){
        int offset = geometry().height() / 4;
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
        int offset = geometry().height() / 4;
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

        painter->setFont(pointToPixel(QFont("Arial", 3)));
        QRectF r(boundingRect());
        r.setWidth(200);
		r.adjust(0, 7, 0, 0);
        painter->drawText(r, Qt::AlignRight, "1\n0");
	} else if (property("signalType").toString() == "flag"){
		painter->drawLine(geometry().width() - (geometry().height() * 2), 0, geometry().width() - (geometry().height() * 2), geometry().height());
		if (property("inverted").toBool()){
		   painter->drawEllipse(geometry().width() - geometry().height(), geometry().height() / 2 - GATE_CP_SIZE / 2, GATE_CP_SIZE, GATE_CP_SIZE);
		} 
    }

	painter->setPen(qdiagramproperty_cast<QColor>(property("textColor")));
	painter->setFont(pointToPixel(qdiagramproperty_cast<QFont>(property("textFont"))));
	QRectF r(boundingRect().adjusted(200, 0, 0, 0));
    painter->drawText(r, Qt::AlignLeft | Qt::AlignVCenter, property("name").toString());

    if (property("state").toBool()){
        painter->save();
        QPen pen;
        pen.setColor(Qt::green);
        pen.setWidth(2);
        painter->setPen(pen);
        painter->drawLine(geometry().width() - geometry().height(), geometry().height() / 2, geometry().width(), geometry().height() / 2);
        painter->restore();
    }
    paintConnectionPoints(painter, option, widget);

    paintFocus(painter, option, widget);
}

QPainterPath QLogicCircuitInputShape::shape() const
{
    QPainterPath p;
	if (property("signalType") == "flag"){
		p.addRect(0, 0, geometry().width() - geometry().height(), geometry().height());
	} else {
	    p.addRoundedRect(0, 0, geometry().width() - geometry().height(), geometry().height(), geometry().height() / 4, geometry().height() / 4);
	}
    p.moveTo(geometry().width() - geometry().height(), geometry().height() / 2);
    p.lineTo(geometry().width(), geometry().height() / 2);
    return p;
}

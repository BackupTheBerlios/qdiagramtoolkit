/******************************************************************************
** Copyright (C) 2011 - 2012 Martin Hoppe martin@2x2hoppe.de
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
#include "qlogiccircuitparametershape.h"
#include "qlogiccircuitparametershape_p.h"

#include <qdiagramgraphicstextitem.h>

#include "qlogiccircuitplugin.h"

#define GATE_BASE_SIZE 50.0
#define GATE_CP_SIZE 40.0

QLogicCircuitParameterShapeConnectionPoint::QLogicCircuitParameterShapeConnectionPoint(QAbstractDiagramShape* shape) :
    QAbstractDiagramShapeConnectionPoint(shape, "inp1", QDiagramToolkit::East)
{
    updatePosition();
}

QLogicCircuitParameterShapeConnectionPoint::~QLogicCircuitParameterShapeConnectionPoint()
{

}

void QLogicCircuitParameterShapeConnectionPoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->save();
    painter->drawRect(rect());
    painter->restore();
}

void QLogicCircuitParameterShapeConnectionPoint::updatePosition()
{
    QRectF r;
	r.setLeft(parentShape()->geometry().width() - GATE_CP_SIZE);
	r.setTop(parentShape()->geometry().height() / 2 - GATE_CP_SIZE / 2);
	r.setWidth(GATE_CP_SIZE);
	r.setHeight(GATE_CP_SIZE);
    setRect(r);
}

QLogicCircuitParameterShape::QLogicCircuitParameterShape(const QMap<QString, QVariant> & properties, QGraphicsItem* parent) :
QAbstractDiagramShape(QLogicCircuitPlugin::staticName(), QLogicCircuitParameterShape::staticItemClass(), properties, parent)
{
	initGeometry(900, 100);
	m_textItem = new QDiagramGraphicsTextItem("name", this);
	m_textItem->setTextColorProperty("textColor");
    addProperty("name", QDiagramToolkit::String, false, properties.value("name", "<input>"));
    addProperty("signalType", QDiagramToolkit::String, true, properties.value("signalType").toString());
	addProperty("textColor", QDiagramToolkit::Color, true, properties.value("textColor"));
	addProperty("textFont", QDiagramToolkit::Font, true, properties.value("textFont"));
	addProperty("lineStyle", QDiagramToolkit::Pen, true, properties.value("lineStyle"));

	m_textItem->setPlainText(properties.value("name", "<input>").toString());

	restoreProperties(properties);

	setAcceptHoverEvents(true);

    addConnectionPoint(new QLogicCircuitParameterShapeConnectionPoint(this));
}

QRectF QLogicCircuitParameterShape::boundingRect() const
{
    QRectF r = geometry();
    r.moveTo(0, 0);
    return r;
}

QPointF QLogicCircuitParameterShape::hotSpot(const QString & id)
{
	Q_UNUSED(id);
	return QPoint(-900, -50);
}

QVariantMap QLogicCircuitParameterShape::defaultProperties(const QString & id)
{
	QVariantMap p;

	p["textColor"] = "black";
	QFont f("Arial");
	f.setPointSizeF(4);
	p["textFont"] = QDiagramProperty::toMap(f);

	QPen pen(Qt::black);
	pen.setWidthF(5);
	p["lineStyle"] = QDiagramProperty::toMap(pen);

    if (id == "parameter.analog"){
        p["signalType"] = "analog";
    } else if (id == "parameter.digital"){
        p["signalType"] = "digital";
	}
	return p;
}

void QLogicCircuitParameterShape::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
	QAbstractDiagramShape::mouseDoubleClickEvent(event);
}

void QLogicCircuitParameterShape::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

	painter->setPen(qdiagramproperty_cast<QPen>(property("lineStyle")));
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
        QFont f;
        f.setFamily("Arial");
        f.setPixelSize(4);
        painter->setFont(f);

        QRectF r(boundingRect());
		r.setWidth(15);
        painter->drawText(r, Qt::AlignRight, "1\n0");
    }

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

QVariant QLogicCircuitParameterShape::itemPropertyHasChanged( const QString & name, const QVariant & value)
{
	if (name == "name"){
		m_textItem->setPlainText(value.toString());
	}
	return QAbstractDiagramShape::itemPropertyHasChanged(name, value);
}

QPainterPath QLogicCircuitParameterShape::shape() const
{
	m_textItem->setPos(15, (boundingRect().height() - m_textItem->boundingRect().height()) / 2);

	QPainterPath p;
    //p.addRoundedRect(0, 0, geometry().width() - geometry().height(), geometry().height(), geometry().height() / 4, geometry().height() / 4);
	p.moveTo(0, 0);
	p.lineTo(geometry().width() - geometry().height() * 1.5 , 0);
	p.lineTo(geometry().width() - geometry().height(), geometry().height() / 2); 
	p.lineTo(geometry().width() - geometry().height() * 1.5 , geometry().height());
	p.lineTo(0, geometry().height());
	p.lineTo(0, 0);

    p.moveTo(geometry().width() - geometry().height(), geometry().height() / 2);
    p.lineTo(geometry().width(), geometry().height() / 2);
    return p;
}

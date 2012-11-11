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

#define GATE_BASE_SIZE 13.0
#define GATE_CP_SIZE 6.0

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
    painter->drawRoundedRect(rect(), 2, 2);
    painter->restore();
}

void QLogicCircuitParameterShapeConnectionPoint::updatePosition()
{
    QRectF r;
    r.setLeft(parentShape()->geometry().width() - parentShape()->geometry().height() / 4);
    r.setTop(parentShape()->geometry().height() / 2 - parentShape()->geometry().height() / 8);
    r.setWidth(parentShape()->geometry().height() / 4);
    r.setHeight(parentShape()->geometry().height() / 4);
    setRect(r);
}

QLogicCircuitParameterShape::QLogicCircuitParameterShape(const QMap<QString, QVariant> & properties, QGraphicsItem* parent) :
    QAbstractDiagramShape(QLogicCircuitPlugin::staticName(), "parameter", properties, parent)
{
	initGeometry(182, 26);
	m_textItem = new QDiagramGraphicsTextItem("name", this);
	//m_textItem->setTextInteractionFlags(Qt::TextEditorInteraction);

    addProperty("name", QDiagramToolkit::String, false, properties.value("name", "<input>"));
    addProperty("signalType", QDiagramToolkit::String, true, properties.value("signalType").toString());
	//if (properties.value("signalType") == "digital"){
	//	addProperty("state", QDiagramToolkit::Bool, false, properties.value("state", false).toBool());
	//} else {
	//	addProperty("value", QDiagramToolkit::Double, false, properties.value("value", 0.0).toDouble());
	//	addProperty("unit", QDiagramToolkit::String, false, properties.value("string").toString());
	//}

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

void QLogicCircuitParameterShape::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
	//if (m_textItem->textInteractionFlags() == Qt::NoTextInteraction){
	//	m_textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
	//}
	QAbstractDiagramShape::mouseDoubleClickEvent(event);
}

void QLogicCircuitParameterShape::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(pen());
    painter->setBrush(QBrush(Qt::white, Qt::SolidPattern));
    painter->drawPath(shape());
    if (property("signalType").toString() == "analog"){
        int offset = geometry().height() / 4;
        painter->save();
        QPainterPath p;
		p.moveTo(1 + offset, 7);
		p.lineTo(1 + offset, 19);
		p.moveTo(1 + offset, 13);
		p.lineTo(19 + offset, 13);

		p.moveTo(1 + offset, 13);
		p.lineTo(4 + offset, 9);
		p.lineTo(7 + offset, 16);
		p.lineTo(10 + offset, 8);
		p.lineTo(15 + offset, 18);
		p.lineTo(19 + offset, 7);

        painter->drawPath(p);
        painter->restore();
    } else if (property("signalType").toString() == "digital"){
        int offset = geometry().height() / 4;
        painter->save();
        QPainterPath p;
        p.moveTo(1 + offset, 19);
        p.lineTo(5 + offset, 19);
        p.lineTo(5 + offset, 7);
        p.lineTo(15 + offset, 7);
        p.lineTo(15 + offset, 19);
        p.lineTo(19 + offset, 19);
        painter->drawPath(p);
        painter->restore();
        QFont f;
        f.setFamily("Arial");
        f.setPixelSize(8);
        painter->setFont(f);

        QRectF r(boundingRect());
        r.setWidth(26 + GATE_BASE_SIZE / 2);
        r.moveTop(3);
        painter->drawText(r, Qt::AlignRight, "1\n0");
    }
    //QFont f;
    //f.setFamily("Arial");
    //f.setPixelSize(10);
    //painter->setFont(f);
    //QRectF r(boundingRect().adjusted(26 + GATE_BASE_SIZE, 0, 0, 0));
    //painter->drawText(r, Qt::AlignLeft | Qt::AlignVCenter, property("name").toString());

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
	m_textItem->setPos(26 + GATE_BASE_SIZE, (boundingRect().height() - m_textItem->boundingRect().height()) / 2);

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

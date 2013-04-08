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
#include "qsysmlaction.h"

#include <qdiagramstylesheet.h>
#include <qdiagramgraphicstextitem.h>

#include "qsysmlplugin.h"
#include "qsysmlcontrolconnectionpoint.h"
#include "qsysmldataconnectionpoint.h"

QSysMLAction::QSysMLAction(const QMap<QString, QVariant> &properties, QGraphicsItem *parent) :
QAbstractDiagramShape(QSysMLPlugin::staticName(), QSysMLAction::staticItemClass(), properties, parent)
{
	initGeometry(400, 300);
	textItem()->setTextColorProperty("textColor");
	textItem()->setTextProperty("name");
	textItem()->setDefaultTextAlignment(Qt::AlignCenter);

    addProperty("actionType", QDiagramToolkit::String, true, properties.value("actionType"));
    addProperty("background", QDiagramToolkit::Brush, false, properties.value("background", QColor("wheat")));
    addProperty("textColor", QDiagramToolkit::Color, false, properties.value("textColor", QColor("black")));
    addProperty("textFont", QDiagramToolkit::Font, false, properties.value("textFont"));
    addProperty("lineStyle", QDiagramToolkit::Pen, false, properties.value("lineStyle"));
    addProperty("name", QDiagramToolkit::String, false, "action");

    addSizeGripHandle(new QDiagramShapeSizeGripHandle(QDiagramShapeSizeGripHandle::Left, this));
    addSizeGripHandle(new QDiagramShapeSizeGripHandle(QDiagramShapeSizeGripHandle::Right, this));

	addConnectionPoint(new QSysMLControlConnectionPointIn(this, "controlIn", 0, 1));
	addConnectionPoint(new QSysMLControlConnectionPointOut(this, "controlOut", 0, 1));

	addConnectionPoint(new QSysMLDataConnectionPointIn(this, "dataIn1", 0, 1));
	addConnectionPoint(new QSysMLDataConnectionPointOut(this, "dataOut1", 0, 1));

    restoreProperties(properties);
}

QRectF QSysMLAction::boundingRect() const
{
    QRectF r = geometry();
    r.moveTo(0, 0);
    return r;
}

QVariantMap QSysMLAction::defaultProperties(const QString & id)
{
	QVariantMap m;

	m["actionType"] = "default";
	m["textFont"] = QDiagramProperty::toMap(QFont("Arial", 5));

    if (id == "action.accept_event"){
        m["actionType"] = "event";
	} else if (id == "action.send_signal"){
        m["actionType"] = "sendSignal";
	}
	// 223, 195, 150
	QPen pen(QColor(223, 195, 150));
	pen.setWidthF(7.5);
	m["lineStyle"] = QDiagramProperty::toMap(pen);

	return m;
}

QPointF QSysMLAction::hotSpot(const QString & id)
{
	Q_UNUSED(id);
	return QPointF(-200, 0);
}

void QSysMLAction::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
	if (event->button() == Qt::LeftButton){
		textItem()->setTextWidth(geometry().width());
		textItem()->updatePosition();
		textItem()->setEditModeEnabled(true);
	}
	QAbstractDiagramShape::mouseDoubleClickEvent(event);
}

void QSysMLAction::paint(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);

    painter->save();

	painter->setPen(diagram()->styleSheet()->lineStyle("ControlFlowLineStyle").pen());
	painter->drawPath(shapeConnectionLines());

    painter->restore();
    painter->save();

	QBrush b = qdiagramproperty_cast<QBrush>(property("background"));
    painter->setBrush(b);

	painter->setPen(qdiagramproperty_cast<QPen>(property("lineStyle")));
	painter->drawPath(shapeInternal());

    painter->restore();
    paintFocus(painter, option, widget);
}


QPainterPath QSysMLAction::shapeConnectionLines() const
{
	QPainterPath p;
	p.moveTo(geometry().width() / 2, 0);
	p.lineTo(geometry().width() / 2, 50);
	p.moveTo(geometry().width() / 2, geometry().height() - 50);
	p.lineTo(geometry().width() / 2, geometry().height());
	return p;
}

QPainterPath QSysMLAction::shapeInternal() const
{
	QPainterPath p;
	if (property("actionType") == "default"){
		p.addRoundedRect(0, 50, geometry().width(), geometry().height() - 100, 40, 40);
	} else if (property("actionType") == "event"){
		p.moveTo(0, 50);
		p.lineTo(geometry().width(), 50);
		p.lineTo(geometry().width(), geometry().height() - 50);
		p.lineTo(0, geometry().height() - 50);
		p.lineTo((geometry().height() - 100) / 2, geometry().height() / 2);
		p.closeSubpath();
	} else if (property("actionType") == "sendSignal"){
		p.moveTo(0, 50);
		p.lineTo(geometry().width() - (geometry().height() - 100) / 2, 50);
		p.lineTo(geometry().width(), geometry().height() / 2);
		p.lineTo(geometry().width() - (geometry().height() - 100) / 2, geometry().height() - 50);
		p.lineTo(0, geometry().height() - 50);
		p.closeSubpath();
	}
	return p;
}

QPainterPath QSysMLAction::shape() const
{
    QPainterPath p;
	p.connectPath(shapeInternal());
	p.connectPath(shapeConnectionLines());
    return p;
}

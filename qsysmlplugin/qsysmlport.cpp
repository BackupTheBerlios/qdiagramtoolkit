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
#include "qsysmlport.h"

#include <qdiagramgraphicstextitem.h>

#include "qsysmldataconnectionpoint.h"
#include "qsysmlplugin.h"

QSysMLPort::QSysMLPort(const QMap<QString,QVariant> & properties, QGraphicsItem* parent) :
QAbstractDiagramShape(QSysMLPlugin::staticName(), QSysMLPort::staticItemClass(), properties, parent)
{
	initGeometry(400, 100);
	textItem()->setTextProperty("text");

	addProperty("portType", QDiagramToolkit::String, true, properties.value("portType"));
    addProperty("background", QDiagramToolkit::Brush, false, properties.value("background", QColor("wheat")));
    addProperty("lineStyle", QDiagramToolkit::Pen, false, properties.value("lineStyle"));
    addProperty("text", QDiagramToolkit::String, false, properties.value("text"));
    addProperty("textFont", QDiagramToolkit::Font, false, properties.value("textFont"));
    addProperty("textColor", QDiagramToolkit::Color, false, properties.value("textColor"));

	if (properties.value("portType") == "input"){
		addConnectionPoint(new QSysMLDataConnectionPointOut(this, "dataOut1", 0, 1));
	} else if (properties.value("portType") == "output"){
		addConnectionPoint(new QSysMLDataConnectionPointIn(this, "dataIn1", 0, 1));
	}

    addSizeGripHandle(new QDiagramShapeSizeGripHandle(QDiagramShapeSizeGripHandle::Bottom, this));
    addSizeGripHandle(new QDiagramShapeSizeGripHandle(QDiagramShapeSizeGripHandle::Left, this));
    addSizeGripHandle(new QDiagramShapeSizeGripHandle(QDiagramShapeSizeGripHandle::Right, this));
    addSizeGripHandle(new QDiagramShapeSizeGripHandle(QDiagramShapeSizeGripHandle::Top, this));

    restoreProperties(properties);
	updateConnectionPoints();
}

QRectF QSysMLPort::boundingRect() const
{
    QRectF r(geometry());
    r.moveTo(0, 0);
    return r;
}

QVariantMap QSysMLPort::defaultProperties(const QString & id)
{
	Q_UNUSED(id);
	QVariantMap m;
	m["textFont"] = QDiagramProperty::toMap(QFont("Arial", 5));
	m["textColor"] = QDiagramProperty::toMap(QColor(Qt::black));

	QPen pen(QColor(223, 195, 150));
	pen.setWidthF(5.0);
	m["lineStyle"] = QDiagramProperty::toMap(pen);

    if (id == "port.input"){
        m["portType"] = "input";
    } else if (id == "port.output"){
        m["portType"] = "output";
	}
	return m;
}

QPointF QSysMLPort::hotSpot(const QString & id)
{
	Q_UNUSED(id);
	return QPointF(-250, 0);
}

void QSysMLPort::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->save();
	QBrush b = qdiagramproperty_cast<QBrush>(property("background"));
    painter->setBrush(b);
	painter->setPen(qdiagramproperty_cast<QPen>(property("lineStyle")));
	if (property("portType") == "input"){
		painter->drawRect(0, 0, geometry().width() - 20, geometry().height());
	} else if (property("portType") == "output"){
		painter->drawRect(20, 0, geometry().width() - 20, geometry().height());
	}
	painter->setBrush(QBrush(Qt::white));
	if (property("portType") == "input"){
		painter->drawRect(geometry().width() - 40, geometry().height() / 2 - 20, 40, 40);
	} else if (property("portType") == "output"){
		painter->drawRect(0, geometry().height() / 2 - 20, 40, 40);
	}
    painter->restore();

	paintFocus(painter, option, widget);
}

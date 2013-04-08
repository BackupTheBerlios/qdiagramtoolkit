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
#include "qsysmlblock.h"

#include "qsysmlplugin.h"
#include "qsysmlcontrolconnectionpoint.h"
#include "qsysmldataconnectionpoint.h"

QSysMLBlock::QSysMLBlock(const QMap<QString, QVariant> &properties, QGraphicsItem *parent) :
QAbstractDiagramShape(QSysMLPlugin::staticName(), QSysMLBlock::staticItemClass(), properties, parent)
{
	initGeometry(400, 300);
    addProperty("blockType", QDiagramToolkit::String, true, properties.value("blockType"));
    addProperty("backgroundColor", QDiagramToolkit::Color, false, QColor("white"));
    addProperty("name", QDiagramToolkit::String, false, "action");
    addProperty("textFont", QDiagramToolkit::Font, false, properties.value("textFont"));

    addSizeGripHandle(new QDiagramShapeSizeGripHandle(QDiagramShapeSizeGripHandle::Left, this));
    addSizeGripHandle(new QDiagramShapeSizeGripHandle(QDiagramShapeSizeGripHandle::Right, this));

	addConnectionPoint(new QSysMLControlConnectionPointIn(this, "controlIn", 0, 1));
	addConnectionPoint(new QSysMLControlConnectionPointOut(this, "controlOut", 0, 1));

	addConnectionPoint(new QSysMLDataConnectionPointIn(this, "dataIn1", 0, 1));
	addConnectionPoint(new QSysMLDataConnectionPointOut(this, "dataOut1", 0, 1));

    restoreProperties(properties);
}

QRectF QSysMLBlock::boundingRect() const
{
    QRectF r = geometry();
    r.moveTo(0, 0);
    return r;
}

QVariantMap QSysMLBlock::defaultProperties(const QString & id)
{
	Q_UNUSED(id);
	QVariantMap m;
	m["textFont"] = QDiagramProperty::toMap(QFont("Arial", 5));
	m["textColor"] = QDiagramProperty::toMap(QColor(Qt::black));

	QPen pen(QColor(223, 195, 150));
	pen.setWidthF(5.0);
	m["lineStyle"] = QDiagramProperty::toMap(pen);
	m["name"] = "block";
	return m;
}

QPointF QSysMLBlock::hotSpot(const QString & id)
{
	Q_UNUSED(id);
	return QPointF(-250, 0);
}

void QSysMLBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	QColor color = qdiagramproperty_cast<QColor>(property("backgroundColor"));
    painter->setBrush(QBrush(color, Qt::SolidPattern));
    painter->drawRect(0, 0, geometry().width(), geometry().height());

    QFont f = pointToPixel(qdiagramproperty_cast<QFont>(property("textFont")));
 
    QFontMetricsF fm(f);
    qreal pixelsWide = fm.width("M");
    qreal pixelsHigh = fm.height();
    // « »
    QRect br = boundingRect().adjusted(-2, -2, -2, -2).toRect();
    QRect r(0, 5, geometry().width(), pixelsHigh);
    painter->setFont(f);
    painter->drawText(r, Qt::AlignHCenter, "«block»");
    f.setBold(true);
    painter->setFont(f);
    painter->drawText(0, r.height() + fm.leading(), geometry().width(), geometry().height(), Qt::AlignHCenter | Qt::TextWordWrap, property("name").toString(), &br);

    paintFocus(painter, option, widget);
}

QPainterPath QSysMLBlock::shape() const
{
    QPainterPath p;
    p.addRect(0, 0, geometry().width(), geometry().height());
    return p;
}

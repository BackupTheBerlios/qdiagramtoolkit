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

#define GATE_BASE_SIZE 13.0
#define GATE_CP_SIZE 6.0

QLogicCircuitOutputShapeConnectionPoint::QLogicCircuitOutputShapeConnectionPoint(QAbstractDiagramShape* shape) :
    QAbstractDiagramShapeConnectionPoint(shape, "out1", QAbstractDiagramShapeConnectionPoint::West, 1)
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
    painter->drawRoundedRect(rect(), 2, 2);
    painter->restore();
}

void QLogicCircuitOutputShapeConnectionPoint::updatePosition()
{
//    setRect(QRectF(0, GATE_BASE_SIZE - GATE_CP_SIZE / 2, GATE_CP_SIZE, GATE_CP_SIZE));
    QRectF r;
    r.setLeft(0);
    r.setTop(parentShape()->geometry().height() / 2 - parentShape()->geometry().height() / 8);
    r.setWidth(parentShape()->geometry().height() / 4);
    r.setHeight(parentShape()->geometry().height() / 4);
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
    QAbstractDiagramShape(properties, parent)
{
    addProperty("name", QDiagramGraphicsItemMetaProperty::String, false, properties.value("name", "<output>").toString());
    addProperty("signalType", QDiagramGraphicsItemMetaProperty::String, true, properties.value("signalType").toString());

	restoreProperties(properties);

    setAcceptHoverEvents(true);

    addConnectionPoint(new QLogicCircuitOutputShapeConnectionPoint(this));
}

//QLogicCircuitOutputShape::QLogicCircuitOutputShape(const QString & uuid, const QString & type, const QString & plugin, QGraphicsItem* parent) :
//    QAbstractDiagramShape(uuid, "output", plugin, parent)
//{
//    addProperty("name", QDiagramGraphicsItemMetaProperty::String, false, "<output>");
////    addProperty("signalType", QDiagramGraphicsItemMetaProperty::String, true, properties.value("signalType").toString());
//
//    setAcceptHoverEvents(true);
//
//    addConnectionPoint(new QLogicCircuitOutputShapeConnectionPoint(this));
//}

QRectF QLogicCircuitOutputShape::boundingRect() const
{
    QRectF r = geometry();
    r.moveTo(0, 0);
    return r;
}

void QLogicCircuitOutputShape::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
//    setConnectionPointsVisible(true);
}

void QLogicCircuitOutputShape::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
//    setConnectionPointsVisible(false);
}

void QLogicCircuitOutputShape::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(pen());
    painter->setBrush(QBrush(Qt::white, Qt::SolidPattern));
    painter->drawPath(shape());
    if (property("signalType").toString() == "analog"){
        int offset = geometry().height() + geometry().height() / 4;
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
        int offset = geometry().height() + geometry().height() / 4;
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
        QFont mFont;
        mFont.setFamily("Arial");
        mFont.setPixelSize(8);
        painter->setFont(mFont);
        QRectF mRect(boundingRect());
        mRect.setWidth(26 + offset);
        mRect.moveTop(3);
        painter->drawText(mRect, Qt::AlignRight, "1\n0");
    }
    QFont mFont;
    mFont.setFamily("Arial");
    mFont.setPixelSize(10);
    painter->setFont(mFont);
    QRectF mRect(boundingRect().adjusted(0, 0, -GATE_BASE_SIZE / 2, 0));
    painter->drawText(mRect, Qt::AlignRight | Qt::AlignVCenter, property("name").toString());
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

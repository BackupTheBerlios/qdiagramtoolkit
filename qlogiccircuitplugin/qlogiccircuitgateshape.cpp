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
#include "qlogiccircuitgateshape.h"

#include "qlogiccircuitgateshape_p.h"

#define GATE_BASE_SIZE 13.0
#define GATE_CP_SIZE 8.0

QLogicGateInputConnectionPoint::QLogicGateInputConnectionPoint(QAbstractDiagramShape* shape, const QString & id, int index, int maxConnections) :
    QAbstractDiagramShapeConnectionPoint(shape, id, QAbstractDiagramShapeConnectionPoint::West, maxConnections)
{
    m_index = index;
    setDirection(QAbstractDiagramShapeConnectionPoint::In);
    updatePosition();
}

QLogicGateInputConnectionPoint::~QLogicGateInputConnectionPoint()
{
}

void QLogicGateInputConnectionPoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->save();
    painter->drawRoundedRect(rect(), 2, 2);
    painter->restore();
}

void QLogicGateInputConnectionPoint::updatePosition()
{
    if (parentShape()->property("inputs").toInt() == 1){
        setRect(QRectF(0, parentShape()->boundingRect().center().y() - GATE_CP_SIZE / 2, GATE_CP_SIZE, GATE_CP_SIZE));
    } else {
        setRect(QRectF(0, GATE_BASE_SIZE + (m_index * GATE_BASE_SIZE * 2) - GATE_CP_SIZE / 2, GATE_CP_SIZE, GATE_CP_SIZE));
    }
}

QLogicGateOutputConnectionPoint::QLogicGateOutputConnectionPoint(QAbstractDiagramShape* shape, const QString & id, int maxConnections) :
    QAbstractDiagramShapeConnectionPoint(shape, id, QAbstractDiagramShapeConnectionPoint::East, maxConnections)
{
    setDirection(QAbstractDiagramShapeConnectionPoint::Out);
    updatePosition();
}

QLogicGateOutputConnectionPoint::~QLogicGateOutputConnectionPoint()
{
}

void QLogicGateOutputConnectionPoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->save();
    painter->setBrush(Qt::red);
    painter->drawRoundedRect(rect(), 2, 2);
    painter->restore();
}

void QLogicGateOutputConnectionPoint::updatePosition()
{
    setRect(QRectF(parentShape()->boundingRect().width() - GATE_CP_SIZE, parentShape()->boundingRect().center().y() - GATE_CP_SIZE / 2, GATE_CP_SIZE, GATE_CP_SIZE));
}

QLogicCircuitGateShape::QLogicCircuitGateShape(QGraphicsItem* parent) :
    QAbstractDiagramShape(parent)
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    setAcceptHoverEvents(true);
}

QLogicCircuitGateShape::QLogicCircuitGateShape(const QMap<QString, QVariant> & properties, QGraphicsItem* parent) :
    QAbstractDiagramShape(properties, parent)
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    setAcceptHoverEvents(true);
    addProperty("gateType", QDiagramGraphicsItemMetaProperty::String, true, properties.value("gateType").toString());
    addProperty("showState", QDiagramGraphicsItemMetaProperty::Bool, false, false);

    if (properties.value("gateType").toString() == "not"){
        addProperty("inputs", QDiagramGraphicsItemMetaProperty::Int, false, 1);
        addConnectionPoint(new QLogicGateInputConnectionPoint(this, "In1", 0, 1));
    } else {
        addProperty("inputs", QDiagramGraphicsItemMetaProperty::Int, false, 2);
        addConnectionPoint(new QLogicGateInputConnectionPoint(this, "In1", 0, 1));
        addConnectionPoint(new QLogicGateInputConnectionPoint(this, "In2", 1, 1));
    }
    addConnectionPoint(new QLogicGateOutputConnectionPoint(this, "Out",1 ));

	restoreProperties(properties);
}

QRectF QLogicCircuitGateShape::boundingRect() const
{
    return  QRectF(0, 0, geometry().width(), geometry().height());
}

QList<QAction*> QLogicCircuitGateShape::createActions(QWidget* parent)
{
	QAction* a;
	QList<QAction*> l;
	QVariantMap p;

	if (property("gateType") != "not"){
		a = new QAction(QObject::tr("2 Inputs"), parent);
		a->setObjectName("2i");
		a->setCheckable(true);
		a->setChecked(property("inputs").toInt() == 2);
		p["inputs"] = 2;
		a->setData(p);
		l << a;

		a = new QAction(QObject::tr("4 Inputs"), parent);
		a->setObjectName("4i");
		a->setCheckable(true);
		a->setChecked(property("inputs").toInt() == 4);
		p["inputs"] = 4;
		a->setData(p);
		l << a;

		a = new QAction(QObject::tr("6 Inputs"), parent);
		a->setObjectName("6i");
		a->setCheckable(true);
		a->setChecked(property("inputs").toInt() == 6);
		p["inputs"] = 6;
		a->setData(p);
		l << a;

		a = new QAction(QObject::tr("8 Inputs"), parent);
		a->setObjectName("8i");
		a->setCheckable(true);
		a->setChecked(property("inputs").toInt() == 8);
		p["inputs"] = 8;
		a->setData(p);
		l << a;
	}
	return l;
}

QPoint QLogicCircuitGateShape::hotSpot() const
{
    return QPoint(0, geometry().height() / (property("inputs").toInt() * 2));
}

void QLogicCircuitGateShape::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
//    setConnectionPointsVisible(true);
}

void QLogicCircuitGateShape::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
//    setConnectionPointsVisible(false);
}

QList<QAbstractDiagramShapeConnector*> QLogicCircuitGateShape::inputs() const
{
	QList<QAbstractDiagramShapeConnector*> l;
	Q_FOREACH(QAbstractDiagramShapeConnectionPoint* p, connectionPoints(QAbstractDiagramShapeConnectionPoint::West)){
		Q_FOREACH(QAbstractDiagramShapeConnectionPoint::Connection c, p->connections()){
			l.append(c.connector);
		}
	}
	return l;
}

QVariant QLogicCircuitGateShape::itemPropertyChange(const QString &name, const QVariant &value)
{
    if (name == "inputs"){
        int inputs = value.toInt();
        if (property("gateType").toString() == "not"){
            return 1;
        }
        if (inputs < 2){
            inputs = 2;
        }
        if (inputs < connectionPoints(QAbstractDiagramShapeConnectionPoint::West).size()){

        } else {
            while(connectionPoints(QAbstractDiagramShapeConnectionPoint::West).size() < inputs){
                addConnectionPoint(new QLogicGateInputConnectionPoint(this, QString("In%1").arg(connectionPoints(QAbstractDiagramShapeConnectionPoint::West).size() + 1), connectionPoints(QAbstractDiagramShapeConnectionPoint::West).size(), 1));
            }
        }
        blockUndoCommands(true);
        int offset = geometry().height() / (property("inputs").toInt() * 2);
        QRectF r = geometry();
        r.setHeight(offset * 2 + offset * 2 * (inputs - 1));
        setProperty("geometry", r);
        blockUndoCommands(false);
        prepareGeometryChange();
        return inputs;
    }
    return QAbstractDiagramGraphicsItem::itemPropertyChange(name, value);
}

QVariant QLogicCircuitGateShape::itemPropertyHasChanged(const QString &name, const QVariant &value)
{
    if (name == "showState"){
        update();
    } else if (name == "state"){
        update();
    }
    update();
    return QAbstractDiagramGraphicsItem::itemPropertyHasChanged(name, value);
}

void QLogicCircuitGateShape::paintState(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (property("showState").toBool()){
        painter->save();
        painter->setBrush(Qt::red);
        painter->drawText(boundingRect().center().x(), boundingRect().center().y(), "I");
//        painter->drawRoundedRect(GATE_BASE_SIZE * 5 - 5, boundingRect().center().y() - 5, 10, 10, 2, 2);
        painter->restore();
    }
}

void QLogicCircuitGateShape::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setPen(pen());
    painter->setBrush(QBrush(Qt::white, Qt::SolidPattern));
    painter->drawPath(shape());
    QFont mFont;
    mFont.setFamily("Arial");
    mFont.setPixelSize(12);
    painter->setFont(mFont);
    if (property("gateType").toString() == "and"){
        painter->drawText(boundingRect(), Qt::AlignHCenter, "&");
    } else if (property("gateType").toString() == "nand"){
        painter->drawText(boundingRect(), Qt::AlignHCenter, "&");
        painter->drawEllipse(QPoint(5 * GATE_BASE_SIZE + 5, boundingRect().center().y()), 5, 5);
    } else if (property("gateType").toString() == "or"){
        painter->drawText(boundingRect(), Qt::AlignHCenter, ">=1");
    } else if (property("gateType").toString() == "nor"){
        painter->drawText(boundingRect(), Qt::AlignHCenter, ">=1");
        painter->drawEllipse(QPoint(5 * GATE_BASE_SIZE + 5, boundingRect().center().y()), 5, 5);
    } else if (property("gateType").toString() == "not"){
        painter->drawText(boundingRect(), Qt::AlignHCenter, "1");
        painter->drawEllipse(QPoint(5 * GATE_BASE_SIZE + 5, boundingRect().center().y()), 5, 5);
    } else if (property("gateType").toString() == "xor"){
        painter->drawText(boundingRect(), Qt::AlignHCenter, "=1");
    } else if (property("gateType").toString() == "xnor"){
        painter->drawText(boundingRect(), Qt::AlignHCenter, "=1");
        painter->drawEllipse(QPoint(5 * GATE_BASE_SIZE + 5, boundingRect().center().y()), 5, 5);
    }
    paintConnectionPoints(painter, option, widget);
    paintFocus(painter, option, widget);
    paintState(painter, option, widget);
}

QPainterPath QLogicCircuitGateShape::shape() const
{
    QPainterPath p;
    int offset = 0;
    if (property("gateType").toString() == "not"){
        offset = geometry().height() / 4;
        p.addRect(offset, 0, geometry().width() - offset * 2, geometry().height());
        p.moveTo(0, geometry().height() / 2);
        p.lineTo(offset, geometry().height() / 2);
    } else {
        offset = geometry().height() / (property("inputs").toInt() * 2);
        p.addRect(offset, 0, geometry().width() - offset * 2, geometry().height());
        for (int i = 0; i < property("inputs").toInt(); i++){
            p.moveTo(0, offset + (offset * 2 * i));
            p.lineTo(offset, offset + (offset * 2 * i));
        }
    }
    p.moveTo(geometry().width() - offset, geometry().height() / 2);
    p.lineTo(geometry().width(), geometry().height() / 2);

    return p;
}

void QLogicCircuitGateShape::triggerAction(const QString & name, const QVariant & data)
{
	if (!name.isEmpty() && !data.isNull()){
		QMapIterator<QString, QVariant> it(data.toMap());
		while(it.hasNext()){
			it.next();
			setProperty(it.key(), it.value());
		}
	}
}

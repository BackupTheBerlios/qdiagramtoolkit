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
#include "qlogiccircuitfunctionshape.h"
#include "qlogiccircuitfunctionshape_p.h"

#include <qdiagrammetaenum.h>
#include <qdiagrammetadata.h>

#include "qlogiccircuitplugin.h"

#define BASE_SIZE 50.0
#define GATE_CP_SIZE 40.0


QLogicCircuitFunctionInputConnectionPoint::QLogicCircuitFunctionInputConnectionPoint(QAbstractDiagramShape* shape, const QString & id, int index, int maxConnections) :
    QAbstractDiagramShapeConnectionPoint(shape, id, QDiagramToolkit::West, maxConnections)
{
    m_index = index;
    setDirection(QAbstractDiagramShapeConnectionPoint::In);
    updatePosition();
}

QLogicCircuitFunctionInputConnectionPoint::~QLogicCircuitFunctionInputConnectionPoint()
{
}

void QLogicCircuitFunctionInputConnectionPoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->save();
    painter->drawRect(rect());
    painter->restore();
}

void QLogicCircuitFunctionInputConnectionPoint::updatePosition()
{
    int offset = 0;
    if (parentShape()->property("function").toString() == "comparator"){
        offset = parentShape()->geometry().height() / 4;
	} else if (parentShape()->property("function").toString() == "timer"){
        offset = parentShape()->geometry().height() / 4;
    }
    setRect(QRectF(0, offset + (offset * 2 * m_index) - GATE_CP_SIZE / 2, GATE_CP_SIZE, GATE_CP_SIZE));
}

QLogicCircuitFunctionOutputConnectionPoint::QLogicCircuitFunctionOutputConnectionPoint(QAbstractDiagramShape* shape, const QString & id, int maxConnections) :
    QAbstractDiagramShapeConnectionPoint(shape, id, QDiagramToolkit::East, maxConnections)
{
    updatePosition();
}

QLogicCircuitFunctionOutputConnectionPoint::~QLogicCircuitFunctionOutputConnectionPoint()
{
}

void QLogicCircuitFunctionOutputConnectionPoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->save();
    painter->setBrush(Qt::red);
    painter->drawRect(rect());
    painter->restore();
}

void QLogicCircuitFunctionOutputConnectionPoint::updatePosition()
{
    setRect(QRectF(parentShape()->boundingRect().width() - GATE_CP_SIZE, parentShape()->boundingRect().center().y() - GATE_CP_SIZE / 2, GATE_CP_SIZE, GATE_CP_SIZE));
}

QLogicCircuitFunctionShape::QLogicCircuitFunctionShape(QGraphicsItem *parent) :
    QAbstractDiagramShape(parent)
{
}

QLogicCircuitFunctionShape::QLogicCircuitFunctionShape(const QMap<QString, QVariant> &properties, QGraphicsItem *parent) :
QAbstractDiagramShape(QLogicCircuitPlugin::staticName(), QLogicCircuitFunctionShape::staticItemClass(), properties, parent)
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    setAcceptHoverEvents(true);
	initGeometry(300, 200);

    addProperty("function", QDiagramToolkit::String, true, properties.value("function").toString());
	addProperty("background", QDiagramToolkit::Brush, true, properties.value("background"));
	addProperty("textColor", QDiagramToolkit::Color, true, properties.value("textColor"));
	addProperty("textFont", QDiagramToolkit::Font, true, properties.value("textFont"));
	addProperty("lineStyle", QDiagramToolkit::Pen, true, properties.value("lineStyle"));

    if (properties.value("function").toString() == "comparator"){
        QMap<int,QString> pairs;
        pairs[0] = "equal";
        pairs[1] = "less";
        pairs[2] = "greater";
        addProperty("mode", QDiagramToolkit::Enumeration, pairs, properties.value("mode", 0));

        addConnectionPoint(new QLogicCircuitFunctionInputConnectionPoint(this, "x", 0, 1));
        addConnectionPoint(new QLogicCircuitFunctionInputConnectionPoint(this, "y", 1, 1));
        addConnectionPoint(new QLogicCircuitFunctionOutputConnectionPoint(this, "out"));
    } else if (properties.value("function").toString() == "counter"){
        addConnectionPoint(new QLogicCircuitFunctionInputConnectionPoint(this, "enable", 0, 1));
        addProperty("unit", QDiagramToolkit::String, false, properties.value("unit"));
        addProperty("digits", QDiagramToolkit::String, false, properties.value("digits"));
        addConnectionPoint(new QLogicCircuitFunctionOutputConnectionPoint(this, "out"));
    } else if (properties.value("function").toString() == "computation"){
        QMap<int,QString> pairs;
        pairs[0] = "addtion";
        pairs[1] = "subtraction";
        pairs[2] = "division";
        addProperty("mode", QDiagramToolkit::Enumeration, pairs, properties.value("mode", 0));

        addConnectionPoint(new QLogicCircuitFunctionInputConnectionPoint(this, "x", 0, 1));
        addConnectionPoint(new QLogicCircuitFunctionInputConnectionPoint(this, "y", 1, 1));
        addConnectionPoint(new QLogicCircuitFunctionOutputConnectionPoint(this, "out"));
    } else if (properties.value("function").toString() == "operatingHoursCounter"){
        addProperty("unit", QDiagramToolkit::String, true, "h");
        addProperty("digits", QDiagramToolkit::Int, false, properties.value("digits"));
        addConnectionPoint(new QLogicCircuitFunctionOutputConnectionPoint(this, "out"));
    } else if (properties.value("function").toString() == "timer"){
        QMap<int,QString> pairs;
        pairs[0] = "delay";
        addProperty("mode", QDiagramToolkit::Enumeration, pairs, properties.value("mode", 0));
        addProperty("time", QDiagramToolkit::Int, false, "sec");
        addConnectionPoint(new QLogicCircuitFunctionInputConnectionPoint(this, "trigger", 0, 1));
        addConnectionPoint(new QLogicCircuitFunctionInputConnectionPoint(this, "parameter", 1, 1));
        addConnectionPoint(new QLogicCircuitFunctionOutputConnectionPoint(this, "out"));
    }
    restoreProperties(properties);
}

QLogicCircuitFunctionShape::~QLogicCircuitFunctionShape()
{
}

QRectF QLogicCircuitFunctionShape::boundingRect() const
{
    return QRectF(0, 0, geometry().width(), geometry().height());
}

QList<QAction*> QLogicCircuitFunctionShape::createActions(QWidget* parent)
{
	QAction* a;
	QList<QAction*> l;
	QMenu* m = new QMenu(QObject::tr("Mode"), parent);
	if (property("function") == "comparator" || property("function") == "computation"){
		int index = metaData()->indexOfProperty("mode");
        for (int i = 0; i < metaData()->property(index).enumerator().keyCount(); i++){
			a = new QAction(metaData()->property(index).enumerator().key(i), parent);
			QVariantMap p;
			p["mode"] = metaData()->property(index).enumerator().value(i);
			a->setData(p);
			a->setCheckable(true);
			a->setChecked(property("mode").value() == metaData()->property(index).enumerator().value(i));
			m->addAction(a);
        }
		l << m->menuAction();
	}
	return l;
}

QVariantMap QLogicCircuitFunctionShape::defaultProperties(const QString & id)
{
	QVariantMap p;

	p["textColor"] = "black";
	QFont f("Arial");
	f.setPointSizeF(4);
	p["textFont"] = QDiagramProperty::toMap(f);

	QPen pen(Qt::black);
	pen.setWidthF(5);
	p["lineStyle"] = QDiagramProperty::toMap(pen);

	QBrush brush(Qt::white);
	p["background"] = QDiagramProperty::toMap(brush);

    if (id == "flipflop.asyncron"){
        p["function"] = "comparator";
    } else if (id == "function.comparator"){
        p["function"] = "comparator";
    } else if (id == "function.computation"){
        p["function"] = "computation";
    } else if (id == "function.counter"){
        p["function"] = "counter";
    } else if (id == "function.counter.operating_hours"){
        p["function"] = "operatingHoursCounter";
    } else if (id == "function.timer"){
        p["function"] = "timer";
        p["mode"] = "delay";
        p["time"] = 5;
	}
	return p;
}

QPointF QLogicCircuitFunctionShape::hotSpot(const QString & id)
{
	Q_UNUSED(id);
	return QPointF(0, -BASE_SIZE);
}

void QLogicCircuitFunctionShape::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setPen(qdiagramproperty_cast<QPen>(property("lineStyle")));
	painter->setBrush(qdiagramproperty_cast<QBrush>(property("background")));
    painter->drawPath(shape());

	painter->setFont(pointToPixel(qdiagramproperty_cast<QFont>(property("textFont"))));
	
    if (property("function").toString() == "comparator"){
        if (property("mode").toInt() == 0){
            painter->drawText(boundingRect(), Qt::AlignHCenter, "=");
        } else if (property("mode").toInt() == 1){
            painter->drawText(boundingRect(), Qt::AlignHCenter, "<");
        } else if (property("mode").toInt() == 2){
            painter->drawText(boundingRect(), Qt::AlignHCenter, ">");
        }
    } else if (property("function").toString() == "counter"){
        QRectF mRect(2 + BASE_SIZE, 2, BASE_SIZE * 4 - 4, BASE_SIZE);
        painter->drawRect(mRect);
    } else if (property("function").toString() == "operatingHoursCounter"){
        QRectF mRect(2 + BASE_SIZE, 2, BASE_SIZE * 3 - 4, BASE_SIZE);
        painter->drawRect(mRect);
    } else if (property("function").toString() == "computation"){
        painter->drawText(boundingRect(), Qt::AlignHCenter, "S");
    } else if (property("function").toString() == "timer"){
		QString t = QString("T%1\n%2s").arg(property("mode").toString()).arg(property("time").toString());
        painter->drawText(boundingRect(), Qt::AlignHCenter, t);
		QFontMetrics fm(painter->font());
		painter->drawText(BASE_SIZE + fm.width('I'), BASE_SIZE * 3 + fm.height() / 2 - 5, "P");
    }
    paintConnectionPoints(painter, option, widget);
    paintFocus(painter, option, widget);
}

QPainterPath QLogicCircuitFunctionShape::shape() const
{
    QPainterPath p;
    int offset = geometry().height() / 4;
    p.addRect(offset, 0, geometry().width() - offset * 2, geometry().height());

    if (property("function").toString() == "comparator"){
        p.moveTo(0, BASE_SIZE);
        p.lineTo(BASE_SIZE, BASE_SIZE);
        p.moveTo(0, BASE_SIZE * 3);
        p.lineTo(BASE_SIZE, BASE_SIZE * 3);

        p.moveTo(boundingRect().right() - BASE_SIZE, boundingRect().center().y());
        p.lineTo(boundingRect().right(), boundingRect().center().y());
    } else if (property("function").toString() == "counter" || property("function").toString() == "operatingHoursCounter"){
        p.moveTo(0, BASE_SIZE);
        p.lineTo(BASE_SIZE, BASE_SIZE);

        p.moveTo(boundingRect().right() - BASE_SIZE, boundingRect().center().y());
        p.lineTo(boundingRect().right(), boundingRect().center().y());
    } else if (property("function").toString() == "computation"){
        p.moveTo(0, offset);
        p.lineTo(offset, offset);
        p.moveTo(0, geometry().height() - offset);
        p.lineTo(offset, geometry().height() - offset);

        p.moveTo(geometry().width() - offset, geometry().height() / 2);
        p.lineTo(geometry().width(), geometry().height() / 2);
	} else if (property("function").toString() == "timer"){
        p.moveTo(0, BASE_SIZE);
        p.lineTo(BASE_SIZE, BASE_SIZE);
        p.moveTo(0, BASE_SIZE * 3);
        p.lineTo(BASE_SIZE, BASE_SIZE * 3);

        p.moveTo(boundingRect().right() - BASE_SIZE, boundingRect().center().y());
        p.lineTo(boundingRect().right(), boundingRect().center().y());
    }
    return p;
}

void QLogicCircuitFunctionShape::triggerAction(const QString & name, const QVariant & data)
{
	Q_UNUSED(name);
	if (!data.isNull()){
		QMapIterator<QString, QVariant> it(data.toMap());
		while(it.hasNext()){
			it.next();
			setProperty(it.key(), it.value());
		}
	}
}

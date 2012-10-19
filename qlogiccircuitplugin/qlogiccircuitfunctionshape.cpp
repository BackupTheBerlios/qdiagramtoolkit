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

#include <qdiagramgraphicsitemmetaenum.h>
#include <qdiagramgraphicsitemmetadata.h>

#define BASE_SIZE 13.0
#define GATE_CP_SIZE 8.0


QLogicCircuitFunctionInputConnectionPoint::QLogicCircuitFunctionInputConnectionPoint(QAbstractDiagramShape* shape, const QString & id, int index, int maxConnections) :
    QAbstractDiagramShapeConnectionPoint(shape, id, QAbstractDiagramShapeConnectionPoint::West, maxConnections)
{
    m_index = index;
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
    painter->drawRoundedRect(rect(), 2, 2);
    painter->restore();
}

void QLogicCircuitFunctionInputConnectionPoint::updatePosition()
{
    int offset;
    if (parentShape()->property("function") == "comparator"){
        offset = parentShape()->geometry().height() / 4;
    }
//    if (parentShape()->property("inputs").toInt() == 1){
//        setRect(QRectF(0, parentShape()->boundingRect().center().y() - GATE_CP_SIZE / 2, GATE_CP_SIZE, GATE_CP_SIZE));
//    } else {
//        setRect(QRectF(0, GATE_BASE_SIZE + (m_index * GATE_BASE_SIZE * 2) - GATE_CP_SIZE / 2, GATE_CP_SIZE, GATE_CP_SIZE));
//    }
    setRect(QRectF(0, offset + (offset * 2 * m_index) - GATE_CP_SIZE / 2, GATE_CP_SIZE, GATE_CP_SIZE));
}

QLogicCircuitFunctionOutputConnectionPoint::QLogicCircuitFunctionOutputConnectionPoint(QAbstractDiagramShape* shape, const QString & id, int maxConnections) :
    QAbstractDiagramShapeConnectionPoint(shape, id, QAbstractDiagramShapeConnectionPoint::East, maxConnections)
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
    painter->drawRoundedRect(rect(), 2, 2);
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
    QAbstractDiagramShape(properties, parent)
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    setAcceptHoverEvents(true);

    addProperty("function", QDiagramGraphicsItemMetaProperty::String, true, properties.value("function").toString());

    if (properties.value("function").toString() == "comparator"){
        QMap<int,QString> pairs;
        pairs[0] = "equal";
        pairs[1] = "less";
        pairs[2] = "greater";
        addProperty("mode", QDiagramGraphicsItemMetaProperty::Enumeration, pairs, properties.value("mode", 0));

        addConnectionPoint(new QLogicCircuitFunctionInputConnectionPoint(this, "x", 0, 1));
        addConnectionPoint(new QLogicCircuitFunctionInputConnectionPoint(this, "y", 1, 1));
        addConnectionPoint(new QLogicCircuitFunctionOutputConnectionPoint(this, "out"));
    } else if (properties.value("function").toString() == "counter"){
        addConnectionPoint(new QLogicCircuitFunctionInputConnectionPoint(this, "enable", 0, 1));
        addProperty("unit", QDiagramGraphicsItemMetaProperty::String, false, properties.value("unit"));
        addProperty("digits", QDiagramGraphicsItemMetaProperty::String, false, properties.value("digits"));
        addConnectionPoint(new QLogicCircuitFunctionOutputConnectionPoint(this, "out"));
    } else if (properties.value("function").toString() == "computation"){
        QMap<int,QString> pairs;
        pairs[0] = "addtion";
        pairs[1] = "subtraction";
        pairs[2] = "division";
        addProperty("mode", QDiagramGraphicsItemMetaProperty::Enumeration, pairs, properties.value("mode", 0));

        addConnectionPoint(new QLogicCircuitFunctionInputConnectionPoint(this, "x", 0, 1));
        addConnectionPoint(new QLogicCircuitFunctionInputConnectionPoint(this, "y", 1, 1));
        addConnectionPoint(new QLogicCircuitFunctionOutputConnectionPoint(this, "out"));
    } else if (properties.value("function").toString() == "operatingHoursCounter"){
        addProperty("unit", QDiagramGraphicsItemMetaProperty::String, true, "h");
        addProperty("digits", QDiagramGraphicsItemMetaProperty::Int, false, properties.value("digits"));
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
        for (int i = 0; i < metaData()->property(index).enumerator().keys(); i++){
			a = new QAction(metaData()->property(index).enumerator().key(i), parent);
			QVariantMap p;
			p["mode"] = metaData()->property(index).enumerator().value(i);
			a->setData(p);
			a->setCheckable(true);
			a->setChecked(property("mode") == metaData()->property(index).enumerator().value(i));
			m->addAction(a);
        }
		l << m->menuAction();
	}
	return l;
}

void QLogicCircuitFunctionShape::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setPen(pen());
    painter->setBrush(QBrush(Qt::white, Qt::SolidPattern));
    painter->drawPath(shape());
    QFont font;
    font.setFamily("Arial");
    font.setPixelSize(10);
    painter->setFont(font);

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
    }
    paintConnectionPoints(painter, option, widget);
    paintFocus(painter, option, widget);
}

QPainterPath QLogicCircuitFunctionShape::shape() const
{
    QPainterPath mPath;
    int offset = geometry().height() / 4;
    mPath.addRect(offset, 0, geometry().width() - offset * 2, geometry().height());

    if (property("function").toString() == "comparator"){
        mPath.moveTo(0, BASE_SIZE);
        mPath.lineTo(BASE_SIZE, BASE_SIZE);
        mPath.moveTo(0, BASE_SIZE * 3);
        mPath.lineTo(BASE_SIZE, BASE_SIZE * 3);

        mPath.moveTo(boundingRect().right() - BASE_SIZE, boundingRect().center().y());
        mPath.lineTo(boundingRect().right(), boundingRect().center().y());
    } else if (property("function").toString() == "counter" || property("function").toString() == "operatingHoursCounter"){
        mPath.moveTo(0, BASE_SIZE);
        mPath.lineTo(BASE_SIZE, BASE_SIZE);

        mPath.moveTo(boundingRect().right() - BASE_SIZE, boundingRect().center().y());
        mPath.lineTo(boundingRect().right(), boundingRect().center().y());
    } else if (property("function").toString() == "computation"){
        mPath.moveTo(0, offset);
        mPath.lineTo(offset, offset);
        mPath.moveTo(0, geometry().height() - offset);
        mPath.lineTo(offset, geometry().height() - offset);

        mPath.moveTo(geometry().width() - offset, geometry().height() / 2);
        mPath.lineTo(geometry().width(), geometry().height() / 2);
    }
    return mPath;
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

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
#include "qsysmlnode.h"

#include <qdiagramstylesheet.h>

#include "qsysmlplugin.h"
#include "qsysmlcontrolconnectionpoint.h"

QSysMLNode::QSysMLNode(const QMap<QString, QVariant> &properties, QGraphicsItem *parent) :
    QAbstractDiagramShape(QSysMLPlugin::staticName(), QSysMLNode::staticItemClass(), properties, parent)
{
	initGeometry(400, 400);
    addProperty("nodeType", QDiagramToolkit::String, true, properties.value("nodeType"));
    addProperty("background", QDiagramToolkit::Brush, false, properties.value("background", QColor("white")));
    addProperty("font", QDiagramToolkit::Font, false, properties.value("font"));
    addProperty("lineStyle", QDiagramToolkit::Pen, false, properties.value("lineStyle"));
    addProperty("textColor", QDiagramToolkit::Color, false, properties.value("textColor", QColor("black")));
    if (properties.value("nodeType") == "decision"){
		initGeometry(200, 200);
		addConnectionPoint(new QSysMLControlConnectionPointIn(this, "controlIn", 0, 1));
		addConnectionPoint(new QSysMLControlConnectionPointOut(this, "controlOut1", QDiagramToolkit::West, 0, 1));
		addConnectionPoint(new QSysMLControlConnectionPointOut(this, "controlOut2", QDiagramToolkit::East, 1, 1));
	} else if (properties.value("nodeType") == "initial"){
		initGeometry(100, 150);
		addConnectionPoint(new QSysMLControlConnectionPointOut(this, "controlOut", 0, 1));
    } else if (properties.value("nodeType") == "final.activity"){
		initGeometry(100, 150);
		addConnectionPoint(new QSysMLControlConnectionPointIn(this, "controlIn", 0, 1));
    } else if (properties.value("nodeType") == "final.flow"){
		initGeometry(100, 150);
		addConnectionPoint(new QSysMLControlConnectionPointIn(this, "controlIn", 0, 1));
    } else if (properties.value("nodeType") == "fork"){
		initGeometry(500, 100);
		addProperty("orientation", QDiagramToolkit::Orientation, false, properties.value("orientation", Qt::Horizontal));
        addProperty("branches", QDiagramToolkit::Int, false, properties.value("branches", 2));

		addConnectionPoint(new QSysMLControlConnectionPointIn(this, "controlIn", 0, 1));
		for (int i = 0; i < properties.value("branches", 2).toInt(); i++){
			addConnectionPoint(new QSysMLControlConnectionPointOut(this, QString("controlOut%1").arg(i + 1), i, 1));
		}
        addSizeGripHandle(new QDiagramShapeSizeGripHandle(QDiagramShapeSizeGripHandle::Left, this));
        addSizeGripHandle(new QDiagramShapeSizeGripHandle(QDiagramShapeSizeGripHandle::Right, this));
    } else if (properties.value("nodeType") == "join"){
		initGeometry(500, 100);
		addProperty("orientation", QDiagramToolkit::Orientation, false, properties.value("orientation", Qt::Horizontal));
        addProperty("branches", QDiagramToolkit::Int, false, properties.value("branches", 2));

		addConnectionPoint(new QSysMLControlConnectionPointOut(this, "controlOut", 0, 1));
		for (int i = 0; i < properties.value("branches", 2).toInt(); i++){
			addConnectionPoint(new QSysMLControlConnectionPointIn(this, QString("controlIn%1").arg(i + 1), i, 1));
		}
        addSizeGripHandle(new QDiagramShapeSizeGripHandle(QDiagramShapeSizeGripHandle::Left, this));
        addSizeGripHandle(new QDiagramShapeSizeGripHandle(QDiagramShapeSizeGripHandle::Right, this));

		addConnectionPoint(new QSysMLControlConnectionPointOut(this, "controlOut", 0, 1));
    } else if (properties.value("nodeType") == "merge"){
		initGeometry(200, 200);
		addConnectionPoint(new QSysMLControlConnectionPointIn(this, "controlIn1", 0, 1));
		addConnectionPoint(new QSysMLControlConnectionPointIn(this, "controlIn2", QDiagramToolkit::East, 1, 1));
		addConnectionPoint(new QSysMLControlConnectionPointIn(this, "controlIn3", QDiagramToolkit::West, 2, 1));
		addConnectionPoint(new QSysMLControlConnectionPointOut(this, "controlOut", QDiagramToolkit::South, 3, 1));
    }
    restoreProperties(properties);
	updateConnectionPoints();
}

QRectF QSysMLNode::boundingRect() const
{
    QRectF r(geometry());
    r.moveTo(0, 0);
    return r;
}

QVariantMap QSysMLNode::defaultProperties(const QString & id)
{
	QVariantMap m;

	if (id == "node.decision"){
        m["nodeType"] = "decision";
		m["font"] = QDiagramProperty::toMap(QFont("Arial", 10));
		QPen pen(Qt::black);
		pen.setWidthF(5.0);
		pen.setStyle(Qt::SolidLine);
		m["lineStyle"] = QDiagramProperty::toMap(pen);
    } else if (id == "node.initial"){
		QBrush b(Qt::black);
		m["background"] = QDiagramProperty::toMap(b);
        m["nodeType"] = "initial";
    } else if (id == "node.final.activity"){
        m["nodeType"] = "final.activity";
    } else if (id == "node.final.flow"){
        m["nodeType"] = "final.flow";
    } else if (id == "node.fork.horizontal"){
        m["nodeType"] = "fork";
        m["alignment"] = Qt::Horizontal;
    } else if (id == "node.fork"){
        m["nodeType"] = "fork";
        m["alignment"] = Qt::Horizontal;
    } else if (id == "node.join"){
        m["nodeType"] = "join";
    } else if (id == "node.merge"){
        m["nodeType"] = "merge";
	}
	return m;
}

QPointF QSysMLNode::hotSpot(const QString & id)
{
	if (id == "node.initial"){
		return QPointF(-50, -150);
	} else if (id == "final.flow"){
		return QPointF(-50, 0);
	} else if (id == "node.decision"){
		return QPointF(-50, 0);
	}
	return QPointF(0, 0);
}

QIcon QSysMLNode::icon(const QVariantMap & properties)
{
	Q_UNUSED(properties);
	return QIcon();
}

QVariant QSysMLNode::itemPropertyChange(const QString & name, const QVariant & value)
{
	if (name == "branches"){
		int branches = value.toInt();
		if (branches < 2){
			branches = 2;
		}
		if (property("nodeType") == "fork"){
			if (branches < connectionPoints(QDiagramToolkit::South).size()){

			} else {
				while(connectionPoints(QDiagramToolkit::South).size() < branches){
					addConnectionPoint(new QSysMLControlConnectionPointOut(this, QString("controlOut%1").arg(connectionPoints(QDiagramToolkit::South).size() + 1), connectionPoints(QDiagramToolkit::South).size(), 1));
				}
			}
		} else if (property("nodeType") == "join"){
			if (branches < connectionPoints(QDiagramToolkit::North).size()){

			} else {
				while(connectionPoints(QDiagramToolkit::North).size() < branches){
					addConnectionPoint(new QSysMLControlConnectionPointIn(this, QString("controlIn%1").arg(connectionPoints(QDiagramToolkit::North).size() + 1), connectionPoints(QDiagramToolkit::North).size(), 1));
				}
			}
		}
        return branches;
    }
    return QAbstractDiagramGraphicsItem::itemPropertyChange(name, value);
}

QVariant QSysMLNode::itemPropertyHasChanged( const QString & name, const QVariant & value)
{
	if (name == "branches"){
	    updateConnectionPoints();
	}
    return QAbstractDiagramGraphicsItem::itemPropertyHasChanged(name, value);
}

QString QSysMLNode::nodeType() const
{
    return property("nodeType").toString();
}

void QSysMLNode::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->save();
	QPen pen(Qt::black);
	pen.setWidthF(1.0);
	painter->setPen(pen);
    if (nodeType() == "decision"){
		paintDecision(painter, option, widget);
    } else if (property("nodeType") == "initial"){
		paintInitial(painter, option, widget);
    } else if (property("nodeType") == "final.activity"){
		paintActivityFinal(painter, option, widget);
    } else if (property("nodeType") == "final.flow"){
		paintFlowFinal(painter, option, widget);
    } else if (property("nodeType") == "fork"){
		paintFork(painter, option, widget);
	} else if (property("nodeType") == "join"){
		paintJoin(painter, option, widget);
	} else if (property("nodeType") == "merge"){
		paintMerge(painter, option, widget);
    }
    painter->restore();

    paintFocus(painter, option, widget);
}

void QSysMLNode::paintDecision(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	painter->save();
	painter->setBrush(qdiagramproperty_cast<QBrush>(property("background")));
	painter->setPen(qdiagramproperty_cast<QPen>(property("lineStyle")));
	painter->drawPath(shapeInternal());
	painter->restore();

	painter->save();
	painter->setPen(diagram()->styleSheet()->lineStyle("ControlFlowLineStyle").pen());
	painter->drawPath(shapeConnectionLines());
	painter->restore();
}

void QSysMLNode::paintFork(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	painter->save();
	QBrush b(Qt::black);
	painter->setBrush(b);
	painter->drawRect(0, geometry().height() / 2 - 10, geometry().width(), 20);
	painter->setPen(diagram()->styleSheet()->lineStyle("ControlFlowLineStyle").pen());
	painter->drawLine(geometry().width() / 2, 0, geometry().width() / 2, geometry().height() / 2 - 20);
	Q_FOREACH(QAbstractDiagramShapeConnectionPoint* cp, connectionPoints()){
		if (cp->direction() == QAbstractDiagramShapeConnectionPoint::Out){
			painter->drawLine(cp->rect().center().x(), geometry().height() / 2, cp->rect().center().x(), boundingRect().height());
		}
	}
	painter->restore();
}

void QSysMLNode::paintActivityFinal(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	painter->save();

	painter->setBrush(qdiagramproperty_cast<QBrush>(property("background")));
	painter->drawPath(shapeInternal());
	painter->restore();
	
	painter->save();
	painter->setBrush(QBrush(Qt::black));
	painter->drawEllipse(QPointF(50, 100), 30, 30);
	painter->restore();

	painter->save();
	painter->setPen(diagram()->styleSheet()->lineStyle("ControlFlowLineStyle").pen());
	painter->drawPath(shapeConnectionLines());
	painter->restore();
}

void QSysMLNode::paintFlowFinal(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	painter->save();

	painter->setBrush(qdiagramproperty_cast<QBrush>(property("background")));
	painter->drawPath(shapeInternal());
	painter->restore();
	
	painter->save();
	QTransform t(painter->transform());
	t.translate(50, 100 - QLineF(0, 0, 100, 100).length() / 2);
	t.rotate(45);
	painter->setTransform(t);
	painter->drawLine(50, 0, 50, 100);
	painter->drawLine(0, 50, 100, 50);
	painter->restore();
	painter->save();
	painter->setPen(diagram()->styleSheet()->lineStyle("ControlFlowLineStyle").pen());
	painter->drawPath(shapeConnectionLines());
	painter->restore();
}

void QSysMLNode::paintInitial(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	
	painter->save();

	painter->setPen(diagram()->styleSheet()->lineStyle("ControlFlowLineStyle").pen());
	painter->drawPath(shapeConnectionLines());

	painter->restore();
	painter->save();

	painter->setBrush(qdiagramproperty_cast<QBrush>(property("background")));
	painter->drawPath(shapeInternal());

	painter->restore();
}

void QSysMLNode::paintJoin(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	painter->save();
	QBrush b(Qt::black);
	painter->setBrush(b);
	painter->drawRect(0, geometry().height() / 2 - 2, geometry().width(), 2);

	QPen pen(Qt::black);
	pen.setStyle(Qt::DashLine);
	pen.setWidthF(1.0);
	painter->setPen(pen);
	painter->drawLine(geometry().width() / 2, geometry().height() / 2 + 2, geometry().width() / 2, geometry().height());
	Q_FOREACH(QAbstractDiagramShapeConnectionPoint* cp, connectionPoints()){
		if (cp->direction() == QAbstractDiagramShapeConnectionPoint::In){
			painter->drawLine(cp->rect().center().x(), 0, cp->rect().center().x(), boundingRect().height() / 2);
		}
	}
	painter->restore();
}

void QSysMLNode::paintMerge(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	painter->save();
	painter->setBrush(qdiagramproperty_cast<QBrush>(property("background")));
	painter->setPen(qdiagramproperty_cast<QPen>(property("lineStyle")));
	painter->drawPath(shapeInternal());
	painter->restore();

	painter->save();
	painter->setPen(diagram()->styleSheet()->lineStyle("ControlFlowLineStyle").pen());
	painter->drawPath(shapeConnectionLines());
	painter->restore();
}

void QSysMLNode::paintTransition(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	painter->save();
	painter->setPen(qdiagramproperty_cast<QPen>(property("lineStyle")));
	painter->setBrush(qdiagramproperty_cast<QBrush>(property("background")));
	painter->drawPath(shapeInternal());
	painter->setPen(diagram()->styleSheet()->lineStyle("ControlFlowLineStyle").pen());
	painter->drawPath(shapeConnectionLines());

	QFont f = qdiagramproperty_cast<QFont>(property("font"));
	QFontInfo fi(f);
	QFontMetrics fm(f);
	f.setPixelSize(fi.pixelSize());
	painter->setFont(f);
	painter->setPen(qdiagramproperty_cast<QPen>(property("textColor")));
    QRect br = boundingRect().adjusted(-2, -2, -2, -2).toRect();
	if (property("transition") == "abort"){
		QPen pen(painter->pen());
		pen.setColor(Qt::white);
		painter->setPen(pen);
	    painter->drawText(0, 13, 52, 52, Qt::AlignCenter | Qt::TextWordWrap,"A", &br);
	} else if (property("transition") == "common"){
		QPen pen(painter->pen());
		pen.setColor(Qt::white);
		painter->setPen(pen);
	    painter->drawText(0, 13, 78, 52, Qt::AlignCenter | Qt::TextWordWrap,"T", &br);
	} else if (property("transition") == "FOCL"){
	    painter->drawText(0, 13, 52, 52, Qt::AlignCenter | Qt::TextWordWrap,"T", &br);
	} else if (property("transition") == "FONCL"){
		QPen pen(painter->pen());
		pen.setColor(Qt::white);
		painter->setPen(pen);
	    painter->drawText(0, 13, 52, 52, Qt::AlignCenter | Qt::TextWordWrap,"T", &br);
	} else if (property("transition") == "FWD"){
		QPen pen(painter->pen());
		pen.setColor(Qt::white);
		painter->setPen(pen);
	    painter->drawText(13, 13, 52, 52, Qt::AlignCenter | Qt::TextWordWrap,"T", &br);
	}
	painter->restore();
}

QPainterPath QSysMLNode::shapeConnectionLines() const
{
	QPainterPath p;
	if (property("nodeType") == "decision"){
		p.moveTo(geometry().width() / 2, 0);
		p.lineTo(geometry().width() / 2, 50);
		p.moveTo(0, geometry().height() / 2);
		p.lineTo(50, geometry().height() / 2);
		p.moveTo(geometry().width() - 50, geometry().height() / 2);
		p.lineTo(geometry().width(), geometry().height() / 2);
    } else if (property("nodeType") == "final.activity"){
		p.moveTo(50, 0);
		p.lineTo(50, 50);
    } else if (property("nodeType") == "final.flow"){
		p.moveTo(50, 0);
		p.lineTo(50, 50);
	} else if (property("nodeType") == "initial"){
		p.moveTo(50, 50);
		p.lineTo(50, 150);
	} else if (property("nodeType") == "merge"){
		p.moveTo(boundingRect().width() - 50, geometry().height() / 2);
		p.lineTo(boundingRect().width(), geometry().height() / 2);
		p.moveTo(0, geometry().height() / 2);
		p.lineTo(50, geometry().height() / 2);
		p.moveTo(geometry().width() / 2, geometry().height() - 50);
		p.lineTo(geometry().width() / 2, geometry().height());
	}
	p.closeSubpath();
	return p;
}

QPainterPath QSysMLNode::shapeInternal() const
{
    QPainterPath p;
	if (nodeType() == "decision"){
		p.moveTo(50, geometry().height() / 2);
		p.lineTo(geometry().width() / 2, 50);
		p.lineTo(geometry().width() - 50, geometry().height() / 2);
		p.lineTo(geometry().width() / 2, geometry().height() - 50);
		p.closeSubpath();
    } else if (property("nodeType") == "final.activity"){
		p.addEllipse(0, 50, 100, 100);
    } else if (property("nodeType") == "final.flow"){
		p.addEllipse(0, 50, 100, 100);
	} else if (nodeType() == "fork"){
		p.addRect(0, geometry().height() / 2 - 10, geometry().width(), 20);
    } else if (property("nodeType") == "initial"){
		p.addEllipse(0, 0, 100, 100);
		p.closeSubpath();
	} else if (nodeType() == "merge"){
		p.moveTo(50, geometry().height() / 2);
		p.lineTo(geometry().width() / 2, 50);
		p.lineTo(geometry().width() - 50, geometry().height() / 2);
		p.lineTo(geometry().width() / 2, geometry().height() - 50);
		p.closeSubpath();
	}
	return p;
}

QPainterPath QSysMLNode::shape() const
{
    QPainterPath p;
	p.connectPath(shapeInternal());
	p.connectPath(shapeConnectionLines());
    return p;
}

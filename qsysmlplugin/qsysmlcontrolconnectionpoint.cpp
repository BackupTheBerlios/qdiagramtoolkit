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
#include "qsysmlcontrolconnectionpoint.h"

#include <qabstractdiagramshape.h>

#include "qsysmlaction.h"

#define BASE_SIZE 50.0
#define GATE_CP_SIZE 40.0
#define GATE_CP_HSIZE 20.0

QSysMLControlConnectionPointIn::QSysMLControlConnectionPointIn(QAbstractDiagramShape* shape, const QString & id, int index, int maxConnections) :
    QAbstractDiagramShapeConnectionPoint(shape, id, QDiagramToolkit::North, maxConnections)
{
    m_index = index;
    setDirection(QAbstractDiagramShapeConnectionPoint::In);
    updatePosition();
}

QSysMLControlConnectionPointIn::QSysMLControlConnectionPointIn(QAbstractDiagramShape* shape, const QString & id, QDiagramToolkit::ConnectionPointOrientation orientation, int index, int maxConnections) :
    QAbstractDiagramShapeConnectionPoint(shape, id, orientation, maxConnections)
{
    m_index = index;
    setDirection(QAbstractDiagramShapeConnectionPoint::In);
    updatePosition();
}

QSysMLControlConnectionPointIn::~QSysMLControlConnectionPointIn()
{
}

void QSysMLControlConnectionPointIn::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->save();
	painter->setPen(Qt::NoPen);
	painter->setBrush(QColor("firebrick"));
    painter->drawRect(rect());
    painter->restore();
}

void QSysMLControlConnectionPointIn::updatePosition()
{
	if (parentShape()->metaData()->itemClass() == QSysMLAction::staticItemClass()){
		setRect(QRectF(parentShape()->geometry().width() / 2 - GATE_CP_HSIZE, -GATE_CP_HSIZE, GATE_CP_SIZE, GATE_CP_SIZE));
	} else if (parentShape()->property("nodeType") == "fork"){
		setRect(QRectF(parentShape()->geometry().width() / 2 - GATE_CP_SIZE / 2, 0, GATE_CP_SIZE, GATE_CP_SIZE));
	} else if (parentShape()->property("nodeType") == "decision"){
		if (parentShape()->property("transition") == "FOCL" || parentShape()->property("transition") == "FOCL"){
			setRect(QRectF(26 - GATE_CP_SIZE / 2, 0, GATE_CP_SIZE, GATE_CP_SIZE));
		} else if (parentShape()->property("transition") == "FWD"){
			setRect(QRectF(39 - GATE_CP_SIZE / 2, 0, GATE_CP_SIZE, GATE_CP_SIZE));
		}
	} else if (parentShape()->property("nodeType") == "merge"){
		if (orientation() == QDiagramToolkit::East){
			setRect(QRectF(parentShape()->boundingRect().width() - GATE_CP_SIZE, parentShape()->boundingRect().center().y() - GATE_CP_HSIZE, GATE_CP_SIZE, GATE_CP_SIZE));
		} else if (orientation() == QDiagramToolkit::North){
			setRect(QRectF(parentShape()->boundingRect().center().x() - GATE_CP_SIZE / 2, 0, GATE_CP_SIZE, GATE_CP_SIZE));
		} else if (orientation() == QDiagramToolkit::West){
			setRect(QRectF(0, parentShape()->boundingRect().center().y() - GATE_CP_SIZE / 2, GATE_CP_SIZE, GATE_CP_SIZE));
		}
	} else {
		if (parentShape()->hasProperty("branches")){
			qreal w = parentShape()->geometry().width();
			w -= BASE_SIZE * 2;
			w /= (parentShape()->property("branches").toDouble() - 1);
			
			setRect(QRectF(BASE_SIZE + m_index * w - GATE_CP_SIZE / 2, 0, GATE_CP_SIZE, GATE_CP_SIZE));
		} else {
			setRect(QRectF(parentShape()->geometry().width() / 2 - GATE_CP_SIZE / 2, 0, GATE_CP_SIZE, GATE_CP_SIZE));
		}
	}
}

QSysMLControlConnectionPointOut::QSysMLControlConnectionPointOut(QAbstractDiagramShape* shape, const QString & id, int index, int maxConnections) :
    QAbstractDiagramShapeConnectionPoint(shape, id, QDiagramToolkit::South, maxConnections)
{
    m_index = index;
    setDirection(QAbstractDiagramShapeConnectionPoint::Out);
    updatePosition();
}

QSysMLControlConnectionPointOut::QSysMLControlConnectionPointOut(QAbstractDiagramShape* shape, const QString & id, QDiagramToolkit::ConnectionPointOrientation orientation, int index, int maxConnections) :
    QAbstractDiagramShapeConnectionPoint(shape, id, orientation, maxConnections)
{
    m_index = index;
    setDirection(QAbstractDiagramShapeConnectionPoint::Out);
    updatePosition();
}

QSysMLControlConnectionPointOut::~QSysMLControlConnectionPointOut()
{
}

QVariantMap QSysMLControlConnectionPointOut::customConnectorProperties() const
{
	QVariantMap p;
	if (parentShape()->property("nodeType") == "decision"){
		if (orientation() == QDiagramToolkit::East){
			p["text"] = "CONFIRMED";
		} else if (orientation() == QDiagramToolkit::West){
			p["text"] = "NOT OK";
		} else if (orientation() == QDiagramToolkit::South){
			p["text"] = "OK";
		}
	}
	return p;
}

void QSysMLControlConnectionPointOut::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->save();
	painter->setPen(Qt::NoPen);
	painter->setBrush(QColor("firebrick"));
    painter->drawRect(rect());
    painter->restore();
}

void QSysMLControlConnectionPointOut::updatePosition()
{
	if (parentShape()->metaData()->itemClass() == QSysMLAction::staticItemClass()){
		setRect(QRectF(parentShape()->geometry().width() / 2 - GATE_CP_HSIZE, parentShape()->geometry().height() - 20, GATE_CP_SIZE, GATE_CP_SIZE));
	} else if (parentShape()->property("nodeType") == "decision"){
		if (orientation() == QDiagramToolkit::East){
			setRect(QRectF(parentShape()->geometry().width() - GATE_CP_SIZE / 2, parentShape()->geometry().height() / 2 - GATE_CP_SIZE / 2, GATE_CP_SIZE, GATE_CP_SIZE));
		} else if (orientation() == QDiagramToolkit::West){
			setRect(QRectF(0, parentShape()->geometry().height() / 2 - GATE_CP_SIZE / 2, GATE_CP_SIZE, GATE_CP_SIZE));
		} else if (orientation() == QDiagramToolkit::South){
			if (parentShape()->property("transition") == "FOCL" || parentShape()->property("transition") == "FOCL"){
				setRect(QRectF(26 - GATE_CP_SIZE / 2, parentShape()->geometry().height() - GATE_CP_SIZE, GATE_CP_SIZE, GATE_CP_SIZE));
			} else if (parentShape()->property("transition") == "FWD"){
				setRect(QRectF(39 - GATE_CP_SIZE / 2, parentShape()->geometry().height() - GATE_CP_SIZE, GATE_CP_SIZE, GATE_CP_SIZE));
			}
		}
	} else if (parentShape()->property("nodeType") == "join"){
		setRect(QRectF(parentShape()->geometry().width() / 2 - GATE_CP_SIZE / 2, parentShape()->geometry().height() - GATE_CP_SIZE, GATE_CP_SIZE, GATE_CP_SIZE));
	} else {
		if (parentShape()->hasProperty("branches")){
			qreal w = parentShape()->geometry().width();
			w -= BASE_SIZE * 2;
			w /= (parentShape()->property("branches").toDouble() - 1);
			
			setRect(QRectF(BASE_SIZE + m_index * w - GATE_CP_SIZE / 2, parentShape()->geometry().height() - GATE_CP_SIZE, GATE_CP_SIZE, GATE_CP_SIZE));
		} else {
			setRect(QRectF(parentShape()->geometry().width() / 2 - GATE_CP_SIZE / 2, parentShape()->geometry().height() - GATE_CP_SIZE, GATE_CP_SIZE, GATE_CP_SIZE));
		}
	}
}

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
#include "qsysmldataconnectionpoint.h"

#include <qabstractdiagramshape.h>

#define BASE_SIZE 50.0
#define CP_SIZE 40.0

QSysMLDataConnectionPointIn::QSysMLDataConnectionPointIn(QAbstractDiagramShape* shape, const QString & id, int index, int maxConnections) :
    QAbstractDiagramShapeConnectionPoint(shape, id, QDiagramToolkit::North, maxConnections)
{
    m_index = index;
    setDirection(QAbstractDiagramShapeConnectionPoint::In);
    updatePosition();
}

QSysMLDataConnectionPointIn::~QSysMLDataConnectionPointIn()
{
}

void QSysMLDataConnectionPointIn::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->save();
	painter->setBrush(Qt::green);
    painter->drawRect(rect());
    painter->restore();
}

void QSysMLDataConnectionPointIn::updatePosition()
{
    //if (parentShape()->property("inputs").toInt() == 1){
    //    setRect(QRectF(0, parentShape()->boundingRect().center().y() - GATE_CP_SIZE / 2, GATE_CP_SIZE, GATE_CP_SIZE));
    //} else {
//        setRect(QRectF(0, BASE_SIZE + (m_index * BASE_SIZE * 2) - GATE_CP_SIZE / 2, GATE_CP_SIZE, GATE_CP_SIZE));
    //}
	setRect(QRectF(0, parentShape()->geometry().height() / 2 - CP_SIZE / 2, CP_SIZE, CP_SIZE));
}

QSysMLDataConnectionPointOut::QSysMLDataConnectionPointOut(QAbstractDiagramShape* shape, const QString & id, int index, int maxConnections) :
    QAbstractDiagramShapeConnectionPoint(shape, id, QDiagramToolkit::South, maxConnections)
{
    m_index = index;
    setDirection(QAbstractDiagramShapeConnectionPoint::In);
    updatePosition();
}

QSysMLDataConnectionPointOut::~QSysMLDataConnectionPointOut()
{
}

void QSysMLDataConnectionPointOut::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->save();
	painter->setBrush(Qt::green);
    painter->drawRect(rect());
    painter->restore();
}

void QSysMLDataConnectionPointOut::updatePosition()
{
	setRect(QRectF(parentShape()->geometry().width() - CP_SIZE, parentShape()->geometry().height() / 2 - CP_SIZE / 2, CP_SIZE, CP_SIZE));
}

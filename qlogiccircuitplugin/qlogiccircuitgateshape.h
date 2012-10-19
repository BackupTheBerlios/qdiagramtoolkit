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
#ifndef QLOGICGATESHAPE_H
#define QLOGICGATESHAPE_H

#include "qlogiccircuitplugin_global.h"

#include <qabstractdiagramshape.h>

class QAbstractDiagramShapeConnector;

class QLogicCircuitGateShape : public QAbstractDiagramShape
{
public:
    explicit QLogicCircuitGateShape(QGraphicsItem* parent = 0);

    explicit QLogicCircuitGateShape(const QMap<QString,QVariant> & properties, QGraphicsItem* parent = 0);

    QRectF boundingRect() const;

	QList<QAction*> createActions(QWidget* parent);

    QPoint hotSpot() const;

	QList<QAbstractDiagramShapeConnector*> inputs() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    /**
      * Returns the shape of this item as a QPainterPath in local coordinates.
      */
    QPainterPath shape() const;

	void triggerAction(const QString & name, const QVariant & data);
protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);
    QVariant itemPropertyChange( const QString & name, const QVariant & value);
    QVariant itemPropertyHasChanged( const QString & name, const QVariant & value);
    void paintState(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
private:
};

#endif // QLOGICGATESHAPE_H

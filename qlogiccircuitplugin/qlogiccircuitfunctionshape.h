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
#ifndef QLOGICCIRCUITFUNCTIONSHAPE_H
#define QLOGICCIRCUITFUNCTIONSHAPE_H

#include <qabstractdiagramshape.h>

#include "qlogiccircuitplugin_global.h"

class QLogicCircuitFunctionShape : public QAbstractDiagramShape
{
public:
    QLogicCircuitFunctionShape(QGraphicsItem* parent = 0);
    /**
      * Constructs a QLogicCircuitFunctionShape with the given @p properties and @p parent.
      */
    QLogicCircuitFunctionShape(const QMap<QString,QVariant> & properties, QGraphicsItem* parent = 0);
    /**
      *
      */
//    explicit QLogicCircuitFunctionShape(const QString & uuid, const QString & type, const QString & plugin, QGraphicsItem* parent = 0);

	~QLogicCircuitFunctionShape();

    QRectF boundingRect() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    /**
      * Returns the shape of this item as a QPainterPath in local coordinates.
      */
    QPainterPath shape() const;
};

#endif // QLOGICCIRCUITFUNCTIONSHAPE_H

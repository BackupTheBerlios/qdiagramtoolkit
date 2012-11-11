/******************************************************************************
** Copyright (C) 2011 - 2012 Martin Hoppe martin@2x2hoppe.de
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
#ifndef QABSTRACTDIAGRAMSHAPE_P_H
#define QABSTRACTDIAGRAMSHAPE_P_H

#include "qabstractdiagramshape.h"

class QAbstractDiagramShapeDummy : public QAbstractDiagramShape
{
	
public:
	QAbstractDiagramShapeDummy(QGraphicsItem* parent = 0) :
	  QAbstractDiagramShape(parent){
	  };
	~QAbstractDiagramShapeDummy()
	{
	};
	QRectF boundingRect() const
	{
	 return QRectF();
	};
	void paint(QPainter* p, const QStyleOptionGraphicsItem* i,QWidget * w)
	{
	};
};

Q_DECLARE_METATYPE(QAbstractDiagramShapeDummy*)

#endif // QABSTRACTDIAGRAMSHAPE_P_H
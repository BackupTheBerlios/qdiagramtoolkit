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
#ifndef QSYSMLFLOWCONNECTOR_H
#define QSYSMLFLOWCONNECTOR_H

#include <qabstractdiagramshapeconnector.h>

class QSysMLFlowConnector : public QAbstractDiagramShapeConnector
{
public:
	QSysMLFlowConnector(const QString &itemClass, const QVariantMap & properties);
	~QSysMLFlowConnector();

	QRectF focusRect() const;

	QLineF line() const;

	qreal radius() const;
protected:
	void drawPolyline(QPainter* painter, const QList<QPointF> & points);

	void paintArrow(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
	
	void paintFocus(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
};

#endif // QSYSMLFLOWCONNECTOR_H

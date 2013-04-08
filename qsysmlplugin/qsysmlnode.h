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
#ifndef QSYSMLNODE_H
#define QSYSMLNODE_H

#include <qabstractdiagramshape.h>

class QSysMLNode : public QAbstractDiagramShape
{
public:
	QDIAGRAM_DECLARE_SHAPE(QSysMLNode, Node);

	QSysMLNode(const QMap<QString,QVariant> & properties, QGraphicsItem* parent = 0);

    QRectF boundingRect() const;
	/**
	 * Returns the default properties the for shape specified by the given @p id;
	 */
	static QVariantMap defaultProperties(const QString & id);
	/**
	 * Returns the shape's hot spot.
	 */
	static QPointF hotSpot(const QString & id);

	static QIcon icon(const QVariantMap & properties = QVariantMap());

    QString nodeType() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	QPainterPath shapeConnectionLines() const;

	QPainterPath shapeInternal() const;
    /**
      * Returns the shape of this item as a QPainterPath in local coordinates.
      */
    QPainterPath shape() const;
protected:
	QVariant itemPropertyChange(const QString &name, const QVariant &value);
    QVariant itemPropertyHasChanged( const QString & name, const QVariant & value);
private:
	void paintDecision(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
	void paintFork(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
	void paintActivityFinal(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
	void paintFlowFinal(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
	void paintInitial(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
	void paintJoin(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
	void paintMerge(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
	void paintTransition(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
};

#endif // QSYSMLNODE_H

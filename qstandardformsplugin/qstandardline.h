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
#ifndef QSTANDARDLINE_H
#define QSTANDARDLINE_H

#include <qabstractdiagramgraphicsitem.h>

class QDiagramEndOfLineStyle;
class QDiagramLineItemPositionHandle;

//! The QStandardLine class
/**
  * @ingroup StandardForms
  */
class QStandardLine : public QAbstractDiagramGraphicsItem
{
public:
    QStandardLine(const QMap<QString,QVariant> & properties, QGraphicsItem* parent = 0);

    QRectF boundingRect() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void paintFocus(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    QPainterPath shape() const;   
protected:
    QPointF endPos() const;

    QVariant itemPropertyHasChanged(const QString &name, const QVariant &value);

    QVariant itemSelectedHasChanged( const QVariant & value );

    void paintEndOfLine(QPainter* painter);

    void paintStartOfLine(QPainter* painter);

    void paintPositionHandles(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    QPointF startPos() const;
private:
    QList<QDiagramLineItemPositionHandle*> m_handles;
    QPainterPath m_endOfLine;
    QPainterPath m_startOfLine;
};

#endif // QSTANDARDLINE_H

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
#ifndef QDIAGRAMGRAPHICSCONNECTORITEM_H
#define QDIAGRAMGRAPHICSCONNECTORITEM_H

#include "qdiagramlib_global.h"

#include <qabstractdiagramshapeconnector.h>

class QDiagramShape;

/**
  * Properties
  * arrows none,end,both
  * class default,straight,line,curved
  * arrowStyleEnd line,arrow
  * arrowStyleStart line,arrow
  */
class QDIAGRAMLIBSHARED_EXPORT QDiagramShapeConnector : public QAbstractDiagramShapeConnector
{
public:
    enum Arrows {
        NoArrows,
        Arrow
    };

    enum ConnectorType {
        DefaultConnector,
        LineConnector,
        StraightConnector,
        CurvedConnector
    };

//    explicit QDiagramShapeConnector(const QString & uuid);
//    explicit QDiagramShapeConnector(const QString & style, QAbstractDiagramShapeConnectionPoint* point);
//    explicit QDiagramShapeConnector(const QString & uuid, const QString & style, const QString & endConnectionPoint, const QString & startConnectionPoint);
//    explicit QDiagramShapeConnector(const QString & uuid, const QString & style, QAbstractDiagramShapeConnectionPoint* endConnectionPoint, QAbstractDiagramShapeConnectionPoint* startConnectionPoint);
    ~QDiagramShapeConnector();

    QRectF boundingRect() const;

    QPainterPath shape() const;

    void rebuildConnection();

    void temporaryPosition(const QPointF & pos, QDiagramToolkit::ConnectionPointOrientation orientation = QDiagramToolkit::ConnectionPointOrientationInvalid, const QRectF & rect = QRectF());

    void updatePosition();
protected:
    QList<QPointF> breakPoints() const;

    void paintBreakPoints(QPainter * painter, const QList<QPointF> & points);

    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
private:
    void calcBoundingRect();

    QList<QPointF> defaultConnector() const;

    QList<QPointF> lineConnector() const;

    QList<QPointF> straightConnector() const;

    void paintArrows( const QLineF & line, QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

    QPainterPath shapeDefaultConnector() const;
    QPainterPath shapeLineConnector() const;
    QPainterPath shapeStraightConnector() const;

    QPolygonF m_arrowFrom;
    QPolygonF m_arrowTo;
    QList<QPointF> m_breakPoints;
    QRectF m_boundingRect;
    QLineF m_line;
    QPointF m_tempEndPos;
    QDiagramToolkit::ConnectionPointOrientation m_tempOrientationAtEnd;
    QRectF m_tempToRect;
};

#endif // QDIAGRAMGRAPHICSCONNECTORITEM_H

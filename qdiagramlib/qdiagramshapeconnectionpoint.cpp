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
#include "stdafx.h"
#include "qdiagramshapeconnectionpoint.h"

#include "qabstractdiagramshape.h"

QDiagramShapeConnectionPoint::QDiagramShapeConnectionPoint(QAbstractDiagramShape* shape, QDiagramShapeConnectionPoint::Position position, QDiagramToolkit::ConnectionPointOrientation orientation) :
    QAbstractDiagramShapeConnectionPoint(shape, toString(position), orientation)
{
    m_pos = position;
    m_size = QSizeF(6, 6);
    updatePosition();
}

QDiagramShapeConnectionPoint::~QDiagramShapeConnectionPoint()
{

}


QPointF QDiagramShapeConnectionPoint::calcConnectionPointPosition(qreal x, qreal y)
{
    QLineF mLine(parentShape()->boundingRect().center(), QPointF(x, y));
    QPolygonF mPolygon;
    QPointF mIntersect;
    QLineF mPolyLine;

    mPolygon = parentItem()->shape().toFillPolygon();
    if (mPolygon.isEmpty()){
        return QPointF();
    }
    mPolyLine.setP1(mPolygon.at(0));

    for (int iPolygon = 1; iPolygon < mPolygon.size(); iPolygon++){
        mPolyLine.setP2(mPolygon.at(iPolygon));
        if (mPolyLine.intersect(mLine, &mIntersect) == QLineF::BoundedIntersection){
            break;
        }
        mPolyLine.setP1(mPolyLine.p2());
    }
    return mIntersect;
}

QString QDiagramShapeConnectionPoint::toString(QDiagramShapeConnectionPoint::Position pos) const
{
    if (pos == North){
        return "N";
    } else if (pos == NorthNorthEast){
        return "NNE";
    } else if (pos == NorthEast){
        return "NE";
    } else if (pos == East){
        return "E";
    } else if (pos == EastNorthEast){
        return "ENE";
    } else if (pos == EastSouthEast){
        return "ESE";
    } else if (pos == SouthEast){
        return "SE";
    } else if (pos == SouthSouthEast){
        return "SSE";
    } else if (pos == South){
        return "S";
    } else if (pos == SouthSouthWest){
        return "SSW";
    } else if (pos == SouthWest){
        return "SW";
    } else if (pos == WestSouthWest){
        return "WSW";
    } else if (pos == West){
        return "W";
    } else if (pos == WestNorthWest){
        return "WNW";
    } else if (pos == NorthWest){
        return "NW";
    } else if (pos == NorthNorthWest){
        return "NNW";
    }
    return "N/A";
}

void QDiagramShapeConnectionPoint::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->save();
    painter->drawRect(rect());
    painter->restore();
}

void QDiagramShapeConnectionPoint::updatePosition()
{
    if (m_pos == North){
        setRect(QRectF(calcConnectionPointPosition(parentShape()->boundingRect().center().x(), parentShape()->boundingRect().top()), m_size).translated(-m_size.width() / 2, 0));
    }
    if (m_pos == NorthEast){
        setRect(QRectF(calcConnectionPointPosition(parentShape()->boundingRect().right(), parentShape()->boundingRect().top()), m_size).translated(-m_size.width() / 2, -m_size.height() /2));
    }
    if (m_pos == East){
        setRect(QRectF(calcConnectionPointPosition(parentShape()->boundingRect().right(), parentShape()->boundingRect().center().y()), m_size).translated(-m_size.width(), -m_size.height() / 2));
    }
    if (m_pos == SouthEast){
        setRect(QRectF(calcConnectionPointPosition(parentShape()->boundingRect().right(), parentShape()->boundingRect().bottom()), m_size).translated(-m_size.width() / 2, -m_size.height() /2));
    }
    if (m_pos == South){
        setRect(QRectF(calcConnectionPointPosition(parentShape()->boundingRect().center().x(), parentShape()->boundingRect().bottom()), m_size).translated(-m_size.width() / 2, -m_size.height()));
    }
    if (m_pos == SouthWest){
        setRect(QRectF(calcConnectionPointPosition(parentShape()->boundingRect().left(), parentShape()->boundingRect().bottom()), m_size).translated(-m_size.width() / 2, -m_size.height() / 2));
    }
    if (m_pos == West){
        setRect(QRectF(calcConnectionPointPosition(parentShape()->boundingRect().left(), parentShape()->boundingRect().center().y()), m_size).translated(0, -m_size.height() / 2));
    }
    if (m_pos == NorthWest){
        setRect(QRectF(calcConnectionPointPosition(parentShape()->boundingRect().left(), parentShape()->boundingRect().top()), m_size).translated(-m_size.width() / 2, -m_size.height() / 2));
    }
}

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
#include "qdiagramshape.h"
#include "qdiagramgraphicsitem_p.h"

#include "qdiagramshapeconnector.h"
#include "qdiagramgraphicsscene.h"
#include "qdiagramundocommand.h"
#include "qdiagramshapeconnectionpoint.h"

//QDiagramGraphicsTextItem::QDiagramGraphicsTextItem(QGraphicsItem* parent) :
//    QGraphicsTextItem(parent)
//{
//}
//
//void QDiagramGraphicsTextItem::setAlignment(Qt::Alignment alignment)
//{
//    QTextBlockFormat mFormat;
//    mFormat.setAlignment(alignment);
//    QTextCursor cursor = textCursor();
//    cursor.select(QTextCursor::Document);
//    cursor.mergeBlockFormat(mFormat);
//    cursor.clearSelection();
//    setTextCursor(cursor);
//}

QDiagramShape::QDiagramShape(QGraphicsItem* parent) :
    QAbstractDiagramShape(parent)
{
    setAcceptHoverEvents(true);
    setBoundingRect(0,0,100,50);

    initConnectionPoint(QDiagramShapeConnectionPoint::North, QAbstractDiagramShapeConnectionPoint::North);
    initConnectionPoint(QDiagramShapeConnectionPoint::NorthEast, QAbstractDiagramShapeConnectionPoint::North);
    initConnectionPoint(QDiagramShapeConnectionPoint::East, QAbstractDiagramShapeConnectionPoint::East);
    initConnectionPoint(QDiagramShapeConnectionPoint::SouthEast, QAbstractDiagramShapeConnectionPoint::South);
    initConnectionPoint(QDiagramShapeConnectionPoint::South, QAbstractDiagramShapeConnectionPoint::South);
    initConnectionPoint(QDiagramShapeConnectionPoint::SouthWest, QAbstractDiagramShapeConnectionPoint::South);
    initConnectionPoint(QDiagramShapeConnectionPoint::West, QAbstractDiagramShapeConnectionPoint::West);
    initConnectionPoint(QDiagramShapeConnectionPoint::NorthWest, QAbstractDiagramShapeConnectionPoint::North);
}

//QDiagramShape::QDiagramShape(const QString & uuid, const QString & shape, QGraphicsItem* parent) :
//    QAbstractDiagramShape(uuid, shape, "<standard>", parent)
//{
//    setAcceptHoverEvents(true);
//    setBoundingRect(0,0,100,50);

//    initConnectionPoint(QDiagramShapeConnectionPoint::North, QAbstractDiagramShapeConnectionPoint::North);
//    initConnectionPoint(QDiagramShapeConnectionPoint::NorthEast, QAbstractDiagramShapeConnectionPoint::North);
//    initConnectionPoint(QDiagramShapeConnectionPoint::East, QAbstractDiagramShapeConnectionPoint::East);
//    initConnectionPoint(QDiagramShapeConnectionPoint::SouthEast, QAbstractDiagramShapeConnectionPoint::South);
//    initConnectionPoint(QDiagramShapeConnectionPoint::South, QAbstractDiagramShapeConnectionPoint::South);
//    initConnectionPoint(QDiagramShapeConnectionPoint::SouthWest, QAbstractDiagramShapeConnectionPoint::South);
//    initConnectionPoint(QDiagramShapeConnectionPoint::West, QAbstractDiagramShapeConnectionPoint::West);
//    initConnectionPoint(QDiagramShapeConnectionPoint::NorthWest, QAbstractDiagramShapeConnectionPoint::North);
//}

QDiagramShape::~QDiagramShape()
{
}

void QDiagramShape::initConnectionPoint(QDiagramShapeConnectionPoint::Position position, QAbstractDiagramShapeConnectionPoint::Orientation orientation)
{
    // TODO
//    Q_FOREACH(QAbstractDiagramShapeConnectionPoint* mPoint, connectionPoints()){
//        if (mPoint->index() == position){
//            return;
//        }
//    }
    addConnectionPoint(new QDiagramShapeConnectionPoint(this, position, orientation));
}

QRectF QDiagramShape::boundingRect() const
{
    return m_boundingRect;
}

void QDiagramShape::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
//    setConnectionPointsVisible(true);
}

void QDiagramShape::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
//    setConnectionPointsVisible(false);
}

void QDiagramShape::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
//    cConnectionsPointsVisible = true;
//    update();
//    Q_FOREACH(QRectF mRect, cTiePointRects){
//        if (mRect.contains(mapFromScene(event->scenePos()))){
////            qDebug() << mRect;
//        }
//    }
}

void QDiagramShape::paintTiePoints(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (!connectionPointsAreVisible()){
        return;
    }
    Q_FOREACH(QAbstractDiagramShapeConnectionPoint* mPoint, connectionPoints()){
        mPoint->paint(painter, option, widget);
    }
}

void QDiagramShape::setBoundingRect( qreal x, qreal y, qreal width, qreal height )
{
    setBoundingRect((QRectF(x, y, width, height)));
}

void QDiagramShape::setBoundingRect( const QRectF & rect )
{
    prepareGeometryChange();
    m_boundingRect = rect;
}

QPainterPath QDiagramShape::shape() const
{
    return QAbstractDiagramShape::shape();
}

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
#include "qabstractdiagramshapeconnector.h"

#include "qabstractdiagram.h"
#include "qabstractdiagramshape.h"
#include "qabstractdiagramshapeconnectionpoint.h"

QAbstractDiagramShapeConnector::QAbstractDiagramShapeConnector(const QVariantMap & properties) :
    QAbstractDiagramGraphicsItem(properties.value("uuid").toString(), "connector")
{
    addProperty("plugin", QDiagramGraphicsItemMetaProperty::String, true, properties.value("plugin").toString());
    addProperty("style", QDiagramGraphicsItemMetaProperty::String, true, properties.value("style"));
    addProperty("shapeAtStart", QDiagramGraphicsItemMetaProperty::UUID, false);
    addProperty("pointAtStart", QDiagramGraphicsItemMetaProperty::Int, false);
    addProperty("shapeAtEnd", QDiagramGraphicsItemMetaProperty::UUID, false);
    addProperty("pointAtEnd", QDiagramGraphicsItemMetaProperty::Int, false);
    m_connectionPointAtStart = 0;
    c_connectionPointAtEnd = 0;

	restoreProperties(properties);
    setFlag(QGraphicsItem::ItemIsSelectable);
}

//QAbstractDiagramShapeConnector::QAbstractDiagramShapeConnector(const QString & uuid, const QString & style) :
//    QAbstractDiagramGraphicsItem(uuid, "connector")
//{
//    addProperty("style", QDiagramGraphicsItemMetaProperty::String, true, style);
//    addProperty("shapeAtStart", QDiagramGraphicsItemMetaProperty::UUID, false);
//    addProperty("pointAtStart", QDiagramGraphicsItemMetaProperty::Int, false);
//    addProperty("shapeAtEnd", QDiagramGraphicsItemMetaProperty::UUID, false);
//    addProperty("pointAtEnd", QDiagramGraphicsItemMetaProperty::Int, false);
//    m_connectionPointAtStart = 0;
//    c_connectionPointAtEnd = 0;
//}

QAbstractDiagramShapeConnector::~QAbstractDiagramShapeConnector()
{
    disconnect();
}

bool QAbstractDiagramShapeConnector::canConnect(QAbstractDiagramShapeConnectionPoint* start, QAbstractDiagramShapeConnectionPoint* end) const
{
    return true;
}

void QAbstractDiagramShapeConnector::disconnect()
{
    if(m_connectionPointAtStart){
        m_connectionPointAtStart->remove(this);
        m_connectionPointAtStart = 0;
    }
    if(c_connectionPointAtEnd){
        c_connectionPointAtEnd->remove(this);
        c_connectionPointAtEnd = 0;
    }
}

void QAbstractDiagramShapeConnector::drawPolyline(QPainter * painter, const QList<QPointF> & points)
{
    if (points.size() < 2){
        return;
    }
    QLineF mLine;
    mLine.setP1(points.at(0));
    for(int iPoints = 1; iPoints < points.size(); iPoints++){
        mLine.setP2(points.at(iPoints));
        painter->drawLine(mLine);
        mLine.setP1(mLine.p2());
    }
}

QPointF QAbstractDiagramShapeConnector::endPos() const
{
    if (c_connectionPointAtEnd){
        return c_connectionPointAtEnd->scenePos() + c_connectionPointAtEnd->boundingRect().center();
    }
    return m_tempEndPos;
}

QAbstractDiagramShapeConnectionPoint* QAbstractDiagramShapeConnector::endConnectionPoint() const
{
    return m_connectionPointAtStart;
}

bool QAbstractDiagramShapeConnector::isTemporary() const
{
    return m_connectionPointAtStart == 0 && c_connectionPointAtEnd == 0;
}

QPointF QAbstractDiagramShapeConnector::intersectPoint(QGraphicsItem* item, const QLineF & line) const
{
    return intersectPoint(item->scenePos() + item->boundingRect().center(), item->shape().toFillPolygon(), line);
}

QPointF QAbstractDiagramShapeConnector::intersectPoint(const QPointF & pos, const QPolygonF & shape, const QLineF & line) const
{
    QLineF mPolyLine;
    QPointF mIntersectPoint;

    if (!shape.isEmpty()){
        mPolyLine.setP1(shape.at(0) + pos);
        for (int iPolygon = 1; iPolygon < shape.size(); iPolygon++){
            mPolyLine.setP2(shape.at(iPolygon) + pos);
            if (mPolyLine.intersect(line, &mIntersectPoint) == QLineF::BoundedIntersection){
                return mIntersectPoint;
            }
            mPolyLine.setP1(mPolyLine.p2());
        }
    }
    return pos;
}

QVariant QAbstractDiagramShapeConnector::itemSceneHasChanged(const QVariant & value)
{
	reconnect();
	return value;
}

QAbstractDiagramShapeConnectionPoint::Orientation QAbstractDiagramShapeConnector::orientationAtEnd() const
{
    if (c_connectionPointAtEnd){
        return c_connectionPointAtEnd->orientation();
    }
    return m_tempOrientationAtEnd;
}

QAbstractDiagramShapeConnectionPoint::Orientation QAbstractDiagramShapeConnector::orientationAtStart() const
{
    if (m_connectionPointAtStart){
        return m_connectionPointAtStart->orientation();
    }
    return m_tempOrientationAtStart;
}

void QAbstractDiagramShapeConnector::reconnect()
{
    QAbstractDiagramShape* shape = 0;
	if (diagram() == 0){
		return;
	}
    shape = diagram()->shape(property("shapeAtStart").toString());
    if (shape){
        m_connectionPointAtStart = shape->connectionPoint(property("pointAtStart").toString());
    }
    if (m_connectionPointAtStart){
        m_connectionPointAtStart->append(this, QAbstractDiagramShapeConnectionPoint::Out);
        setProperty("shapeAtStart", m_connectionPointAtStart->parentShape()->uuid());
        setProperty("pointAtStart", m_connectionPointAtStart->id());
    }
    shape = diagram()->shape(property("shapeAtEnd").toString());
    if (shape){
        c_connectionPointAtEnd = shape->connectionPoint(property("pointAtEnd").toString());
    }
    if (c_connectionPointAtEnd){
        c_connectionPointAtEnd->append(this, QAbstractDiagramShapeConnectionPoint::In);
        setProperty("shapeAtEnd", c_connectionPointAtEnd->parentShape()->uuid());
        setProperty("pointAtEnd", c_connectionPointAtEnd->id());
    }
    updatePosition();
}

void QAbstractDiagramShapeConnector::remove(QAbstractDiagramShapeConnectionPoint* point)
{
    if (m_connectionPointAtStart == point){
        m_connectionPointAtStart->remove(this);
        m_connectionPointAtStart = 0;
    }
    if (c_connectionPointAtEnd == point){
        c_connectionPointAtEnd->remove(this);
        c_connectionPointAtEnd = 0;
    }
}

void QAbstractDiagramShapeConnector::restoreFromProperties(const QMap<QString,QVariant> & properties)
{
    QAbstractDiagramShape* mShape = 0;
    mShape = diagram()->shape(properties.value("shapeAtStart").toString());
    if (mShape){
        m_connectionPointAtStart = mShape->connectionPoint(properties.value("pointAtStart").toString());
    }
    if (m_connectionPointAtStart){
        m_connectionPointAtStart->append(this, QAbstractDiagramShapeConnectionPoint::Out);
        setProperty("shapeAtStart", m_connectionPointAtStart->parentShape()->uuid());
        setProperty("pointAtStart", m_connectionPointAtStart->id());
    }
    mShape = diagram()->shape(properties.value("shapeAtEnd").toString());
    if (mShape){
        c_connectionPointAtEnd = mShape->connectionPoint(properties.value("pointAtEnd").toString());
    }
    if (c_connectionPointAtEnd){
        c_connectionPointAtEnd->append(this, QAbstractDiagramShapeConnectionPoint::In);
        setProperty("shapeAtEnd", c_connectionPointAtEnd->parentShape()->uuid());
        setProperty("pointAtEnd", c_connectionPointAtEnd->id());
    }
    updatePosition();
}

void QAbstractDiagramShapeConnector::setFrom(QAbstractDiagramShapeConnectionPoint* item)
{
    m_connectionPointAtStart = item;
    if (m_connectionPointAtStart){
        setProperty("shapeAtStart", m_connectionPointAtStart->parentShape()->uuid());
        setProperty("pointAtStart", m_connectionPointAtStart->id());
    }
}

void QAbstractDiagramShapeConnector::setTo(QAbstractDiagramShapeConnectionPoint* item)
{
    c_connectionPointAtEnd = item;
    if (c_connectionPointAtEnd){
        setProperty("shapeAtEnd", c_connectionPointAtEnd->parentShape()->uuid());
        setProperty("pointAtEnd", c_connectionPointAtEnd->id());
    }
}

void QAbstractDiagramShapeConnector::setTemporaryStart(const QPointF & position, QAbstractDiagramShapeConnectionPoint::Orientation orientation)
{
    m_tempStartPos = position;
    m_tempOrientationAtStart = orientation;
    updatePosition();
}

void QAbstractDiagramShapeConnector::setTemporaryEnd(const QPointF & position, QAbstractDiagramShapeConnectionPoint::Orientation orientation)
{
    m_tempEndPos = position;
    m_tempOrientationAtEnd = orientation;
    updatePosition();
}

QPointF QAbstractDiagramShapeConnector::startPos() const
{
    if (m_connectionPointAtStart){
        return m_connectionPointAtStart->scenePos() + m_connectionPointAtStart->boundingRect().center();
    }
    return m_tempStartPos;
}

QAbstractDiagramShapeConnectionPoint* QAbstractDiagramShapeConnector::startConnectionPoint() const
{
    return c_connectionPointAtEnd;
}

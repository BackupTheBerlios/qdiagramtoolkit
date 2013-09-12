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

#include "qdiagramconnectionpoint.h"
#include "qdiagramgraphicstextitem.h"

QAbstractDiagramShapeConnector::QAbstractDiagramShapeConnector(const QString & plugin, const QString & itemClass, const QVariantMap & properties, QGraphicsItem* parent) :
    QAbstractDiagramGraphicsItem(properties.value("uuid").toString(), plugin, "Connector", itemClass, parent)
{
	addProperty("start", QDiagramToolkit::ConnectionPoint, false, properties.value("start"));
    addProperty("end", QDiagramToolkit::ConnectionPoint, false, properties.value("end"));
    addProperty("textColor", QDiagramToolkit::Color, false, properties.value("textColor", QColor("black")));
    addProperty("textFont", QDiagramToolkit::Font, false, properties.value("textFont"));
    addProperty("text", QDiagramToolkit::String, false, properties.value("text"));
	
	m_connectionPointAtStart = 0;
    m_connectionPointAtEnd = 0;

	m_tempConnectionPointAtStart = false;

	m_textItem = new QDiagramGraphicsTextItem(this);
	m_textItem->setPlainText(properties.value("text").toString());

	restoreProperties(properties);
    setFlag(QGraphicsItem::ItemIsSelectable);
}

QAbstractDiagramShapeConnector::~QAbstractDiagramShapeConnector()
{
    disconnect();
}

bool QAbstractDiagramShapeConnector::canConnect(QAbstractDiagramShapeConnectionPoint* start, QAbstractDiagramShapeConnectionPoint* end) const
{
    return true;
}

bool QAbstractDiagramShapeConnector::canStartWith(QAbstractDiagramShapeConnectionPoint* cp) const
{
	if (cp){
		return true;
	}
	return false;
}

void QAbstractDiagramShapeConnector::disconnect()
{
    if(m_connectionPointAtStart){
        m_connectionPointAtStart->remove(this);
        m_connectionPointAtStart = 0;
    }
    if(m_connectionPointAtEnd){
        m_connectionPointAtEnd->remove(this);
        m_connectionPointAtEnd = 0;
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
    if (m_connectionPointAtEnd){
        return m_connectionPointAtEnd->scenePos() + m_connectionPointAtEnd->boundingRect().center();
    }
    return m_tempEndPos;
}

QAbstractDiagramShapeConnectionPoint* QAbstractDiagramShapeConnector::connectionPointAtEnd() const
{
    return m_connectionPointAtEnd;
}

bool QAbstractDiagramShapeConnector::isTemporary() const
{
    return m_connectionPointAtStart == 0 && m_connectionPointAtEnd == 0;
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

QVariant QAbstractDiagramShapeConnector::itemPositionHasChanged(const QVariant & value)
{
	QVariant v = QAbstractDiagramGraphicsItem::itemPositionHasChanged(value);
	updateTextItemPosition();
	return v;
}

QVariant QAbstractDiagramShapeConnector::itemPropertyHasChanged( const QString & name, const QVariant & value)
{
//	if (name == "text"){
	if (m_textItem){
		m_textItem->itemPropertyHasChanged(this, name, value);
		updateTextItemPosition();
	}
	return QAbstractDiagramGraphicsItem::itemPropertyHasChanged(name, value);
}

QVariant QAbstractDiagramShapeConnector::itemSceneHasChanged(const QVariant & value)
{
	reconnect();
	updateTextItemPosition();
	return value;
}

QDiagramToolkit::ConnectionPointOrientation QAbstractDiagramShapeConnector::orientationAtEnd() const
{
    if (m_connectionPointAtEnd){
        return m_connectionPointAtEnd->orientation();
    }
    return m_tempOrientationAtEnd;
}

QDiagramToolkit::ConnectionPointOrientation QAbstractDiagramShapeConnector::orientationAtStart() const
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
	QDiagramConnectionPoint start = property("start").toConnectionPoint();
    shape = diagram()->shape(start.uuid());
    if (shape){
        m_connectionPointAtStart = shape->connectionPoint(start.id());
    }
    if (m_connectionPointAtStart){
        m_connectionPointAtStart->append(this, QAbstractDiagramShapeConnectionPoint::Out);
		m_connectionPointAtStart->updatePosition();
		QDiagramConnectionPoint cp(m_connectionPointAtStart->parentShape()->uuid(), m_connectionPointAtStart->id());
		setProperty("start", qVariantFromValue(cp));
    }
	QDiagramConnectionPoint end = property("end").toConnectionPoint();
    shape = diagram()->shape(end.uuid());
    if (shape){
        m_connectionPointAtEnd = shape->connectionPoint(end.id());
    }
    if (m_connectionPointAtEnd){
        m_connectionPointAtEnd->append(this, QAbstractDiagramShapeConnectionPoint::In);
		m_connectionPointAtEnd->updatePosition();
		QDiagramConnectionPoint cp(m_connectionPointAtEnd->parentShape()->uuid(), m_connectionPointAtEnd->id());
		setProperty("end", qVariantFromValue(cp));
    }
	if (m_connectionPointAtStart && m_connectionPointAtEnd){
		updatePosition();
	}
}

void QAbstractDiagramShapeConnector::remove(QAbstractDiagramShapeConnectionPoint* point)
{
    if (m_connectionPointAtStart == point){
        m_connectionPointAtStart->remove(this);
        m_connectionPointAtStart = 0;
    }
    if (m_connectionPointAtEnd == point){
        m_connectionPointAtEnd->remove(this);
        m_connectionPointAtEnd = 0;
    }
}

void QAbstractDiagramShapeConnector::restoreProperties(const QVariantMap & p)
{
	QAbstractDiagramGraphicsItem::restoreProperties(p);
	m_textItem->setPlainText(p.value("text").toString());
	updateTextItemPosition();
}

void QAbstractDiagramShapeConnector::restoreFromProperties(const QMap<QString,QVariant> & properties)
{
	QAbstractDiagramShape* s = 0;
	QDiagramConnectionPoint cp;

	QVariantMap m;

	m = properties.value("start").toMap();
	s = diagram()->shape(m.value("uuid").toString());
    if (s){
        m_connectionPointAtStart = s->connectionPoint(m.value("id").toString());
    }
    if (m_connectionPointAtStart){
        m_connectionPointAtStart->append(this, QAbstractDiagramShapeConnectionPoint::Out);
		setProperty("start", qVariantFromValue(QDiagramConnectionPoint(m_connectionPointAtStart->parentShape()->uuid(), m_connectionPointAtStart->id())));
    }

	m = properties.value("end").toMap();
	s = diagram()->shape(m.value("uuid").toString());
    if (s){
        m_connectionPointAtEnd = s->connectionPoint(m.value("id").toString());
    }
    if (m_connectionPointAtEnd){
        m_connectionPointAtEnd->append(this, QAbstractDiagramShapeConnectionPoint::In);
		setProperty("start", qVariantFromValue(QDiagramConnectionPoint(m_connectionPointAtEnd->parentShape()->uuid(), m_connectionPointAtEnd->id())));
    }
    updatePosition();
}

void QAbstractDiagramShapeConnector::setConnectionPointAtStart(QAbstractDiagramShapeConnectionPoint* point)
{
	m_connectionPointAtStart = point;
}

void QAbstractDiagramShapeConnector::setFrom(QAbstractDiagramShapeConnectionPoint* item)
{
    m_connectionPointAtStart = item;
    if (m_connectionPointAtStart){
		setProperty("start", qVariantFromValue(QDiagramConnectionPoint(m_connectionPointAtStart->parentShape()->uuid(), m_connectionPointAtStart->id())));
    }
}

void QAbstractDiagramShapeConnector::setTo(QAbstractDiagramShapeConnectionPoint* item)
{
    m_connectionPointAtEnd = item;
    if (m_connectionPointAtEnd){
		setProperty("end", qVariantFromValue(QDiagramConnectionPoint(m_connectionPointAtEnd->parentShape()->uuid(), m_connectionPointAtEnd->id())));
    }
}

void QAbstractDiagramShapeConnector::setTemporaryStart(const QPointF & position, QDiagramToolkit::ConnectionPointOrientation orientation)
{
    m_tempStartPos = position;
    m_tempOrientationAtStart = orientation;
    updatePosition();
}

void QAbstractDiagramShapeConnector::setTemporaryEnd(const QPointF & position, QDiagramToolkit::ConnectionPointOrientation orientation)
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

QAbstractDiagramShapeConnectionPoint* QAbstractDiagramShapeConnector::connectionPointAtStart() const
{
    return m_connectionPointAtStart;
}

QDiagramGraphicsTextItem* QAbstractDiagramShapeConnector::textItem() const
{
	return m_textItem;
}

void QAbstractDiagramShapeConnector::updateTextItemPosition()
{
}

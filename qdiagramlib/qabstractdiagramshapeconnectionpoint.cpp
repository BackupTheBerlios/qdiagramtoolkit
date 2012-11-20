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
#include "qabstractdiagramshapeconnectionpoint.h"

#include "qdiagramshapeconnector.h"
#include "qabstractdiagramshape.h"

class sdAbstractDiagramShapeConnectionPoint : public QSharedData
{
public:
    sdAbstractDiagramShapeConnectionPoint()
    {
        index = -1;
        orientation = QDiagramToolkit::North;
        shape = 0;
    }

    sdAbstractDiagramShapeConnectionPoint(const sdAbstractDiagramShapeConnectionPoint & other) :
        QSharedData(other)
    {
        index = other.index;
        orientation = other.orientation;
        rect = other.rect;
        shape = other.shape;
    }

    ~sdAbstractDiagramShapeConnectionPoint()
    {
    }


    QList<QAbstractDiagramShapeConnectionPoint::Connection> connections;
    int index;
    QDiagramToolkit::ConnectionPointOrientation orientation;
    QRectF rect;
    QAbstractDiagramShape* shape;

};

QAbstractDiagramShapeConnectionPoint::QAbstractDiagramShapeConnectionPoint(QAbstractDiagramShape* shape, const QString & id, QDiagramToolkit::ConnectionPointOrientation orientation, int maxConnections) :
    QGraphicsItem(shape)
{
    m_dir = QAbstractDiagramShapeConnectionPoint::InOut;
    m_id = id;
    m_maxConnections = maxConnections;
    m_orientation = orientation;
    setVisible(false);
}

QAbstractDiagramShapeConnectionPoint::~QAbstractDiagramShapeConnectionPoint()
{
//    Q_FOREACH(QAbstractDiagramShapeConnectionPoint::Connection mConnection, d->connections){
    Q_FOREACH(QAbstractDiagramShapeConnectionPoint::Connection mConnection, m_connections){
        mConnection.connector->remove(this);
    }
}

void QAbstractDiagramShapeConnectionPoint::append(QAbstractDiagramShapeConnector* connector, QAbstractDiagramShapeConnectionPoint::Direction direction)
{
    //    Q_FOREACH(QAbstractDiagramShapeConnectionPoint::Connection mConnection, d->connections){
    Q_FOREACH(QAbstractDiagramShapeConnectionPoint::Connection mConnection, m_connections){
        if (mConnection.connector == connector){
            return;
        }
    }
    QAbstractDiagramShapeConnectionPoint::Connection mConnection;
    mConnection.connector = connector;
    mConnection.direction = direction;
//    d->connections.append(mConnection);
    m_connections.append(mConnection);
}

void QAbstractDiagramShapeConnectionPoint::append(const QAbstractDiagramShapeConnector* connector, QAbstractDiagramShapeConnectionPoint::Direction direction)
{
    //    Q_FOREACH(QAbstractDiagramShapeConnectionPoint::Connection mConnection, d->connections){
    Q_FOREACH(QAbstractDiagramShapeConnectionPoint::Connection mConnection, m_connections){
        if (mConnection.connector == connector){
            return;
        }
    }
    QAbstractDiagramShapeConnectionPoint::Connection mConnection;
    mConnection.connector = (QAbstractDiagramShapeConnector*)connector;
    mConnection.direction = direction;
//    d->connections.append(mConnection);
    m_connections.append(mConnection);
}

QRectF QAbstractDiagramShapeConnectionPoint::boundingRect() const
{
    return m_rect;
}

bool QAbstractDiagramShapeConnectionPoint::canConnect(QAbstractDiagramShapeConnector* connector) const
{
    Q_UNUSED(connector);
    return true;
}

QList<QAbstractDiagramShapeConnectionPoint::Connection> QAbstractDiagramShapeConnectionPoint::connections() const
{
    return m_connections;
}

QAbstractDiagramShapeConnector* QAbstractDiagramShapeConnectionPoint::connector(int index) const
{
	if (index < m_connections.size()){
		return m_connections.at(index).connector;
	}
	return 0;
}

QList<QAbstractDiagramShapeConnector*> QAbstractDiagramShapeConnectionPoint::connectors() const
{
	QList<QAbstractDiagramShapeConnector*> l;
	Q_FOREACH(QAbstractDiagramShapeConnectionPoint::Connection c, m_connections){
		l << c.connector;
	}
	return l;
}

int QAbstractDiagramShapeConnectionPoint::count() const
{
	return m_connections.size();
}

QVariantMap QAbstractDiagramShapeConnectionPoint::customConnectorProperties() const
{
	return QVariantMap();
}

QPointF QAbstractDiagramShapeConnectionPoint::diagramPos() const
{
    return scenePos() + boundingRect().center();
}

QAbstractDiagramShapeConnectionPoint::Direction QAbstractDiagramShapeConnectionPoint::direction() const
{
    return m_dir;
}

QString QAbstractDiagramShapeConnectionPoint::id() const
{
    return m_id;
}

bool QAbstractDiagramShapeConnectionPoint::isConnected() const
{
	return !m_connections.isEmpty();
}

int QAbstractDiagramShapeConnectionPoint::maxConnections() const
{
    return m_maxConnections;
}

QString QAbstractDiagramShapeConnectionPoint::name() const
{
    return m_name;
}

QDiagramToolkit::ConnectionPointOrientation QAbstractDiagramShapeConnectionPoint::orientation() const
{
    return m_orientation;
}

QRectF QAbstractDiagramShapeConnectionPoint::rect() const
{
    return m_rect;
}

void QAbstractDiagramShapeConnectionPoint::remove(QAbstractDiagramShapeConnector* connector)
{
//    for(int iConnections = 0; iConnections < d->connections.size(); iConnections++){
//        if (d->connections.at(iConnections).connector == connector){
//            d->connections.removeAt(iConnections);
//            return;
//        }
//    }
    for(int iConnections = 0; iConnections < m_connections.size(); iConnections++){
        if (m_connections.at(iConnections).connector == connector){
            m_connections.removeAt(iConnections);
            return;
        }
    }
}

//QPointF QAbstractDiagramShapeConnectionPoint::scenePos() const
//{
////    if (d->shape){
////        return d->shape->pos() + d->rect.center();
////    }
//    return QPointF();
//}

void QAbstractDiagramShapeConnectionPoint::setRect(const QRectF & rect)
{
//    d->rect = rect;
    m_rect = rect;
}

void QAbstractDiagramShapeConnectionPoint::setRect(const QPointF & topLeft, const QSizeF & size)
{
//    d->rect = QRectF(topLeft, size);
    m_rect = QRectF(topLeft, size);
}

QAbstractDiagramShape* QAbstractDiagramShapeConnectionPoint::parentShape() const
{
    return dynamic_cast<QAbstractDiagramShape*>(parentItem());
    //    return d->shape;
}

void QAbstractDiagramShapeConnectionPoint::setDirection(QAbstractDiagramShapeConnectionPoint::Direction dir)
{
    m_dir = dir;
}

QPainterPath QAbstractDiagramShapeConnectionPoint::shape() const
{
    QPainterPath mPath;
    mPath.addRect(m_rect);
    return mPath;
}

void QAbstractDiagramShapeConnectionPoint::updateConnectors()
{
    //    Q_FOREACH(QAbstractDiagramShapeConnectionPoint::Connection mConnection, d->connections){
    Q_FOREACH(QAbstractDiagramShapeConnectionPoint::Connection mConnection, m_connections){
        mConnection.connector->updatePosition();
    }
}

//QAbstractDiagramShapeConnectionPoint & QAbstractDiagramShapeConnectionPoint::operator =( const QAbstractDiagramShapeConnectionPoint & other )
//{
//    if (this != &other){
//        d = other.d;
//    }
//    return *this;
//}


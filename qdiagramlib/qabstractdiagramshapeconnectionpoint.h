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

#ifndef QABSTRACTDIAGRAMSHAPECONNECTIONPOINT_H
#define QABSTRACTDIAGRAMSHAPECONNECTIONPOINT_H
#include "qdiagramlib_global.h"

#include <QExplicitlySharedDataPointer>

#include <qabstractdiagramgraphicsitem.h>

class QAbstractDiagramShape;
class sdAbstractDiagramShapeConnectionPoint;

class QDIAGRAMLIBSHARED_EXPORT QAbstractDiagramShapeConnectionPoint : public QGraphicsItem
{
public:
    enum Direction {
        In,
        InOut,
        Out,
        InvalidDirection
    };
    //! This enum describes the orientation of a connection point.
    enum Orientation {
        North,
        East,
        South,
        West,
        Invalid
    };
    enum {
        Type = QGraphicsItem::UserType + 4203
    };
    typedef struct {
        int direction;
        QAbstractDiagramShapeConnector* connector;
    } Connection;

    /**
      * Constructs a QGraphicsItem with the given @p index, @p orientation and @p parentShape.
      */
    QAbstractDiagramShapeConnectionPoint(QAbstractDiagramShape* parentShape, const QString & id, QAbstractDiagramShapeConnectionPoint::Orientation orientation, int maxConnections = -1 );
    /**
      * Destroys the QAbstractDiagramShapeConnectionPoint.
      */
    ~QAbstractDiagramShapeConnectionPoint();
    /**
      *
      */
    void append(QAbstractDiagramShapeConnector* connector, QAbstractDiagramShapeConnectionPoint::Direction direction);
    /**
      *
      */
    void append(const QAbstractDiagramShapeConnector* connector, QAbstractDiagramShapeConnectionPoint::Direction direction);
    /**
      * @reimp QGraphicsItem::boundingRect()
      */
    QRectF boundingRect() const;
    /**
      * Returns true if the specified @p connector can connect to this connection point. Otherwise false.
      * canConnect() can be used to implemented directed connection, to prevent e.g input to input connections between shapes.
      *
      * The default implemetation returns true.
      */
    virtual bool canConnect(QAbstractDiagramShapeConnector* connector) const;
    /**
      * Returns a list connections attached to this tie point.
      */
    QList<QAbstractDiagramShapeConnectionPoint::Connection> connections() const;
    /**
      * Returns the connector's position in the diagram.
      */
    QPointF diagramPos() const;
    /**
      *
      */
    QAbstractDiagramShapeConnectionPoint::Direction direction() const;
    /**
      * Returns the connection point's id.
      */
    QString id() const;
    /**
      * Returns the maximum number of connections allowed for this connection point.
      * The default is -1.
      */
    int maxConnections() const;
    /**
      * Return the connection poings name.
      */
    QString name() const;
    /**
      * Returns the connection point's orientation.
      */
    QAbstractDiagramShapeConnectionPoint::Orientation orientation() const;
    /**
      * This function, which is usually called by QGraphicsView, paints the contents of an item in local coordinates.
      * Reimplement this function in a QAbstractDiagramShapeConnectionPoint subclass to provide the item's painting implementation, using painter.
      * The option parameter provides style options for the item, such as its state, exposed area and its level-of-detail hints.
      * The widget argument is optional. If provided, it points to the widget that is being painted on; otherwise, it is 0. For cached painting, widget is always 0.
      * @see QGraphicsItem::paint()
      */
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) = 0;

    QRectF rect() const;
    /**
      * Removes (dsiconnects) the given @p connector.
      */
    void remove(QAbstractDiagramShapeConnector* connector);
    /**
      * Returns the tie point's scene position.
      */
//    QPointF scenePos() const;
    /**
      * Returns the diagram shape this connection point is attacted to.
      */
    QAbstractDiagramShape* parentShape() const;
    /**
      *
      */
    void setDirection(QAbstractDiagramShapeConnectionPoint::Direction dir);
    /**
      * Returns the shape of this item as a QPainterPath in local coordinates.
      * @see QGraphicsItem::shape()
      */
    QPainterPath shape() const;

    void updateConnectors();

    virtual void updatePosition() = 0;
    /**
      * Returns the type of an item as an int.
      * @see QGraphicsItem::type()
      */
    int type() const
    {
        return Type;
    }

//    QAbstractDiagramShapeConnectionPoint & operator =( const QAbstractDiagramShapeConnectionPoint & other );
protected:
    void setRect(const QRectF & rect);

    void setRect(const QPointF & topLeft, const QSizeF & size);
private:
//    QExplicitlySharedDataPointer<sdAbstractDiagramShapeConnectionPoint> d;
    QList<QAbstractDiagramShapeConnectionPoint::Connection> m_connections;
    Direction m_dir;
    QString m_id;
    int m_maxConnections;
    QString m_name;
    QAbstractDiagramShapeConnectionPoint::Orientation m_orientation;
    QRectF m_rect;
};

#endif // QABSTRACTDIAGRAMSHAPECONNECTIONPOINT_H

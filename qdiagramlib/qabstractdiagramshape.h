/******************************************************************************
** Copyright (C) 2011-2012 Martin Hoppe martin@2x2hoppe.de
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

#ifndef QABSTRACTDIAGRAMSHAPE_H
#define QABSTRACTDIAGRAMSHAPE_H

#include "qdiagramlib_global.h"

#include <qabstractdiagramgraphicsitem.h>
#include <qabstractdiagramshapeconnectionpoint.h>

#include <qdiagramshapesizegrip.h>

//! The QAbstractDiagramShape class is the base of all shapes in a QAbstractDiagram class.
class QDIAGRAMLIBSHARED_EXPORT QAbstractDiagramShape : public QAbstractDiagramGraphicsItem
{
public:
    enum {
        Type = QGraphicsItem::UserType + 4201
    };
    /**
      * Constructs a QAbstractDiagramShape with the given @p parent item.
      */
    explicit QAbstractDiagramShape(QGraphicsItem* parent = 0);
    /**
      * Destroys the QAbstractDiagramShape and frees allocated resources.
      */
    ~QAbstractDiagramShape();

    void addConnection(QAbstractDiagramGraphicsItem::ConnectorDirection direction, QAbstractDiagramShapeConnector* connector);  
    /**
      * This pure virtual function defines the outer bounds of the shape as a rectangle; all painting must be restricted to inside an item's bounding rect.
      */
    virtual QRectF boundingRect() const = 0;
    /**
      * Returns the connection point specified by the given @p id or 0 if @p id does not exists.
      */
    QAbstractDiagramShapeConnectionPoint* connectionPoint(const QString & id) const;
    /**
      * Returns the connection point specified by the given @p position or 0 if no connection point can be found at the specified @p position.
      */
    QAbstractDiagramShapeConnectionPoint* connectionPoint(const QPointF & position) const;
    /**
      * Returns a list of connection points.
      */
    QList<QAbstractDiagramShapeConnectionPoint*> connectionPoints() const;
    /**
      * Returns a list of connection points matching the given @p orientation.
      */
    QList<QAbstractDiagramShapeConnectionPoint*> connectionPoints(QDiagramToolkit::ConnectionPointOrientation orientation) const;
    /**
      * Returns a list of connectors connected with this shape.
      */
    QList<QAbstractDiagramShapeConnector*> connectors() const;
	/**
	 * Returns the shape's focus rectangle. The default implementation returns the bounding rectangle (boundingRect()).
	 * @see paintFocus()
	 */
	virtual QRectF focusRect() const;
    /**
      * Returns true if the @p connector is allowed to connect. Otherwise false.
      * The default implementation return true.
      */
    virtual bool isConnectionAllowed(QAbstractDiagramShapeConnector* connector) const;

    virtual void restoreFromProperties(const QVariantMap & properties);

    void setConnectionPointsVisible(bool visible);
    /**
      * Returns true if the connection points are visible. Otherwise false.
      * @see setConnectionPointsVisible()
      */
    bool connectionPointsAreVisible() const;

    virtual QPainterPath shape() const;
    /**
      * Return the shape's selection.
      */
    QColor selectionColor() const;
    /**
      * Returns the type of the QAbstractDiagramShapeConnectionPoint as an int.
      * @see QGraphicsItem::type()
      */
    int type() const
    {
        return Type;
    }
protected:
    /**
      * Constructs a diagram shape with the given @p properties.
      */
	QAbstractDiagramShape(const QString & plugin, const QString & itemClass, const QVariantMap & properties, QGraphicsItem* parent = 0);
    /**
      * Adds the given size grid @p handle to the shape.
      */
    void addSizeGripHandle(QDiagramShapeSizeGripHandle* handle);
    /**
      * Adds the given connection @p point to the shape.
      */
    void addConnectionPoint(QAbstractDiagramShapeConnectionPoint* point);
	/**
      * This virtual function is called by QAbstractDiagramShape to notify custom items that the geometry has changed.
      * The @p value argument is the new geometry (the same as geometry()), and QAbstractDiagramShape ignores the return value for this notification (i.e., a read-only notification).
      */
	virtual QVariant itemGeometryHasChanged(const QVariant & value);
    /**
      * This virtual function is called by QAbstractDiagramShape to notify custom items that the position has changed.
      * The @p value argument is the new position (the same as pos()), and QAbstractDiagramShape ignores the return value for this notification (i.e., a read-only notification).
      */
    virtual QVariant itemPositionHasChanged( const QVariant & value );
	virtual QVariant itemSceneHasChanged(const QVariant & value);
    virtual QVariant itemSelectedHasChanged( const QVariant & value );

    virtual void paintConnectionPoints(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    virtual void paintFocus(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    virtual void updateConnectedItems();

    void updateConnectionPoints();

    virtual void updateSizeGripHandles();
private:
    struct ConnectionData
    {
        ConnectorDirection direction;
        int index;
        QAbstractDiagramShapeConnector* connector;
    };
    QList<ConnectionData> m_connections;
    bool m_connectionPointsVisible;
    QList<QAbstractDiagramShapeConnectionPoint*> m_connectionPoints;
    QList<QDiagramShapeSizeGripHandle*> m_handles;
};

Q_DECLARE_METATYPE(QAbstractDiagramShape*)
Q_DECLARE_METATYPE(QList<QAbstractDiagramShape*>)

#endif // QABSTRACTDIAGRAMSHAPE_H

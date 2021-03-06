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

#ifndef QABSTRACTDIAGRAMSHAPECONNECTOR_H
#define QABSTRACTDIAGRAMSHAPECONNECTOR_H

#include <qabstractdiagramgraphicsitem.h>
#include <qabstractdiagramshape.h>

#include <qabstractdiagramshapeconnectionpoint.h>

class QDiagramGraphicsTextItem;

//! The QAbstractDiagramShapeConnector provides an abstract base class for all connectors.
/**
  *
  * @property style
  */
class QDIAGRAMLIBSHARED_EXPORT QAbstractDiagramShapeConnector : public QAbstractDiagramGraphicsItem
{
	Q_OBJECT
public:
    enum {
        Type = QGraphicsItem::UserType + 4202
    };
    /**
      * Constructs a QAbstractDiagramShapeConnector with the given @p properties.
      */
    QAbstractDiagramShapeConnector(const QString & plugin, const QString & itemClass, const QVariantMap & properties, QGraphicsItem* parent = 0);
    /**
      * Constructs a QAbstractDiagramShapeConnector with the given @p uuid and @p style.
      */
//    explicit QAbstractDiagramShapeConnector(const QString & uuid, const QString & style);
    /**
      * Destroys the QAbstractDiagramShapeConnector.
      */
    ~QAbstractDiagramShapeConnector();

	virtual QList<QPointF> breakPoints() const = 0;
    /**
      * Returns true if this connector can connect to the connection points @p start and @end. Otherwise false.
      * canConnect() can be used to implemented directed connection, to prevent e.g input to input connections between shapes.
      *
      * The default implemetation returns true.
      */
    virtual bool canConnect(QAbstractDiagramShapeConnectionPoint* start, QAbstractDiagramShapeConnectionPoint* end) const;
	/**
	 *
	 */
	virtual bool canStartWith(QAbstractDiagramShapeConnectionPoint* cp) const;
    /**
      * Returns the connection point at the end of this connector.
      * @see connectionPointAtStart()
      */
    QAbstractDiagramShapeConnectionPoint* connectionPointAtEnd() const;
    /**
      * Returns the connection point at the start of this connector.
      * @see connectionPointAtEnd()
      */
    QAbstractDiagramShapeConnectionPoint* connectionPointAtStart() const;
	/**
	 * Disconnects all connector attached to this connection point.
	 */
    void disconnect();
    /**
      * Returns the end position of this connector.
      * @see startPos()
      */
    QPointF endPos() const;
    /**
      * Returns true if the connector represents a temporary connection. Otherwise false.
      */
    bool isTemporary() const;
    /**
      *
      */
    QDiagramToolkit::ConnectionPointOrientation orientationAtEnd() const;
    /**
      * Return the connector's orientation at the start position.
      */
    QDiagramToolkit::ConnectionPointOrientation orientationAtStart() const;

    void remove(QAbstractDiagramShapeConnectionPoint* point);

	void reconnect();

    virtual void restoreFromProperties(const QMap<QString,QVariant> & properties);

	void setConnectionPointAtStart(QAbstractDiagramShapeConnectionPoint* point);
    /**
      * Sets a temporary start @p position with the given @p orientation.
      */
    void setTemporaryStart(const QPointF & position, QDiagramToolkit::ConnectionPointOrientation orientation);
    /**
      * Sets a temporary end @p position with the given @p orientation.
      */
    void setTemporaryEnd(const QPointF & position, QDiagramToolkit::ConnectionPointOrientation orientation = QDiagramToolkit::ConnectionPointOrientationInvalid);
    /**
      * Returns the starting position of this shape connector.
      */
    QPointF startPos() const;
    /**
      * Returns the type of the QAbstractDiagramShapeConnector as an int.
      */
    int type() const
    {
        return Type;
    }
    /**
      * Update the connector's position.
      */
    virtual void updatePosition() = 0;
protected:
    virtual void drawPolyline(QPainter* painter, const QList<QPointF> & points);

    QPointF intersectPoint(QGraphicsItem* item, const QLineF & line) const;
    QPointF intersectPoint(const QPointF & pos, const QPolygonF & diagramShape, const QLineF & line) const;
	/**
	 * @remp QAbstractDiagramGraphicsItem::itemPositionChange()
	 */
	virtual QVariant itemPositionHasChanged(const QVariant & value);
	/**
	 * @remp QAbstractDiagramGraphicsItem::itemPropertyHasChanged()
	 */
	virtual QVariant itemPropertyHasChanged( const QString & name, const QVariant & value);

	virtual QVariant itemSceneHasChanged(const QVariant & value);
	/**
	 * @remp QAbstractDiagramGraphicsItem
	 */
	virtual void restoreProperties(const QVariantMap & p);

    void setFrom(QAbstractDiagramShapeConnectionPoint* item);
    void setTo(QAbstractDiagramShapeConnectionPoint* item);
	/**
	 * Returns the connector's text item.
	 */
	QDiagramGraphicsTextItem* textItem() const;
	/**
	 * Updates the text item's position and is called from itemPropertyHasChanged().
	 * @remark The default implementation does nothing.
	 */
	virtual void updateTextItemPosition();
private:
	bool m_tempConnectionPointAtStart;
    QAbstractDiagramShapeConnectionPoint* m_connectionPointAtStart;
    QAbstractDiagramShapeConnectionPoint* m_connectionPointAtEnd;
    QPointF m_tempStartPos;
    QDiagramToolkit::ConnectionPointOrientation m_tempOrientationAtStart;
    QPointF m_tempEndPos;
    QDiagramToolkit::ConnectionPointOrientation m_tempOrientationAtEnd;
	QDiagramGraphicsTextItem* m_textItem;
};

Q_DECLARE_METATYPE(QAbstractDiagramShapeConnector*)
Q_DECLARE_METATYPE(QList<QAbstractDiagramShapeConnector*>)

#endif // QABSTRACTDIAGRAMSHAPECONNECTOR_H

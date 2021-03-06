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
#ifndef QABSTRACTDIAGRAMGRAPHICSITEM_H
#define QABSTRACTDIAGRAMGRAPHICSITEM_H

#include "qdiagramlib_global.h"

#include <QAction>
#include <QBrush>
#include <QGraphicsItem>
#include <QPen>

#include <qdiagramtoolkit.h>
#include <qdiagrammetaproperty.h>
#include <qdiagramproperty.h>

class QAbstractDiagramShapeConnector;
class QAbstractDiagram;

class QDiagramSheet;
class QDiagramMetaData;

//! The QAbstractDiagramGraphicsItem class is the base of all diagram graphics item.
/**
 * The class defines the functions for the functionality shared by all diagram graphics item.
 */
class QDIAGRAMLIBSHARED_EXPORT QAbstractDiagramGraphicsItem : public QGraphicsObject
{
	Q_OBJECT
public:
    enum {
        Type = QGraphicsItem::UserType + 4200
    };
    enum ConnectorDirection {
        IncomingConnection,
        OutgoingConnection,
        InvalidConnectorDirection
    };
    /**
      * Constructs a QAbstractDiagramGraphicsItem with the given @p parent item.
      */
    explicit QAbstractDiagramGraphicsItem(QGraphicsItem* parent = 0);
    /**
      * Constructs a QAbstractDiagramGraphicsItem with the given @p uuid and @p type.
      */
    explicit QAbstractDiagramGraphicsItem(const QString & uuid, const QString & pluginName, const QString & itemType, const QString & itemClass, QGraphicsItem* parent = 0);
    /**
      * Destroys the QAbstractDiagramGraphicsItem.
      */
    ~QAbstractDiagramGraphicsItem();
    /**
      * Returns the (possibly empty) list of this item's actions.
      */
    QList<QAction*> actions() const;
    /**
      * Appends the action @p action to this item's list of actions.
      * The list of action is used to create the item's context menu.
      * @see actions()
      */
    void addAction(QAction* action);
    /**
      * If @p block is true, undo commands for this item are blocked. If @p block is false, no such blocking will occur.
      * @see undoCommandsBlocked()
      */
    bool blockUndoCommands(bool block);
    /**
      * This pure virtual function defines the outer bounds of the item as a rectangle; all painting must be restricted to inside an item's bounding rect.
      */
    QRectF boundingRect() const = 0;
	/**
	 *
	 */
	void bringForward();
	/**
	 * Brings the shape to the front.
	 * @see sendToBack()
	 */
	void bringToFront();

	virtual QList<QAction*> createActions(QWidget* parent);
	/**
	 * Returns the user data as set in QAbstractDiagramGraphicsItem::setData.
	 * @see setData()
	 */
	QVariant data() const;
    /**
      * Returns the diagram containing this item.
      */
    QAbstractDiagram* diagram() const;
    /**
      * Returns the item's geometry.
      */
    QRectF geometry() const;
    /**
      * Returns true if the shape has a property with the given @p name; otherwise returns false.
      */
    bool hasProperty(const QString & name) const;
    /**
      * Returns a pointer to the meta-data of this object.
      */
    QDiagramMetaData* metaData() const;
	/**
	 * Returns the sheet containing this item.
	 */
	QDiagramSheet* sheet() const;
    /**
      *
      */
    bool parse(const QString & data);
    /**
      * Returns the shape's name.
      */
//    QString name() const;
    /**
      * Returns the item's pen, or a black solid 1-width pen if no pen has been set.
      */
    QPen pen() const;
    /**
      * Returns a map with all properties of this diagram graphics item.
      */
    QMap<QString,QVariant> properties() const;
    /**
      * Returns the property specified by the given @p name.
      */
	QDiagramProperty property(const QString & name) const;

    virtual void restoreFromProperties(const QVariantMap & properties);

	virtual QByteArray serialize() const;
    /**
      * Returns the current selection color.
      * @see QDiagram::selectionColor()
      */
    QColor selectionColor() const;
	/**
	 * Sets the item's internal data to the given @p data.
	 * @see data()
	 */
	void setData(const QVariant & data);
    /**
      * Sets the item's pen to pen. If no pen is set, the line will be painted using a black solid 1-width pen.
      */
    void setPen(const QPen & pen);
    /**
      * Sets the value of the item's @p name property to @p value.
      * If the property is defined in the class using addStaticProperty() then true is returned on success and false otherwise.
      * If the property is not defined using addStaticProperty, and therefore not listed in the meta-data, it is added as a dynamic property and false is returned.
      */
    bool setProperty(const QString & name, const QVariant & value);
	/**
	 * Sends the shape one level backward.
	 */
	void sendBackward();
	/**
	 * Sends the item to the back.
	 * @see bringToFront()
	 */
	void sendToBack();

	virtual void triggerAction(QAction* action);

	virtual void triggerAction(const QString & name, const QVariant & data);
    /**
      * Returns the type of the QAbstractDiagramGraphicsItem as an int.
      */
    int type() const
    {
        return Type;
    }
    /**
      * Returns true if undo commands are blocked. Otherwise false.
      * @see blockUndoCommands()
      */
    bool undoCommandsBlocked() const;
    /**
      * Returns the item's uuid.
      */
    QString uuid() const;
protected:
//    QAbstractDiagramGraphicsItem(const QString & diagramShape, QGraphicsItem* parent = 0);

	void addDynamicProperty(const QString & name, const QVariant & value);

    void addProperty(const QString & name, QDiagramToolkit::PropertyType type, bool readOnly, const QVariant & value = QVariant());

    void addProperty(const QString & name, QDiagramToolkit::PropertyType type, const QMap<int, QString> &enums, const QVariant & value = QVariant());

    QBrush brush() const;

    virtual void contextMenuEvent( QGraphicsSceneContextMenuEvent* event );

	void initGeometry(qreal width, qreal height);

	virtual QVariant itemGeometryHasChanged(const QVariant & value);
    /**
      * This virtual function is called by QAbstractDiagramShape to notify custom items that the position has changed.
      * The @p value argument is the new position (the same as pos()), and QAbstractDiagramShape ignores the return value for this notification (i.e., a read-only notification).
      */
    virtual QVariant itemPositionHasChanged( const QVariant & value );
    /**
      * This virtual function is called by QAbstractDiagramGraphicsItem to notify custom items that the @p value of property @p name shall be changed.
      */
    virtual QVariant itemPropertyChange( const QString & name, const QVariant & value);
    /**
      * This virtual function is called by QAbstractDiagramGraphicsItem to notify custom items that the @p value of property @p name has changed.
      */
    virtual QVariant itemPropertyHasChanged( const QString & name, const QVariant & value);

    virtual QVariant itemSelectedChange( const QVariant & value );

    virtual QVariant itemSelectedHasChanged( const QVariant & value );
    /**
      * This virtual function is called by QAbstractDiagramShape to notify custom items that the z value has changed.
      * The @p value argument is the z value (the same as zValue()), and QAbstractDiagramShape ignores the return value for this notification (i.e., a read-only notification).
      */
    virtual QVariant itemZValueHasChanged(const QVariant & value );

    void setBrush( const QBrush & brush );
protected:
	/**
	 * Sets the item's geometry to @p r.
	 */
	void changeGeometry(const QRectF & r);
    /**
      * This virtual function is called by QAbstractDiagramShape to notify custom items that the position changes.
      * The @p value argument is the new position (i.e., a QPointF).
      */
    virtual QVariant itemPositionChange( const QVariant & value );
    /**
      * This virtual function is called by QAbstractDiagramShape to notify custom items that the scene has changed.
      */
    virtual QVariant itemSceneHasChanged(const QVariant & value);
	/**
	 * Restories the item's properties values from the given property map @p.
	 */
	virtual void restoreProperties(const QVariantMap & p);
    virtual void updateSizeGripHandles();
private:
	friend class QDiagramProperty;
	friend class QDiagramChangePropertyCommand;
	bool changeProperty(const QString & name, const QVariant & value);

    QVariant itemChange(GraphicsItemChange change, const QVariant & value);

	QVariant propertyValue(int index) const;

    QBrush brush(const QString & attrs);
    void initMetaData(const QString & uuid, const QString & pluginName, const QString & itemType, const QString & itemClass);
    QPen pen(const QString & attrs);

    QList<QAction*> m_actions;
    bool m_blockUndoCommands;
	QVariant m_data;
	int m_index;
    QDiagramMetaData* m_metadata;

    QBrush m_brush;
    QMap<QString,QVariant> m_properties;
//	QMap<int, QDiagramPropertyValue> m_values;
	QVariantMap m_dynamicProperties;

    QString m_name;
    QPen m_pen;
};

Q_DECLARE_METATYPE(QAbstractDiagramGraphicsItem*)
Q_DECLARE_METATYPE(QList<QAbstractDiagramGraphicsItem*>)

#endif // QABSTRACTDIAGRAMGRAPHICSITEM_H

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

#ifndef QABSTRACTDIAGRAMPLUGIN_H
#define QABSTRACTDIAGRAMPLUGIN_H

#include <QAction>

#include "qdiagramlib_global.h"

#include <qdiagramconnectorstyle.h>
#include <qdiagramendoflinestyle.h>
#include <qdiagramlinestyle.h>

class QAbstractDiagram;
class QAbstractDiagramGraphicsItem;
class QAbstractDiagramShape;
class QAbstractDiagramShapeConnectionPoint;
class QAbstractDiagramShapeConnector;
class QAbstractDiagramGraphicsItemFactory;

class QDiagram;
class QGraphicsScene;

typedef QAbstractDiagramShapeConnector* (*QDiagramConnectorCreator)(const QVariantMap & propertiest);
typedef QAbstractDiagram* (*QDiagramCreator)(const QString & pluginName, QObject* parent);
typedef QAbstractDiagramShape* (*QDiagramShapeCreator)(const QVariantMap & properties);
//typedef QPointF (*shapeHotSpot)(const QVariantMap & properties);
typedef QPointF (*shapeHotSpot)(const QString & id);
typedef QVariantMap (*shapeProperties)(const QString & id);
typedef QVariantMap (*QDiagramShapeStaticMetaData)();

struct PluginDiagramData {
	QString title;
	QIcon icon;
	QDiagramCreator creator;
};

struct ShapeData {
	QString group;
	QIcon icon;
	QString title;
};

struct ShapeClass {
	QMap<QString, ShapeData> shapeData;
	QDiagramShapeCreator creator;
	shapeHotSpot hotSpot;
	shapeProperties defaultProperties;
	QDiagramShapeStaticMetaData staticMetaData;
};

struct ShapeConnectorClass {
	QDiagramConnectorCreator creator;
	QIcon icon;
	QString title;
};

struct ShapeProperties {
	QVariantMap property;
};
//! The QAbstractDiagramPlugin class provides an abstract base for custom QDiagram plugins.
/**
 * 
 */
class QDIAGRAMLIBSHARED_EXPORT QAbstractDiagramPlugin
{
public:
    virtual ~QAbstractDiagramPlugin();
    /**
      * Returns a list of connector names provided by the plugin.
      */
    virtual QList<QDiagramConnectorStyle> connectors() const;
    /**
      * Creates a diagram item based on the given @p metaData and @p properties. The item is added to the graphics scene @p scene.
      */
    virtual QAbstractDiagramGraphicsItem* createItem(const QMap<QString,QVariant> & metaData, const QMap<QString,QVariant> & properties);
    /**
      * Returns the default properties for the shape specified by the given @p name.
      */
    virtual QMap<QString,QVariant> defaultProperties(const QString & name) const;

    virtual QAbstractDiagram* diagram(const QString & id = "default", QObject* parent = 0) const;
    /**
      * Returns a list of diagram ids provided by this plugin;
      */
    virtual QStringList diagrams() const;

    virtual QList<QDiagramEndOfLineStyle> endOfLineStyles() const;

    virtual QStringList groups(QAbstractDiagram* diagram = 0) const;
    /**
      * Returns the hot spot for the diagram graphics item specified by the given @p metaData and @p properties.
	  * @remark The default implementation returns QPoint(0, 0);
      */
	QPointF hotSpot(const QString & id) const;
    //virtual QPointF hotSpot(const QVariantMap & metaData, const QVariantMap & properties) const;

	virtual void initialize();
    /**
      * Returns the icon for the connector or shape specified by the given @p name.
      */
    virtual QIcon icon(const QString & name) const;
	/**
	 *
	 */
	QString itemClass(const QString & id) const;
    /**
      * Returns a list of line styles provided by the plugin.
      */
    virtual QList<QDiagramLineStyle> lineStyles() const = 0;
	/**
	 * Returns the meta data for the shape specified by the given @p name.
	 */
	virtual QVariantMap metaData(const QString & name) const;
    /**
      * Returns the plugin's name.
      */
    virtual QString name() const = 0;
    /**
      * Returns a list of actions defined for the shape specified by the given @p style.
      * @remarks
      * The default implementation returns a empty list.
      */
    virtual QList<QAction*> shapeActions(const QString & style) const;
    /**
      * Returns a list of names for the shapes provided by the plugin.
      */
    virtual QStringList shapes(const QString & group = QString::null, QAbstractDiagram* diagram = 0) const;
    /**
      * Returns the (user-readable) title of the connector, diagram or shape specified by the given @p id.
      */
    virtual QString title(const QString & id) const;
    /**
      * Returns the @p shape's tool tip.
      */
    virtual QString toolTip(const QString & shape) const = 0;

	QMap<QString, ShapeClass> registeredShapes() const
	{
		return m_shapes;
	}

protected:
	template<typename T> inline void registerConnector(const QString & id, const QString & title, const QIcon & icon)
	{
//		name(), QIcon(":/emcsplugin/flow.control"), "Control Flow", "flow.control"
		ShapeConnectorClass c;
		c.creator = T::Creator;
		c.icon = icon;
		c.title = title;
		m_connectors[id] = c;
	}

	template<typename T> inline void registerDiagram(const QString & title, const QIcon & icon)
	{
		//plugin->registerDiagram(name, T::Creator);
		PluginDiagramData d;
		d.creator = T::Creator;
		d.icon = icon;
		d.title = title;
		m_diagrams[T::staticType()] = d;
	}
	
	template<typename T> inline void registerShape(const QString & id, const QString & group, const QString & title, const QIcon & icon)
	{
		ShapeData d;

//		QString className = QString(typeid(T).name());
		QString className = T::staticItemClass();
		className.replace("class ", "");
		d.group = group;
		d.icon = icon;
		d.title = title;

		if (m_shapes.contains(className)){
			if (m_shapes.value(className).shapeData.contains(id)){
				return;
			}
			m_shapes[className].shapeData[id] = d;
		} else {
			ShapeClass c;		
			c.shapeData[id] = d;
			c.creator = T::Creator;
			c.defaultProperties = T::defaultProperties;
			c.hotSpot = T::hotSpot;
			c.staticMetaData = T::staticMetaData;

			m_shapes[className] = c;
		}
	}
private:
	QMap<QString, ShapeConnectorClass> m_connectors;
	QMap<QString, PluginDiagramData> m_diagrams;
	QMap<QString, ShapeClass> m_shapes;
};

Q_DECLARE_INTERFACE(QAbstractDiagramPlugin, "qdiagramlib.plugin/1.0")

#endif // QABSTRACTDIAGRAMPLUGIN_H

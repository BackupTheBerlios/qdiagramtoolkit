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
    virtual QList<QDiagramConnectorStyle> connectors() const = 0;
    /**
      * Creates a diagram item based on the given @p metaData and @p properties. The item is added to the graphics scene @p scene.
      */
    virtual QAbstractDiagramGraphicsItem* createItem(const QMap<QString,QVariant> & metaData, const QMap<QString,QVariant> & properties, QGraphicsScene* scene) = 0;
    /**
      * Returns the default properties for the shape specified by the given @p name.
      */
    virtual QMap<QString,QVariant> defaultProperties(const QString & name) const = 0;

    virtual QDiagram* diagram(const QString & type = "default", QObject* parent = 0) const = 0;
    /**
      *
      */
    virtual QStringList diagrams() const = 0;

    virtual QList<QDiagramEndOfLineStyle> endOfLineStyles() const = 0;

    virtual QStringList groups(QAbstractDiagram* diagram = 0) const = 0;
    /**
      * Returns the hot spot for the diagram graphics item specified by the given @p metaData and @p properties.
	  * @remark The default implementation returns QPoint(0, 0);
      */
    virtual QPointF hotSpot(const QVariantMap & metaData, const QVariantMap & properties) const;
    /**
      * Returns the icon for the connector or shape specified by the given @p name.
      */
    virtual QIcon icon(const QString & name) const = 0;
    /**
      * Returns a list of line styles provided by the plugin.
      */
    virtual QList<QDiagramLineStyle> lineStyles() const = 0;
	/**
	 * Returns the meta data for the shape specified by the given @p name.
	 */
	virtual QVariantMap metaData(const QString & name) const = 0;
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
    virtual QStringList shapes(const QString & group = QString::null, QAbstractDiagram* diagram = 0) const = 0;
    /**
      * Returns the (user-readable) title of the connector or shape specified by the given @p name.
      */
    virtual QString title(const QString & name) const = 0;
    /**
      * Returns the @p shape's tool tip.
      */
    virtual QString toolTip(const QString & shape) const = 0;
protected:
    //void restoreProperties(QAbstractDiagramGraphicsItem* item, const QMap<QString,QVariant> & properties);
};

Q_DECLARE_INTERFACE(QAbstractDiagramPlugin, "qdiagramlib.plugin/1.0")

#endif // QABSTRACTDIAGRAMPLUGIN_H

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
#ifndef QLOGICCIRCUITPLUGIN_H
#define QLOGICCIRCUITPLUGIN_H

#include <qabstractdiagramplugin.h>

//! The QLogicCircuitPlugin
/**
  * @defgroup QLogicCircuitPlugin Logic Circuit
  */
class QLogicCircuitPlugin : public QObject, public QAbstractDiagramPlugin
{
    Q_OBJECT
    Q_INTERFACES(QAbstractDiagramPlugin)
public:
    QList<QDiagramConnectorStyle> connectors() const;

    QAbstractDiagramShapeConnector* createConnection(const QString & uuid, QAbstractDiagramShapeConnectionPoint* from, QAbstractDiagramShapeConnectionPoint* to, const QString & style);

    QAbstractDiagramGraphicsItem* createItem(const QMap<QString,QVariant> & properties, QGraphicsScene* scene);

    QAbstractDiagram* diagram(const QString & type = "default", QObject* parent = 0) const;

    QStringList diagrams() const;

    QList<QDiagramEndOfLineStyle> endOfLineStyles() const;
    /**
      * Returns the list of groups provided by the plugin.
      */
    QStringList groups(QAbstractDiagram* diagram = 0) const;

    QPointF hotSpot(const QString & name) const;
    /**
      * Returns the icon for the connector of shape specified by the given @p name.
      */
    QIcon icon(const QString & name) const;

    QList<QDiagramLineStyle> lineStyles() const;

    QString name() const;

    QAbstractDiagramShape* restoreShape(const QString & uuid, const QString & style, const QMap<QString,QVariant> & properties, QGraphicsScene* scene);
    /**
      * Returns a list of actions defined for the shape specified by the given @p style.
      */
    QList<QAction*> shapeActions(const QString & style) const;
    /**
      * Returns a list of names for the shapes provided by the logic circuit plugin.
      */
    QStringList shapes(const QString & group = QString::null, QAbstractDiagram* diagram = 0) const;
    /**
      * Returns the default properties for the shape specified by the given @p name.
      */
    QMap<QString,QVariant> defaultProperties(const QString & name) const;
    /**
      * Returns the (user-readable) title of the connector or shape specified by the given @p name.
      */
    QString title(const QString & name) const;
    /**
      * Returns the @p shape's tool tip.
      */
    QString toolTip(const QString & shape) const;
};

#endif // QLOGICCIRCUITPLUGIN_H

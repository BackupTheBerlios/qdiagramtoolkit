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
#include "qdiagramundocommand.h"

#include "qdiagrampluginloader.h"
#include <qdiagramshape.h>
#include "qabstractdiagramshapeconnector.h"
#include "qabstractdiagramgraphicsitem.h"

//QDiagramUndoCommand::QDiagramUndoCommand(QAbstractDiagram* diagram, const QString & plugin, QUndoCommand* parent) :
//    QUndoCommand(parent)
//{
//    cDiagram = diagram;
//    cPlugin = plugin;
//}

QDiagramUndoCommand::QDiagramUndoCommand(QAbstractDiagram* diagram, const QString & uuid, const QString & plugin, QUndoCommand* parent) :
    QUndoCommand(parent)
{
    m_diagram = diagram;
    m_uuid = uuid;
    m_plugin = plugin;
}

QDiagramUndoCommand::QDiagramUndoCommand(QAbstractDiagram* diagram, const QString & uuid, const QString & shape, const QString & plugin, QUndoCommand* parent) :
    QUndoCommand(parent)
{
    m_diagram = diagram;
    m_plugin = plugin;
    m_shape = shape;
    m_uuid = uuid;
}

QDiagramUndoCommand::QDiagramUndoCommand(QAbstractDiagram* diagram, const QString & uuid, const QString & shape, const QMap<QString,QVariant> & properties, const QString & plugin, QUndoCommand* parent) :
    QUndoCommand(parent)
{
    m_diagram = diagram;
    m_plugin = plugin;
    m_properties = properties;
    m_shape = shape;
    m_uuid = uuid;
    m_properties["plugin"] = plugin;
    m_properties["uuid"] = uuid;
}

QAbstractDiagram* QDiagramUndoCommand::diagram() const
{
    return m_diagram;
}

QString QDiagramUndoCommand::plugin() const
{
    return m_plugin;
}

QMap<QString,QVariant> QDiagramUndoCommand::properties() const
{
    return m_properties;
}

void QDiagramUndoCommand::setProperty(const QString & name, const QVariant & value)
{
	m_properties[name] = value;
}

void QDiagramUndoCommand::setProperties(const QMap<QString,QVariant> & properties)
{
    m_properties = properties;
}

QString QDiagramUndoCommand::shape() const
{
    return m_shape;
}

QString QDiagramUndoCommand::uuid() const
{
    return m_uuid;
}

QDiagramConnectShapesCommand::QDiagramConnectShapesCommand(QAbstractDiagram* diagram, QAbstractDiagramShapeConnector* connector, const QString & plugin, QUndoCommand* parent) :
    QDiagramUndoCommand(diagram, connector->uuid(), "connector", connector->properties(), plugin, parent)
{
    setText(QObject::tr("Connect Shape (%1)").arg(connector->property("style").toString()));
}

QDiagramConnectShapesCommand::QDiagramConnectShapesCommand(QAbstractDiagram* diagram, const QString & uuid, QAbstractDiagramShapeConnectionPoint* from, QAbstractDiagramShapeConnectionPoint* to, const QDiagramConnectorStyle & style, const QString & plugin, QUndoCommand* parent) :
    QDiagramUndoCommand(diagram, uuid, "connector", plugin, parent)
{
    QMap<QString,QVariant> mProperties(properties());

    mProperties["shapeAtStart"] = from->parentShape()->uuid();
    mProperties["pointAtStart"] = from->id();
    mProperties["shapeAtEnd"] = to->parentShape()->uuid();
    mProperties["pointAtEnd"] = to->id();
    mProperties["plugin"] = style.plugin();
    mProperties["style"] = style.shape();
    mProperties["itemType"] = "connector";
    mProperties["uuid"] = uuid;

    setProperties(mProperties);

    setText(QObject::tr("Add Connector (%1)").arg(uuid));
}

void QDiagramConnectShapesCommand::undo()
{
    QAbstractDiagramGraphicsItem* mItem = diagram()->findItemByUuid(uuid());
    if (mItem){
        mItem->blockUndoCommands(true);
        diagram()->removeItem(uuid());
    }
}

void QDiagramConnectShapesCommand::redo()
{
    diagram()->restoreItem(properties());
}

QDiagramChangePropertyCommand::QDiagramChangePropertyCommand(QAbstractDiagram* diagram, QDiagramShape* item, const QString & property, const QVariant & oldValue, const QVariant & newValue, QUndoCommand* parent) :
    QDiagramUndoCommand(diagram, item->uuid(), "default", parent)
{
    m_old = oldValue;
    m_name = property;
    m_new = newValue;
    setText(QObject::tr("Change Property %1").arg(m_name));
}

bool QDiagramChangePropertyCommand::mergeWith(const QUndoCommand* other)
{
    if (id() != other->id()){
        return false;
    }
    const QDiagramChangePropertyCommand* cmd = static_cast<const QDiagramChangePropertyCommand *>(other);
    if (cmd->uuid() != uuid() || cmd->name() != m_name){
        return false;
    }
    QAbstractDiagramGraphicsItem* item = diagram()->findItemByUuid(uuid());
    if (item == 0){
        return false;
    }
    m_new = cmd->m_new;
    setText(QObject::tr("Change Property %1").arg(m_name));
	
    return true;
}

QString QDiagramChangePropertyCommand::name() const
{
    return m_name;
}

void QDiagramChangePropertyCommand::undo()
{
    QAbstractDiagramGraphicsItem* mItem = diagram()->findItemByUuid(uuid());
    if (mItem){
        mItem->blockUndoCommands(true);
        mItem->setProperty(m_name, m_old);
        mItem->blockUndoCommands(false);
    }
}

void QDiagramChangePropertyCommand::redo()
{
    QAbstractDiagramGraphicsItem* mItem = diagram()->findItemByUuid(uuid());
    if (mItem){
        mItem->setProperty(m_name, m_new);
    }
}

QDiagramRemoveItemCommand::QDiagramRemoveItemCommand(QAbstractDiagramGraphicsItem *item, QUndoCommand *parent) :
    QDiagramUndoCommand(item->diagram(), item->uuid(), item->property("shape").toString(), item->properties(), item->property("plugin").toString(), parent)
{
    setText(QObject::tr("Remove %1").arg(uuid()));
}

QDiagramRemoveItemCommand::QDiagramRemoveItemCommand(QAbstractDiagram* diagram, const QString & uuid, const QString & shape, const QMap<QString,QVariant> & props, QUndoCommand* parent ) :
    QDiagramUndoCommand(diagram, uuid, shape, props, "default", parent)
{
    setText(QObject::tr("Remove %1").arg(shape));
}

void QDiagramRemoveItemCommand::undo()
{
    diagram()->restoreItem(properties());
}

void QDiagramRemoveItemCommand::redo()
{
    QAbstractDiagramGraphicsItem* item = diagram()->findItemByUuid(uuid());
    diagram()->takeItem(item);
    delete item;
}

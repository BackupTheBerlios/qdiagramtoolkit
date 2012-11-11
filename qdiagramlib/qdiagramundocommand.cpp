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

QDiagramUndoCommand::QDiagramUndoCommand(QAbstractDiagram* diagram, const QString & uuid, const QVariantMap & metaData, const QVariantMap & properties, QUndoCommand* parent) :
    QUndoCommand(parent)
{
	m_uuid = uuid;
	m_diagram = diagram;
	m_metaData = metaData;
	m_properties = properties;
}

QAbstractDiagram* QDiagramUndoCommand::diagram() const
{
    return m_diagram;
}

QVariantMap QDiagramUndoCommand::metaData() const
{
	return m_metaData;
}

QString QDiagramUndoCommand::plugin() const
{
    return m_plugin;
}

QMap<QString,QVariant> QDiagramUndoCommand::properties() const
{
    return m_properties;
}

void QDiagramUndoCommand::setMetaData(const QVariantMap & data)
{
	m_metaData = data;
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

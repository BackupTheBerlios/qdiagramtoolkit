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
#include "qdiagrammetadata.h"

#include "qdiagrammetaproperty.h"

QDiagramMetaData::QDiagramMetaData()
{
	m_parent = 0;
}

QDiagramMetaData::QDiagramMetaData(const QString & pluginName, const QString & itemType, const QString & itemClass)
{
	m_parent = 0;
    m_itemClass = itemClass;
	m_itemType = itemType;
	m_pluginName = pluginName;
}

QDiagramMetaData::QDiagramMetaData(const QString & pluginName, const QString & itemType, const QString & itemClass, QDiagramMetaData* parent)
{
	m_parent = parent;
    m_itemClass = itemClass;
	m_itemType = itemType;
	m_pluginName = pluginName;
}

bool QDiagramMetaData::addProperty(const QString & name, QDiagramToolkit::PropertyType type, bool readOnly)
{
    Q_FOREACH(QDiagramMetaProperty mProperty, m_properties){
        if (mProperty.name() == name){
            qWarning() << name << "exists";
            return false;
        }
    }
    m_properties.append(QDiagramMetaProperty(-1, name, type, readOnly));
    return true;
}

bool QDiagramMetaData::addProperty(const QString &name, QDiagramToolkit::PropertyType type, const QMap<int, QString> &pairs)
{
    Q_FOREACH(QDiagramMetaProperty mProperty, m_properties){
        if (mProperty.name() == name){
            qWarning() << name << "exists";
            return false;
        }
    }
    m_properties.append(QDiagramMetaProperty(-1, name, type, pairs));
    return true;
}

int QDiagramMetaData::indexOfProperty(const QString & name) const
{
    uint mIndex = 0;
    Q_FOREACH(QDiagramMetaProperty mProperty, m_properties){
        if (mProperty.name() == name){
            return mIndex;
        }
        mIndex++;
    }
    return -1;
}

QString QDiagramMetaData::itemClass() const
{
    return m_itemClass;
}

QString QDiagramMetaData::itemType() const
{
	return m_itemType;
}

QString QDiagramMetaData::pluginName() const
{
	return m_pluginName;
}

QDiagramMetaProperty QDiagramMetaData::property(int index) const
{
    if (index > -1 && index < m_properties.size()){
        return m_properties.at(index);
    }
    return QDiagramMetaProperty();
}

int QDiagramMetaData::propertyCount() const
{
    return m_properties.size();
}

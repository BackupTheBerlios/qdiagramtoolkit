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
#include "qdiagramgraphicsitemmetadata.h"

#include "qdiagramgraphicsitemmetaproperty.h"

QDiagramGraphicsItemMetaData::QDiagramGraphicsItemMetaData(const QString & name)
{
    m_propertyName = name;
}

bool QDiagramGraphicsItemMetaData::addProperty(const QString & name, QDiagramGraphicsItemMetaProperty::Type type, bool readOnly)
{
    Q_FOREACH(QDiagramGraphicsItemMetaProperty mProperty, m_properties){
        if (mProperty.name() == name){
            qWarning() << name << "exists";
            return false;
        }
    }
    m_properties.append(QDiagramGraphicsItemMetaProperty(name, type, readOnly));
    return true;
}

bool QDiagramGraphicsItemMetaData::addProperty(const QString &name, QDiagramGraphicsItemMetaProperty::Type type, const QMap<int, QString> &pairs)
{
    Q_FOREACH(QDiagramGraphicsItemMetaProperty mProperty, m_properties){
        if (mProperty.name() == name){
            qWarning() << name << "exists";
            return false;
        }
    }
    m_properties.append(QDiagramGraphicsItemMetaProperty(name, type, pairs));
    return true;
}

int QDiagramGraphicsItemMetaData::indexOfProperty(const QString & name) const
{
    uint mIndex = 0;
    Q_FOREACH(QDiagramGraphicsItemMetaProperty mProperty, m_properties){
        if (mProperty.name() == name){
            return mIndex;
        }
        mIndex++;
    }
    return -1;
}

QDiagramGraphicsItemMetaProperty QDiagramGraphicsItemMetaData::property(int index) const
{
    if (index > -1 && index < m_properties.size()){
        return m_properties.at(index);
    }
    return QDiagramGraphicsItemMetaProperty();
}

int QDiagramGraphicsItemMetaData::propertyCount() const
{
    return m_properties.size();
}

QString QDiagramGraphicsItemMetaData::shapeName() const
{
    return m_propertyName;
}

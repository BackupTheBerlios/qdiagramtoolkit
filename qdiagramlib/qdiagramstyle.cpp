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
#include "qdiagramstyle.h"

#include "json.h"

QDiagramStyle::QDiagramStyle()
{
}

QDiagramStyle::QDiagramStyle(const QString &name, const QString &plugin)
{
    m_properties["name"] = name;
    m_properties["plugin"] = plugin;
}

void QDiagramStyle::addProperty(const QString &name, const QVariant &value)
{
    if (name != "name" && name != "plugin"){
        m_properties[name] = value;
    }
}

QString QDiagramStyle::id() const
{
    return QString("%1@%2").arg(m_properties.value("name").toString()).arg(m_properties.value("plugin").toString());
}

bool QDiagramStyle::isValid() const
{
    return !m_properties.value("name").isNull() && !m_properties.value("plugin").isNull();
}

QString QDiagramStyle::key(int index) const
{
    if (index < m_properties.size()){
        return m_properties.keys().at(index);
    }
    return QString::null;
}

QString QDiagramStyle::name() const
{
    return m_properties.value("name").toString();
}

int QDiagramStyle::propertyCount() const
{
    return m_properties.size();
}

QVariant QDiagramStyle::property(const QString &name) const
{
    return m_properties.value(name);
}

QVariant QDiagramStyle::property(int index) const
{
    if (index < m_properties.size()){
        return m_properties.value(m_properties.keys().at(index));
    }
    return QVariant();
}

QByteArray QDiagramStyle::serialize() const
{
    QtJson::Json j;

    return j.serialize(m_properties);
}

bool QDiagramStyle::setProperty(const QString &name, const QVariant &value)
{
    if (!m_properties.contains(name)){
        return false;
    }
    m_properties[name] = value;
    return true;
}

QVariant QDiagramStyle::value(const QString &name) const
{
    return m_properties.value(name);
}

QVariant QDiagramStyle::value(int index) const
{
    if (index < m_properties.size()){
        return m_properties.value(m_properties.keys().at(index));
    }
    return QVariant();
}

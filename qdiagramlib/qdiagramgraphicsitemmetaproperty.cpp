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
#include "qdiagramgraphicsitemmetaproperty.h"

QDiagramGraphicsItemMetaProperty::QDiagramGraphicsItemMetaProperty()
{
    m_dynamic = false;
    m_readOnly = true;
	m_objectType = -1;
    m_type = QDiagramGraphicsItemMetaProperty::Invalid;
}

QDiagramGraphicsItemMetaProperty::QDiagramGraphicsItemMetaProperty(int objectType, const QString & name, QDiagramGraphicsItemMetaProperty::Type type, bool readOnly)
{
    m_dynamic = false;
    m_name = name;
	m_objectType = objectType;
    m_readOnly = readOnly;
    m_type = type;
}

QDiagramGraphicsItemMetaProperty::QDiagramGraphicsItemMetaProperty(int objectType, const QString &name, Type type, const QMap<int, QString> &pairs)
{
    m_dynamic = false;
    m_name = name;
	m_objectType = objectType;
    m_readOnly = false;
    m_type = QDiagramGraphicsItemMetaProperty::Enumeration;
    if (type == Enumeration){
        m_enum = QDiagramGraphicsItemMetaEnum(pairs);
    }
    if (type == Flag){
        m_flag = QDiagramGraphicsItemMetaFlag(pairs);
    }
}

QDiagramGraphicsItemMetaEnum QDiagramGraphicsItemMetaProperty::enumerator() const
{
    return m_enum;
}

QDiagramGraphicsItemMetaFlag QDiagramGraphicsItemMetaProperty::flag() const
{
    return m_flag;
}

bool QDiagramGraphicsItemMetaProperty::isDynamicProperty() const
{
    return m_dynamic;
}

bool QDiagramGraphicsItemMetaProperty::isReadOnly() const
{
    return m_readOnly;
}

bool QDiagramGraphicsItemMetaProperty::isValid() const
{
    return !m_name.isNull();
}

QStringList QDiagramGraphicsItemMetaProperty::keys() const
{
    QStringList k;
    if (m_type == TextStyle){
        k << "family" << "size";
    }
    return k;
}

QString QDiagramGraphicsItemMetaProperty::name() const
{
    return m_name;
}

int QDiagramGraphicsItemMetaProperty::objectType() const
{
	return m_objectType;
}

QDiagramGraphicsItemMetaProperty::Type QDiagramGraphicsItemMetaProperty::type() const
{
    return m_type;
}

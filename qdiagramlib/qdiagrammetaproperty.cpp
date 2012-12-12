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
#include "qdiagrammetaproperty.h"
#include "qdiagramproperty.h"

QMap<QString, QMap<QString,int>> s_enums;

QDiagramMetaProperty::QDiagramMetaProperty()
{
    m_dynamic = false;
    m_readOnly = true;
	m_objectType = -1;
    m_type = QDiagramToolkit::PropertyTypeInvalid;
}

QDiagramMetaProperty::QDiagramMetaProperty(int objectType, const QString & name, QDiagramToolkit::PropertyType type, bool readOnly)
{
    m_dynamic = false;
    m_name = name;
	m_objectType = objectType;
    m_readOnly = readOnly;
    m_type = type;

	QDiagramPropertyBag b = QDiagramProperty::propertyBag(type);
	for (QDiagramPropertyBag::Iterator it = b.begin(); it != b.end(); ++it){

		m_properties.append(QDiagramMetaProperty(-1, it.key(), static_cast<QDiagramToolkit::PropertyType>(it.value()), readOnly));
	}
}

QDiagramMetaProperty::QDiagramMetaProperty(int objectType, const QString &name, QDiagramToolkit::PropertyType type, const QMap<int, QString> &pairs)
{
    m_dynamic = false;
    m_name = name;
	m_objectType = objectType;
    m_readOnly = false;
    m_type = QDiagramToolkit::Enumeration;
    if (type == QDiagramToolkit::Enumeration){
        m_enum = QDiagramMetaEnum(pairs);
    }
    if (type == QDiagramToolkit::Flag){
        m_flag = QDiagramMetaFlag(pairs);
    }
}

QDiagramMetaEnum QDiagramMetaProperty::enumerator() const
{
	if (m_enum.isValid()){
		return m_enum;
	}
	return QDiagramMetaEnum::defaultEnum(m_type);
}

QDiagramMetaFlag QDiagramMetaProperty::flag() const
{
	if (m_flag.isValid()){
		return m_flag;
	}
	return QDiagramMetaFlag::defaultFlag(m_type);
}

int QDiagramMetaProperty::indexOf(const QString & name) const
{
	for (int i = 0; i < m_properties.size(); i++){
		if (m_properties.at(i).name() == name){
			return i;
		}
	}
	return -1;
}

bool QDiagramMetaProperty::isDynamic() const
{
    return m_dynamic;
}

bool QDiagramMetaProperty::isEnumeration(QDiagramToolkit::PropertyType type) const
{
	if (type == QDiagramToolkit::PenJoinStyle){
		return true;
	}
	return false;
}

bool QDiagramMetaProperty::isFlag(QDiagramToolkit::PropertyType type)
{
	if (type == QDiagramToolkit::Alignment){
		return true;
	} else if (type == QDiagramToolkit::Orientation){
		return true;
	}
	return false;
}

bool QDiagramMetaProperty::isFlag() const
{
	return isFlag(m_type);
}

bool QDiagramMetaProperty::isReadOnly() const
{
    return m_readOnly;
}

bool QDiagramMetaProperty::isValid() const
{
    return !m_name.isNull();
}

QStringList QDiagramMetaProperty::keys() const
{
    QStringList k;
	if (m_type == QDiagramToolkit::TextStyle){
        k << "family" << "size";
    }
    return k;
}

QString QDiagramMetaProperty::name() const
{
    return m_name;
}

int QDiagramMetaProperty::objectType() const
{
	return m_objectType;
}

QDiagramMetaProperty QDiagramMetaProperty::property(int index) const
{
	if (index < m_properties.count()){
		return m_properties.at(index);
	}
	return QDiagramMetaProperty();
}

int QDiagramMetaProperty::propertyCount() const
{
	return m_properties.size();
}

QDiagramToolkit::PropertyType QDiagramMetaProperty::type() const
{
    return m_type;
}

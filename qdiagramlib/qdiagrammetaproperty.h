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
#ifndef QDIAGRAMMETAPROPERTY_H
#define QDIAGRAMMETAPROPERTY_H

#include "qdiagramlib_global.h"

#include <qdiagramtoolkit.h>
#include <qdiagrammetaenum.h>
#include <qdiagrammetaflag.h>

//! The qdiagrammetaproperty.h class provides meta-data about a property.
/**
  * Property meta-data is obtained from an digram graphic item's meta-data.
  *
  * A property has a name() and a type(), as well as various attributes that specify its behavior: isReadOnly(), isStorable()
  *
  * Property Pen
  * color
  * width
  * style
  */
class QDIAGRAMLIBSHARED_EXPORT QDiagramMetaProperty
{
public:
    QDiagramMetaProperty();
    /**
      *
      */
    QDiagramMetaEnum enumerator() const;
    /**
      *
      */
    QDiagramMetaFlag flag() const;
    /**
      * Finds property @p name and returns its index; otherwise returns -1.
      */
	int indexOf(const QString & name) const;
    /**
      * Returns true if the property has been added during run-time. Otherwise false.
      */
    bool isDynamic() const;
	/**
	 * Returns true if the given @p type is an enumeration. Otherwise false.
	 */
	bool isEnumeration(QDiagramToolkit::PropertyType type) const;

	bool isFlag(QDiagramToolkit::PropertyType type) const;
    /**
      * Returns true if the property is read-only. Otherwise false.
      */
    bool isReadOnly() const;
    /**
      * Returns true if this property is valid (readable); otherwise returns false.
      */
    bool isValid() const;
    /**
      *
      */
    QStringList keys() const;
    /**
      * Returns this property's name.
      */
    QString name() const;
	/**
	 *
	 */
	int objectType() const;

	QDiagramMetaProperty property(int index) const;

	int propertyCount() const;
    /**
      * Returns the storage type of the value stored in the property.
      */
	QDiagramToolkit::PropertyType type() const;
private:
    friend class QDiagramMetaData;
    QDiagramMetaProperty(int objectType, const QString & name, QDiagramToolkit::PropertyType type, bool readOnly);
    QDiagramMetaProperty(int objectType, const QString & name, QDiagramToolkit::PropertyType type, const QMap<int,QString> & pairs);

    bool m_dynamic;
    QString m_name;
    bool m_readOnly;
    bool m_runTime;
    QDiagramToolkit::PropertyType m_type;
	int m_objectType;
    QDiagramMetaEnum m_enum;
    QDiagramMetaFlag m_flag;

	QList<QDiagramMetaProperty> m_properties;
};

Q_DECLARE_METATYPE(QDiagramMetaProperty)
Q_DECLARE_METATYPE(QDiagramMetaProperty*)

#endif // QDIAGRAMMETAPROPERTY_H

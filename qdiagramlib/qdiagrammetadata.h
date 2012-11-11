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
#ifndef QDIAGRAMMETADATA_H
#define QDIAGRAMMETADATA_H

#include "qdiagramlib_global.h"

#include <qdiagrammetaproperty.h>

#include <qdiagramlinestyle.h>
#include <qdiagramgraphicsitemshadow.h>
#include <qdiagramtextstyle.h>

//! The QDiagramMetaData class contains meta-data about QDiagramGraphicsItems.
class QDIAGRAMLIBSHARED_EXPORT QDiagramMetaData
{
public:
	QDiagramMetaData();
    /**
      * Finds property name and returns its index; otherwise returns -1.
      */
    int indexOfProperty(const QString & name) const;

    QString itemClass() const;

	QString itemType() const;
    /**
      * Returns the meta-data for the property with the given index. If no such property exists, a null QDiagramMetaProperty is returned.
      */
    QDiagramMetaProperty property(int index) const;
    /**
      * Returns the number of properties in this class.
      */
    int propertyCount() const;
	/**
	 * Returns the name of the plugin.
	 */
	QString pluginName() const;
private:
	QDiagramMetaData(const QString & pluginName, const QString & itemType, const QString & itemClass, QDiagramMetaData* parent);

	friend class QAbstractDiagramGraphicsItem;
    QDiagramMetaData(const QString & pluginName, const QString & itemType, const QString & itemClass);

    bool addProperty(const QString & name, QDiagramToolkit::PropertyType type, bool readOnly);

    bool addProperty(const QString & name, QDiagramToolkit::PropertyType type, const QMap<int,QString> & pairs);

	static QString toString(Qt::PenStyle style) ;

    QString m_itemClass;

	QString m_itemType;

	QString m_pluginName;

	QDiagramMetaData* m_parent;

    QList<QDiagramMetaProperty> m_properties;
};

Q_DECLARE_METATYPE(QDiagramMetaData)
Q_DECLARE_METATYPE(QDiagramMetaData*)

#endif // QDIAGRAMMETADATA_H

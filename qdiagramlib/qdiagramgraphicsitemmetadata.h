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
#ifndef QDIAGRAMGRAPHICSITEMMETADATA_H
#define QDIAGRAMGRAPHICSITEMMETADATA_H

#include "qdiagramlib_global.h"

#include "qdiagramgraphicsitemmetaproperty.h"

//! The QDiagramGraphicsItemMetaData class contains meta-data about QDiagramGraphicsItems.
class QDIAGRAMLIBSHARED_EXPORT QDiagramGraphicsItemMetaData
{
public:
    /**
      * Finds property name and returns its index; otherwise returns -1.
      */
    int indexOfProperty(const QString & name) const;
    /**
      * Returns the meta-data for the property with the given index. If no such property exists, a null QDiagramGraphicsItemMetaProperty is returned.
      */
    QDiagramGraphicsItemMetaProperty property(int index) const;
    /**
      * Returns the number of properties in this class.
      */
    int propertyCount() const;

    QString shapeName() const;
private:
    friend class QAbstractDiagramGraphicsItem;
    QDiagramGraphicsItemMetaData(const QString & name);

    bool addProperty(const QString & name, QDiagramGraphicsItemMetaProperty::Type type, bool readOnly);

    bool addProperty(const QString & name, QDiagramGraphicsItemMetaProperty::Type type, const QMap<int,QString> & pairs);

    QString m_propertyName;

    QList<QDiagramGraphicsItemMetaProperty> m_properties;
};

#endif // QDIAGRAMGRAPHICSITEMMETADATA_H

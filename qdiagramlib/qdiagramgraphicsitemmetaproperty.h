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
#ifndef QDIAGRAMGRAPHICSITEMMETAPROPERTY_H
#define QDIAGRAMGRAPHICSITEMMETAPROPERTY_H

#include "qdiagramlib_global.h"
#include "qdiagramgraphicsitemmetaenum.h"
#include "qdiagramgraphicsitemmetaflag.h"

//! The QDiagramGraphicsItemMetaProperty class provides meta-data about a property.
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
class QDIAGRAMLIBSHARED_EXPORT QDiagramGraphicsItemMetaProperty
{
public:
    /**
      * This enum type defines the types of variable that a QDiagramGraphicsItemMetaProperty can contain.
      */
    enum Type {
        Invalid, /*!< no type. */
        Angle, /*!< an angle value */
        Bool, /*!< a boolean value */
        Brush, /*!< a QBrush */
        Color, /*!< a QColor. */
        Double, /*!< a Double. */
        EndOfLineStyle,
        Enumeration,
        Flag,
        Int, /*!< an Int. */
        LineStyle, /*!< a line style */
        Pen,
        Percent, /*!< a percentage value */
        Point, /*!< a point */
        Rect, /*!< a QRectF */
        Shadow, /*<! a shadow */
        String, /*!< a QString. */
        Text,
        TextStyle, /*!< a text style */
        UUID /*!< an UUID. */
    };
    /**
      *
      */
    QDiagramGraphicsItemMetaEnum enumerator() const;
    /**
      *
      */
    QDiagramGraphicsItemMetaFlag flag() const;
    /**
      * Returns true if the property has been added during run-time. Otherwise false.
      */
    bool isDynamicProperty() const;
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
      * Returns the storage type of the value stored in the property.
      */
    Type type() const;
private:
    friend class QDiagramGraphicsItemMetaData;
    QDiagramGraphicsItemMetaProperty();
    QDiagramGraphicsItemMetaProperty(const QString & name, QDiagramGraphicsItemMetaProperty::Type type, bool readOnly);
    QDiagramGraphicsItemMetaProperty(const QString & name, QDiagramGraphicsItemMetaProperty::Type type, const QMap<int,QString> & pairs);

    bool m_dynamic;
    QString m_name;
    bool m_readOnly;
    bool m_runTime;
    Type m_type;
    QDiagramGraphicsItemMetaEnum m_enum;
    QDiagramGraphicsItemMetaFlag m_flag;
};

#endif // QDIAGRAMGRAPHICSITEMMETAPROPERTY_H

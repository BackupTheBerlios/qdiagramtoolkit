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
#ifndef QDIAGRAMPROPERTY_H
#define QDIAGRAMPROPERTY_H

#include "qdiagramlib_global.h"

#include <qdiagramconnectionpoint.h>
#include <qdiagrammetadata.h>
#include <qdiagramlinestyle.h>
#include <qdiagramgraphicsitemshadow.h>

class sdDiagramProperty;
class QAbstractDiagramGraphicsItem;

typedef QMap<QString, int> QDiagramPropertyBag;
typedef QMap<int,QString> QDiagramPropertyEnumMap;

class QDIAGRAMLIBSHARED_EXPORT QDiagramProperty
{
public:
	/**
	 * Construtcs an invalid QDiagramProperty.
	 */
	QDiagramProperty();
	/**
	 *
	 */
	QDiagramProperty(QDiagramToolkit::PropertyType type, const QVariant & value);
	/**
	 * Construtcs a copy of @p other.
	 */
	QDiagramProperty(const QDiagramProperty & other);
	/**
	 * Destroys this QDiagramProperty.
	 */
	~QDiagramProperty();

	static QVariant defaultValue(QDiagramToolkit::PropertyType type);

	static QDiagramPropertyEnumMap enumValues(QDiagramToolkit::PropertyType type);

	bool isValid() const;

	QAbstractDiagramGraphicsItem* item() const;

	QDiagramMetaData* metaData() const;

	QDiagramProperty property(const QString & name) const;

	static QDiagramPropertyBag propertyBag(QDiagramToolkit::PropertyType type);

	void setValue(const QVariant & value);

	bool toBool() const;

	QBrush toBrush() const;

	Qt::BrushStyle toBrushStyle() const;

	QColor toColor() const;

	QDiagramConnectionPoint toConnectionPoint() const;

	double toDouble() const;

	QFont toFont() const;

	int toInt() const;

	QDiagramLineStyle toLineStyle() const;

	QVariantMap toMap() const;

	static QVariantMap toMap(const QVariant & v);
	/**
	 * Returns the property as a QPen if the property has type() Point; otherwise returns a null QPen.
	 */
	QPen toPen() const;

	Qt::PenStyle toPenStyle() const;
	/**
	 * Returns the property as a QPointF if the property has type() Point; otherwise returns a null QPointF.
	 */
	QPointF toPointF() const;
	/**
	 * Returns the property as a QRectF if the property has type() Point; otherwise returns a null QRectF.
	 */
	QRectF toRectF() const;

	QDiagramGraphicsItemShadow toShadow() const;

	QSizeF toSizeF() const;

	QString toString() const;

	QDiagramToolkit::PropertyType type() const;

	QVariant value() const;

	static QVariant fromVariant(QDiagramToolkit::PropertyType type, const QVariant & value);
	/**
	 * Returns true if other is equal to this engineering element; otherwise returns false.
	 */
    bool operator==(const QString & other) const;
	/**
	 * Returns true if other is not equal to this engineering element; otherwise returns false.
	 */
    bool operator!=(const QString & other) const;

	QDiagramProperty & operator =( const QDiagramProperty & other );

	static bool cast_helper(const QDiagramProperty & p, QDiagramToolkit::PropertyType t, void* ptr);

	static Qt::BrushStyle toBrushStyle(const QString & text);

	static Qt::PenStyle toPenStyle(const QString & text);

	static QString toString(Qt::PenJoinStyle style);

	static QString toString(Qt::PenStyle style);
	
	static QString toString(Qt::BrushStyle style);
private:
	friend class QAbstractDiagramGraphicsItem;

	QDiagramProperty(const QAbstractDiagramGraphicsItem* item, int index);

	QDiagramProperty(sdDiagramProperty* other);
	QDiagramProperty(QAbstractDiagramGraphicsItem* i, int p, int c);

	static bool cast_helper(const QVariant & v, QDiagramToolkit::PropertyType t, void* ptr);

	QExplicitlySharedDataPointer<sdDiagramProperty> d;
};

template<typename T> inline QDiagramToolkit::PropertyType qdiagramPropertyType()
{
	T t;
	if (typeid(t) == typeid(bool)){
		return QDiagramToolkit::Bool;
	} else if (typeid(t) == typeid(double)){
		return QDiagramToolkit::Double;
	} else if (typeid(t) == typeid(QBrush)){
		return QDiagramToolkit::Brush;
	} else if (typeid(t) == typeid(QColor)){
		return QDiagramToolkit::Color;
	} else if (typeid(t) == typeid(QDiagramConnectionPoint)){
		return QDiagramToolkit::ConnectionPoint;
	} else if (typeid(t) == typeid(QFont)){
		return QDiagramToolkit::Font;
	} else if (typeid(t) == typeid(QDiagramLineStyle)){
		return QDiagramToolkit::LineStyle;
	} else if (typeid(t) == typeid(QPen)){
		return QDiagramToolkit::Pen;
	} else if (typeid(t) == typeid(QPointF)){
		return QDiagramToolkit::Point;
	} else if (typeid(t) == typeid(QRectF)){
		return QDiagramToolkit::Rect;
	} else if (typeid(t) == typeid(QDiagramGraphicsItemShadow)){
		return QDiagramToolkit::Shadow;
	} else if (typeid(t) == typeid(QSizeF)){
		return QDiagramToolkit::Size;
	} else if (typeid(t) == typeid(QString)){
		return QDiagramToolkit::String;
	}
	return QDiagramToolkit::PropertyTypeInvalid;
}

template<typename T> inline T qdiagramproperty_cast(QDiagramProperty & p)
{
	T t;
	QDiagramToolkit::PropertyType type = qdiagramPropertyType<T>();
	if (QDiagramProperty::cast_helper(p, type, &t)){
		return t;
	}
	return T();
}

Q_DECLARE_METATYPE(QDiagramProperty)
Q_DECLARE_METATYPE(QDiagramProperty*)

#endif // QDIAGRAMPROPERTY_H


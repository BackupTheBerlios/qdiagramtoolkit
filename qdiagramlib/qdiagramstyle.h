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
#ifndef QDIAGRAMSTYLE_H
#define QDIAGRAMSTYLE_H

#include <qdiagramlib_global.h>

#include <qdiagramtoolkit.h>
#include <qdiagrammetaproperty.h>

class QDIAGRAMLIBSHARED_EXPORT QDiagramStyle
{
public:
 //   /**
 //     * This enum type defines the types of variable that a QDiagramMetaProperty can contain.
 //     */
 //   enum PropertyType {
 //       Invalid, /*!< no type. */
	//	Alignment, 
 //       Bool, /*!< a boolean value */
 //       Brush, /*!< a QBrush */
 //       Color, /*!< a QColor. */
 //       Double, /*!< a Double. */
 //       Int, /*!< an Int. */
 //       Pen,
 //       String /*!< a QString. */
	//};

    QString id() const;

    bool isValid() const;

    QString key(int index) const;

    QString name() const;

    int propertyCount() const;

    QVariant property(const QString & name) const;

    QVariant property(int index) const;

    QByteArray serialize() const;

    bool setProperty(const QString & name, const QVariant & value);

	QDiagramToolkit::PropertyType type(const QString & name) const;

    QVariant value(const QString & name) const;

    QVariant value(int index) const;
protected:
    QDiagramStyle();
    QDiagramStyle(const QString & name, const QString & plugin);

    void addProperty(const QString & name, QDiagramToolkit::PropertyType type, const QVariant & value);
private:
	QString toString(Qt::PenStyle style) const;

    QVariantMap m_properties;
	QVariantMap m_types;
};

#endif // QDIAGRAMSTYLE_H

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
#ifndef QDIAGRAMMETAENUM_H
#define QDIAGRAMMETAENUM_H

#include <qdiagramlib_global.h>
#include <qdiagramtoolkit.h>

#include <QMap>

//! The QDiagramMetaEnum class provides meta-data about an enumerator.
class QDIAGRAMLIBSHARED_EXPORT QDiagramMetaEnum
{
public:
    QDiagramMetaEnum();

	static int fromString(QDiagramToolkit::PropertyType type, const QString & text);

	QIcon icon(int index) const;

	int indexOf(int value) const;
	/**
	 * Returns true if this enum is valid; otherwise returns false.
	 */
    bool isValid() const;
	/**
	 * Returns the key with the given @p index, or QString::null if no such key exists.
	 */
    QString key(int index) const;
	/**
	 * Returns the number of keys.
	 */
    int keyCount() const;
	 
	static QString toString(QDiagramToolkit::PropertyType type, int value);

    int value(int index) const;
private:
    friend class QDiagramMetaProperty;
	QDiagramMetaEnum(const QMap<int,QString> pairs, const QMap<int,QIcon> icons = QMap<int,QIcon>());

	static QDiagramMetaEnum defaultEnum(QDiagramToolkit::PropertyType type);

    QMap<int,QString> m_pairs;
	QMap<int,QIcon> m_icons;
};

#endif // QDIAGRAMMETAENUM_H

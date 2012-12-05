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
#ifndef QDIAGRAMGRAPHICSITEMMETAFLAG_H
#define QDIAGRAMGRAPHICSITEMMETAFLAG_H

#include <qdiagramlib_global.h>
#include <qdiagramtoolkit.h>

//! The QDiagramMetaFlag class provides meta-data about a flag
class QDIAGRAMLIBSHARED_EXPORT QDiagramMetaFlag
{
public:
	/**
	 * Constructs an invalid QDiagramMetaFlag.
	 */
    QDiagramMetaFlag();
	/**
	 * Returns true if the meta flag object contains a valid (not empty) definition. Otherwise false.
	 */
    bool isValid() const;
	/**
	 * Return the number of keys.
	 */
    int keyCount() const;
	/**
	 * Returns the key at the specified @p index or QString::null is not key exists at @p index.
	 */
    QString key(int index) const;
	/**
	 * Returns a map of flag masks defined for the given @p type.
	 */
	static QMap<QString, int> masks(QDiagramToolkit::PropertyType type);
	/**
	 * Returns a list of keys matching the given @p value.
	 */
	QStringList matchingKeys(int value) const;
	/**
	 * Returns the value at the given @p index or 0 if no value exists at @p index.
	 */
    int value(int index) const;
	/**
	 * Returns the value for the given @p key.
	 */
	int value(const QString & key) const;
private:
    friend class QDiagramMetaProperty;
    QDiagramMetaFlag(const QMap<int, QString> &flags);

	static QDiagramMetaFlag defaultFlag(QDiagramToolkit::PropertyType type);

    QMap<int,QString> m_flags;
};

#endif // QDIAGRAMGRAPHICSITEMMETAFLAG_H

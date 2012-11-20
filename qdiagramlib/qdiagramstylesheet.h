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
#ifndef QDIAGRAMSTYLESHEET_H
#define QDIAGRAMSTYLESHEET_H

#include <QPointer>

#include <qdiagramlib_global.h>

#include <qabstractdiagram.h>
#include <qdiagramconnectorstyle.h>
#include <qdiagramlinestyle.h>

class QAbstractDiagramPlugin;

class QDIAGRAMLIBSHARED_EXPORT QDiagramStyleSheet
{
public:
	QDiagramStyleSheet(QAbstractDiagram* diagram = 0);
	~QDiagramStyleSheet();

	QDiagramConnectorStyle connector(const QString & plugin, const QString & name) const;

	QList<QDiagramConnectorStyle> connectors() const;

	QDiagramLineStyle lineStyle(const QString & name) const;
	/**
	 *
	 */
	QDiagramLineStyles lineStyles() const;
private:
	QPointer<QAbstractDiagram> m_diagram;
};

Q_DECLARE_METATYPE(QDiagramStyleSheet*)
#endif // QDIAGRAMSTYLESHEET_H

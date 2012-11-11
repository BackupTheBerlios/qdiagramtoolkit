/******************************************************************************
** Copyright (C) 2012 Martin Hoppe martin@2x2hoppe.de
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
#ifndef QDIAGRAMCONNECTIONPOINT_H
#define QDIAGRAMCONNECTIONPOINT_H

#include <QMetaType>

#include "qdiagramlib_global.h"

class QDIAGRAMLIBSHARED_EXPORT QDiagramConnectionPoint
{
public:
	QDiagramConnectionPoint();
	QDiagramConnectionPoint(const QString & uuid, const QString & id);
	~QDiagramConnectionPoint();

	QString id() const;

	bool isValid() const;

	void setId(const QString & id);

	void setUUID(const QString & uuid);

	QString uuid() const;
private:
	QString m_uuid;
	QString m_id;
};

Q_DECLARE_METATYPE(QDiagramConnectionPoint)

#endif // QDIAGRAMCONNECTIONPOINT_H
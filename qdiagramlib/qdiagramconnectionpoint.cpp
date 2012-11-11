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
#include "stdafx.h"
#include "qdiagramconnectionpoint.h"

QDiagramConnectionPoint::QDiagramConnectionPoint()
{
}

QDiagramConnectionPoint::QDiagramConnectionPoint(const QString & uuid, const QString & id)
{
	m_uuid = uuid;
	m_id = id;
}

QDiagramConnectionPoint::~QDiagramConnectionPoint()
{
}

QString QDiagramConnectionPoint::id() const
{
	return m_id;
}

bool QDiagramConnectionPoint::isValid() const
{
	return !m_uuid.isEmpty() && !m_id.isEmpty();
}

void QDiagramConnectionPoint::setId(const QString & id)
{
	m_id = id;
}

void QDiagramConnectionPoint::setUUID(const QString & uuid)
{
	m_uuid = uuid;
}

QString QDiagramConnectionPoint::uuid() const
{
	return m_uuid;
}

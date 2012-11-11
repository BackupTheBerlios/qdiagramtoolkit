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
#include "stdafx.h"
#include "qdiagrammetaenum.h"

QDiagramMetaEnum::QDiagramMetaEnum()
{
}

QDiagramMetaEnum::QDiagramMetaEnum(const QMap<int, QString> pairs, const QMap<int,QIcon> icons)
{
    m_pairs = pairs;
	m_icons = icons;
}

QIcon QDiagramMetaEnum::icon(int index) const
{
	return m_icons.value(index);
}

bool QDiagramMetaEnum::isValid() const
{
    return !m_pairs.isEmpty();
}

QString QDiagramMetaEnum::key(int index) const
{
    if (index < m_pairs.size()){
        return m_pairs.values().at(index);
    }
    return QString::null;
}

int QDiagramMetaEnum::keys() const
{
    return m_pairs.keys().size();
}

int QDiagramMetaEnum::value(int index) const
{
    if (index < m_pairs.size()){
        return m_pairs.keys().at(index);
    }
    return -1;
}

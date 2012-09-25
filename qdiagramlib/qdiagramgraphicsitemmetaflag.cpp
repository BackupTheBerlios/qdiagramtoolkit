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
#include "qdiagramgraphicsitemmetaflag.h"

QDiagramGraphicsItemMetaFlag::QDiagramGraphicsItemMetaFlag()
{
}

QDiagramGraphicsItemMetaFlag::QDiagramGraphicsItemMetaFlag(const QMap<int, QString> &flags)
{
    m_flags = flags;
}

int QDiagramGraphicsItemMetaFlag::keyCount() const
{
    return m_flags.keys().size();
}

QString QDiagramGraphicsItemMetaFlag::key(int index) const
{
    if (index < m_flags.values().size()){
        return m_flags.values().at(index);
    }
    return QString::null;
}

bool QDiagramGraphicsItemMetaFlag::isValid() const
{
    return !m_flags.isEmpty();
}

int QDiagramGraphicsItemMetaFlag::value(int index) const
{
    if (index < m_flags.keys().size()){
        return m_flags.keys().at(index);
    }
    return 0;
}

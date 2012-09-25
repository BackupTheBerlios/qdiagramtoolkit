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
#include "qdiagramendoflinestyle.h"

QDiagramEndOfLineStyle::QDiagramEndOfLineStyle() :
    QDiagramStyle()
{
    addProperty("width", 10.0);
}

QDiagramEndOfLineStyle::QDiagramEndOfLineStyle(const QString &caption, const QString &name, const QString &plugin) :
    QDiagramStyle(name, plugin)
{
    m_caption = caption;
    addProperty("width", 10.0);
}

QDiagramEndOfLineStyle::QDiagramEndOfLineStyle(const QIcon &icon, const QString &caption, const QString &name, const QString &plugin) :
    QDiagramStyle(name, plugin)
{
    m_caption = caption;
    addProperty("width", 10.0);
    m_icon = icon;
}

QString QDiagramEndOfLineStyle::caption() const
{
    return m_caption;
}

QIcon QDiagramEndOfLineStyle::icon() const
{
    return m_icon;
}

qreal QDiagramEndOfLineStyle::width() const
{
    return property("width").toDouble();
}

void QDiagramEndOfLineStyle::setWidth(qreal width)
{
    setProperty("width", width);
}

QDebug operator<<(QDebug dbg, const QDiagramEndOfLineStyle &s)
{
    dbg.nospace() << "QDiagramLineEndStyle(id:" << s.id() << ", width:" << s.width() << ")";

    return dbg.maybeSpace();
}

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
#include "qdiagramlinestyle.h"

QDiagramLineStyle::QDiagramLineStyle()
{
}

QDiagramLineStyle::QDiagramLineStyle(const QString &plugin, const QString &name, const QString &title, const QPen &pen, const QIcon &icon)
{
    m_icon = icon;
    m_name = name;
    m_pen = pen;
    m_plugin = plugin;
    m_title = title;
}

QDiagramLineStyle::~QDiagramLineStyle()
{
}

QColor QDiagramLineStyle::color() const
{
    return m_pen.color();
}

QIcon QDiagramLineStyle::icon() const
{
    return m_icon;
}

QString QDiagramLineStyle::id() const
{
    return QString("%1@%2").arg(m_name).arg(m_plugin);
}

bool QDiagramLineStyle::isValid() const
{
    return !m_name.isNull() && !m_plugin.isNull();
}

QString QDiagramLineStyle::name() const
{
    return m_name;
}

QPen QDiagramLineStyle::pen() const
{
    return m_pen;
}

QString QDiagramLineStyle::plugin() const
{
    return m_plugin;
}

void QDiagramLineStyle::setColor(const QColor &color)
{
    m_pen.setColor(color);
}

void QDiagramLineStyle::setWidth(qreal width)
{
    m_pen.setWidthF(width);
}

QString QDiagramLineStyle::title() const
{
    return m_title;
}

qreal QDiagramLineStyle::width() const
{
    return m_pen.widthF();
}

QDebug operator<<(QDebug dbg, const QDiagramLineStyle &s)
{
    dbg.nospace() << "QDiagramLineStyle(color:" << s.color().name() << ", width:" << s.width() << ")";

    return dbg.maybeSpace();
}

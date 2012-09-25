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
#include "qdiagramconnectorstyle.h"

QDiagramConnectorStyle::QDiagramConnectorStyle()
{
}

QDiagramConnectorStyle::QDiagramConnectorStyle(const QString & plugin, const QIcon & icon, const QString & name, const QString & style)
{
    m_icon = icon;
    m_name = name;
    m_plugin = plugin;
    m_style = style;
}

QDiagramConnectorStyle::~QDiagramConnectorStyle()
{

}

QIcon QDiagramConnectorStyle::icon() const
{
    return m_icon;
}

bool QDiagramConnectorStyle::isNull() const
{
    return m_name.isNull() || m_plugin.isNull();
}

QString QDiagramConnectorStyle::name() const
{
    return m_name;
}

QString QDiagramConnectorStyle::plugin() const
{
    return m_plugin;
}

QString QDiagramConnectorStyle::shape() const
{
    return m_style;
}

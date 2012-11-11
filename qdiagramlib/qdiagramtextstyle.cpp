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
#include "qdiagramtextstyle.h"

QDiagramTextStyle::QDiagramTextStyle() :
    QDiagramStyle()
{
    m_alignment = Qt::AlignCenter;

    addProperty("alignment", QDiagramToolkit::Alignment, Qt::AlignCenter);
    addProperty("family", QDiagramToolkit::String, "Arial");
    addProperty("size", QDiagramToolkit::Int, 10);
	addProperty("bold", QDiagramToolkit::Bool, false);
	addProperty("color", QDiagramToolkit::Color, QColor(Qt::black));
	addProperty("italic", QDiagramToolkit::Bool, false);
	addProperty("underline", QDiagramToolkit::Bool, false);
	addProperty("strikeOut", QDiagramToolkit::Bool, false);
}

QDiagramTextStyle::QDiagramTextStyle(const QString &name, const QString &plugin) :
    QDiagramStyle(name, plugin)
{
    addProperty("alignment", QDiagramToolkit::Alignment, Qt::AlignCenter);
    addProperty("family", QDiagramToolkit::String, "Arial");
    addProperty("size", QDiagramToolkit::Int, 10);
	addProperty("bold", QDiagramToolkit::Bool, false);
	addProperty("color", QDiagramToolkit::Color, QColor(Qt::black));
	addProperty("italic", QDiagramToolkit::Bool, false);
	addProperty("underline", QDiagramToolkit::Bool, false);
	addProperty("strikeOut", QDiagramToolkit::Bool, false);
}

Qt::Alignment QDiagramTextStyle::alignment() const
{
    return static_cast<Qt::Alignment>(property("alignment").toInt());
}

bool QDiagramTextStyle::bold() const
{
    return property("bold").toBool();
}

QColor QDiagramTextStyle::color() const
{
    return qvariant_cast<QColor>(property("color"));
}

QString QDiagramTextStyle::family() const
{
    return property("family").toString();
}

QFont QDiagramTextStyle::font() const
{
    QFont f(property("family").toString(), property("size").toInt());
    f.setBold(property("bold").toBool());
    f.setItalic(property("italic").toBool());
    f.setUnderline(property("underline").toBool());
    f.setStrikeOut(property("strikeOut").toBool());
    return f;
}

bool QDiagramTextStyle::italic() const
{
    return property("italic").toBool();
}

void QDiagramTextStyle::setAlignment(Qt::Alignment alignment)
{
    m_alignment = alignment;
}

void QDiagramTextStyle::setBold(bool on)
{
    setProperty("bold", on);
}

void QDiagramTextStyle::setColor(const QColor &color)
{
    setProperty("color", color);
}

void QDiagramTextStyle::setFamily(const QString &name)
{
    setProperty("family", name);
}

void QDiagramTextStyle::setItalic(bool on)
{
    setProperty("italic", on);
}

void QDiagramTextStyle::setSize(int value)
{
    setProperty("size", value);
}

void QDiagramTextStyle::setStrikeOut(bool on)
{
    setProperty("strikeOut", on);
}

void QDiagramTextStyle::setUnderline(bool on)
{
    setProperty("underline", on);
}

int QDiagramTextStyle::size() const
{
    return property("size").toInt();
}

bool QDiagramTextStyle::strikeOut() const
{
    return property("strikeOut").toBool();
}

bool QDiagramTextStyle::underline() const
{
    return property("underline").toBool();
}

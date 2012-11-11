/******************************************************************************
** Copyright (C) 2011-2012 Martin Hoppe martin@2x2hoppe.de
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
#include "qdiagramgraphicsitemshadow.h"

QDiagramGraphicsItemShadow::QDiagramGraphicsItemShadow() :
 QDiagramStyle()
{
	addProperty("visible", QDiagramToolkit::Bool, false);
    addProperty("color", QDiagramToolkit::Color, QColor(Qt::lightGray));
    addProperty("blurRadius", QDiagramToolkit::Int, 8);
	addProperty("offsetX", QDiagramToolkit::Int, 8);
    addProperty("offsetY", QDiagramToolkit::Int, 8);
}

qreal QDiagramGraphicsItemShadow::blurRadius() const
{
    return property("blurRadius").toDouble();
}

QColor QDiagramGraphicsItemShadow::color() const
{
    return qvariant_cast<QColor>(property("color"));
}

bool QDiagramGraphicsItemShadow::isVisible() const
{
    return property("visible").toBool();
}

qreal QDiagramGraphicsItemShadow::offsetX() const
{
    return property("offsetX").toDouble();
}

qreal QDiagramGraphicsItemShadow::offsetY() const
{
    return property("offsetY").toDouble();
}

void QDiagramGraphicsItemShadow::setBlurRadius(qreal radius)
{
	setProperty("blurRadius", radius);
}

void QDiagramGraphicsItemShadow::setColor(const QColor & color)
{
	setProperty("color", color);
}

void QDiagramGraphicsItemShadow::setOffsetX(qreal dx)
{
	setProperty("offsetX", dx);
}

void QDiagramGraphicsItemShadow::setOffsetY(qreal dy)
{
	setProperty("offsetY", dy);
}

void QDiagramGraphicsItemShadow::setVisible(bool on)
{
	setProperty("visible", on);
}

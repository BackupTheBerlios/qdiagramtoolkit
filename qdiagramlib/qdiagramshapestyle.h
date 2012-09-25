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
#ifndef QDIAGRAMSHAPESTYLE_H
#define QDIAGRAMSHAPESTYLE_H

#include "qdiagramlib_global.h"

#include <QIcon>

class QDIAGRAMLIBSHARED_EXPORT QDiagramShapeStyle
{
public:
    /**
      * Constructs an empty (invalid) QDiagramShapeStyle.
      */
    QDiagramShapeStyle();
    /**
      * Constructs a shape style with the given @p icon, @p title and @p name.
      */
    QDiagramShapeStyle(const QIcon & icon, const QString & title, const QString & name);
    /**
      * Retuns the shape style's icon.
      */
    QIcon icon() const;
    /**
      * Returns the shape style's name.
      */
    QString name() const;
    /**
      * Sets the shape style's tool tip to @p toolTip. If @p toolTip is empty, the item's tool tip is cleared.
      */
    void setToolTip(const QString & toolTip);

    QString title() const;

    QString toolTip() const;
private:
    QString m_group;
    QIcon m_icon;
    QString m_name;
    QString m_title;
    QString m_toolTip;
};

#endif // QDIAGRAMSHAPESTYLE_H

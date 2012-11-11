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
#ifndef QDIAGRAMGRAPHICSITEMSHADOW_H
#define QDIAGRAMGRAPHICSITEMSHADOW_H

#include <QMetaType>

#include "qdiagramlib_global.h"
#include <qdiagramstyle.h>

class QDIAGRAMLIBSHARED_EXPORT QDiagramGraphicsItemShadow : public QDiagramStyle
{
public:
    /**
      * Constructs an empty (invalid) QDiagramGraphicsItemShadow.
      */
	QDiagramGraphicsItemShadow();
	/**
	 * Returns the shadow's blur radius.
	 */
	qreal blurRadius() const;
	/**
	 * Returns the color of the drop shadow.
	 */
	QColor color() const;
	/**
	 * Returns true if the shadow is visible.
	 * @see setVisible();
	 */
	bool isVisible() const;
	/**
	 * Returns the horizontal (x) offset.
	 */
	qreal offsetX() const;
	/**
	 * Returns the vertical (y) offset.
	 */
	qreal offsetY() const;
	/**
	 * Sets the shadow's blur @p radius.
	 * @see blurRadius()
	 */
	void setBlurRadius(qreal radius);
	/**
	 * Sets the shadow color to @p color.
	 * @see color()
	 */
	void setColor(const QColor & color);
	/**
	 * Sets the horizontal (x) offset to @p dx.
	 */
	void setOffsetX(qreal dx);
	/**
	 * Sets the vertical (y) offset to @p dy.
	 */
	void setOffsetY(qreal dy);
	/**
	 * If @p on is true the drop shadow is visible.
	 */
	void setVisible(bool on);
};

Q_DECLARE_METATYPE(QDiagramGraphicsItemShadow)

#endif // QDIAGRAMGRAPHICSITEMSHADOW_H

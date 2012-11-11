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
#ifndef QDIAGRAMLINESTYLE_H
#define QDIAGRAMLINESTYLE_H

#include <QMetaType>
#include <QPen>

#include <qdiagramlib_global.h>
#include <qdiagramstyle.h>

class QDIAGRAMLIBSHARED_EXPORT QDiagramLineStyle : public QDiagramStyle
{
public:
    /**
      * Constructs an invalid line style.
      */
    QDiagramLineStyle();
    /**
      *
      */
    QDiagramLineStyle(const QString & plugin, const QString & name, const QString & title, const QPen & pen, const QIcon & icon = QIcon());

    ~QDiagramLineStyle();
    /**
      * Returns the line color.
      * @see setColor()
      */
    QColor color() const;
    /**
      *
      */
    QIcon icon() const;
    /**
      *
      */
    QString id() const;
    /**
      * Returns true if the style is valid. Otherwise false.
      */
    bool isValid() const;

    QString name() const;
    /**
      *
      */
	Qt::PenStyle penStyle() const;
    /**
      * Returns the name of the plugin providing this style.
      */
    QString plugin() const;
    /**
     * Sets the line @p color.
     * @see color()
     */
    void setColor(const QColor & color);
    /**
      * Sets the line @p width.
      * @see width()
      */
    void setWidth(qreal width);
    /**
      *
      */
    QString title() const;
    /**
      * Returns the line width.
      * @see setWidth()
      */
    qreal width() const;
private:
    QColor m_color;
    QIcon m_icon;
    QString m_name;
    QString m_plugin;
    QPen m_pen;
    QString m_title;
};

QDebug operator<<(QDebug dbg, const QDiagramLineStyle &s);

Q_DECLARE_METATYPE(QDiagramLineStyle)
#endif // QDIAGRAMLINESTYLE_H

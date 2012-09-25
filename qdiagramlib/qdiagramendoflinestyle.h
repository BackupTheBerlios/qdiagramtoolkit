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
#ifndef QDIAGRAMENDOFLINESTYLE_H
#define QDIAGRAMENDOFLINESTYLE_H

#include <QMetaType>
#include <qdiagramstyle.h>
#include <qdiagramlib_global.h>

class QDIAGRAMLIBSHARED_EXPORT QDiagramEndOfLineStyle : public QDiagramStyle
{
public:
    enum Style {
        DefaultArrow,
        SmallArrow,
        ConcaveArrow,
        None
    };
    /**
      * Constructs a default black solid line style with 0 width.
      */
    QDiagramEndOfLineStyle();

    QDiagramEndOfLineStyle(const QString & caption, const QString & name, const QString & plugin);

    QDiagramEndOfLineStyle(const QIcon & icon, const QString & caption, const QString & name, const QString & plugin);

    QString caption() const;

    QIcon icon() const;

    qreal width() const;

    void setWidth(qreal width);
private:
    QString m_caption;
    QIcon m_icon;
};

QDIAGRAMLIBSHARED_EXPORT QDebug operator<<(QDebug dbg, const QDiagramEndOfLineStyle &s);

Q_DECLARE_METATYPE(QDiagramEndOfLineStyle)

#endif // QDIAGRAMENDOFLINESTYLE_H

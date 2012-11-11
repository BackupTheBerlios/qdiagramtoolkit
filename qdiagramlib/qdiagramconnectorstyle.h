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
#ifndef QDIAGRAMCONNECTORSTYLE_H
#define QDIAGRAMCONNECTORSTYLE_H

#include <QIcon>
#include <QMetaType>

#include "qdiagramlib_global.h"

class QDIAGRAMLIBSHARED_EXPORT QDiagramConnectorStyle
{
public:
    QDiagramConnectorStyle();
    QDiagramConnectorStyle(const QString & plugin, const QIcon & icon, const QString & name, const QString & style);
    ~QDiagramConnectorStyle();

    QIcon icon() const;

    bool isNull() const;

	bool isValid() const;
    /**
      *
      */
    QString name() const;
    /**
      *
      */
    QString plugin() const;
    /**
      * Returns the name of connector's shape.
      */
    QString shape() const;
private:
    QIcon m_icon;
    QString m_name;
    QString m_plugin;
    QString m_style;
};

Q_DECLARE_METATYPE(QDiagramConnectorStyle)
Q_DECLARE_METATYPE(QDiagramConnectorStyle*)
Q_DECLARE_METATYPE(QList<QDiagramConnectorStyle>)

#endif // QDIAGRAMCONNECTORSTYLE_H

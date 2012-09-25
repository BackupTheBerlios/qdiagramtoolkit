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
#ifndef QDIAGRAMSHAPELISTWIDGET_H
#define QDIAGRAMSHAPELISTWIDGET_H

#include "qdiagramlib_global.h"

#include <QListWidget>

class QDiagram;

class QDIAGRAMLIBSHARED_EXPORT QDiagramShapeListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit QDiagramShapeListWidget(QWidget *parent = 0);

    void addShape(const QIcon & icon, const QString & title, const QString & name, const QMap<QString,QVariant> & properties, const QString & plugin = "default");
    void removeShape(const QString &name, const QString &plugin);
    void startDrag(Qt::DropActions supportedActions);
signals:

public slots:

};

#endif // QDIAGRAMSHAPELISTWIDGET_H

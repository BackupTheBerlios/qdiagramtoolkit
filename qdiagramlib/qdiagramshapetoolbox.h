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
#ifndef QDIAGRAMSHAPETOOLBOX_H
#define QDIAGRAMSHAPETOOLBOX_H

#include "qdiagramlib_global.h"

#include <QWidget>

class QAbstractDiagram;
class QAbstractDiagramPlugin;

namespace Ui {
    class QDiagramShapeToolBox;
}

class QDiagram;

class QDIAGRAMLIBSHARED_EXPORT QDiagramShapeToolBox : public QWidget
{
    Q_OBJECT

public:
    explicit QDiagramShapeToolBox(QWidget *parent = 0);
    ~QDiagramShapeToolBox();

    void addShapes(QAbstractDiagram* diagram);

    void addShapes(QAbstractDiagramPlugin* plugin);
    /**
      * Removes the shape specified by the given @p name from the tool box.
      */
    void removeShape(const QString & name, const QString & plugin);
private:
    Ui::QDiagramShapeToolBox *ui;
};

#endif // QDIAGRAMSHAPETOOLBOX_H

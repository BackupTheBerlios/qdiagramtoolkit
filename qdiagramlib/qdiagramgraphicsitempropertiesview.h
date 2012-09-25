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
#ifndef QDIAGRAMGRAPHICSITEMPROPERTIESVIEW_H
#define QDIAGRAMGRAPHICSITEMPROPERTIESVIEW_H

#include "qdiagramlib_global.h"

#include <QStandardItemModel>
#include <QWidget>

#include <qabstractdiagramgraphicsitem.h>

namespace Ui {
    class QDiagramGraphicsItemPropertiesView;
}

class QPropertiesModel;

//! The QDiagramGraphicsItemPropertiesView class provides a widget for displaying the properties of a QDiagramItem
class QDIAGRAMLIBSHARED_EXPORT QDiagramGraphicsItemPropertiesView : public QWidget
{
    Q_OBJECT
public:
    /**
      * Constructs a QDiagramGraphicsItemPropertiesView. parent is passed to QWidget's constructor.
      */
    explicit QDiagramGraphicsItemPropertiesView(QWidget* parent = 0);
    /**
      * Destructs the QDiagramGraphicsItemPropertiesView object.
      */
    ~QDiagramGraphicsItemPropertiesView();
    /**
      *
      */
    void clear();
signals:
    /**
      * This signal is emitted whenever a property changes in the property editor.
      * The property that changed and its new value are specified by name and value respectively.
      */
    void propertyChanged(const QString & name, const QVariant & value);
public slots:
    void showProperties(QList<QAbstractDiagramGraphicsItem*> items);
private:
    QPropertiesModel* m_model;
    Ui::QDiagramGraphicsItemPropertiesView* ui;
};

#endif // QDIAGRAMGRAPHICSITEMPROPERTIESVIEW_H

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
#include "qdiagramshapelistwidget.h"

#include "qabstractdiagramplugin.h"
#include "qdiagram.h"
#include "qdiagrampluginloader.h"

QDiagramShapeListWidget::QDiagramShapeListWidget(QWidget *parent) :
    QListWidget(parent)
{
    setDragEnabled(true);
    setViewMode(QListView::IconMode);
    setIconSize(QSize(48,48));
    setSpacing(10);
    setAcceptDrops(true);
    setDropIndicatorShown(true);
}

//void QDiagramShapeListWidget::addShape(const QString & plugin, const QString & id, const QString & title, const QIcon & icon)
//{
//    QListWidgetItem* i = new QListWidgetItem(icon, title);
//    i->setData(Qt::UserRole, "Shape");
//    i->setData(Qt::UserRole + 1, plugin);
//    i->setData(Qt::UserRole + 2, id);
//    addItem(i);
//}

void QDiagramShapeListWidget::addShape(const QString & plugin, const QString & itemClass, const QString & title, const QVariantMap & properties, const QIcon & icon)
{
	QVariantMap m;
	m["itemType"] = "Shape";
	m["itemClass"] = itemClass;
	m["plugin"] = plugin;
    QListWidgetItem* i = new QListWidgetItem(icon, title);
    i->setData(Qt::UserRole, m);
    i->setData(Qt::UserRole + 1, properties);
    addItem(i);
}

void QDiagramShapeListWidget::addShape(const QString & id, const QString & plugin, const QString & title, const QIcon & icon)
{
    QListWidgetItem* i = new QListWidgetItem(icon, title);
    i->setData(Qt::UserRole, plugin);
    i->setData(Qt::UserRole + 1, id);
    addItem(i);
}

void QDiagramShapeListWidget::removeShape(const QString & itemClass, const QString & plugin)
{
    for (int i = 0; i < count(); i++){
		if (item(i)->data(Qt::UserRole).toMap().value("plugin") == plugin && item(i)->data(Qt::UserRole).toMap().value("itemClass") == itemClass){
            delete takeItem(i);
        }
    }
}

void QDiagramShapeListWidget::startDrag(Qt::DropActions /*supportedActions*/)
{
    QListWidgetItem* i = currentItem();

    QByteArray d;
    QDataStream s(&d, QIODevice::WriteOnly);

//    s << i->data(Qt::UserRole) << i->data(Qt::UserRole + 1) << i->data(Qt::UserRole + 2) << i->data(Qt::UserRole + 3);
//    s << i->data(Qt::UserRole) << i->data(Qt::UserRole + 1) << i->data(Qt::UserRole + 2);
	s << i->data(Qt::UserRole) << i->data(Qt::UserRole + 1);

    QMimeData* md = new QMimeData;
    md->setData("application/qdiagram.shape", d);

    QDrag* drag = new QDrag(this);
    drag->setMimeData(md);

    QPixmap pm(i->icon().pixmap(48,48));
//    mDrag->setHotSpot(QPoint(mPixmap.width()/2, mPixmap.height()/2));
    drag->setPixmap(pm);

    drag->exec(Qt::MoveAction);
}


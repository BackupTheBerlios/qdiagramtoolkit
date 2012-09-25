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

void QDiagramShapeListWidget::addShape(const QIcon & icon, const QString & title, const QString & name, const QMap<QString,QVariant> & properties, const QString & plugin)
{
    QListWidgetItem* i = new QListWidgetItem(icon, title);
    i->setData(Qt::UserRole, plugin);
    i->setData(Qt::UserRole + 1, name);
    i->setData(Qt::UserRole + 2, properties);
    addItem(i);
}

void QDiagramShapeListWidget::removeShape(const QString & name, const QString & plugin)
{
    for (int i = 0; i < count(); i++){
        if (item(i)->data(Qt::UserRole) == plugin && item(i)->data(Qt::UserRole + 1) == name){
            delete takeItem(i);
        }
    }
}

void QDiagramShapeListWidget::startDrag(Qt::DropActions /*supportedActions*/)
{
    QListWidgetItem* mItem = currentItem();

    QByteArray mItemData;
    QDataStream mDataStream(&mItemData, QIODevice::WriteOnly);

    mDataStream << mItem->data(Qt::UserRole) << mItem->data(Qt::UserRole + 1)<< mItem->data(Qt::UserRole + 2);

    QMimeData* mMimeData = new QMimeData;
    mMimeData->setData("application/qdiagram.shape", mItemData);

    QDrag* mDrag = new QDrag(this);
    mDrag->setMimeData(mMimeData);

    QPixmap mPixmap(mItem->icon().pixmap(48,48));
//    mDrag->setHotSpot(QPoint(mPixmap.width()/2, mPixmap.height()/2));
    mDrag->setPixmap(mPixmap);

    mDrag->exec(Qt::MoveAction);
}


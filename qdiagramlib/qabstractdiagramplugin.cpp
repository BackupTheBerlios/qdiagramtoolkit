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
#include "qabstractdiagramplugin.h"

#include "qabstractdiagramgraphicsitem.h"

QAbstractDiagramPlugin::~QAbstractDiagramPlugin()
{
}

QPointF QAbstractDiagramPlugin::hotSpot(const QVariantMap & metaData, const QVariantMap & properties) const
{
	Q_UNUSED(metaData);
	Q_UNUSED(properties);
    return QPointF(0, 0);
}
//
//void QAbstractDiagramPlugin::restoreProperties(QAbstractDiagramGraphicsItem* item, const QMap<QString,QVariant> & properties)
//{
//    if (item == 0){
//        return;
//    }
//    //QMap<QString,QVariant>::const_iterator mIt;
//    //for (mIt = properties.constBegin(); mIt != properties.constEnd(); ++mIt){
//    //    item->setProperty(mIt.key(), mIt.value());
//    //}
//    //if (item->property("shape").toString() != "connection"){
//    //    item->setPos(properties.value("x", 0).toReal(), properties.value("y", 0).toReal());
//    //}
//}

QList<QAction*> QAbstractDiagramPlugin::shapeActions(const QString &style) const
{
    return QList<QAction*>();
}

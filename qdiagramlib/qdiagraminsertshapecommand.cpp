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
#include "qdiagraminsertshapecommand.h"

#include <qdiagrampluginloader.h>

QDiagramInsertShapeCommand::QDiagramInsertShapeCommand(QAbstractDiagram* diagram, const QString & uuid, const QVariantMap & metaData, const QVariantMap & properties, QUndoCommand* parent) :
    QDiagramUndoCommand(diagram, uuid, metaData, properties, parent)
{
    QAbstractDiagramPlugin* p = QDiagramPluginLoader::load(metaData.value("plugin").toString());
    if (p){
        setText(QObject::tr("Insert %1").arg(p->title(metaData.value("itemClass").toString())));
    }
}

void QDiagramInsertShapeCommand::undo()
{
    QAbstractDiagramGraphicsItem* i = diagram()->findItemByUuid(uuid());
    diagram()->takeItem(i);
    delete i;
}

void QDiagramInsertShapeCommand::redo()
{
    diagram()->restoreItem(metaData(), properties());
}

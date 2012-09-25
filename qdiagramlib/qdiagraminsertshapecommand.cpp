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

QDiagramInsertShapeCommand::QDiagramInsertShapeCommand(QAbstractDiagram* diagram, const QString & uuid, const QString & shape, const QMap<QString,QVariant> & props, const QString & plugin, QUndoCommand* parent ) :
    QDiagramUndoCommand(diagram, uuid, shape, props, plugin, parent)
{
    QAbstractDiagramPlugin* mPlugin = QDiagramPluginLoader::load(plugin);
    if (mPlugin){
        setText(QObject::tr("Insert %1").arg(mPlugin->title(shape)));
    }
}

void QDiagramInsertShapeCommand::undo()
{
    QAbstractDiagramGraphicsItem* item = diagram()->findItemByUuid(uuid());
    diagram()->takeItem(item);
    delete item;
}

void QDiagramInsertShapeCommand::redo()
{
    diagram()->restoreItem(properties());
}

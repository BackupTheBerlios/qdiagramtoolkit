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
#ifndef QDIAGRAMREMOVEITEMCOMMAND_H
#define QDIAGRAMREMOVEITEMCOMMAND_H

#include <qdiagramlib_global.h>
#include <qdiagramundocommand.h>

class QDIAGRAMLIBSHARED_EXPORT QDiagramRemoveItemCommand : public QDiagramUndoCommand
{
public:
    enum {
        Id = 44
    };
    QDiagramRemoveItemCommand(QAbstractDiagramGraphicsItem* item, QUndoCommand* parent = 0);
    QDiagramRemoveItemCommand(QAbstractDiagram* diagram, const QString & uuid, const QString & shape, const QMap<QString,QVariant> & properties, QUndoCommand* parent = 0 );
    /**
      * Returns the id of this command.
      */
    int id() const
    {
        return Id;
    }

    void undo();

    void redo();
};

#endif // QDIAGRAMREMOVEITEMCOMMAND_H

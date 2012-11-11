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
#ifndef QDIAGRAMINSERTSHAPECOMMAND_H
#define QDIAGRAMINSERTSHAPECOMMAND_H

#include <qdiagramundocommand.h>

//! The QDiagramInsertShapeCommand provides an undo command for inserting a shape into a diagram.
class QDIAGRAMLIBSHARED_EXPORT QDiagramInsertShapeCommand : public QDiagramUndoCommand
{
public:
    enum {
        Id = 4201
    };
    /**
      * Constructs a QDiagramInsertShapeCommand.
      */
	QDiagramInsertShapeCommand(QAbstractDiagram* diagram, const QString & uuid, const QVariantMap & metaData, const QVariantMap & properties, QUndoCommand* parent = 0);
    /**
      * Returns the id of this command.
      */
    int id() const
    {
        return Id;
    }
    /**
      * Reverts a shape insert to the diagram.
      */
    void undo();
    /**
      *
      */
    void redo();
private:
    QMap<QString,QVariant> m_properties;
};

#endif // QDIAGRAMINSERTSHAPECOMMAND_H

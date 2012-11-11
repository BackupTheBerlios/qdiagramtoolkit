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
#ifndef QDIAGRAMCHANGEPROPERTYCOMMAND_H
#define QDIAGRAMCHANGEPROPERTYCOMMAND_H

#include <qdiagramlib_global.h>
#include <qdiagramundocommand.h>

class QDIAGRAMLIBSHARED_EXPORT QDiagramChangePropertyCommand : public QDiagramUndoCommand
{
public:
    enum {
        Id = 43
    };
    QDiagramChangePropertyCommand(QAbstractDiagram* diagram, QDiagramShape* item, const QString & name, const QVariant & oldValue, const QVariant & newValue, QUndoCommand* parent = 0);
    /**
      * Returns the id of this command.
      */
    int id() const
    {
        return Id;
    }
    /**
      * Attempts to merge this command with @p other. Returns true on success; otherwise returns false.
      * @reimp
      */
    bool mergeWith(const QUndoCommand* other);

    QString name() const;

    void undo();

    void redo();
private:
    QVariant m_old;
    QString m_name;
    QVariant m_new;
};

#endif // QDIAGRAMCHANGEPROPERTYCOMMAND_H

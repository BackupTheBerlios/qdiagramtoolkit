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
#ifndef QDIAGRAMUNDOCOMMAND_H
#define QDIAGRAMUNDOCOMMAND_H

#include "qdiagramlib_global.h"

#include <QUndoCommand>

#include <qdiagram.h>

class QDiagramShape;

//! The QDiagramUndoCommand class is the base class of all diagram commands stored on a QUndoStack.
class QDIAGRAMLIBSHARED_EXPORT QDiagramUndoCommand : public QUndoCommand
{
public:
    QDiagramUndoCommand(QAbstractDiagram* diagram, const QString & plugin = "default", QUndoCommand* parent = 0);
    QDiagramUndoCommand(QAbstractDiagram* diagram, const QString & uuid, const QString & plugin = "default", QUndoCommand* parent = 0);
    QDiagramUndoCommand(QAbstractDiagram* diagram, const QString & uuid, const QString & shape, const QString & plugin = "default", QUndoCommand* parent = 0);
    QDiagramUndoCommand(QAbstractDiagram* diagram, const QString & uuid, const QString & shape, const QMap<QString,QVariant> & properties, const QString & plugin = "default", QUndoCommand* parent = 0);

    QAbstractDiagram* diagram() const;
    /**
      * Returns the plugin name. Default is set to <i>default</i>.
      */
    QString plugin() const;
    /**
      * Returns the command's properties.
      */
    QMap<QString,QVariant> properties() const;

    QString shape() const;
    /**
      * Returns the item's uuid.
      * @see QAbstractDiagramGraphicsItem::uuid()
      */
    QString uuid() const;
protected:
    /**
      * Sets the @p name of the plugin.
      * @see plugin()
      */
    void setPlugin(const QString & name);
	void setProperty(const QString & name, const QVariant & value);
    void setProperties(const QMap<QString,QVariant> & properties);
private:
    QAbstractDiagram* m_diagram;
    QString m_plugin;
    QString m_shape;
    QString m_uuid;
    QMap<QString,QVariant> m_properties;
};

class QDIAGRAMLIBSHARED_EXPORT QDiagramConnectShapesCommand : public QDiagramUndoCommand
{
public:
    QDiagramConnectShapesCommand(QAbstractDiagram* diagram, QAbstractDiagramShapeConnector* connector, const QString & plugin = "default", QUndoCommand* parent = 0);
    QDiagramConnectShapesCommand(QAbstractDiagram* diagram, const QString & uuid, QAbstractDiagramShapeConnectionPoint* from, QAbstractDiagramShapeConnectionPoint* to, const QDiagramConnectorStyle & style, const QString & plugin = "default", QUndoCommand* parent = 0);
    void undo();

    void redo();
};

class QDIAGRAMLIBSHARED_EXPORT QDiagramChangePropertyCommand : public QDiagramUndoCommand
{
public:
    enum {
        Id = 43
    };
    QDiagramChangePropertyCommand(QAbstractDiagram* diagram, QDiagramShape* item, const QString & property, const QVariant & oldValue, const QVariant & newValue, QUndoCommand* parent = 0);
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

#endif // QDIAGRAMUNDOCOMMAND_H

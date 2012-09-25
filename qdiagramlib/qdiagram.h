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

#ifndef QDIAGRAM_H
#define QDIAGRAM_H

#include "qdiagramlib_global.h"

#include <QUndoStack>

#include <qabstractdiagram.h>
#include <qabstractdiagramgraphicsitem.h>

#include <qdiagramconnectorstyle.h>
#include <qdiagramlinestyle.h>
#include <qdiagramshape.h>
#include <qdiagramgraphicsscene.h>

class QAbstractDiagramShapeConnectionPoint;

//! The QDiagram class provides a default implementation of a diagram.
/**
  * <b>Reading and Writing Diagram Files</b>
  *
  *
  */
class QDIAGRAMLIBSHARED_EXPORT QDiagram  : public QAbstractDiagram
{
    Q_OBJECT
public:
    /**
      * Constructs a QDiagram with the given @p parent.
      */
    explicit QDiagram(QObject* parent = 0);

    virtual QAbstractDiagramGraphicsItem* addItem(const QString & uuid, const QString & shape, const QMap<QString,QVariant> & properties, const QString & plugin = "default");
    /**
      *
      */
    QString addShape(const QString & name, const QPointF & pos, const QMap<QString,QVariant> & properties, const QString & plugin);

    void addConnection(QAbstractDiagramShapeConnectionPoint* from, QAbstractDiagramShapeConnectionPoint* to, const QDiagramConnectorStyle & style);

    QAbstractDiagramShapeConnector* createConnection(const QString & from, const QString & fromId, const QString & to, const QString & toId, const QDiagramConnectorStyle & style);

    static QDiagramLineStyle lineStyle(const QString & id);
    /**
      *
      */
    static QList<QDiagramLineStyle> linesStyles();
    /**
      * Returns the name of the plugin providing this diagram.
      */
    virtual QString plugin() const;
    /**
      * Removes the item specified by the given @uuid.
      */
    virtual bool removeItem( const QString & uuid );
    /**
      * Restores an item from the given set of @p properties.
      */
    virtual QAbstractDiagramGraphicsItem* restoreItem(const QMap<QString,QVariant> & properties);
    /**
      * Begins a restore operation.
      *
      * When a diagram is restored it means that any previous data reported from the diagram is now invalid and has to be queried for again.
      */
    virtual void beginRestoreDiagram();
    virtual void endRestoreDiagram();
    /**
      * Returns the diagram type.
      */
    virtual QString type() const;

    static QByteArray serialize(QAbstractDiagram* diagram, bool includeItems = true);
signals:
    void itemAdded(QDiagramShape* findItemByUuid);
    void itemRestored(QAbstractDiagramGraphicsItem* findItemByUuid);
protected:

    /**
      * This event handler, for event event, can be reimplemented to receive drag enter events for this diagram.
      * Drag enter events are generated as the cursor enters the item's area.
      *
      * The default implementation does nothing.
      */
    virtual void contextMenuEventHandler(QGraphicsSceneContextMenuEvent* event);
    /**
      * This event handler, for event event, can be reimplemented to receive drag enter events for this diagram.
      * Drag enter events are generated as the cursor enters the item's area.
      *
      * The default implementation does nothing.
      */
    virtual void dragEnterEventHandler(QGraphicsSceneDragDropEvent* event);
    /**
      * This event handler, for event event, can be reimplemented to receive drag leave events for this diagram.
      * Drag leave events are generated as the cursor leaves the diagram's area.
      * Most often you will not need to reimplement this function, but it can be useful for resetting state in your item (e.g., highlighting).
      *
      * The default implementation does nothing.
      * @see dragEnterEventHandler() dragMoveEventHandler() dropEventHandler()
      */
    virtual void dragLeaveEventHandler( QGraphicsSceneDragDropEvent* event );
    /**
      * This event handler, for event event, can be reimplemented to receive drag move events for this diagram.
      * Drag move events are generated as the cursor moves around inside the diagram's area.
      * Most often you will not need to reimplement this function; it is used to indicate that only parts of the item can accept drops.
      *
      * The default implementation does nothing.
      */
    virtual void dragMoveEventHandler( QGraphicsSceneDragDropEvent* event );
    /**
      * This event handler, for event event, can be reimplemented in a subclass to receive drop events for the diagram.
      *
      * The default implementation does nothing.
      */
    virtual void dropEventHandler( QGraphicsSceneDragDropEvent* event );
protected slots:
    virtual void connectItemsEventHandler( QDiagramShape* from, QDiagramShape* to, const QMap<QString,QVariant> & properties);
    virtual void itemMoved(QGraphicsItem* findItemByUuid, const QPointF & oldPos, const QPointF & newPos);
    virtual void itemRestoredHandler(QAbstractDiagramGraphicsItem* findItemByUuid);
private:
};

#endif // QDIAGRAM_H

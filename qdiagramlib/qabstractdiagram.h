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
#ifndef QABSTRACTDIAGRAM_H
#define QABSTRACTDIAGRAM_H

#include "qdiagramlib_global.h"

#include <QGraphicsSceneDragDropEvent>
#include <QColor>
#include <QObject>
#include <QPrinter>
#include <QUndoStack>

#include <qdiagramlayers.h>
#include <qdiagrammetaproperty.h>
#include <qdiagramconnectorstyle.h>

class QAbstractDiagramGraphicsItem;
class QAbstractDiagramScene;
class QAbstractDiagramShape;
class QAbstractDiagramShapeConnectionPoint;
class QAbstractDiagramShapeConnector;
class QDiagramStyleSheet;

class QGraphicsItem;
class QDiagramEndOfLineStyle;

//! The QAbstractDiagram class is the base of all diagrams
/**
 * The class defines the functions for the functionality shared by all diagrams.
 * By inheriting this class, you can create custom diagrams that plug into the rest of the diagram framework.
 */
class QDIAGRAMLIBSHARED_EXPORT QAbstractDiagram : public QObject
{
    Q_OBJECT
    //! @property(author)
    /**
      * This property holds the name of the diagram's author.
      * @see author() setAuthor()
      */
    Q_PROPERTY(QString author READ author WRITE setAuthor)
    //! @property(title)
    /**
      * This property holds the diagram title.
      * @see title() and setTitle()
      */
    Q_PROPERTY(QString title READ title WRITE setTitle)
    //! @property(type)
    /**
      *
      */
    Q_PROPERTY(QString type READ type)
public:
    /**
      * Constructs a QAbstractDiagram with the given @p parent.
      */
    explicit QAbstractDiagram(QObject* parent = 0);
	/**
	 * 
	 */
	~QAbstractDiagram();
    /**
      * Adds a new item from.
      */
    virtual QAbstractDiagramGraphicsItem* addItem(const QString & uuid, const QString & shape, const QMap<QString,QVariant> & properties, const QString & plugin = "default") = 0;
    /**
      * Adds the action @p action to the list of actions used to create a standard context menu.
      */
    void addItemContextMenuAction(QAction* action);
    /**
      * Adds the plugin specified by the given @p name to the list of available plugins.
      * @see plugins()
      */
    void addPlugin(const QString & name);
    /**
      * Adds a shape of the given @p style.
      */
    virtual QString addShape(const QString & style, const QPointF & pos, const QMap<QString,QVariant> & properties, const QString & plugin = "default") = 0;
    /**
      * Adds a connection between the specified @p start and @p end connection points.
      */
    virtual void addConnection(QAbstractDiagramShapeConnectionPoint* start, QAbstractDiagramShapeConnectionPoint* end, const QDiagramConnectorStyle & style) = 0;
    /**
      * Returns the author's name.
      * @see setAuthor()
      */
    QString author() const;
	/**
	 * Clears the current selection.
	 */
	void clearSelection();
    /**
      *
      */
    QList<QAbstractDiagramShapeConnector*> connectors() const;
	/**
	 * Returns the default value for the property of the specified @p type.
	 * If no default value can be retrived, defaultValue return an invalid QVariant.
	 */
	QVariant defaultValue(QDiagramToolkit::PropertyType type) const;
    /**
      * Returns the end of line style specified by the given @p id.
      */
    QDiagramEndOfLineStyle endOfLineStyle(const QString & id) const;
    /**
      *
      */
    QList<QDiagramEndOfLineStyle> endOfLineStyles() const;
    /**
      * Returns the item specified by the given @p uuid or 0 if @p uuid does not exsist.
      */
    QAbstractDiagramGraphicsItem* findItemByUuid( const QString & uuid ) const;
    /**
      * Returns the coordinates of the given @point aligned to the current grid settings.
      */
    QPointF gridPos(const QPointF & point) const;
    /**
      * Returns true if the diagram is modified. Otherwise false.
      */
    bool isModified() const;
    /**
      * Returns a list of diagram items.
      */
    QList<QAbstractDiagramGraphicsItem*> items() const;
	/**
	 *
	 */
	QDiagramLayers* layers() const;
    /**
      * Returns the name of the plugin providing this diagram.
      */
    virtual QString plugin() const = 0;
    /**
      * Returns a list of plugins available for the diagram.
      * @see addPlugin()
      */
    QStringList plugins() const;
    /**
      * Removes the item specified by the given @uuid.
      */
    virtual bool removeItem( const QString & uuid ) = 0;
    /**
      * Restores an item from a repository or from the undo stack.
      */
    virtual QAbstractDiagramGraphicsItem* restoreItem(const QMap<QString,QVariant> & metaData, const QMap<QString,QVariant> & properties ) = 0;
    /**
      * Returns the current scene for the diagram.
      */
    QAbstractDiagramScene* scene() const;
	/**
	 * Selects the item specified by the given @p uuid.
	 */
	void select(const QString & uuid);
	/**
	 * Selects all items.
	 * @see clearSelection()
	 */
	void selectAll();
    /**
      * Returns a list of all currently selected items. The items are returned in no particular order.
	  * @see selectAll()
      */
    QList<QAbstractDiagramGraphicsItem*> selectedItems() const;
    /**
      * Returns the diagram's current selection color.
      */
    QColor selectionColor() const;
    /**
      * Sets the diagram's author to @p author.
      * @see author
      */
    void setAuthor(const QString & author);
	/**
	 * Sets the diagram @p title.
	 * @see title()
	 */
	void setTitle(const QString & title);
    /**
      * Returns the shape specified with the given @p uuid or 0, if @p uuid does not exist.
      */
    QAbstractDiagramShape* shape(const QString & uuid) const;
	/**
	 *
	 */
	QList<QAbstractDiagramShape*> shapes() const;
    /**
      * @see addItemContextMenuAction();
      */
    QList<QAction*> standardItemContextMenuActions() const;

	QDiagramStyleSheet* styleSheet() const;
    /**
      * Removes the @p item from the diagram without deleting it.
      */
    void takeItem(QAbstractDiagramGraphicsItem* findItemByUuid);
	/**
	 * Returns the diagram title.
	 * @see setTitle()
	 */
	QString title() const;
    /**
      * Returns the diagram type.
      */
    virtual QString type() const = 0;
    /**
      * Returns the diagram's undo stack.
      */
    QUndoStack* undoStack() const;
signals:
    /**
      * This signal is emitted whenever the diagram's content changes; for example, when an item is inserted or deleted, or when formatting is applied.
      */
    void contentsChanged();
    /**
      *
      */
    void contextMenuRequested(const QPointF & scenePos, const QPointF & screenPos);
    /**
      * This signal is emitted when an item has been added to the diagram. The item's @p uuid is passed.
      */
    void itemAdded(const QString & uuid);
    /**
      * This signal is emitted by QDiagram whenever the selection changes. You can call selectedItems() to get the new list of selected items.
      */
    void selectionChanged();
    /**
      * This signal is emitted whenever a shape is added to the diagram. The shape's @p uuid is passed as parameter.
      */
    void shapeAdded(const QString & uuid);
public slots:
    void print(QPrinter* printer);
protected:
    friend class QAbstractDiagramScene;
    virtual void dragEnterEventHandler( QGraphicsSceneDragDropEvent* event ) = 0;
    virtual void contextMenuEventHandler(QGraphicsSceneContextMenuEvent* event) = 0;
protected slots:
    /**
      * This event handler, for event event, can be reimplemented to receive drag enter events for this diagram.
      * Drag enter events are generated as the cursor enters the item's area.
      *
      * The default implementation does nothing.
      */
//    virtual void dragEnterEventHandler( QGraphicsSceneDragDropEvent* event ) = 0;
    /**
      * This event handler, for event event, can be reimplemented to receive drag leave events for this diagram.
      * Drag leave events are generated as the cursor leaves the diagram's area.
      * Most often you will not need to reimplement this function, but it can be useful for resetting state in your item (e.g., highlighting).
      *
      * The default implementation does nothing.
      * @see dragEnterEventHandler() dragMoveEventHandler() dropEventHandler()
      */
    virtual void dragLeaveEventHandler( QGraphicsSceneDragDropEvent* event ) = 0;
    /**
      * This event handler, for event event, can be reimplemented to receive drag move events for this diagram.
      * Drag move events are generated as the cursor moves around inside the diagram's area.
      * Most often you will not need to reimplement this function; it is used to indicate that only parts of the item can accept drops.
      *
      * The default implementation does nothing.
      */
    virtual void dragMoveEventHandler( QGraphicsSceneDragDropEvent* event ) = 0;
    /**
      * This event handler, for event event, can be reimplemented in a subclass to receive drop events for the diagram.
      *
      * The default implementation does nothing.
      */
    virtual void dropEventHandler( QGraphicsSceneDragDropEvent* event ) = 0;

    virtual void itemMoved(QGraphicsItem* findItemByUuid, const QPointF & oldPos, const QPointF & newPos) = 0;
private slots:
    void undoStackIndexChanged(int index);
private:
    QString m_author;
    QList<QAction*> m_standardItemContextMenuActions;
	QDiagramLayers* m_layers;
    QStringList m_plugins;
    QAbstractDiagramScene* m_scene;
	QDiagramStyleSheet* m_styleSheet;
	QString m_title;
    QUndoStack* m_undostack;
};

Q_DECLARE_METATYPE(QAbstractDiagram*)

#endif // QABSTRACTDIAGRAM_H

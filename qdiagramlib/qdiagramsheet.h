/******************************************************************************
** Copyright (C) 2013 Martin Hoppe martin@2x2hoppe.de
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
#ifndef QDIAGRAMSHEET_H
#define QDIAGRAMSHEET_H

#include "qdiagramlib_global.h"

#include <QGraphicsScene>

#include <qdiagramtoolkit.h>

class QAbstractDiagram;
class QAbstractDiagramGraphicsItem;
class QAbstractDiagramShape;

class QDIAGRAMLIBSHARED_EXPORT QDiagramSheet : public QGraphicsScene
{
    Q_OBJECT
    Q_PROPERTY(QSize snapSize READ snapSize WRITE setSnapSize)
	Q_PROPERTY(QString name READ name WRITE setName)
	Q_PROPERTY(QDiagramToolkit::PaperOrientation paperOrientation READ paperOrientation WRITE setPaperOrientation)
    Q_PROPERTY(QColor selectionColor READ selectionColor WRITE setSelectionColor)
public:
    explicit QDiagramSheet(QAbstractDiagram* diagram = 0);
    /**
      * Returns the diagram associated with the scene.
      */
    QAbstractDiagram* diagram() const;

    QColor selectionColor() const;
	/**
	 *
	 */
	int index() const;
    /**
      * Returns true if snap to grid is enabled. Otherwise false.
      */
    bool isSnapEnabled() const;
    /**
      * Returns the item speicified by the given @p uuid or 0 if @p uuid cannot be retrieved.
      */
    QAbstractDiagramGraphicsItem* item( const QString & uuid ) const;
	/**
	 * Returns the name of this page.
	 * @see setName()
	 */
	QString name() const;

	QDiagramToolkit::PaperSize paperSize() const;

	QSizeF paperSize(QDiagramToolkit::Unit unit) const;
	/**
	 * Returns the current paper orientation.
	 */
	QDiagramToolkit::PaperOrientation paperOrientation() const;
    /**
      * Sets the snap @p size.
      */
    void setSnapSize( const QSize & size );
	/**
	 * Sets the page's name to @p name.
	 * @name()
	 */
	void setName(const QString & name);
	/**
	 * Sets the paper orientation to @p orientation.
	 */
	void setPaperOrientation(QDiagramToolkit::PaperOrientation orientation);
	/**
	 * Sets the scene's paper size.
	 */
	void setPaperSize(QDiagramToolkit::PaperSize paperSize);
	/**
	 *
	 */
	void setPaperSize(const QSizeF & paperSize, QDiagramToolkit::Unit unit);
    /**
      * Sets the selection @p color.
      */
    void setSelectionColor(const QColor & color);
	/**
	 * If @p on is true the snap mode is enabled.
	 * @see isSnapEnabled()
	 */
	void setSnapEnabled(bool on);
    /**
      * Returns the diagram's snap size.
      */
    QSize snapSize() const;
signals:
    void handleConnectItemsEvent(QAbstractDiagramShape* from, QAbstractDiagramShape* to, const QMap<QString,QVariant> & properties);
    void handleDragEnterEvent(QGraphicsSceneDragDropEvent *event);
    void handleDragLeaveEvent(QGraphicsSceneDragDropEvent *event);
    void handleDragMoveEvent(QGraphicsSceneDragDropEvent *event);
    void handleDropEvent(QGraphicsSceneDragDropEvent *event);

    void itemMoved(QGraphicsItem* item, const QPointF & oldPos, const QPointF & newPos);
	void itemPropertyChanged(QAbstractDiagramGraphicsItem* item, const QString & name);

	void propertiesChanged();
public slots:

protected:
//    void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);
    void dragEnterEvent(QGraphicsSceneDragDropEvent* event);
    void dragLeaveEvent(QGraphicsSceneDragDropEvent *event);
    void dragMoveEvent(QGraphicsSceneDragDropEvent* event);
	void drawForeground(QPainter* painter, const QRectF & rect);
    void dropEvent(QGraphicsSceneDragDropEvent* event);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
private:
	friend QAbstractDiagramGraphicsItem;
	void emitItemPropertyChanged(QAbstractDiagramGraphicsItem* item, const QString & name);
	QString m_name;
    QGraphicsItem* m_movingItem;
	QDiagramToolkit::PaperSize m_paperSize;
    QPointF m_oldPos;
	QDiagramToolkit::PaperOrientation m_paperOrientation;
    QColor m_selectionColor;
    bool m_snapEnabled;
    QSize m_snapSize;
};

#endif // QABSTRACTDIAGRAMSHEET_H

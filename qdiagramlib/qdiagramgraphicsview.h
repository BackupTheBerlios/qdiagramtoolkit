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
#ifndef QDIAGRAMGRAPHICSVIEW_H
#define QDIAGRAMGRAPHICSVIEW_H

#include "qdiagramlib_global.h"

#include <QGraphicsView>
#include <QVarLengthArray>

#include <qdiagram.h>
#include <qdiagramconnectorstyle.h>

class QAbstractDiagramShape;
class QDiagramShape;

//! The QDiagramGraphicsView class provides a widget for displaying the contents of a QDiagram.
class QDIAGRAMLIBSHARED_EXPORT QDiagramGraphicsView : public QGraphicsView
{
    Q_OBJECT
	//! @property(gridColor)
	/**
	 * This property holds the current grid color.
	 */
    Q_PROPERTY(QColor gridColor READ gridColor WRITE setGridColor)
    //! @property(gridVisible)
    /**
      * This property holds whether the grid is displayed.
      * @see isGridVisible(), setGridVisible()
      */
    Q_PROPERTY(bool gridVisible READ isGridVisible WRITE showGrid)
    //    Q_PROPERTY(int zoom READ zoom WRITE setZoom)
public:
    explicit QDiagramGraphicsView(QWidget* parent = 0);
    /**
      * Returns a pointer to the diagram that is currently visualized in the view. If no diagram is currently visualized, 0 is returned.
      * @see setDiagram()
      */
    QDiagram* diagram() const;
    /**
      Returns the grid color.
      * @see setGridColor()
      */
    QColor gridColor() const;
    /**
      * Returns true if the grid is visible. Otherwise false.
      * @see setGridVisible()
      */
    bool isGridVisible() const;

	QDiagramToolkit::PointerMode mode() const;

    void setConnectorName(const QString & name);

    void setCurrentShapeName(const QString & name);
    /**
      * Sets the grid @p color.
      * @see gridColor
      */
    void setGridColor( const QColor & color );

    void setMode(QDiagramToolkit::PointerMode mode);
    /**
      * Returns the shape at the given @p pos or 0 if no shape is found.
      */
    QAbstractDiagramShape* shapeAt(const QPoint & pos) const;

	int zoom() const;
public slots:
    /**
      * Zooms in on the diagram.
      */
    void zoomIn();
    /**
      * Zooms out on the diagram.
      */
    void zoomOut();

    void setConnectorStyle(const QDiagramConnectorStyle & style);

    void showGrid(bool on);
    /**
      * Sets the zoom factor in @p percent and scales the view.
      **/
    void setZoom( int percent );
signals:
    void connectorCompleted(QAbstractDiagramShapeConnector* connector);
    void connectorDropped(QAbstractDiagramShapeConnector* connector);
    void connectorStarted(QAbstractDiagramShapeConnector* connector);

    void connectItems(QDiagramShape* from, QDiagramShape* to);
    void mouseScenePositionChanged(const QPointF & pos);
    /**
      * This signal is emitted when the view's scale has changed.
      */
    void zoomChanged(int percent);
protected:
    /**
      * Returns the connection point at position @p pos, which is in viewport coordinates.
      */
    QAbstractDiagramShapeConnectionPoint* connectionPointAt(const QPoint & pos);

    virtual void drawBackground(QPainter *painter, const QRectF &rect);
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);
private:

    QPointF gridPosition( const QPointF & pos, const QSizeF & grid ) const;
    QString m_connectorName;
    QDiagramConnectorStyle m_connectorStyle;
    QColor m_gridColor;
    QDiagramToolkit::PointerMode m_mode;
    bool m_showGrid;
    QSizeF m_gridSize;
    QPointF m_pos;
    QVarLengthArray<QLineF, 100> m_grid;
    int m_currentZoom;
    QAbstractDiagramShape* m_shapeUnderCursor;
    QAbstractDiagramShapeConnector* m_tempConnector;
    QAbstractDiagramShapeConnectionPoint* m_tempStartConnectionPoint;
    QAbstractDiagramShapeConnectionPoint* m_tempEndConnectionPoint;
};

#endif // QDIAGRAMGRAPHICSVIEW_H

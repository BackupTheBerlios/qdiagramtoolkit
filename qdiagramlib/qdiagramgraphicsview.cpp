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
#include "qdiagramgraphicsview.h"

#include <cmath>

#include "qabstractdiagramshapeconnector.h"
#include "qabstractdiagramshape.h"

#include "qdiagrampluginloader.h"
#include "qdiagramshape.h"
#include "qdiagramgraphicsscene.h"

QDiagramGraphicsView::QDiagramGraphicsView(QWidget* parent) :
    QGraphicsView(parent)
{
    m_currentZoom = 100;
    m_gridColor = QColor(Qt::lightGray);
    m_mode = QDiagramGraphicsView::Select;
    m_showGrid = true;
    m_gridSize = QSizeF(13, 13);
    m_shapeUnderCursor = 0;
    m_tempConnector = 0;
    m_tempStartConnectionPoint = 0;
    m_tempEndConnectionPoint = 0;

    setTransformationAnchor(AnchorUnderMouse);
}

QAbstractDiagramShapeConnectionPoint* QDiagramGraphicsView::connectionPointAt(const QPoint & pos)
{
    Q_FOREACH(QGraphicsItem* mItem, items(pos)){
        if (mItem->type() == QAbstractDiagramShapeConnectionPoint::Type){
            return dynamic_cast<QAbstractDiagramShapeConnectionPoint*>(mItem);
        }
    }
    return 0;
}

QDiagram* QDiagramGraphicsView::diagram() const
{
    return qobject_cast<QDiagram*>(scene()->parent());
}

void QDiagramGraphicsView::drawBackground(QPainter* painter, const QRectF & rect)
{
    if (!m_showGrid){
        return;
    }
    const int gridSize = m_gridSize.width();

    const int realLeft = static_cast<int>(std::floor(rect.left()));
    const int realRight = static_cast<int>(std::ceil(rect.right()));
    const int realTop = static_cast<int>(std::floor(rect.top()));
    const int realBottom = static_cast<int>(std::ceil(rect.bottom()));

    // Draw grid.
    const int firstLeftGridLine = realLeft - (realLeft % gridSize);
    const int firstTopGridLine = realTop - (realTop % gridSize);

    QVarLengthArray<QLine, 100> lines;

    for (qreal x = firstLeftGridLine; x <= realRight; x += gridSize)
        lines.append(QLine(x, realTop, x, realBottom));
    for (qreal y = firstTopGridLine; y <= realBottom; y += gridSize)
        lines.append(QLine(realLeft, y, realRight, y));

    //painter->setRenderHint(QPainter::Antialiasing);
    QPen mPen(m_gridColor);
    mPen.setStyle(Qt::DotLine);
    mPen.setCosmetic(true);
    painter->setPen(mPen);
    painter->drawLines(lines.data(), lines.size());
}

QColor QDiagramGraphicsView::gridColor() const
{
    return m_gridColor;
}

QPointF QDiagramGraphicsView::gridPosition( const QPointF & pos, const QSizeF & grid ) const
{
    QPointF mPos(pos);
    int mX = (pos.x() / grid.width());
    mX *= grid.width();
    int mY = (pos.y() / grid.height());
    mY *= grid.height();
    mPos.setX(mX - 1);
    mPos.setY(mY - 1);
    return mPos;
}

bool QDiagramGraphicsView::isGridVisible() const
{
    return m_showGrid;
}

QDiagramGraphicsView::Mode QDiagramGraphicsView::mode() const
{
	return m_mode;
}

void QDiagramGraphicsView::mouseMoveEvent(QMouseEvent* event)
{
    if (m_mode == QDiagramGraphicsView::Connect){
		QAbstractDiagramShapeConnectionPoint* cp = connectionPointAt(event->pos());
		if (event->buttons().testFlag(Qt::LeftButton) && m_tempConnector){
			QAbstractDiagramShape* shape = 0;
			if (cp == 0){
				shape = shapeAt(event->pos());
			} else {
				shape = cp->parentShape();
				if (!cp->canConnect(m_tempConnector)){
					cp = 0;
				}
			}
            // Hide previous shape's connection points
            if (m_shapeUnderCursor && m_shapeUnderCursor != shape){
//
//                m_shapeUnderCursor->setConnectionPointsVisible(false);
            }
            // Show current shape's connection points
            if (shape){
                shape->setConnectionPointsVisible(true);
            }
            //
            if (shape == 0){
                m_tempEndConnectionPoint = 0;
                m_tempConnector->setTemporaryEnd(mapToScene(event->pos()));
                setCursor(Qt::CrossCursor);
            } else {
                if (cp == 0 && m_tempEndConnectionPoint == 0){
                    m_tempConnector->setTemporaryEnd(mapToScene(event->pos()));
                    setCursor(Qt::CrossCursor);
                } else if (cp == 0 && m_tempEndConnectionPoint != 0){
                    if (!m_tempConnector->canConnect(m_tempStartConnectionPoint, m_tempEndConnectionPoint)){
                        m_tempConnector->setTemporaryEnd(mapToScene(event->pos()));
                        setCursor(Qt::ForbiddenCursor);
                    } else {
                        m_tempConnector->setTemporaryEnd(m_tempEndConnectionPoint->diagramPos(), m_tempEndConnectionPoint->orientation());
                        setCursor(Qt::CrossCursor);
                    }
                } else {
                    m_tempEndConnectionPoint = cp;
                    if (!m_tempConnector->canConnect(m_tempStartConnectionPoint, m_tempEndConnectionPoint)){
                        m_tempConnector->setTemporaryEnd(mapToScene(event->pos()));
                        setCursor(Qt::ForbiddenCursor);
                    } else {
                        m_tempConnector->setTemporaryEnd(m_tempEndConnectionPoint->diagramPos(), m_tempEndConnectionPoint->orientation());
                        setCursor(Qt::CrossCursor);
                    }
                }
            }

            m_shapeUnderCursor = shape;
		} else {
			if (cp){
				if (cp->connectors().size() < cp->maxConnections()){
					QAbstractDiagramPlugin* plugin = QDiagramPluginLoader::plugin(m_connectorStyle.plugin());
					if (plugin){
						// Start temporary connector
						QMap<QString,QVariant> pm;
						QVariantMap mm;
						mm["itemType"] = "connector";
						mm["itemClass"] = m_connectorStyle.shape();
						mm["plugin"] = plugin->name();
						pm["uuid"] = "{temp}";
						pm["style"] = m_connectorStyle.shape();
						//                        m_tempConnector = mPlugin->createConnector(diagram(), "<temporary>", mProperties);
						QAbstractDiagramShapeConnector* c = dynamic_cast<QAbstractDiagramShapeConnector*>(plugin->createItem(mm, pm, scene()));
						if (c && c->canStartWith(cp)){
							setCursor(Qt::CrossCursor);
						} else {
							setCursor(Qt::ForbiddenCursor);
						}
						//                    scene()->addItem(cTempConnector);
					} else {
						setCursor(Qt::CrossCursor);
					}
				} else {
					setCursor(Qt::ForbiddenCursor);
				}
			} else {
				setCursor(Qt::CrossCursor);
			}
		}
    } else {
    }
    QGraphicsView::mouseMoveEvent(event);
    emit mouseScenePositionChanged(mapToScene(event->pos()));
}

void QDiagramGraphicsView::mousePressEvent( QMouseEvent* event )
{
    if (event->button() == Qt::LeftButton && m_mode == Connect){
        QAbstractDiagramShapeConnectionPoint* cp = connectionPointAt(event->pos());
		m_tempStartConnectionPoint = 0;
        if (cp != 0){
            if (cp->maxConnections() == -1 || cp->connections().size() < cp->maxConnections()){
                cp->parentItem()->setFlag(QGraphicsItem::ItemIsMovable, false);
                cp->parentItem()->setFlag(QGraphicsItem::ItemIsSelectable, false);
                if (diagram()){
                    QAbstractDiagramPlugin* plugin = QDiagramPluginLoader::plugin(m_connectorStyle.plugin());
                    if (plugin){
                        // Start temporary connector
                        QMap<QString,QVariant> pm;
						QVariantMap mm;
                        mm["itemType"] = "connector";
                        mm["itemClass"] = m_connectorStyle.shape();
						mm["plugin"] = plugin->name();
                        pm["uuid"] = "{temp}";
                        pm["style"] = m_connectorStyle.shape();
//                        m_tempConnector = mPlugin->createConnector(diagram(), "<temporary>", mProperties);
                        m_tempConnector = dynamic_cast<QAbstractDiagramShapeConnector*>(plugin->createItem(mm, pm, scene()));
						if (m_tempConnector && m_tempConnector->canStartWith(cp)){
							scene()->addItem(m_tempConnector);
                            m_tempConnector->setTemporaryStart(cp->scenePos() + cp->boundingRect().center(), cp->orientation());
                            m_tempConnector->setTemporaryEnd(mapToScene(event->pos()), QDiagramToolkit::ConnectionPointOrientationInvalid);
		                    m_tempStartConnectionPoint = cp;
							qDebug() << cp->id();
						} else {
							delete m_tempConnector;
							m_tempConnector = 0;
						}
                    }
                }
            }
        }
    } else {
        QGraphicsView::mousePressEvent(event);
    }
}

void QDiagramGraphicsView::mouseReleaseEvent(QMouseEvent* event )
{
    if (m_tempConnector){
        m_tempStartConnectionPoint->parentItem()->setFlag(QGraphicsItem::ItemIsMovable, true);
        m_tempStartConnectionPoint->parentItem()->setFlag(QGraphicsItem::ItemIsSelectable, true);

        QAbstractDiagramShapeConnectionPoint* mPoint = connectionPointAt(event->pos());
        if (mPoint != 0){
            m_tempEndConnectionPoint = mPoint;
        }
        if (m_tempEndConnectionPoint != 0 && m_tempEndConnectionPoint != m_tempStartConnectionPoint){
            if (m_tempConnector->canConnect(m_tempStartConnectionPoint, m_tempEndConnectionPoint)){
                diagram()->addConnection(m_tempStartConnectionPoint, m_tempEndConnectionPoint, m_connectorStyle);
            }
        }
        delete m_tempConnector;
        m_tempConnector = 0;
        //
        if (m_tempStartConnectionPoint){
//            m_tempStartConnectionPoint->parentShape()->setConnectionPointsVisible(false);
        }
        m_tempStartConnectionPoint = 0;
        if (m_tempEndConnectionPoint){
//            m_tempEndConnectionPoint->parentShape()->setConnectionPointsVisible(false);
        }
        m_tempEndConnectionPoint = 0;
        m_shapeUnderCursor = 0;
        //
        setCursor(Qt::ArrowCursor);
    }
    QGraphicsView::mouseReleaseEvent(event);
}

void QDiagramGraphicsView::setConnectorStyle(const QDiagramConnectorStyle & style)
{
    m_connectorStyle = style;
}

void QDiagramGraphicsView::setConnectorName(const QString & name)
{
    m_connectorName = name;
}

void QDiagramGraphicsView::setGridColor( const QColor & color )
{
    m_gridColor = color;
    update();
}

void QDiagramGraphicsView::setMode(QDiagramGraphicsView::Mode mode)
{
    m_mode = mode;
    if (m_tempConnector){
        delete m_tempConnector;
        m_tempConnector = 0;
    }
    //
    Q_FOREACH(QAbstractDiagramGraphicsItem* i, diagram()->items()){
        QAbstractDiagramShape* s = qgraphicsitem_cast<QAbstractDiagramShape*>(i);
        if (s){
            s->setConnectionPointsVisible(QDiagramGraphicsView::Connect == mode);
        }
    }

    if (m_mode == QDiagramGraphicsView::Connect){
        setCursor(Qt::CrossCursor);
    } else {
        setCursor(Qt::ArrowCursor);
    }
}

void QDiagramGraphicsView::setZoom( int percent )
{
    if (percent != m_currentZoom){
        if (percent >= 25 && percent <= 800){
            m_currentZoom = percent;
            qreal mScale = m_currentZoom / 100.0;
            mScale /= matrix().m11();
            scale(mScale, mScale);
            emit zoomChanged(m_currentZoom);
        }
    }
}

QAbstractDiagramShape* QDiagramGraphicsView::shapeAt(const QPoint & pos) const
{
    Q_FOREACH(QGraphicsItem* mItem, items(pos)){
        if (mItem->type() == QAbstractDiagramShape::Type){
            return dynamic_cast<QAbstractDiagramShape*>(mItem);
        }
    }

    return 0;
}

void QDiagramGraphicsView::showGrid( bool on )
{
    m_showGrid = on;
    viewport()->update();
}

void QDiagramGraphicsView::wheelEvent(QWheelEvent* event)
{
    if (event->modifiers().testFlag(Qt::ControlModifier)){
        if (event->delta() < 0){
            zoomOut();
        } else {
            zoomIn();
        }
    } else {
        QGraphicsView::wheelEvent(event);
    }
}

void QDiagramGraphicsView::zoomIn()
{
    if (m_currentZoom > 175){
        setZoom(m_currentZoom + 200);
    } else {
        setZoom(m_currentZoom + 25);
    }
}

void QDiagramGraphicsView::zoomOut()
{
    if (m_currentZoom > 200){
        setZoom(m_currentZoom - 200);
    } else {
        setZoom(m_currentZoom - 25);
    }
}

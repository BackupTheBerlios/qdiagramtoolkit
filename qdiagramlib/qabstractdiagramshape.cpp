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
#include "qabstractdiagramshape.h"

#include "qabstractdiagramshapeconnectionpoint.h"
#include "qdiagramgraphicstextitem.h"
#include "qdiagrammetadata.h"
#include "qdiagrammetaproperty.h"
#include "qdiagramshapeconnector.h"
#include "qdiagramsheet.h"

QAbstractDiagramShape::QAbstractDiagramShape(QGraphicsItem* parent) :
    QAbstractDiagramGraphicsItem(parent)
{
    m_connectionPointsVisible = false;

    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
	setFlag(QGraphicsItem::ItemIsFocusable);

	m_textItem = new QDiagramGraphicsTextItem(this);
}

QAbstractDiagramShape::QAbstractDiagramShape(const QString & plugin, const QString & itemClass, const QMap<QString, QVariant> &properties, QGraphicsItem *parent) :
    QAbstractDiagramGraphicsItem(properties.value("uuid").toString(), plugin, "Shape", itemClass, parent)
{
    m_connectionPointsVisible = false;
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
	setFlag(QGraphicsItem::ItemIsFocusable);

	m_textItem = new QDiagramGraphicsTextItem(this);
//	m_textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
}

void QAbstractDiagramShape::addSizeGripHandle(QDiagramShapeSizeGripHandle* handle)
{
    m_handles.append(handle);
}

QAbstractDiagramShape::~QAbstractDiagramShape()
{
    while(!m_connectionPoints.isEmpty()){
        delete m_connectionPoints.takeFirst();
    }
}

void QAbstractDiagramShape::addConnection(QAbstractDiagramGraphicsItem::ConnectorDirection direction, QAbstractDiagramShapeConnector* connector)
{
    Q_FOREACH(ConnectionData d, m_connections){
        if (d.connector == connector){
            return;
        }
    }
    ConnectionData d;
    d.direction = direction;
    d.connector = connector;
    m_connections.append(d);
}

void QAbstractDiagramShape::addConnectionPoint(QAbstractDiagramShapeConnectionPoint* point)
{
//    Q_FOREACH(QAbstractDiagramShapeConnectionPoint* mPoint, m_connectionPoints){
//        if (mPoint->index() == point->index()){
//            return;
//        }
//    }
    m_connectionPoints.append(point);
}

QAbstractDiagramShapeConnectionPoint* QAbstractDiagramShape::connectionPoint(const QString & id) const
{
    Q_FOREACH(QAbstractDiagramShapeConnectionPoint* p, m_connectionPoints){
        if (p->id() == id){
            return p;
        }
    }
    return 0;
}

QAbstractDiagramShapeConnectionPoint* QAbstractDiagramShape::connectionPoint(const QPointF & pos) const
{
    Q_FOREACH(QAbstractDiagramShapeConnectionPoint* p, m_connectionPoints){
        if (p->rect().contains(pos)){
            return p;
        }
    }
    return 0;
}

QList<QAbstractDiagramShapeConnectionPoint*> QAbstractDiagramShape::connectionPoints() const
{
    return m_connectionPoints;
}

QList<QAbstractDiagramShapeConnectionPoint *> QAbstractDiagramShape::connectionPoints(QDiagramToolkit::ConnectionPointOrientation orientation) const
{
    QList<QAbstractDiagramShapeConnectionPoint*> l;
    Q_FOREACH(QAbstractDiagramShapeConnectionPoint* cp, m_connectionPoints){
        if (cp->orientation() == orientation){
            l.append(cp);
        }
    }
    return l;
}

QList<QAbstractDiagramShapeConnector *> QAbstractDiagramShape::connectors() const
{
    QList<QAbstractDiagramShapeConnector *> l;

    Q_FOREACH(QAbstractDiagramShapeConnectionPoint* cp, m_connectionPoints){
        Q_FOREACH(QAbstractDiagramShapeConnectionPoint::Connection c, cp->connections()){
            l << c.connector;
        }
    }

    return l;
}

QRectF QAbstractDiagramShape::focusRect() const
{
	return boundingRect();
}

bool QAbstractDiagramShape::isConnectionAllowed(QAbstractDiagramShapeConnector* connector) const
{
    Q_UNUSED(connector);
    return true;
}

void QAbstractDiagramShape::keyPressEvent(QKeyEvent* event)
{
	if (textItem()){
		if (textItem()->textInteractionFlags().testFlag(Qt::TextEditorInteraction)){
			if (event->key() == Qt::Key_Escape){
				textItem()->setEditModeEnabled(false);
				event->accept();
				return;
			}
		} else {
			if (!event->text().isEmpty()){
				QChar c(event->text().at(0));
				if (c.isPrint()){
					textItem()->setPlainText(event->text());
					textItem()->setEditModeEnabled(true, QTextCursor::End);
					event->accept();
					return;
				}
			}
		}
	}
	QAbstractDiagramGraphicsItem::keyPressEvent(event);
}

QFont QAbstractDiagramShape::pointToPixel(const QFont & font) const
{
	QFont f(font);
	QFontInfo fi(f);
	if (f.pointSizeF() != -1){
		f.setPixelSize(fi.pixelSize() * 10);
	}
	return f;
}

void QAbstractDiagramShape::restoreProperties(const QVariantMap & p)
{
	QAbstractDiagramGraphicsItem::restoreProperties(p);
	if (textItem()){
		textItem()->restoreProperties(p);
	}
	updateSizeGripHandles();
}

void QAbstractDiagramShape::restoreFromProperties(const QVariantMap & properties)
{
	blockUndoCommands(true);
	for (int i = 0; i < metaData()->propertyCount(); i++){
		if (properties.contains(metaData()->property(i).name())){
			if (metaData()->property(i).type() == QDiagramToolkit::Rect){
				if (properties[metaData()->property(i).name()].canConvert(QVariant::Map)){
					QRectF r;
					QVariantMap m = properties.value(metaData()->property(i).name()).toMap();
					r.setX(m.value("x").toDouble());
					r.setY(m.value("y").toDouble());
					r.setWidth(m.value("width").toDouble());
					r.setHeight(m.value("height").toDouble());

					setProperty(metaData()->property(i).name(), r);
				}
			}
		}
	}
    updateConnectionPoints();
	blockUndoCommands(false);
}

QVariant QAbstractDiagramShape::itemGeometryHasChanged(const QVariant & value)
{
	updateConnectionPoints();
	return value;
}

QVariant QAbstractDiagramShape::itemPositionHasChanged( const QVariant & value )
{
    QVariant v = QAbstractDiagramGraphicsItem::itemPositionHasChanged(value);
    updateConnectionPoints();
    updateConnectedItems();
    updateSizeGripHandles();
    return v;
}

QVariant QAbstractDiagramShape::itemPropertyHasChanged(const QString &name, const QVariant &value)
{
	if (textItem()){
		textItem()->itemPropertyHasChanged(this, name, value);
	}
	return QAbstractDiagramGraphicsItem::itemPropertyHasChanged(name, value);
}

QVariant QAbstractDiagramShape::itemSceneHasChanged(const QVariant & value)
{
    updateConnectionPoints();
	return value;
}

QVariant QAbstractDiagramShape::itemSelectedHasChanged( const QVariant & value )
{
    update();
    Q_FOREACH(QDiagramShapeSizeGripHandle* h, m_handles){
        h->setVisible(value.toBool());
    }
    return value;
}

void QAbstractDiagramShape::paintConnectionPoints(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (connectionPointsAreVisible()){
        Q_FOREACH(QAbstractDiagramShapeConnectionPoint* p, connectionPoints()){
            p->paint(painter, option, widget);
        }
    }
}

void QAbstractDiagramShape::paintFocus(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->save();
    if (isSelected()){
        painter->setBrush(Qt::NoBrush);
		QPen p(QColor("cadetblue"));
		p.setWidthF(5);
		p.setStyle(Qt::DashDotDotLine);
		setPen(p);
        painter->setPen(p);
        painter->drawRect(focusRect());
    }
    painter->restore();
}

QColor QAbstractDiagramShape::selectionColor() const
{
    QDiagramSheet* s = qobject_cast<QDiagramSheet*>(sheet());
    if (s){
        return s->selectionColor();
    }
    return Qt::magenta;
}

void QAbstractDiagramShape::setConnectionPointsVisible(bool visible)
{
    if (visible != m_connectionPointsVisible){
        updateConnectionPoints();
        m_connectionPointsVisible = visible;
        Q_FOREACH(QAbstractDiagramShapeConnectionPoint* p, m_connectionPoints){
            p->setVisible(m_connectionPointsVisible);
        }
        update();
    }
}

QPainterPath QAbstractDiagramShape::shape() const
{
    return QGraphicsItem::shape();
}

bool QAbstractDiagramShape::connectionPointsAreVisible() const
{
    return m_connectionPointsVisible;
}

QDiagramGraphicsTextItem* QAbstractDiagramShape::textItem() const
{
	return m_textItem;
}

void QAbstractDiagramShape::updateConnectedItems()
{
    Q_FOREACH(QAbstractDiagramShapeConnectionPoint* p, m_connectionPoints){
        p->updateConnectors();
    }
}

void QAbstractDiagramShape::updateConnectionPoints()
{
    Q_FOREACH(QAbstractDiagramShapeConnectionPoint* p, m_connectionPoints){
        p->updatePosition();
    }
}

void QAbstractDiagramShape::updateSizeGripHandles()
{
    Q_FOREACH(QDiagramShapeSizeGripHandle* handle, m_handles){
        handle->updatePosition();
    }
}

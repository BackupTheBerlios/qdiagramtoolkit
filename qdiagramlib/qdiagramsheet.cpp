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
#include "qdiagramsheet.h"

#include "qabstractdiagram.h"
#include "qabstractdiagramgraphicsitem.h"

QMap<QDiagramToolkit::PaperSize, QSizeF> sPaperSizes;

QDiagramSheet::QDiagramSheet(QAbstractDiagram* diagram) :
    QGraphicsScene(diagram)
{
	if (sPaperSizes.isEmpty()){
		sPaperSizes[QDiagramToolkit::A0] = QSizeF(8410, 11890);
		sPaperSizes[QDiagramToolkit::A1] = QSizeF(5940, 8410);
		sPaperSizes[QDiagramToolkit::A2] = QSizeF(4200, 5940);
		sPaperSizes[QDiagramToolkit::A3] = QSizeF(2970, 4200);
		sPaperSizes[QDiagramToolkit::A4] = QSizeF(2100, 2970);
	}
    m_movingItem = 0;
	setPaperSize(QDiagramToolkit::A4);
	m_paperOrientation = QDiagramToolkit::Portrait;
    m_snapEnabled = true;
    m_snapSize = QSize(50, 50);
	setBackgroundBrush(Qt::white);
}

QAbstractDiagram* QDiagramSheet::diagram() const
{
    return qobject_cast<QAbstractDiagram*>(parent());
}

void QDiagramSheet::dragEnterEvent( QGraphicsSceneDragDropEvent* event )
{
    diagram()->dragEnterEventHandler(event);
}

void QDiagramSheet::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
    diagram()->dragLeaveEventHandler(event);
}

void QDiagramSheet::dragMoveEvent(QGraphicsSceneDragDropEvent* event)
{
    diagram()->dragMoveEventHandler(event);
}

void QDiagramSheet::drawForeground(QPainter* painter, const QRectF & rect)
{
	diagram()->drawDiagramFrame(painter, this);
	QGraphicsScene::drawForeground(painter, rect);
}

void QDiagramSheet::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    diagram()->dropEventHandler(event);
}

void QDiagramSheet::emitItemPropertyChanged(QAbstractDiagramGraphicsItem* item, const QString & name)
{
	emit itemPropertyChanged(item, name);
}

int QDiagramSheet::index() const
{
	if (diagram()){
		return diagram()->indexOf((QDiagramSheet*)this);
	}
	return -1;
}

bool QDiagramSheet::isSnapEnabled() const
{
    return m_snapEnabled;
}

QAbstractDiagramGraphicsItem* QDiagramSheet::item( const QString & uuid ) const
{
    Q_FOREACH(QGraphicsItem* mGraphicsItem, items()){
        QAbstractDiagramGraphicsItem* i = dynamic_cast<QAbstractDiagramGraphicsItem*>(mGraphicsItem);
        if (i && i->uuid() == uuid){
            return i;
        }
    }
    return 0;
}

void QDiagramSheet::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    m_movingItem = itemAt(event->buttonDownScenePos(Qt::LeftButton));

    if (m_movingItem != 0 && event->button() == Qt::LeftButton) {
        if (m_movingItem->parentItem()){
            m_oldPos = m_movingItem->parentItem()->pos();
            m_movingItem = m_movingItem->parentItem();
        } else {
            m_oldPos = m_movingItem->pos();
        }
    }
    QGraphicsScene::mousePressEvent(event);
}

void QDiagramSheet::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_movingItem != 0 && event->button() == Qt::LeftButton) {
        if (m_oldPos != m_movingItem->pos()){
            emit itemMoved(m_movingItem, m_oldPos, m_movingItem->pos());
        }
        m_movingItem = 0;
    }
    QGraphicsScene::mouseReleaseEvent(event);
}

QString QDiagramSheet::name() const
{
	return m_name;
}

QDiagramToolkit::PaperOrientation QDiagramSheet::paperOrientation() const
{
	return m_paperOrientation;
}

QDiagramToolkit::PaperSize QDiagramSheet::paperSize() const
{
	return m_paperSize;
}

QSizeF QDiagramSheet::paperSize(QDiagramToolkit::Unit unit) const
{
	QSizeF s(sceneRect().size());

	return s;
}

QColor QDiagramSheet::selectionColor() const
{
    return m_selectionColor;
}

void QDiagramSheet::setSnapSize( const QSize & size )
{
    m_snapSize = size;
}

void QDiagramSheet::setName(const QString & name)
{
	if (m_name != name){
		m_name = name;
		emit propertiesChanged();
	}
}

void QDiagramSheet::setPaperOrientation(QDiagramToolkit::PaperOrientation orientation)
{
	if (m_paperOrientation != orientation){
		setSceneRect(0, 0, sceneRect().height(), sceneRect().width());
		m_paperOrientation = orientation;
	}
}

void QDiagramSheet::setPaperSize(QDiagramToolkit::PaperSize paperSize)
{
	if (m_paperSize != paperSize){
		if (m_paperOrientation == QDiagramToolkit::Landscape){
			QRectF r(0, 0, 0, 0);
			r.setHeight(sPaperSizes.value(paperSize).width());
			r.setWidth(sPaperSizes.value(paperSize).height());
			setSceneRect(r);
		} else {
			setSceneRect(QRectF(QPointF(0, 0), sPaperSizes.value(paperSize)));
		}
		m_paperSize = paperSize;
	}
}

void QDiagramSheet::setPaperSize(const QSizeF & paperSize, QDiagramToolkit::Unit unit)
{
	QSizeF s;
	if (unit == QDiagramToolkit::Millimeter){
		s.setWidth(paperSize.width() * 10);
		s.setHeight(paperSize.height() * 10);
	} else {
		s = QSizeF(paperSize);
	}
	QMapIterator<QDiagramToolkit::PaperSize, QSizeF> it(sPaperSizes);
	if (m_paperOrientation == QDiagramToolkit::Landscape){
		while(it.hasNext()){
			it.next();
			if (it.value().width() == s.height() && it.value().height() == s.width()){
				setPaperSize(it.key());
				return;
			}
		}
	} else {
		while(it.hasNext()){
			it.next();
			if (it.value().width() == s.width() && it.value().height() == s.height()){
				setPaperSize(it.key());
				return;
			}
		}
	}
	if (unit == QDiagramToolkit::Millimeter){
		m_paperSize = QDiagramToolkit::Custom;
		setSceneRect(0, 0, s.width(), s.height());
	}
}

void QDiagramSheet::setSelectionColor(const QColor & color)
{
    m_selectionColor = color;
}

void QDiagramSheet::setSnapEnabled(bool on)
{
	m_snapEnabled = on;
}

QSize QDiagramSheet::snapSize() const
{
    return m_snapSize;
}

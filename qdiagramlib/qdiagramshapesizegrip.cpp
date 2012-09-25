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
#include "qdiagramshapesizegrip.h"

#include "qabstractdiagramscene.h"
#include "qabstractdiagramshape.h"

QDiagramShapeSizeGripHandle::QDiagramShapeSizeGripHandle(QDiagramShapeSizeGripHandle::HandlePosition position, QGraphicsItem* parent) :
    QGraphicsRectItem(0, 0, 6, 6, parent)
{
    m_active = false;
    m_posFlag = position;
    setBrush(QBrush(Qt::lightGray));

    setAcceptsHoverEvents(true);
    if (m_posFlag & QDiagramShapeSizeGripHandle::Bottom || m_posFlag & QDiagramShapeSizeGripHandle::Top){
        setCursor(QCursor(Qt::SizeVerCursor));
    } else if (m_posFlag & QDiagramShapeSizeGripHandle::BottomLeft || m_posFlag & QDiagramShapeSizeGripHandle::TopRight){
        setCursor(QCursor(Qt::SizeBDiagCursor));
    } else if (m_posFlag & QDiagramShapeSizeGripHandle::BottomRight || m_posFlag & QDiagramShapeSizeGripHandle::TopLeft){
        setCursor(QCursor(Qt::SizeFDiagCursor));
    } else if (m_posFlag & QDiagramShapeSizeGripHandle::Left || m_posFlag & QDiagramShapeSizeGripHandle::Right){
        setCursor(QCursor(Qt::SizeHorCursor));
    }
    setVisible(false);
}

QDiagramShapeSizeGripHandle::~QDiagramShapeSizeGripHandle()
{
}

QDiagramShapeSizeGripHandle::HandlePosition QDiagramShapeSizeGripHandle::handlePosition() const
{
    return m_posFlag;
}

void QDiagramShapeSizeGripHandle::updatePosition()
{
    if (m_posFlag & QDiagramShapeSizeGripHandle::Bottom){
        setPos(parentItem()->boundingRect().center().x() - boundingRect().width() / 2, parentItem()->boundingRect().height() - boundingRect().height());
    } else if (m_posFlag & QDiagramShapeSizeGripHandle::BottomLeft){
        setPos(0, parentItem()->boundingRect().height() - boundingRect().height());
    } else if (m_posFlag & QDiagramShapeSizeGripHandle::BottomRight){
        setPos(parentItem()->boundingRect().width() - boundingRect().width(), parentItem()->boundingRect().height() - boundingRect().height());
    } else if (m_posFlag & QDiagramShapeSizeGripHandle::Left){
        setPos(0, parentItem()->boundingRect().center().y() - boundingRect().height() / 2);
    } else if (m_posFlag & QDiagramShapeSizeGripHandle::Right){
        setPos(parentItem()->boundingRect().width() - boundingRect().width(), parentItem()->boundingRect().center().y() - boundingRect().height() / 2);
    } else if (m_posFlag & QDiagramShapeSizeGripHandle::Top){
        setPos(parentItem()->boundingRect().center().x() - boundingRect().width() / 2, 0);
    } else if (m_posFlag & QDiagramShapeSizeGripHandle::TopLeft){
        setPos(0, 0);
    } else if (m_posFlag & QDiagramShapeSizeGripHandle::TopRight){
        setPos(parentItem()->boundingRect().width() - boundingRect().width(), 0);
    }
}

qreal QDiagramShapeSizeGripHandle::alignHorizontal(qreal value)
{
    QAbstractDiagramScene* sc = qobject_cast<QAbstractDiagramScene*>(scene());
    if (sc && sc->isSnapToGridEnabled()){
        return sc->gridSize().width() * (int)(value / sc->gridSize().width());
    }
    return value;
}

qreal QDiagramShapeSizeGripHandle::alignVertical(qreal value)
{
    QAbstractDiagramScene* sc = qobject_cast<QAbstractDiagramScene*>(scene());
    if (sc && sc->isSnapToGridEnabled()){
        return sc->gridSize().height() * (int)(value / sc->gridSize().height());
    }
    return value;
}

void QDiagramShapeSizeGripHandle::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    Q_UNUSED(event);
    m_active = true;
    parentItem()->setFlag(ItemIsMovable, false);
}

void QDiagramShapeSizeGripHandle::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    Q_UNUSED(event);
    m_active = false;
    parentItem()->setFlag(ItemIsMovable);
}

void QDiagramShapeSizeGripHandle::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->buttons().testFlag(Qt::LeftButton)){
        QAbstractDiagramShape* shape = dynamic_cast<QAbstractDiagramShape*>(parentItem());
        if (shape){
//            QRectF r = shape->property("geometry").toRectF();
            QRectF r = shape->geometry();
            if (m_posFlag == QDiagramShapeSizeGripHandle::Bottom || m_posFlag == QDiagramShapeSizeGripHandle::BottomLeft || m_posFlag == QDiagramShapeSizeGripHandle::BottomRight){
                if (alignVertical(event->scenePos().y()) > r.top()){
                    r.setBottom(alignVertical(event->scenePos().y()));
                }
            }
            if (m_posFlag == QDiagramShapeSizeGripHandle::Left || m_posFlag == QDiagramShapeSizeGripHandle::BottomLeft || m_posFlag == QDiagramShapeSizeGripHandle::TopLeft){
                if (alignHorizontal(event->scenePos().x()) < r.right()){
                    r.setLeft(alignHorizontal(event->scenePos().x()));
                }
            }
            if (m_posFlag == QDiagramShapeSizeGripHandle::Right || m_posFlag == QDiagramShapeSizeGripHandle::BottomRight || m_posFlag == QDiagramShapeSizeGripHandle::TopRight){
                if (alignHorizontal(event->scenePos().x()) > r.left()){
                    r.setRight(alignHorizontal(event->scenePos().x()));
                }
            }
            if (m_posFlag == QDiagramShapeSizeGripHandle::Top || m_posFlag == QDiagramShapeSizeGripHandle::TopLeft || m_posFlag == QDiagramShapeSizeGripHandle::TopRight){
                if (alignVertical(event->scenePos().y()) < r.bottom()){
                    r.setTop(alignVertical(event->scenePos().y()));
                }
            }
            if (event->modifiers().testFlag(Qt::ShiftModifier)){
                if (m_posFlag == QDiagramShapeSizeGripHandle::BottomLeft
                        || m_posFlag == QDiagramShapeSizeGripHandle::BottomRight
                        || m_posFlag == QDiagramShapeSizeGripHandle::TopLeft
                        || m_posFlag == QDiagramShapeSizeGripHandle::TopRight){
                    if (r.width() < r.height()){
                        r.setWidth(r.height());
                    } else {
                        r.setHeight(r.width());
                    }
                }
            }
            shape->setProperty("geometry", r);
        }
    }
}

void QDiagramShapeSizeGripHandle::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::LeftButton){
        event->accept();
        m_mousePos = event->scenePos();
        QAbstractDiagramShape* sp = dynamic_cast<QAbstractDiagramShape*>(parentItem());
        if (sp){
            m_rect = sp->property("geometry").toRectF();
        }
    } else {
        event->ignore();
    }
}

void QDiagramShapeSizeGripHandle::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton){
//        QAbstractDiagramScene* sc = qobject_cast<QAbstractDiagramScene*>(scene());
//        QAbstractDiagramShape* sp = dynamic_cast<QAbstractDiagramShape*>(parentItem());
//        if (sp && sc){
//            if (m_pos == QDiagramShapeSizeGripHandle::Bottom){
//                qreal dy = sc->gridSize().height() * (int)((event->scenePos().y() - m_mousePos.y()) / sc->gridSize().height());
//                QRectF r = sp->property("geometry").toRectF();
//                r.adjust(0, 0, 0, dy);
//                if (r.height() > 0){
//                    sp->setProperty("geometry", r);
//                }
//            } else if (m_pos == QDiagramShapeSizeGripHandle::Top){
//                qreal dy = sc->gridSize().height() * (int)((event->scenePos().y() - m_mousePos.y()) / sc->gridSize().height());
//                QRectF r = sp->property("geometry").toRectF();
//                r.adjust(0, 0, 0, -dy);
//                r.setY(event->scenePos().y());
//                if (r.height() > 0){
//                    sp->setProperty("geometry", r);
//                }
//            }
//        }
    }
}

QPointF QDiagramShapeSizeGripHandle::restrictPosition(const QPointF & newPos)
{
    QPointF p = pos();
    if (m_posFlag & QDiagramShapeSizeGripHandle::Top || m_posFlag & QDiagramShapeSizeGripHandle::Bottom){
        p.setY(alignVertical(newPos.y()));
    } else if (m_posFlag & QDiagramShapeSizeGripHandle::Left || m_posFlag & QDiagramShapeSizeGripHandle::Right){
        p.setX(alignHorizontal(newPos.x()));
    }
    return p;
}

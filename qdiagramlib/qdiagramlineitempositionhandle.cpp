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
#include "qdiagramlineitempositionhandle.h"

#include <qabstractdiagramgraphicsitem.h>

QDiagramLineItemPositionHandle::QDiagramLineItemPositionHandle(PositionFlag flag, QGraphicsItem *parent) :
    QAbstractDiagramGraphicsItemHandle(parent)
{
    m_posFlag = flag;
    if (m_posFlag == Start){
        setRect(0, 0, 8, 8);
    } else {
        setRect(0, 0, 6, 6);
    }
    setBrush(QBrush(Qt::red));
    setVisible(false);
}

QDiagramLineItemPositionHandle::PositionFlag QDiagramLineItemPositionHandle::positionFlag() const
{
    return m_posFlag;
}

void QDiagramLineItemPositionHandle::updatePosition()
{
    QAbstractDiagramGraphicsItem* item= dynamic_cast<QAbstractDiagramGraphicsItem*>(parentItem());
    if (item){
        QLineF l(item->property("p1").toPointF(), item->property("p2").toPointF());
        QRectF r = item->shape().boundingRect();
        if (m_posFlag == End){
            if (l.angle() < 90.0){
                setPos(r.width() - boundingRect().width() / 2, boundingRect().height() / -2);
            } else if (l.angle() >= 90.0 && l.angle() < 180.0){
                setPos(boundingRect().width() / -2, boundingRect().height() / -2);
            } else if (l.angle() >= 180.0 && l.angle() < 270.0){
                setPos(boundingRect().width() / -2, r.height() - boundingRect().height() / 2);
            } else if (l.angle() >= 270.0){
                setPos(r.width() - boundingRect().width() / 2, r.height() - boundingRect().height() / 2);
            }
        } else if (m_posFlag == Start){
            if (l.angle() < 90.0){
                setPos(boundingRect().width() / -2, r.height() - boundingRect().height() / 2);
            } else if (l.angle() >= 90.0 && l.angle() < 180.0){
                setPos(r.width() - boundingRect().width() / 2, r.height() - boundingRect().height() / 2);
            } else if (l.angle() >= 180.0 && l.angle() < 270.0){
                setPos(r.width() - boundingRect().width() / 2, boundingRect().height() / -2);
            } else if (l.angle() >= 270.0){
                setPos(boundingRect().width() / -2, boundingRect().height() / -2);
            }
        }
    }
}

void QDiagramLineItemPositionHandle::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->buttons().testFlag(Qt::LeftButton)){
        QAbstractDiagramGraphicsItem* item= dynamic_cast<QAbstractDiagramGraphicsItem*>(parentItem());
        if (item){
            if (m_posFlag == End){
                item->setProperty("p2", event->scenePos());
            } else if (m_posFlag == Start){
                item->setProperty("p1", event->scenePos());
            }
        }
    }
}

void QDiagramLineItemPositionHandle::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton){
        m_scenePos = event->scenePos();
        event->accept();
    } else {
        event->ignore();
    }
}

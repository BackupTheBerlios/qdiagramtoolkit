/******************************************************************************
** Copyright (C) 2011 - 2012 Martin Hoppe martin@2x2hoppe.de
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
#include "qstandardline.h"

#include <qdiagramendoflinestyle.h>
#include <qdiagramlineitempositionhandle.h>
#include <qdiagramlinestyle.h>

#define PI 3.14159265

QStandardLine::QStandardLine(const QMap<QString, QVariant> &properties, QGraphicsItem *parent) :
    QAbstractDiagramGraphicsItem(properties.value("uuid").toString(), "line", parent)
{
    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
    setFlag(ItemSendsGeometryChanges);

    addProperty("plugin", QDiagramGraphicsItemMetaProperty::String, true, properties.value("plugin").toString());
    addProperty("p1", QDiagramGraphicsItemMetaProperty::Point, false, properties.value("p1"));
    addProperty("p2", QDiagramGraphicsItemMetaProperty::Point, false, properties.value("p2"));
    addProperty("text", QDiagramGraphicsItemMetaProperty::Text, false, properties.value("text"));
    addProperty("lineStyle", QDiagramGraphicsItemMetaProperty::LineStyle, false, properties.value("lineStyle"));
    addProperty("startOfLine", QDiagramGraphicsItemMetaProperty::EndOfLineStyle, false, properties.value("startOfLine"));
    addProperty("endOfLine", QDiagramGraphicsItemMetaProperty::EndOfLineStyle, false, properties.value("endOfLine"));

    m_handles.append(new QDiagramLineItemPositionHandle(QDiagramLineItemPositionHandle::Start, this));
    m_handles.append(new QDiagramLineItemPositionHandle(QDiagramLineItemPositionHandle::End, this));
}

QRectF QStandardLine::boundingRect() const
{
    QRectF r = QRectF(property("p1").toPointF(), property("p2").toPointF()).normalized();
    qreal w = 0;
    qreal h = 0;
    Q_FOREACH(QDiagramLineItemPositionHandle* handle, m_handles){
        if (handle->boundingRect().width() > w){
            w = handle->boundingRect().width();
        }
        if (handle->boundingRect().height() > h){
            h = handle->boundingRect().height();
        }
    }
    r.moveTo(0, 0);
    qreal extraX = 0.0;
    qreal extraY = 0.0;
    if (m_startOfLine.boundingRect().width() < m_endOfLine.boundingRect().width()){
        extraX = m_endOfLine.boundingRect().width();
    } else {
        extraX = m_startOfLine.boundingRect().width();
    }
    if (m_startOfLine.boundingRect().height() < m_endOfLine.boundingRect().height()){
        extraY = m_endOfLine.boundingRect().height();
    } else {
        extraY = m_startOfLine.boundingRect().height();
    }
    r.adjust(-extraX,-extraY, extraX, extraY);
    return r;
}

QVariant QStandardLine::itemPropertyHasChanged(const QString &name, const QVariant &value)
{
    if (name == "p1"){
        prepareGeometryChange();
        QLineF l(property("p1").toPointF(), property("p2").toPointF());
        if (l.angle() < 90.0){
            setPos(l.p1().x(), l.p2().y());
        } else if (l.angle() >= 90.0 && l.angle() < 180.0){
            setPos(l.p2());
        } else if (l.angle() >= 180.0 && l.angle() < 270.0){
            setPos(l.p2().x(), l.p1().y());
        } else if (l.angle() >= 270.0){
            setPos(l.p1());
        }
    } else if (name == "p2"){
        prepareGeometryChange();
        QLineF l(property("p1").toPointF(), property("p2").toPointF());
        if (l.angle() < 90.0){
            setPos(l.p1().x(), l.p2().y());
        } else if (l.angle() >= 90.0 && l.angle() < 180.0){
            setPos(l.p2());
        } else if (l.angle() >= 180.0 && l.angle() < 270.0){
            setPos(l.p2().x(), l.p1().y());
        } else if (l.angle() >= 270.0){
            setPos(l.p1());
        }
    }
    return value;
}

QVariant QStandardLine::itemSelectedHasChanged( const QVariant & value )
{
    Q_FOREACH(QDiagramLineItemPositionHandle* handle, m_handles){
        handle->setVisible(value.toBool());
    }
    return value;
}

void QStandardLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QLineF l(property("p1").toPointF(), property("p2").toPointF());

    QDiagramLineStyle lineStyle = qvariant_cast<QDiagramLineStyle>(property("lineStyle"));

    painter->setPen(lineStyle.pen());
    painter->setBrush(QBrush(lineStyle.pen().color()));
    painter->drawPath(shape());
    painter->setPen(lineStyle.pen().color());
    paintStartOfLine(painter);
    paintEndOfLine(painter);
    painter->rotate(l.angle());
    painter->drawText(boundingRect(), Qt::AlignCenter, property("text").toString());

    Q_FOREACH(QDiagramLineItemPositionHandle* handle, m_handles){
        handle->updatePosition();
    }
}

void QStandardLine::paintEndOfLine(QPainter* painter)
{
    QDiagramEndOfLineStyle s = qvariant_cast<QDiagramEndOfLineStyle>(property("endOfLine"));
    if (!s.isValid() || s.name() == "none"){
        return;
    }
    QLineF line(startPos(), endPos());
    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0){
        angle = (PI * 2) - angle;
    }
    m_endOfLine = QPainterPath();
    if (s.name() == "arrow"){
        QPointF p1 = startPos() - QPointF(sin(angle + PI / 3) * s.width(), cos(angle + PI / 3) * s.width());
        QPointF p2 = startPos() - QPointF(sin(angle + PI - PI / 3) * s.width(), cos(angle + PI - PI / 3) * s.width());
        QPolygonF p;
        p << endPos() << p1 << p2 << endPos();
        m_endOfLine.addPolygon(p);
    } else if (s.name() == "arrow.small"){
        QPointF p1 = QPointF(-cos(angle) * s.width(), sin(angle) * s.width());
        QPointF p2 = QPointF(-cos(angle + PI / 2) * s.width() / 5, sin(angle + PI / 2) * s.width() / 5);
        QPointF p3 = QPointF(-cos(angle + PI + PI / 2) * s.width() / 5, sin(angle + PI + PI / 2) * s.width() / 5);
        QPolygonF p;
        p << endPos() << endPos() + p1 + p2 << endPos() + p1 + p3 << endPos();
        m_endOfLine.addPolygon(p);
    } else if (s.name() == "circle"){
        m_endOfLine.addEllipse(endPos() + QPointF(-cos(angle) * s.width() / 2, sin(angle) * s.width() / 2), s.width() / 2, s.width() / 2);
    }
    painter->drawPath(m_endOfLine);
}

void QStandardLine::paintStartOfLine(QPainter* painter)
{
    QDiagramEndOfLineStyle s = qvariant_cast<QDiagramEndOfLineStyle>(property("startOfLine"));
    if (!s.isValid() || s.name() == "none"){
        return;
    }
    QLineF line(startPos(), endPos());
    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0){
        angle = PI - angle;
    } else {
        angle += PI;
    }
    m_startOfLine = QPainterPath();
    if (s.name() == "arrow"){
        QPointF p1 = startPos() - QPointF(sin(angle + PI / 3) * s.width(), cos(angle + PI / 3) * s.width());
        QPointF p2 = startPos() - QPointF(sin(angle + PI - PI / 3) * s.width(), cos(angle + PI - PI / 3) * s.width());
        QPolygonF p;
        p << startPos() << p1 << p2 << startPos();
        m_startOfLine.addPolygon(p);
    } else if (s.name() == "arrow.double"){
        QPolygonF p;
        QPointF px = startPos() + QPointF(-cos(angle) * s.width() / 2, sin(angle) * s.width() / 2);
        QPointF p1 = QPointF(sin(angle + PI / 3) * s.width(), cos(angle + PI / 3) * s.width());
        QPointF p2 = QPointF(sin(angle + PI - PI / 3) * s.width(), cos(angle + PI - PI / 3) * s.width());
        p << startPos() << startPos() - p1 << startPos() - p2 << startPos();
        m_startOfLine.addPolygon(p);
        p.clear();
        p << px << px - p1 << px - p2 << px;
        m_startOfLine.addPolygon(p);

    } else if (s.name() == "arrow.small"){
        QPointF p1 = QPointF(-cos(angle) * s.width(), sin(angle) * s.width());
        QPointF p2 = QPointF(-cos(angle + PI / 2) * s.width() / 5, sin(angle + PI / 2) * s.width() / 5);
        QPointF p3 = QPointF(-cos(angle + PI + PI / 2) * s.width() / 5, sin(angle + PI + PI / 2) * s.width() / 5);
        QPolygonF p;
        p << startPos() << startPos() + p1 + p2 << startPos() + p1 + p3 << startPos();
        m_startOfLine.addPolygon(p);
    } else if (s.name() == "circle"){
        m_startOfLine.addEllipse(startPos() + QPointF(-cos(angle) * s.width() / 2, sin(angle) * s.width() / 2), s.width() / 2, s.width() / 2);
    }
    painter->drawPath(m_startOfLine);
}

void QStandardLine::paintFocus(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->save();
    if (isSelected()){
        painter->setBrush(Qt::NoBrush);
        QPen mPen(selectionColor());
        mPen.setStyle(Qt::DotLine);
        painter->setPen(mPen);
        painter->drawRect(boundingRect());
    }
    painter->restore();
}

QPainterPath QStandardLine::shape() const
{
    QPainterPath p;
    QLineF l(property("p2").toPointF(), property("p1").toPointF());
    if (l.angle() < 90.0){
        p.moveTo(0, abs(l.dy()));
        p.lineTo(l.dx(), 0);
    } else if (l.angle() >= 90.0 && l.angle() < 180.0){
        p.moveTo(abs(l.dx()), abs(l.dy()));
        p.lineTo(0, 0);
    } else if (l.angle() >= 180.0 && l.angle() < 270.0){
        p.moveTo(abs(l.dx()), 0);
        p.lineTo(0, l.dy());
    } else if (l.angle() >= 270.0){
        p.moveTo(0, 0);
        p.lineTo(abs(l.dx()), abs(l.dy()));
    }
    return p;
}

void QStandardLine::paintPositionHandles(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
}

QPointF QStandardLine::endPos() const
{
    QLineF l(property("p2").toPointF(), property("p1").toPointF());
    if (l.angle() < 90.0){
        return QPointF(0, abs(l.dy()));
    } else if (l.angle() >= 90.0 && l.angle() < 180.0){
        return QPointF(abs(l.dx()), abs(l.dy()));
    } else if (l.angle() >= 180.0 && l.angle() < 270.0){
        return QPointF(abs(l.dx()), 0);
    }
    return QPointF(0, 0);
}

QPointF QStandardLine::startPos() const
{
    QLineF l(property("p1").toPointF(), property("p2").toPointF());
    if (l.angle() < 90.0){
        return QPointF(0, abs(l.dy()));
    } else if (l.angle() >= 90.0 && l.angle() < 180.0){
        return QPointF(abs(l.dx()), abs(l.dy()));
    } else if (l.angle() >= 180.0 && l.angle() < 270.0){
        return QPointF(abs(l.dx()), 0);
    }
    return QPointF(0, 0);
}

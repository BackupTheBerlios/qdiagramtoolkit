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
#include "qstandardblockshape.h"
#include "qstandardblockshape_p.h"

#include <math.h>

#include <qdiagramlinestyle.h>
#include <qdiagramshadowstyle.h>
#include <qdiagramshapesizegrip.h>
#include <qdiagramtextstyle.h>

#define CP_SIZE 8
#define CP_HALFSIZE 4
#define PI 3.14159265

QStandardBlockShape::QStandardBlockShape(const QMap<QString, QVariant> &properties, QGraphicsItem* parent) :
    QAbstractDiagramShape(properties, parent)
{
	setGraphicsEffect(new QGraphicsDropShadowEffect());
	graphicsEffect()->setEnabled(false);

    setFlag(ItemIsMovable, true);

    addProperty("backgroundColor", QDiagramGraphicsItemMetaProperty::Color, false, QColor("lightgray"));
    addProperty("rotation", QDiagramGraphicsItemMetaProperty::Angle, false, properties.value("rotation", 0.0));
    addProperty("lineStyle", QDiagramGraphicsItemMetaProperty::LineStyle, false, properties.value("lineStyle"));
	addProperty("shadow", QDiagramGraphicsItemMetaProperty::Shadow, false,  properties.value("shadow"));
    addProperty("text", QDiagramGraphicsItemMetaProperty::Text, false, properties.value("text"));
    addProperty("textStyle", QDiagramGraphicsItemMetaProperty::TextStyle, false, properties.value("textStyle"));

    if (property("shape") == "rectangle"){
        addConnectionPoint(new QStandardBlockShapeConnectionPoint(this, "c1", QStandardBlockShapeConnectionPoint::Top));
        addConnectionPoint(new QStandardBlockShapeConnectionPoint(this, "c2", QStandardBlockShapeConnectionPoint::TopRight));
        addConnectionPoint(new QStandardBlockShapeConnectionPoint(this, "c3", QStandardBlockShapeConnectionPoint::Right));
        addConnectionPoint(new QStandardBlockShapeConnectionPoint(this, "c4", QStandardBlockShapeConnectionPoint::BottomRight));
        addConnectionPoint(new QStandardBlockShapeConnectionPoint(this, "c5", QStandardBlockShapeConnectionPoint::Bottom));
        addConnectionPoint(new QStandardBlockShapeConnectionPoint(this, "c6", QStandardBlockShapeConnectionPoint::BottomLeft));
        addConnectionPoint(new QStandardBlockShapeConnectionPoint(this, "c7", QStandardBlockShapeConnectionPoint::Left));
        addConnectionPoint(new QStandardBlockShapeConnectionPoint(this, "c8", QStandardBlockShapeConnectionPoint::TopLeft));
    }
    if (property("shape") == "circle"){
        addProperty("thickness", QDiagramGraphicsItemMetaProperty::Percent, false, properties.value("ratio", 50.0));

        addConnectionPoint(new QStandardBlockShapeConnectionPoint(this, "c1", QStandardBlockShapeConnectionPoint::Top));
        addConnectionPoint(new QStandardBlockShapeConnectionPoint(this, "c2", QStandardBlockShapeConnectionPoint::Right));
        addConnectionPoint(new QStandardBlockShapeConnectionPoint(this, "c3", QStandardBlockShapeConnectionPoint::Bottom));
        addConnectionPoint(new QStandardBlockShapeConnectionPoint(this, "c4", QStandardBlockShapeConnectionPoint::Left));
    }
    if (properties.value("shape") == "rectangle.rounded" || properties.value("shape") == "square.rounded"){
        addProperty("radius", QDiagramGraphicsItemMetaProperty::Double, false, properties.value("radius", 10.0));
    }
    if (property("shape") == "hexagon" || property("shape") == "parallelogramm" || property("shape") == "trapezoid"){
        addProperty("angle", QDiagramGraphicsItemMetaProperty::Double, false, properties.value("angle", 10.0));
    }
    if (property("shape") == "cross" || property("shape") == "octagon"){
        addProperty("ratio", QDiagramGraphicsItemMetaProperty::Percent, false, properties.value("ratio", 50.0));
    }

    addSizeGripHandle(new QDiagramShapeSizeGripHandle(QDiagramShapeSizeGripHandle::Left, this));
    addSizeGripHandle(new QDiagramShapeSizeGripHandle(QDiagramShapeSizeGripHandle::Top, this));
    addSizeGripHandle(new QDiagramShapeSizeGripHandle(QDiagramShapeSizeGripHandle::TopLeft, this));
    addSizeGripHandle(new QDiagramShapeSizeGripHandle(QDiagramShapeSizeGripHandle::TopRight, this));
    addSizeGripHandle(new QDiagramShapeSizeGripHandle(QDiagramShapeSizeGripHandle::Bottom, this));
    addSizeGripHandle(new QDiagramShapeSizeGripHandle(QDiagramShapeSizeGripHandle::BottomLeft, this));
    addSizeGripHandle(new QDiagramShapeSizeGripHandle(QDiagramShapeSizeGripHandle::BottomRight, this));
    addSizeGripHandle(new QDiagramShapeSizeGripHandle(QDiagramShapeSizeGripHandle::Right, this));

    restoreProperties(properties);
}

QRectF QStandardBlockShape::boundingRect() const
{
    QRectF r = geometry();
    r.moveTo(0, 0);
    return r;
}

QVariant QStandardBlockShape::itemPropertyHasChanged(const QString &name, const QVariant &value)
{
	if (name == "shadow"){
        QDiagramShadowStyle s = qvariant_cast<QDiagramShadowStyle>(property(name));
		QGraphicsDropShadowEffect* e = qobject_cast<QGraphicsDropShadowEffect*>(graphicsEffect());
		if (e){
			e->setColor(s.color());
			e->setBlurRadius(s.blurRadius());
			e->setOffset(s.offsetX(), s.offsetY());
			e->setEnabled(s.isVisible());
		}
	}
    return QAbstractDiagramGraphicsItem::itemPropertyHasChanged(name, value);
}

void QStandardBlockShape::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QDiagramLineStyle lineStyle = qvariant_cast<QDiagramLineStyle>(property("lineStyle"));
    painter->setPen(lineStyle.pen());
    QColor color = property("backgroundColor").value<QColor>();
    painter->setBrush(QBrush(color, Qt::SolidPattern));
    painter->drawPath(shape());

    paintText(painter, option, widget);
    paintConnectionPoints(painter, option, widget);
    paintFocus(painter, option, widget);

    setConnectionPointsVisible(!isSelected());
    setRotation(property("rotation").toDouble());
}

void QStandardBlockShape::paintText(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->save();

    QDiagramTextStyle textStyle = qvariant_cast<QDiagramTextStyle>(property("textStyle"));
    painter->setFont(textStyle.font());
    painter->setPen(QPen(textStyle.color()));
    painter->drawText(shape().boundingRect(), Qt::AlignCenter, property("text").toString());

    painter->restore();
}

QPainterPath QStandardBlockShape::shape() const
{
    QPainterPath p;
    if (property("shape") == "circle"){
        qreal a = boundingRect().width() * property("thickness").toDouble() / 100;
        p.addEllipse(boundingRect());
        QRectF r = boundingRect();
        r.adjust(a / 2, a / 2, -a / 2, -a / 2);
        p.addEllipse(r);
    } else if (property("shape") == "cross"){
        qreal a = boundingRect().width() / 2 * property("ratio").toDouble() / 100;
        p.moveTo(a, 0);
        p.lineTo(boundingRect().width() - a , 0);
        p.lineTo(boundingRect().width() - a, a);
        p.lineTo(boundingRect().width(), a);
        p.lineTo(boundingRect().width(), boundingRect().height() - a);
        p.lineTo(boundingRect().width() - a, boundingRect().height() - a);
        p.lineTo(boundingRect().width() - a, boundingRect().height());
        p.lineTo(a, boundingRect().height());
        p.lineTo(a, boundingRect().height() - a);
        p.lineTo(0, boundingRect().height() - a);
        p.lineTo(0, a);
        p.lineTo(a, a);
        p.lineTo(a, 0);
    } else if (property("shape") == "hexagon"){
        qreal a = tan(property("angle").toDouble()*PI/180) * boundingRect().height() / 2;
        p.moveTo(a, 0);
        p.lineTo(boundingRect().width() - a, 0);
        p.lineTo(boundingRect().width(), boundingRect().center().y());
        p.lineTo(boundingRect().width() - a, boundingRect().height());
        p.lineTo(a, boundingRect().height());
        p.lineTo(0, boundingRect().center().y());
        p.lineTo(a, 0);
    } else if (property("shape").toString() == "rectangle" || property("shape").toString() == "square"){
        p.addRect(boundingRect());
    } else if (property("shape") == "square.rounded" || property("shape") == "rectangle.rounded"){
        p.addRoundedRect(geometry(), property("radius").toDouble(), property("radius").toDouble());
    } else if (property("shape").toString() == "diamond"){
        p.moveTo(0, boundingRect().center().y());
        p.lineTo(boundingRect().center().x(), 0);
        p.lineTo(boundingRect().width(), boundingRect().center().y());
        p.lineTo(boundingRect().center().x(), boundingRect().height());
        p.lineTo(0, boundingRect().center().y());
    } else if (property("shape") == "octagon"){
        qreal a = boundingRect().width() / 2 * property("ratio").toDouble() / 100;
        p.moveTo(a, 0);
        p.lineTo(boundingRect().width() - a, 0);
        p.lineTo(boundingRect().width(), a);
        p.lineTo(boundingRect().width(), boundingRect().height() - a);
        p.lineTo(boundingRect().width() - a, boundingRect().height());
        p.lineTo(a, boundingRect().height());
        p.lineTo(0, boundingRect().height() - a);
        p.lineTo(0, a);
        p.lineTo(a, 0);
    } else if (property("shape") == "parallelogramm"){
        qreal a = tan(property("angle").toDouble()*PI/180) * boundingRect().height();
        p.moveTo(a, 0);
        p.lineTo(boundingRect().width(), 0);
        p.lineTo(boundingRect().bottomRight().x() - a, boundingRect().height());
        p.lineTo(boundingRect().bottomLeft());
        p.lineTo(a, 0);
    } else if (property("shape") == "trapezoid"){
        qreal a = tan(property("angle").toDouble()*PI/180) * boundingRect().height();
        p.moveTo(a, boundingRect().height());
        p.lineTo(boundingRect().width() - a, boundingRect().height());
        p.lineTo(boundingRect().topRight());
        p.lineTo(boundingRect().topLeft());
        p.lineTo(a, boundingRect().height());
    } else if (property("shape") == "triangle.isosceles"){
        p.moveTo(boundingRect().bottomLeft());
        p.lineTo(boundingRect().center().x(), 0);
        p.lineTo(boundingRect().bottomRight());
        p.lineTo(boundingRect().bottomLeft());
    } else if (property("shape") == "triangle.right_angle"){
        p.moveTo(0, 0);
        p.lineTo(boundingRect().bottomRight());
        p.lineTo(boundingRect().bottomLeft());
        p.lineTo(0, 0);
    }
    return p;
}

QStandardBlockShapeConnectionPoint::QStandardBlockShapeConnectionPoint(QAbstractDiagramShape* shape, const QString & id, Position position) :
    QAbstractDiagramShapeConnectionPoint(shape, id, QAbstractDiagramShapeConnectionPoint::West, -1)
{
    m_pos = position;
    updatePosition();
}

QStandardBlockShapeConnectionPoint::~QStandardBlockShapeConnectionPoint()
{
}

void QStandardBlockShapeConnectionPoint::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->save();
    painter->drawLine(rect().topLeft(), rect().bottomRight());
    painter->drawLine(rect().bottomLeft(), rect().topRight());
    painter->restore();
}

void QStandardBlockShapeConnectionPoint::updatePosition()
{
    if (m_pos == QStandardBlockShapeConnectionPoint::Bottom){
        setRect(QRectF(parentShape()->boundingRect().center().x() - CP_HALFSIZE, parentShape()->boundingRect().height() - CP_HALFSIZE, CP_SIZE, CP_SIZE));
    } else if (m_pos == QStandardBlockShapeConnectionPoint::BottomLeft){
        setRect(QRectF(-CP_HALFSIZE, parentShape()->boundingRect().height() - CP_HALFSIZE, CP_SIZE, CP_SIZE));
    } else if (m_pos == QStandardBlockShapeConnectionPoint::BottomRight){
        setRect(QRectF(parentShape()->boundingRect().width() - CP_HALFSIZE, parentShape()->boundingRect().height() - CP_HALFSIZE, CP_SIZE, CP_SIZE));
    } else if (m_pos == QStandardBlockShapeConnectionPoint::Left){
        setRect(QRectF(-CP_HALFSIZE, parentShape()->boundingRect().center().y() - CP_HALFSIZE, CP_SIZE, CP_SIZE));
    } else if (m_pos == QStandardBlockShapeConnectionPoint::Right){
        setRect(QRectF(parentShape()->boundingRect().width() - CP_HALFSIZE, parentShape()->boundingRect().center().y() - CP_HALFSIZE, CP_SIZE, CP_SIZE));
    } else if (m_pos == QStandardBlockShapeConnectionPoint::Top){
        setRect(QRectF(parentShape()->boundingRect().center().x() - CP_HALFSIZE, - CP_HALFSIZE, CP_SIZE, CP_SIZE));
    } else if (m_pos == QStandardBlockShapeConnectionPoint::TopLeft){
        setRect(QRectF(-CP_HALFSIZE, -CP_HALFSIZE, CP_SIZE, CP_SIZE));
    } else if (m_pos == QStandardBlockShapeConnectionPoint::TopRight){
        setRect(QRectF(parentShape()->boundingRect().width() -CP_HALFSIZE, -CP_HALFSIZE, CP_SIZE, CP_SIZE));
    }
}

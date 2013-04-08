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
#include <qdiagramgraphicsitemshadow.h>
#include <qdiagramshapesizegrip.h>
#include <qdiagramtextstyle.h>

#include "qstandardformsplugin.h"

#include <qdiagramgraphicstextitem.h>

#define CP_SIZE 4
#define CP_HALFSIZE 2
#define PI 3.14159265

QStandardBlockShape::QStandardBlockShape(const QString & itemClass, const QMap<QString, QVariant> &properties, QGraphicsItem* parent) :
QAbstractDiagramShape(QStandardFormsPlugin::staticName(), itemClass, properties, parent)
{
	initGeometry(30, 30);
	setGraphicsEffect(new QGraphicsDropShadowEffect());
	graphicsEffect()->setEnabled(false);

    setFlag(ItemIsMovable, true);

	textItem()->setTextProperty("text");

    addProperty("background", QDiagramToolkit::Brush, false, properties.value("background"));
    addProperty("rotation", QDiagramToolkit::Angle, false, properties.value("rotation", 0.0));
    addProperty("lineStyle", QDiagramToolkit::LineStyle, false, properties.value("lineStyle"));
    addProperty("pen", QDiagramToolkit::Pen, false, properties.value("pen"));
	addProperty("shadow", QDiagramToolkit::Shadow, false,  properties.value("shadow"));
    addProperty("text", QDiagramToolkit::Text, false, properties.value("text"));
	addProperty("textAlignment", QDiagramToolkit::Alignment, false, properties.value("alignment", Qt::AlignCenter));
    addProperty("textFont", QDiagramToolkit::Font, false, properties.value("textFont"));

    if (metaData()->itemClass() == "rectangle"){
        addConnectionPoint(new QStandardBlockShapeConnectionPoint(this, "c1", QStandardBlockShapeConnectionPoint::Top));
        addConnectionPoint(new QStandardBlockShapeConnectionPoint(this, "c2", QStandardBlockShapeConnectionPoint::TopRight));
        addConnectionPoint(new QStandardBlockShapeConnectionPoint(this, "c3", QStandardBlockShapeConnectionPoint::Right));
        addConnectionPoint(new QStandardBlockShapeConnectionPoint(this, "c4", QStandardBlockShapeConnectionPoint::BottomRight));
        addConnectionPoint(new QStandardBlockShapeConnectionPoint(this, "c5", QStandardBlockShapeConnectionPoint::Bottom));
        addConnectionPoint(new QStandardBlockShapeConnectionPoint(this, "c6", QStandardBlockShapeConnectionPoint::BottomLeft));
        addConnectionPoint(new QStandardBlockShapeConnectionPoint(this, "c7", QStandardBlockShapeConnectionPoint::Left));
        addConnectionPoint(new QStandardBlockShapeConnectionPoint(this, "c8", QStandardBlockShapeConnectionPoint::TopLeft));
    }
    if (metaData()->itemClass() == "circle"){
        addProperty("thickness", QDiagramToolkit::Percent, false, properties.value("ratio", 50.0));

        addConnectionPoint(new QStandardBlockShapeConnectionPoint(this, "c1", QStandardBlockShapeConnectionPoint::Top));
        addConnectionPoint(new QStandardBlockShapeConnectionPoint(this, "c2", QStandardBlockShapeConnectionPoint::Right));
        addConnectionPoint(new QStandardBlockShapeConnectionPoint(this, "c3", QStandardBlockShapeConnectionPoint::Bottom));
        addConnectionPoint(new QStandardBlockShapeConnectionPoint(this, "c4", QStandardBlockShapeConnectionPoint::Left));
    }
    if (properties.value("shape") == "rectangle.rounded" || properties.value("shape") == "square.rounded"){
        addProperty("radius", QDiagramToolkit::Double, false, properties.value("radius", 10.0));
    }
    if (metaData()->itemClass() == "hexagon" || metaData()->itemClass() == "parallelogramm" || metaData()->itemClass() == "trapezoid"){
        addProperty("angle", QDiagramToolkit::Double, false, properties.value("angle", 10.0));
    }
    if (metaData()->itemClass() == "cross" || metaData()->itemClass() == "octagon"){
        addProperty("ratio", QDiagramToolkit::Percent, false, properties.value("ratio", 50.0));
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
		QDiagramGraphicsItemShadow s = qdiagramproperty_cast<QDiagramGraphicsItemShadow>(property(name));
		QGraphicsDropShadowEffect* e = qobject_cast<QGraphicsDropShadowEffect*>(graphicsEffect());
		if (e){
			e->setColor(s.color());
			e->setBlurRadius(s.blurRadius());
			e->setOffset(s.offsetX(), s.offsetY());
			e->setEnabled(s.isVisible());
		}
	}
	textItem()->itemPropertyHasChanged(this, name, value);
    return QAbstractDiagramGraphicsItem::itemPropertyHasChanged(name, value);
}

void QStandardBlockShape::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
	if (event->button() == Qt::LeftButton){
		textItem()->setTextWidth(geometry().width());
		textItem()->updatePosition();
		textItem()->setEditModeEnabled(true);
	}
	QAbstractDiagramShape::mouseDoubleClickEvent(event);
}

void QStandardBlockShape::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QDiagramLineStyle lineStyle = qdiagramproperty_cast<QDiagramLineStyle>(property("lineStyle"));
	QPen p;
	p.setColor(lineStyle.color());
	p.setWidthF(lineStyle.width());
	p.setStyle(lineStyle.penStyle());
    painter->setPen(p);
	QBrush b = qdiagramproperty_cast<QBrush>(property("background"));
    painter->setBrush(b);
    painter->drawPath(shape());

//    paintText(painter, option, widget);
    paintConnectionPoints(painter, option, widget);
    paintFocus(painter, option, widget);

//    setConnectionPointsVisible(!isSelected());
    setRotation(property("rotation").toDouble());
}

void QStandardBlockShape::paintText(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->save();

	QFont f = qdiagramproperty_cast<QFont>(property("textFont"));
	QString t = qdiagramproperty_cast<QString>(property("text"));
    painter->setFont(f);
    painter->drawText(shape().boundingRect(), Qt::AlignCenter, t);

    painter->restore();
}

QPainterPath QStandardBlockShape::shape() const
{
    QPainterPath p;
    if (metaData()->itemClass() == "circle"){
        qreal a = boundingRect().width() * property("thickness").toDouble() / 100;
        p.addEllipse(boundingRect());
        QRectF r = boundingRect();
        r.adjust(a / 2, a / 2, -a / 2, -a / 2);
        p.addEllipse(r);
    } else if (metaData()->itemClass() == "cross"){
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
    } else if (metaData()->itemClass() == "hexagon"){
        qreal a = tan(property("angle").toDouble()*PI/180) * boundingRect().height() / 2;
        p.moveTo(a, 0);
        p.lineTo(boundingRect().width() - a, 0);
        p.lineTo(boundingRect().width(), boundingRect().center().y());
        p.lineTo(boundingRect().width() - a, boundingRect().height());
        p.lineTo(a, boundingRect().height());
        p.lineTo(0, boundingRect().center().y());
        p.lineTo(a, 0);
    } else if (metaData()->itemClass() == "rectangle" || metaData()->itemClass() == "square"){
        p.addRect(boundingRect());
    } else if (metaData()->itemClass() == "square.rounded" || metaData()->itemClass() == "rectangle.rounded"){
        p.addRoundedRect(boundingRect(), property("radius").toDouble(), property("radius").toDouble());
    } else if (metaData()->itemClass() == "diamond"){
        p.moveTo(0, boundingRect().center().y());
        p.lineTo(boundingRect().center().x(), 0);
        p.lineTo(boundingRect().width(), boundingRect().center().y());
        p.lineTo(boundingRect().center().x(), boundingRect().height());
        p.lineTo(0, boundingRect().center().y());
    } else if (metaData()->itemClass() == "octagon"){
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
    } else if (metaData()->itemClass() == "parallelogramm"){
        qreal a = tan(property("angle").toDouble()*PI/180) * boundingRect().height();
        p.moveTo(a, 0);
        p.lineTo(boundingRect().width(), 0);
        p.lineTo(boundingRect().bottomRight().x() - a, boundingRect().height());
        p.lineTo(boundingRect().bottomLeft());
        p.lineTo(a, 0);
    } else if (metaData()->itemClass() == "trapezoid"){
        qreal a = tan(property("angle").toDouble()*PI/180) * boundingRect().height();
        p.moveTo(a, boundingRect().height());
        p.lineTo(boundingRect().width() - a, boundingRect().height());
        p.lineTo(boundingRect().topRight());
        p.lineTo(boundingRect().topLeft());
        p.lineTo(a, boundingRect().height());
    } else if (metaData()->itemClass() == "triangle.isosceles"){
        p.moveTo(boundingRect().bottomLeft());
        p.lineTo(boundingRect().center().x(), 0);
        p.lineTo(boundingRect().bottomRight());
        p.lineTo(boundingRect().bottomLeft());
    } else if (metaData()->itemClass() == "triangle.right_angle"){
        p.moveTo(0, 0);
        p.lineTo(boundingRect().bottomRight());
        p.lineTo(boundingRect().bottomLeft());
        p.lineTo(0, 0);
    }
    return p;
}

QStandardBlockShapeConnectionPoint::QStandardBlockShapeConnectionPoint(QAbstractDiagramShape* shape, const QString & id, Position position) :
    QAbstractDiagramShapeConnectionPoint(shape, id, QDiagramToolkit::West, -1)
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

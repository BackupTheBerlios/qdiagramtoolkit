#include "stdafx.h"
#include "qsysmlaction.h"

#include "qsysmlplugin.h"
#include "qsysmlcontrolconnectionpoint.h"
#include "qsysmldataconnectionpoint.h"

QSysMLAction::QSysMLAction(const QMap<QString, QVariant> &properties, QGraphicsItem *parent) :
QAbstractDiagramShape(QSysMLPlugin::staticName(), "action", properties, parent)
{
	initGeometry(130, 52);
    addProperty("actionType", QDiagramToolkit::String, true, properties.value("actionType"));
    addProperty("background", QDiagramToolkit::Brush, false, properties.value("background", QColor("wheat")));
    addProperty("font", QDiagramToolkit::Font, false, properties.value("font"));
	// 223, 195, 150
	QPen pen(QColor(223, 195, 150));
	pen.setWidthF(2.0);
    addProperty("lineColor", QDiagramToolkit::Pen, false, properties.value("lineColor", pen));
    addProperty("textColor", QDiagramToolkit::Color, false, properties.value("Color", QColor("black")));
    addProperty("name", QDiagramToolkit::String, false, "action");

    addSizeGripHandle(new QDiagramShapeSizeGripHandle(QDiagramShapeSizeGripHandle::Left, this));
    addSizeGripHandle(new QDiagramShapeSizeGripHandle(QDiagramShapeSizeGripHandle::Right, this));

	addConnectionPoint(new QSysMLControlConnectionPointIn(this, "controlIn", 0, 1));
	addConnectionPoint(new QSysMLControlConnectionPointOut(this, "controlOut", 0, 1));

	addConnectionPoint(new QSysMLDataConnectionPointIn(this, "dataIn1", 0, 1));
	addConnectionPoint(new QSysMLDataConnectionPointOut(this, "dataOut1", 0, 1));

    restoreProperties(properties);
}

QRectF QSysMLAction::boundingRect() const
{
    QRectF r = geometry();
    r.moveTo(0, 0);
    return r;
}

void QSysMLAction::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
	QBrush b = qdiagramproperty_cast<QBrush>(property("background"));
    painter->setBrush(b);


	painter->setPen(qdiagramproperty_cast<QPen>(property("lineColor")));
	painter->drawPath(shape());

	QFont f = qdiagramproperty_cast<QFont>(property("font"));
	QFontInfo fi(f);
	f.setPixelSize(fi.pixelSize());
	painter->setPen(qdiagramproperty_cast<QColor>(property("textColor")));
	painter->setFont(f);
    QRect br = boundingRect().adjusted(-2, -2, -2, -2).toRect();
    painter->drawText(0, 0, geometry().width(), geometry().height(), Qt::AlignCenter | Qt::TextWordWrap, property("name").toString(), &br);
    painter->restore();
    paintFocus(painter, option, widget);
}

QPainterPath QSysMLAction::shape() const
{
    QPainterPath p;
	if (property("actionType") == "sendSignal"){
		p.moveTo(0, 0);
		p.lineTo(geometry().width() - geometry().height() / 2, 0);
		p.lineTo(geometry().width(), geometry().height() / 2);
		p.lineTo(geometry().width() - geometry().height() / 2, geometry().height());
		p.lineTo(0, geometry().height());
		p.closeSubpath();
	} else {
		p.addRoundedRect(0, 0, geometry().width(), geometry().height(), 8, 8);
	}
    return p;
}

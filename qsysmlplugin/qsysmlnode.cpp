#include "stdafx.h"
#include "qsysmlnode.h"

#include "qsysmlplugin.h"
#include "qsysmlcontrolconnectionpoint.h"

QSysMLNode::QSysMLNode(const QMap<QString, QVariant> &properties, QGraphicsItem *parent) :
    QAbstractDiagramShape(QSysMLPlugin::staticName(), "node", properties, parent)
{
	initGeometry(39, 39);
    addProperty("nodeType", QDiagramToolkit::String, true, properties.value("nodeType"));
    addProperty("background", QDiagramToolkit::Brush, false, properties.value("background", QColor("white")));
    addProperty("font", QDiagramToolkit::Font, false, properties.value("font"));
    addProperty("lineColor", QDiagramToolkit::Pen, false, properties.value("lineColor"));
    if (properties.value("nodeType") == "decision"){
		initGeometry(52, 52);
		addConnectionPoint(new QSysMLControlConnectionPointIn(this, "controlIn", 0, 1));
		addConnectionPoint(new QSysMLControlConnectionPointOut(this, "controlOut1", QDiagramToolkit::East, 0, 1));
		addConnectionPoint(new QSysMLControlConnectionPointOut(this, "controlOut2", QDiagramToolkit::South, 1, 1));
		addConnectionPoint(new QSysMLControlConnectionPointOut(this, "controlOut3", QDiagramToolkit::West, 2, 1));
	} else if (properties.value("nodeType") == "initial"){
		initGeometry(26, 26);
		addConnectionPoint(new QSysMLControlConnectionPointOut(this, "controlOut", 0, 1));
    } else if (properties.value("nodeType") == "final.activity"){
		initGeometry(26, 26);
		addConnectionPoint(new QSysMLControlConnectionPointIn(this, "controlIn", 0, 1));
    } else if (properties.value("nodeType") == "final.flow"){
		initGeometry(26, 26);
		addConnectionPoint(new QSysMLControlConnectionPointIn(this, "controlIn", 0, 1));
    } else if (properties.value("nodeType") == "fork"){
		initGeometry(78, 26);
		addProperty("orientation", QDiagramToolkit::Orientation, false, properties.value("orientation", Qt::Horizontal));
        addProperty("branches", QDiagramToolkit::Int, false, properties.value("branches", 2));

		addConnectionPoint(new QSysMLControlConnectionPointIn(this, "controlIn", 0, 1));
		for (int i = 0; i < properties.value("branches", 2).toInt(); i++){
			addConnectionPoint(new QSysMLControlConnectionPointOut(this, QString("controlOut%1").arg(i), i, 1));
		}
        addSizeGripHandle(new QDiagramShapeSizeGripHandle(QDiagramShapeSizeGripHandle::Left, this));
        addSizeGripHandle(new QDiagramShapeSizeGripHandle(QDiagramShapeSizeGripHandle::Right, this));
    } else if (properties.value("nodeType") == "join"){
		initGeometry(78, 26);
		addProperty("orientation", QDiagramToolkit::Orientation, false, properties.value("orientation", Qt::Horizontal));
        addProperty("branches", QDiagramToolkit::Int, false, properties.value("branches", 2));

		addConnectionPoint(new QSysMLControlConnectionPointOut(this, "controlIn", 0, 1));
		for (int i = 0; i < properties.value("branches", 2).toInt(); i++){
			addConnectionPoint(new QSysMLControlConnectionPointIn(this, QString("controlIn%1").arg(i), i, 1));
		}
        addSizeGripHandle(new QDiagramShapeSizeGripHandle(QDiagramShapeSizeGripHandle::Left, this));
        addSizeGripHandle(new QDiagramShapeSizeGripHandle(QDiagramShapeSizeGripHandle::Right, this));

		addConnectionPoint(new QSysMLControlConnectionPointOut(this, "controlOut", 0, 1));
    } else if (properties.value("nodeType") == "transition"){
		initGeometry(52, 52);
		addConnectionPoint(new QSysMLControlConnectionPointIn(this, "controlIn", 0, 1));
		addConnectionPoint(new QSysMLControlConnectionPointOut(this, "controlOut", QDiagramToolkit::South, 0, 1));
    }
    restoreProperties(properties);
	updateConnectionPoints();
}

QRectF QSysMLNode::boundingRect() const
{
    QRectF r(geometry());
    r.moveTo(0, 0);
    return r;
}

QString QSysMLNode::nodeType() const
{
    return property("nodeType").toString();
}

void QSysMLNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->save();
	QPen pen(Qt::black);
	pen.setWidthF(1.0);
	painter->setPen(pen);
    if (nodeType() == "decision"){
		paintDecision(painter, option, widget);
    } else if (property("nodeType") == "initial"){
		paintInitial(painter, option, widget);
    } else if (property("nodeType") == "final.activity"){
        painter->drawEllipse(QRectF(3.25, 3.25, 19, 19));
        QBrush b(Qt::black);
        painter->setBrush(b);
        painter->drawEllipse(QRectF(6.5, 6.5, 12.5, 12.5));
    } else if (property("nodeType") == "final.flow"){
		QPen pen(Qt::black);
		pen.setWidthF(2);
		painter->setPen(pen);
		QPainterPath p;
		p.addEllipse(QRectF(3.25, 3.25, 19, 19));
		p.moveTo(13, 3.25);
		p.lineTo(13, 22.25);
		p.moveTo(3.25, 13);
		p.lineTo(22.25, 13);
		painter->save();
		QTransform t(painter->transform());
		t.translate(13, 13 - QLineF(0, 0, 26, 26).length() / 2);
		t.rotate(45);
		painter->setTransform(t);
		painter->drawPath(p);
		painter->restore();
    } else if (property("nodeType") == "fork"){
		paintFork(painter, option, widget);
	} else if (property("nodeType") == "join"){
		paintJoin(painter, option, widget);
	} else if (property("nodeType") == "transition"){
		paintTransition(painter, option, widget);
    }
    painter->restore();

    paintFocus(painter, option, widget);
}

void QSysMLNode::paintDecision(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	painter->save();
	painter->setPen(qdiagramproperty_cast<QPen>(property("lineColor")));
	painter->setBrush(qdiagramproperty_cast<QBrush>(property("background")));
	QPainterPath p;
	p.moveTo(boundingRect().center().x(), 0);
	p.lineTo(boundingRect().width(), boundingRect().center().y());
	p.lineTo(boundingRect().center().x(), boundingRect().height());
	p.lineTo(0, boundingRect().center().y());
	p.closeSubpath();
	painter->drawPath(p);
	painter->restore();
}

void QSysMLNode::paintFork(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	painter->save();
	QBrush b(Qt::black);
	painter->setBrush(b);
	painter->drawRect(0, geometry().height() / 2 - 2, geometry().width(), 4);
	QPen pen(Qt::black);
	pen.setStyle(Qt::DashLine);
	pen.setWidthF(2.0);
	painter->setPen(pen);
	painter->drawLine(geometry().width() / 2, 0, geometry().width() / 2, geometry().height() / 2 - 2);
	Q_FOREACH(QAbstractDiagramShapeConnectionPoint* cp, connectionPoints()){
		if (cp->direction() == QAbstractDiagramShapeConnectionPoint::Out){
			painter->drawLine(cp->rect().center().x(), geometry().height() / 2, cp->rect().center().x(), boundingRect().height());
		}
	}
	painter->restore();
}

void QSysMLNode::paintInitial(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	painter->save();
	QBrush b(Qt::black);
	painter->setBrush(b);
	painter->drawEllipse(QRectF(3.25, 3.25, 19, 19));
	painter->restore();
}

void QSysMLNode::paintJoin(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	painter->save();
	QBrush b(Qt::black);
	painter->setBrush(b);
	painter->drawRect(0, geometry().height() / 2 - 2, geometry().width(), 4);

	QPen pen(Qt::black);
	pen.setWidthF(2);
	painter->setPen(pen);
	painter->drawLine(geometry().width() / 2, geometry().height() / 2 + 2, geometry().width() / 2, geometry().height());
	Q_FOREACH(QAbstractDiagramShapeConnectionPoint* cp, connectionPoints()){
		if (cp->direction() == QAbstractDiagramShapeConnectionPoint::In){
			painter->drawLine(cp->rect().center().x(), 0, cp->rect().center().x(), boundingRect().height() / 2);
		}
	}
	painter->restore();
}

void QSysMLNode::paintTransition(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	painter->save();
	painter->setPen(qdiagramproperty_cast<QPen>(property("lineColor")));
	painter->setBrush(qdiagramproperty_cast<QBrush>(property("background")));
	painter->drawPath(shape());
	QFont f = qdiagramproperty_cast<QFont>(property("font"));
	QFontInfo fi(f);
	f.setPixelSize(fi.pixelSize());
	painter->setFont(f);
    QRect br = boundingRect().adjusted(-2, -2, -2, -2).toRect();
    painter->drawText(0, 0, geometry().width(), geometry().height(), Qt::AlignCenter | Qt::TextWordWrap,"T", &br);
	painter->restore();
}

QPainterPath QSysMLNode::shape() const
{
    QPainterPath p;
	if (nodeType() == "decision" || nodeType() == "transition"){
		p.moveTo(boundingRect().center().x(), 0);
		p.lineTo(boundingRect().width(), boundingRect().center().y());
		p.lineTo(boundingRect().center().x(), boundingRect().height());
		p.lineTo(0, boundingRect().center().y());
		p.closeSubpath();
    } else if (property("nodeType") == "initial"){
		p.addEllipse(QRectF(3.25, 3.25, 19, 19));
    } else if (property("nodeType") == "final.flow"){
		p.addEllipse(QRectF(3.25, 3.25, 19, 19));
    } else if (property("nodeType") == "final.activity"){
		p.addEllipse(QRectF(3.25, 3.25, 19, 19));
    } else if (property("nodeType") == "fork"){
        if (property("orientation").toInt() == Qt::Horizontal){
            p.addRect(0, geometry().height() / 2 - 2, geometry().width(), 4);
        }
    } else if (property("nodeType") == "join"){
        if (property("orientation").toInt() == Qt::Horizontal){
            p.addRect(0, geometry().height() / 2 - 2, geometry().width(), 4);
        }
    }

    return p;
}

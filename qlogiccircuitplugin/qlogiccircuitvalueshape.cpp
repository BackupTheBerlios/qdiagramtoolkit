#include "stdafx.h"
#include "qlogiccircuitvalueshape.h"
#include "qlogiccircuitvalueshape_p.h"

#include "qlogiccircuitplugin.h"

#define GATE_BASE_SIZE 13.0
#define GATE_CP_SIZE 6.0

QLogicCircuitValueShapeConnectionPoint::QLogicCircuitValueShapeConnectionPoint(QAbstractDiagramShape* shape) :
    QAbstractDiagramShapeConnectionPoint(shape, "inp1", QDiagramToolkit::East)
{
    updatePosition();
}

QLogicCircuitValueShapeConnectionPoint::~QLogicCircuitValueShapeConnectionPoint()
{

}

void QLogicCircuitValueShapeConnectionPoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->save();
    painter->drawRoundedRect(rect(), 2, 2);
    painter->restore();
}

void QLogicCircuitValueShapeConnectionPoint::updatePosition()
{
    QRectF r;
    r.setLeft(parentShape()->geometry().width() - parentShape()->geometry().height() / 4);
    r.setTop(parentShape()->geometry().height() / 2 - parentShape()->geometry().height() / 8);
    r.setWidth(parentShape()->geometry().height() / 4);
    r.setHeight(parentShape()->geometry().height() / 4);
    setRect(r);
}

QLogicCircuitValueShape::QLogicCircuitValueShape(QGraphicsItem* parent) :
    QAbstractDiagramShape(parent)
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    setAcceptHoverEvents(true);

    addConnectionPoint(new QLogicCircuitValueShapeConnectionPoint(this));
}

QLogicCircuitValueShape::QLogicCircuitValueShape(const QMap<QString, QVariant> & properties, QGraphicsItem* parent) :
    QAbstractDiagramShape(QLogicCircuitPlugin::staticName(), "value", properties, parent)
{
	initGeometry(182, 26);
    addProperty("signalType", QDiagramToolkit::String, true, "analog");
	addProperty("value", QDiagramToolkit::Double, false, properties.value("value", 0.0).toDouble());
	addProperty("unit", QDiagramToolkit::String, false, properties.value("string").toString());

	restoreProperties(properties);

	setAcceptHoverEvents(true);

    addConnectionPoint(new QLogicCircuitValueShapeConnectionPoint(this));
}

QRectF QLogicCircuitValueShape::boundingRect() const
{
//    return QRectF(0,0, 8 * GATE_BASE_SIZE, 2 * GATE_BASE_SIZE);
    QRectF r = geometry();
    r.moveTo(0, 0);
    return r;
}

void QLogicCircuitValueShape::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(pen());
    painter->setBrush(QBrush(Qt::white, Qt::SolidPattern));
    painter->drawPath(shape());
    if (property("signalType").toString() == "analog"){
        int offset = geometry().height() / 4;
        painter->save();
        QPainterPath p;
		p.moveTo(1 + offset, 7);
		p.lineTo(1 + offset, 19);
		p.moveTo(1 + offset, 13);
		p.lineTo(19 + offset, 13);

		p.moveTo(1 + offset, 13);
		p.lineTo(4 + offset, 9);
		p.lineTo(7 + offset, 16);
		p.lineTo(10 + offset, 8);
		p.lineTo(15 + offset, 18);
		p.lineTo(19 + offset, 7);

        painter->drawPath(p);
        painter->restore();
    } else if (property("signalType").toString() == "digital"){
        int offset = geometry().height() / 4;
        painter->save();
        QPainterPath p;
        p.moveTo(1 + offset, 19);
        p.lineTo(5 + offset, 19);
        p.lineTo(5 + offset, 7);
        p.lineTo(15 + offset, 7);
        p.lineTo(15 + offset, 19);
        p.lineTo(19 + offset, 19);
        painter->drawPath(p);
        painter->restore();
        QFont f;
        f.setFamily("Arial");
        f.setPixelSize(8);
        painter->setFont(f);

        QRectF r(boundingRect());
        r.setWidth(26 + GATE_BASE_SIZE / 2);
        r.moveTop(3);
        painter->drawText(r, Qt::AlignRight, "1\n0");
    }
    QFont f;
    f.setFamily("Arial");
    f.setPixelSize(10);
    painter->setFont(f);
    QRectF r(boundingRect().adjusted(26 + GATE_BASE_SIZE, 0, 0, 0));
	QString t = QString("%1%2").arg(property("value").toString()).arg(property("unit").toString());
    painter->drawText(r, Qt::AlignLeft | Qt::AlignVCenter, t);

    if (property("state").toBool()){
        painter->save();
        QPen pen;
        pen.setColor(Qt::green);
        pen.setWidth(2);
        painter->setPen(pen);
        painter->drawLine(geometry().width() - geometry().height(), geometry().height() / 2, geometry().width(), geometry().height() / 2);
        painter->restore();
    }
    paintConnectionPoints(painter, option, widget);

    paintFocus(painter, option, widget);
}

QPainterPath QLogicCircuitValueShape::shape() const
{
     QPainterPath p;
	p.moveTo(0, 0);
	p.lineTo(geometry().width() - geometry().height() * 1.5 , 0);
	p.lineTo(geometry().width() - geometry().height(), geometry().height() / 2); 
	p.lineTo(geometry().width() - geometry().height() * 1.5 , geometry().height());
	p.lineTo(0, geometry().height());
	p.lineTo(0, 0);

    p.moveTo(geometry().width() - geometry().height(), geometry().height() / 2);
    p.lineTo(geometry().width(), geometry().height() / 2);

	p.moveTo(geometry().width() - geometry().height() * 1.5 , 0);
	p.lineTo(geometry().width() - geometry().height() * 1.5 , geometry().height());

	return p;
//   QPainterPath p;
	////p.moveTo(0, 0);
	////p.lineTo(0, geometry().height());

	////p.moveTo(0, 0);
	////p.lineTo(geometry().width() - geometry().height() * 1.5 , 0);

	////p.moveTo(0, geometry().height());
	////p.lineTo(geometry().width() - geometry().height() * 1.5 , geometry().height());

	//p.moveTo(geometry().width() - geometry().height() * 1.5 , geometry().height());
	//p.lineTo(0, geometry().height());
	//p.lineTo(0, 0);
	//p.lineTo(geometry().width() - geometry().height() * 1.5 , 0);
	//p.closeSubpath();

	//QRectF r;

	//r.setX(geometry().width() - geometry().height() * 1.5 - geometry().height() / 2);
	//r.setY(0);
	//r.setHeight(geometry().height());
	//r.setWidth(geometry().height());

 //   p.moveTo(geometry().width() - geometry().height(), geometry().height() / 2);
	//p.arcTo(r, 270, 180);

	//p.closeSubpath();
 //   p.moveTo(geometry().width() - geometry().height(), geometry().height() / 2);
 //   p.lineTo(geometry().width(), geometry().height() / 2);

	//return p;
}

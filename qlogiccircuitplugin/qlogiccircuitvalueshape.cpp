#include "stdafx.h"
#include "qlogiccircuitvalueshape.h"
#include "qlogiccircuitvalueshape_p.h"

#include "qlogiccircuitplugin.h"

#define GATE_BASE_SIZE 50.0
#define GATE_CP_SIZE 40.0

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
    painter->drawRect(rect());
    painter->restore();
}

void QLogicCircuitValueShapeConnectionPoint::updatePosition()
{
    QRectF r;
	r.setLeft(parentShape()->geometry().width() - GATE_CP_SIZE);
	r.setTop(parentShape()->geometry().height() / 2 - GATE_CP_SIZE / 2);
	r.setWidth(GATE_CP_SIZE);
	r.setHeight(GATE_CP_SIZE);
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
QAbstractDiagramShape(QLogicCircuitPlugin::staticName(), QLogicCircuitValueShape::staticItemClass(), properties, parent)
{
	initGeometry(900, 100);
    addProperty("signalType", QDiagramToolkit::String, true, "analog");
	addProperty("value", QDiagramToolkit::Double, false, properties.value("value", 0.0).toDouble());
	addProperty("unit", QDiagramToolkit::String, false, properties.value("string").toString());

	restoreProperties(properties);

	setAcceptHoverEvents(true);

    addConnectionPoint(new QLogicCircuitValueShapeConnectionPoint(this));
}

QRectF QLogicCircuitValueShape::boundingRect() const
{
    QRectF r = geometry();
    r.moveTo(0, 0);
    return r;
}

QVariantMap QLogicCircuitValueShape::defaultProperties(const QString & id)
{
	QVariantMap p;

	p["textColor"] = "black";
	QFont f("Arial");
	f.setPointSizeF(4);
	p["textFont"] = QDiagramProperty::toMap(f);

	QPen pen(Qt::black);
	pen.setWidthF(5);
	p["lineStyle"] = QDiagramProperty::toMap(pen);

    if (id== "value.analog"){
        p["signalType"] = "analog";
    }

	return p;
}

QPointF QLogicCircuitValueShape::hotSpot(const QString & id)
{
	return QPointF(-900, -50);
}

void QLogicCircuitValueShape::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

	painter->setPen(qdiagramproperty_cast<QPen>(property("lineStyle")));
    painter->setBrush(QBrush(Qt::white, Qt::SolidPattern));
    painter->drawPath(shape());
    if (property("signalType").toString() == "analog"){
        int offset = geometry().height() / 4;
        painter->save();
        QPainterPath p;
		p.moveTo(10 + offset, 20);
		p.lineTo(10 + offset, 80);
		p.moveTo(10 + offset, 50);
		p.lineTo(100 + offset, 50);

		p.moveTo(10 + offset, 50);
		p.lineTo(30 + offset, 30);
		p.lineTo(60 + offset, 70);
		p.lineTo(90 + offset, 20);

        painter->drawPath(p);
        painter->restore();
    } else if (property("signalType").toString() == "digital"){
        int offset = geometry().height() / 4;
        painter->save();
        QPainterPath p;
        p.lineTo(30 + offset, 80);
        p.lineTo(30 + offset, 20);
        p.lineTo(80 + offset, 20);
        p.lineTo(80 + offset, 80);
        p.lineTo(100 + offset, 80);

		painter->drawPath(p);
        painter->restore();
        QFont f;
        f.setFamily("Arial");
        f.setPixelSize(4);
        painter->setFont(f);

        QRectF r(boundingRect());
		r.setWidth(15);
        painter->drawText(r, Qt::AlignRight, "1\n0");
    }
	painter->setFont(pointToPixel(qdiagramproperty_cast<QFont>(property("textFont"))));
    QRectF r(boundingRect().adjusted(200, 0, 0, 0));
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

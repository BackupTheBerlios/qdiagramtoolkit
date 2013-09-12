#include "stdafx.h"
#include "qlogiccircuitflipflopshape.h"
#include "qlogiccircuitflipflopshape_p.h"

#include "qlogiccircuitplugin.h"

// @MS PMincho

#define BASE_SIZE 50.0
#define GATE_CP_SIZE 40.0


QLogicCircuitFlipFlopInputConnectionPoint::QLogicCircuitFlipFlopInputConnectionPoint(QAbstractDiagramShape* shape, const QString & id, int index, int maxConnections) :
    QAbstractDiagramShapeConnectionPoint(shape, id, QDiagramToolkit::West, maxConnections)
{
    m_index = index;
    setDirection(QAbstractDiagramShapeConnectionPoint::In);
    updatePosition();
}

QLogicCircuitFlipFlopInputConnectionPoint::~QLogicCircuitFlipFlopInputConnectionPoint()
{
}

void QLogicCircuitFlipFlopInputConnectionPoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->save();
    painter->drawRect(rect());
    painter->restore();
}

void QLogicCircuitFlipFlopInputConnectionPoint::updatePosition()
{
	int offset = 0;
	offset = parentShape()->geometry().height() / 4;
	setRect(QRectF(0, offset * (m_index + 1) - GATE_CP_SIZE / 2, GATE_CP_SIZE, GATE_CP_SIZE));
}

QLogicCircuitFlipFlopOutputConnectionPoint::QLogicCircuitFlipFlopOutputConnectionPoint(QAbstractDiagramShape* shape, const QString & id, int index, int maxConnections) :
    QAbstractDiagramShapeConnectionPoint(shape, id, QDiagramToolkit::East, maxConnections)
{
    m_index = index;
    setDirection(QAbstractDiagramShapeConnectionPoint::Out);
    updatePosition();
}

QLogicCircuitFlipFlopOutputConnectionPoint::~QLogicCircuitFlipFlopOutputConnectionPoint()
{
}

void QLogicCircuitFlipFlopOutputConnectionPoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->save();
    painter->setBrush(Qt::red);
    painter->drawRect(rect());
    painter->restore();
}

void QLogicCircuitFlipFlopOutputConnectionPoint::updatePosition()
{
	int offset = 0;
	offset = parentShape()->geometry().height() / 4;
	setRect(QRectF(parentShape()->boundingRect().width() - GATE_CP_SIZE, offset * (m_index + 1) - GATE_CP_SIZE / 2, GATE_CP_SIZE, GATE_CP_SIZE));
}

QLogicCircuitFlipFlopShape::QLogicCircuitFlipFlopShape(QGraphicsItem *parent) :
    QAbstractDiagramShape(parent)
{
}

QLogicCircuitFlipFlopShape::QLogicCircuitFlipFlopShape(const QMap<QString,QVariant> & properties, QGraphicsItem* parent) :
    QAbstractDiagramShape(QLogicCircuitPlugin::staticName(), QLogicCircuitFlipFlopShape::staticItemClass(), properties, parent)
{
	initGeometry(300, 200);

    addProperty("type", QDiagramToolkit::String, true, properties.value("type", "SR").toString());


	if (properties.value("type") == "Gated SR"){
		addConnectionPoint(new QLogicCircuitFlipFlopInputConnectionPoint(this, "set", 0, 1));
		addConnectionPoint(new QLogicCircuitFlipFlopInputConnectionPoint(this, "clock", 1, 1));
		addConnectionPoint(new QLogicCircuitFlipFlopInputConnectionPoint(this, "reset", 2, 1));

		addConnectionPoint(new QLogicCircuitFlipFlopOutputConnectionPoint(this, "q", 0, 1));
		addConnectionPoint(new QLogicCircuitFlipFlopOutputConnectionPoint(this, "nq", 2, 1));
	} else if (properties.value("type") == "SR"){
		addConnectionPoint(new QLogicCircuitFlipFlopInputConnectionPoint(this, "set", 0, 1));
		addConnectionPoint(new QLogicCircuitFlipFlopInputConnectionPoint(this, "reset", 2, 1));

		addConnectionPoint(new QLogicCircuitFlipFlopOutputConnectionPoint(this, "q", 0, 1));
		addConnectionPoint(new QLogicCircuitFlipFlopOutputConnectionPoint(this, "nq", 2, 1));
	}

    restoreProperties(properties);
}

QRectF QLogicCircuitFlipFlopShape::boundingRect() const
{
    return QRectF(0, 0, geometry().width(), geometry().height());
}

QVariantMap QLogicCircuitFlipFlopShape::defaultProperties(const QString & id)
{
	QVariantMap p;

	p["textColor"] = "black";
	QFont f("Arial");
	f.setPointSizeF(4);
	p["textFont"] = QDiagramProperty::toMap(f);

	QPen pen(Qt::black);
	pen.setWidthF(5);
	p["lineStyle"] = QDiagramProperty::toMap(pen);

	QBrush brush(Qt::white);
	p["background"] = QDiagramProperty::toMap(brush);

	if (id == "flipflop.jk"){
		p["type"] = "JK";
	} else if (id == "flipflop.sr.basic"){
		p["type"] = "SR";
	} else if (id == "flipflop.sr.gated"){
		p["type"] = "Gated SR";
	}
	return p;
}

QPointF QLogicCircuitFlipFlopShape::hotSpot(const QString & id)
{
	Q_UNUSED(id);
	return QPointF(0, -BASE_SIZE);
}

void QLogicCircuitFlipFlopShape::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setPen(qdiagramproperty_cast<QPen>(property("lineStyle")));
	painter->setBrush(qdiagramproperty_cast<QBrush>(property("background")));
    painter->drawPath(shape());
	painter->setFont(pointToPixel(qdiagramproperty_cast<QFont>(property("textFont"))));

	QFontMetrics fm(painter->font());
	int offset = geometry().height() / 4;

	painter->drawEllipse(QPoint(geometry().width() - offset + 15, geometry().height() - offset), 15, 15);
	if (property("type").toString() == "Gated SR"){
		painter->drawText(BASE_SIZE + fm.width('_'), BASE_SIZE * 1 + fm.height() / 2 - 8, "S");
		painter->drawText(BASE_SIZE + fm.width('_'), BASE_SIZE * 3 + fm.height() / 2 - 8, "R");
		painter->drawText(geometry().width() - BASE_SIZE - fm.width('_') - offset / 2, BASE_SIZE * 1 + fm.height() / 2 - 8, "Q");
		painter->drawText(geometry().width() - BASE_SIZE - fm.width('_') - offset / 2, BASE_SIZE * 3 + fm.height() / 2 - 8, "Q");
		painter->drawLine(geometry().width() - BASE_SIZE - fm.width('_') - offset / 2, BASE_SIZE * 3 - fm.height() / 2,
			geometry().width() - BASE_SIZE + fm.width('_') - offset + 3, BASE_SIZE * 3 - fm.height() / 2);
	} else if (property("type").toString() == "JK"){
		painter->drawText(BASE_SIZE + fm.width('_'), BASE_SIZE * 1 + fm.height() / 2 - 8, "J");
		painter->drawText(BASE_SIZE + fm.width('_'), BASE_SIZE * 3 + fm.height() / 2 - 8, "K");
		painter->drawText(geometry().width() - BASE_SIZE - fm.width('_') - offset / 2, BASE_SIZE * 1 + fm.height() / 2 - 8, "Q");
		painter->drawText(geometry().width() - BASE_SIZE - fm.width('_') - offset / 2, BASE_SIZE * 3 + fm.height() / 2 - 8, "Q");
		painter->drawLine(geometry().width() - BASE_SIZE - fm.width('_') - offset / 2, BASE_SIZE * 3 - fm.height() / 2,
			geometry().width() - BASE_SIZE + fm.width('_') - offset + 3, BASE_SIZE * 3 - fm.height() / 2);
	} else if (property("type").toString() == "SR"){
		painter->drawText(BASE_SIZE + fm.width('_'), BASE_SIZE * 1 + fm.height() / 2 - 8, "S");
		painter->drawText(BASE_SIZE + fm.width('_'), BASE_SIZE * 3 + fm.height() / 2 - 8, "R");
		painter->drawText(geometry().width() - BASE_SIZE - fm.width('_') - offset / 2, BASE_SIZE * 1 + fm.height() / 2 - 8, "Q");
		painter->drawText(geometry().width() - BASE_SIZE - fm.width('_') - offset / 2, BASE_SIZE * 3 + fm.height() / 2 - 8, "Q");
		painter->drawLine(geometry().width() - BASE_SIZE - fm.width('_') - offset / 2, BASE_SIZE * 3 - fm.height() / 2,
			geometry().width() - BASE_SIZE + fm.width('_') - offset + 3, BASE_SIZE * 3 - fm.height() / 2);
	}
    paintConnectionPoints(painter, option, widget);
    paintFocus(painter, option, widget);
}

QPainterPath QLogicCircuitFlipFlopShape::shape() const
{
    QPainterPath p;
    int offset = geometry().height() / 4;
    p.addRect(offset, 0, geometry().width() - offset * 2, geometry().height());
    p.moveTo(0, offset);
    p.lineTo(offset, offset);
	if (property("type").toString() == "JK" || property("type").toString() == "Gated SR"){
	    p.moveTo(0, offset * 2);
		p.lineTo(offset, offset * 2);

		p.moveTo(offset, offset * 2 - 15);
		p.lineTo(offset + 35, offset * 2);
		p.lineTo(offset, offset * 2 + 15);
	}
    p.moveTo(0, geometry().height() - offset);
    p.lineTo(offset, geometry().height() - offset);
	// outputs
    p.moveTo(geometry().width() - offset, offset);
    p.lineTo(geometry().width(), offset);

    p.moveTo(geometry().width() - offset, geometry().height() - offset);
    p.lineTo(geometry().width(), geometry().height() - offset);
    return p;
}

#include "stdafx.h"
#include "qsysmlcontrolconnectionpoint.h"

#include <qabstractdiagramshape.h>

#define BASE_SIZE 13.0
#define GATE_CP_SIZE 8.0

QSysMLControlConnectionPointIn::QSysMLControlConnectionPointIn(QAbstractDiagramShape* shape, const QString & id, int index, int maxConnections) :
    QAbstractDiagramShapeConnectionPoint(shape, id, QDiagramToolkit::North, maxConnections)
{
    m_index = index;
    setDirection(QAbstractDiagramShapeConnectionPoint::In);
    updatePosition();
}

QSysMLControlConnectionPointIn::~QSysMLControlConnectionPointIn()
{
}

void QSysMLControlConnectionPointIn::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->save();
	painter->setPen(Qt::NoPen);
	painter->setBrush(QColor("firebrick"));
    painter->drawRect(rect());
	//painter->setBrush(Qt::NoBrush);
	//painter->setPen(Qt::black);
	//QPainterPath p;
	//if (orientation() == QDiagramToolkit::West){
	//	p.moveTo(rect().topLeft());
	//	p.lineTo(rect().right(), rect().center().y());
	//	p.lineTo(rect().bottomLeft());
	//} else if (orientation() == QDiagramToolkit::East){
	//	p.moveTo(rect().topRight());
	//	p.lineTo(rect().left(), rect().center().y());
	//	p.lineTo(rect().bottomRight());
	//} else if (orientation() == QDiagramToolkit::North){
	//	p.moveTo(rect().topLeft());
	//	p.lineTo(rect().center().x(), rect().bottom());
	//	p.lineTo(rect().topRight());
	//}
	//painter->drawPath(p);
    painter->restore();
}

void QSysMLControlConnectionPointIn::updatePosition()
{
	if (parentShape()->property("nodeType") == "fork"){
		setRect(QRectF(parentShape()->geometry().width() / 2 - GATE_CP_SIZE / 2, 0, GATE_CP_SIZE, GATE_CP_SIZE));
	} else {
		if (parentShape()->hasProperty("branches")){
			qreal w = parentShape()->geometry().width();
			w /= parentShape()->property("branches").toDouble();
			w -= GATE_CP_SIZE;
			setRect(QRectF(GATE_CP_SIZE + m_index * w * 2 - GATE_CP_SIZE / 2, 0, GATE_CP_SIZE, GATE_CP_SIZE));
		} else {
			setRect(QRectF(parentShape()->geometry().width() / 2 - GATE_CP_SIZE / 2, 0, GATE_CP_SIZE, GATE_CP_SIZE));
		}
	}
}

QSysMLControlConnectionPointOut::QSysMLControlConnectionPointOut(QAbstractDiagramShape* shape, const QString & id, int index, int maxConnections) :
    QAbstractDiagramShapeConnectionPoint(shape, id, QDiagramToolkit::South, maxConnections)
{
    m_index = index;
    setDirection(QAbstractDiagramShapeConnectionPoint::Out);
    updatePosition();
}

QSysMLControlConnectionPointOut::QSysMLControlConnectionPointOut(QAbstractDiagramShape* shape, const QString & id, QDiagramToolkit::ConnectionPointOrientation orientation, int index, int maxConnections) :
    QAbstractDiagramShapeConnectionPoint(shape, id, orientation, maxConnections)
{
    m_index = index;
    setDirection(QAbstractDiagramShapeConnectionPoint::Out);
    updatePosition();
}

QSysMLControlConnectionPointOut::~QSysMLControlConnectionPointOut()
{
}

void QSysMLControlConnectionPointOut::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->save();
	painter->setPen(Qt::NoPen);
	painter->setBrush(QColor("firebrick"));
    painter->drawRect(rect());

	//painter->setBrush(Qt::NoBrush);
	//painter->setPen(Qt::black);
	//QPainterPath p;
	//if (orientation() == QDiagramToolkit::East){
	//	p.moveTo(rect().topLeft());
	//	p.lineTo(rect().right(), rect().center().y());
	//	p.lineTo(rect().bottomLeft());
	//} else if (orientation() == QDiagramToolkit::West){
	//	p.moveTo(rect().topRight());
	//	p.lineTo(rect().left(), rect().center().y());
	//	p.lineTo(rect().bottomRight());
	//} else if (orientation() == QDiagramToolkit::South){
	//	p.moveTo(rect().topLeft());
	//	p.lineTo(rect().center().x(), rect().bottom());
	//	p.lineTo(rect().topRight());
	//}
	//painter->drawPath(p);
    painter->restore();
}

void QSysMLControlConnectionPointOut::updatePosition()
{
	if (parentShape()->property("nodeType") == "decision"){
		if (orientation() == QDiagramToolkit::East){
			setRect(QRectF(parentShape()->geometry().width() - GATE_CP_SIZE / 2, parentShape()->geometry().height() / 2 - GATE_CP_SIZE / 2, GATE_CP_SIZE, GATE_CP_SIZE));
		} else if (orientation() == QDiagramToolkit::West){
			setRect(QRectF(0, parentShape()->geometry().height() / 2 - GATE_CP_SIZE / 2, GATE_CP_SIZE, GATE_CP_SIZE));
		} else if (orientation() == QDiagramToolkit::South){
			setRect(QRectF(parentShape()->geometry().width() / 2 - GATE_CP_SIZE / 2, parentShape()->geometry().height() - GATE_CP_SIZE, GATE_CP_SIZE, GATE_CP_SIZE));
		}	
	} else if (parentShape()->property("nodeType") == "join"){
		setRect(QRectF(parentShape()->geometry().width() / 2 - GATE_CP_SIZE / 2, parentShape()->geometry().height() - GATE_CP_SIZE, GATE_CP_SIZE, GATE_CP_SIZE));
	} else {
		if (parentShape()->hasProperty("branches")){
			qreal w = parentShape()->geometry().width();
			w /= parentShape()->property("branches").toDouble();
			w -= GATE_CP_SIZE;

			setRect(QRectF(GATE_CP_SIZE + m_index * w * 2 - GATE_CP_SIZE / 2, parentShape()->geometry().height() - GATE_CP_SIZE, GATE_CP_SIZE, GATE_CP_SIZE));
		} else {
			setRect(QRectF(parentShape()->geometry().width() / 2 - GATE_CP_SIZE / 2, parentShape()->geometry().height() - GATE_CP_SIZE, GATE_CP_SIZE, GATE_CP_SIZE));
		}
	}
}

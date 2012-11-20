#include "stdafx.h"
#include "qsysmlcontrolflowconnector.h"
#include "qsysmlplugin.h"

#include <qdiagramtoolkit.h>

#define PI 3.14159265

QSysMLControlFlowConnector::QSysMLControlFlowConnector(const QVariantMap & properties) :
    QSysMLFlowConnector("flow.control", properties)
{
    addProperty("text", QDiagramToolkit::String, false, properties.value("text"));
	restoreProperties(properties);
}

QRectF QSysMLControlFlowConnector::boundingRect() const
{
    return m_boundingRect.adjusted(-3, -3, 3, 3);
}

QList<QPointF> QSysMLControlFlowConnector::breakPoints() const
{
    return defaultConnector();
}

void QSysMLControlFlowConnector::calcBoundingRect()
{
 //   QPainterPath p;
 //   if (!m_breakPoints.isEmpty()){
 //       p.moveTo(m_breakPoints.first());
 //       for (int iPoints = 1; iPoints < m_breakPoints.size(); iPoints++){
 //           p.lineTo(m_breakPoints.at(iPoints));
 //       }
 //   }
 //   m_boundingRect = p.boundingRect();
	//m_boundingRect.moveTo(0, 0);
    QPainterPath p;
	if (orientationAtStart() == QDiagramToolkit::East){
		p = eastTo();
	} else if (orientationAtStart() == QDiagramToolkit::West){
		p = westTo();
	} else if (orientationAtStart() == QDiagramToolkit::South){
		p = southTo();
	}
	int sy = line().dy() >= 0?1:-1;
	QPainterPath pa;
	pa.moveTo(QPointF(line().p2().x() - 6, line().p2().y() - (sy * 12)));
	pa.lineTo(line().p2());
	pa.lineTo(QPointF(line().p2().x() + 6, line().p2().y() - (sy * 12)));
	p.connectPath(pa);
    m_boundingRect = p.controlPointRect();
}

bool QSysMLControlFlowConnector::canConnect(QAbstractDiagramShapeConnectionPoint* startPoint, QAbstractDiagramShapeConnectionPoint* endPoint) const
{
    if (startPoint == 0 || endPoint == 0){
        return false;
    }
    if (startPoint == endPoint){
        return false;
    }
	// Recursive connection are not allowed
	if (startPoint->parentShape() == endPoint->parentShape()){
		return false;
	}
    //
	if (startPoint->id().left(9) == "controlIn" && endPoint->id().left(10) == "controlOut"){
		return true;
	}
	if (endPoint->id().left(9) == "controlIn" && startPoint->id().left(10) == "controlOut"){
		return true;
	}
    return false;
}

bool QSysMLControlFlowConnector::canStartWith(QAbstractDiagramShapeConnectionPoint* cp) const
{
	if (cp){
		if (cp->id().left(9) == "controlIn" || cp->id().left(10) == "controlOut"){
			return true;
		}
	}
	return false;
}

QList<QPointF> QSysMLControlFlowConnector::defaultConnector() const
{
    QList<QPointF> points;
    QList<QPointF> sections;
    QLineF line(startPos(), endPos());
    QLineF lineEnd;
    QLineF lineStart;

    line = QLineF(startPos(), endPos());

    lineStart.setP1(startPos());
    lineEnd.setP1(endPos());


	sections << QPointF(line.x1(), line.y1() + line.dy() / 2);
	sections << QPointF(line.x2(), line.y1() + line.dy() / 2);
    points << startPos();
    QListIterator<QPointF> it(sections);
    while(it.hasNext()){
        points.append(it.next());
    }
    points << endPos();

	return points;
}

QPainterPath QSysMLControlFlowConnector::eastTo() const
{
	QPainterPath p;
	QLineF l = line();
	qreal r = radius();

	p.moveTo(l.p1());
//	if (orientationAtEnd() == QDiagramToolkit::ConnectionPointOrientationInvalid || orientationAtEnd() == QDiagramToolkit::North){
	if (orientationAtEnd() == QDiagramToolkit::North && l.dy() <= 0){
		if (l.dx() >= 0){
			if (abs(l.dx()) > r){
				p.lineTo(l.p1().x() + l.dx() / 2 - r, l.p1().y());
			}
			p.arcTo(l.p1().x() + l.dx() / 2 - r, l.p1().y() - r, r, r, 270, 90);
			p.lineTo(l.p1().x() + l.dx() / 2, l.p2().y() - r);
			p.arcTo(l.p1().x() + l.dx() / 2, l.p2().y() - r * 2, r, r, 180, -90);
			p.lineTo(l.p2().x() - r, l.p2().y() - r * 2);
			p.arcTo(l.p2().x() - r, l.p2().y() - r * 2, r, r, 90, -90);
		}
	} else {
		if (l.dx() >= 0){
			if (abs(l.dx()) > r){
				p.lineTo(l.p1().x() + l.dx() - r, l.p1().y());
			}
			if (l.dy() >= 0){
				p.arcTo(l.p1().x() + l.dx() - r, l.p1().y(), r, r, 90, -90);
			} else {
				p.arcTo(l.p1().x() + l.dx() - r, l.p1().y() - r, r, r, 270, 90);
			}
		} else {
			if (abs(l.dx()) > r){
				p.lineTo(l.p2().x() + r, l.p1().y());
			}
			if (l.dy() >= 0){
				p.arcTo(l.p2().x(), l.p1().y(), r, r, 90, 90);
			} else {
				p.arcTo(l.p2().x(), l.p1().y() - r, r, r, 270, -90);
			}
		}
	}
		p.lineTo(l.p2());
//	}
	return p;
}

QPainterPath QSysMLControlFlowConnector::westTo() const
{
	QPainterPath p;
	QLineF l = line();
	qreal r = radius();
//	if (endConnectionPoint() == 0){
		p.moveTo(l.p1());
		if (l.dx() >= 0){
			if (abs(l.dx()) > r){
				p.lineTo(l.p1().x() - l.dx() - r, l.p1().y());
			}
			p.arcTo(l.p1().x() - l.dx() + r, l.p1().y(), r, r, 90, -90);
			p.lineTo(l.p2());
		} else {
			if (abs(l.dx()) > r){
				p.lineTo(l.p1().x() + l.dx() + r, l.p1().y());
			}
			if (l.dy() >= 0){
				p.arcTo(l.p1().x() + l.dx(), l.p1().y(), r, r, 90, 90);
			} else {
			}
		}
		p.lineTo(l.p2());
//	}
	return p;
}

QPainterPath QSysMLControlFlowConnector::southTo() const
{
	QLineF l = line();
	QPainterPath p;
	int r = radius();
	int sy = l.dy() >= 0?1:-1;
	p.moveTo(l.p1());
	if (abs(l.dy()) > r * 2){
		p.lineTo(l.p1().x(), l.p1().y() + l.dy() / 2 - (sy * r));
	}
	if (l.dx() >= 0){
		if (l.dy() >= 0){
			p.arcTo(l.p1().x(), l.p1().y() + l.dy() / 2 - r, r, r, 180, 90);
		} else {
			p.arcTo(l.p1().x(), l.p1().y() + l.dy() / 2, r, r, 180, -90);
		}
	} else {
		if (l.dy() >= 0){
			p.arcTo(l.p1().x() - r, l.p1().y() + l.dy() / 2 - r, r, r, 0, -90);
		} else {
			p.arcTo(l.p1().x() - r, l.p1().y() + l.dy() / 2, r, r, 0, 90);
		}
	}
	if (abs(l.dx()) > r * 2){
		if (l.dx() >= 0){
			p.lineTo(l.p2().x() - r, l.p1().y() + (sy * abs(l.dy())) / 2);
		} else {
			p.lineTo(l.p2().x() + r, l.p1().y() + l.dy() / 2);
		}
	}
	if (l.dx() >= 0){
		if (l.dy() >= 0){
			p.arcTo(l.p2().x() - r, l.y1() + l.dy() / 2, r, r, 90, -90);
		} else {
			p.arcTo(l.p2().x() - r, l.y1() + l.dy() / 2 - r, r, r, 270, 90);
		}
	} else {
		if (l.dy() >= 0){
			p.arcTo(l.p2().x(), l.y1() + l.dy() / 2, r, r, 90, 90);
		} else {
			p.arcTo(l.p2().x(), l.y1() + l.dy() / 2 - r , r, r, 270, -90);
		}
	}
	p.lineTo(QPointF(l.p2().x(), l.p2().y()));
	return p;
}

void QSysMLControlFlowConnector::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
	painter->save();
	QPen pen;
	pen.setStyle(Qt::DashLine);
	pen.setWidthF(2);
	painter->setPen(pen);
//	painter->setPen(qdiagramproperty_cast<QPen>(property("lineColor")));

	if (orientationAtStart() == QDiagramToolkit::East){
		painter->drawPath(eastTo());
	} else if (orientationAtStart() == QDiagramToolkit::West){
		painter->drawPath(westTo());
	} else if (orientationAtStart() == QDiagramToolkit::South){
		painter->drawPath(southTo());
	}
	pen.setStyle(Qt::SolidLine);
	pen.setJoinStyle(Qt::RoundJoin);
	painter->setPen(pen);

	int sy = line().dy() >= 0?1:-1;
	QPainterPath p;
	p.moveTo(QPointF(line().p2().x() - 6, line().p2().y() - (sy * 12)));
	p.lineTo(line().p2());
	p.lineTo(QPointF(line().p2().x() + 6, line().p2().y() - (sy * 12)));
	painter->drawPath(p);
	painter->restore();

	if (isSelected()){
		paintFocus(painter, option, widget);
//        paintBreakPoints(painter, m_breakPoints);
    }
}

void QSysMLControlFlowConnector::paintBreakPoints(QPainter * painter, const QList<QPointF> & points)
{
    if (points.size() < 3){
        return;
    }
    painter->save();
    painter->setBrush(QBrush(Qt::red));
    for (int iPoints = 1; iPoints < points.size() - 1; iPoints++){
        painter->drawRect(points.at(iPoints).x() - 2, points.at(iPoints).y() - 2, 4, 4);
    }
    painter->restore();
}

void QSysMLControlFlowConnector::paintEndOfLine(QPainter* painter, const QPointF & pos, double angle)
{
    double arrowSize = 20;
    QPointF arrowP1 = pos - QPointF(sin(angle + PI / 3) * arrowSize,
                                           cos(angle + PI / 3) * arrowSize);
    QPointF arrowP2 = pos - QPointF(sin(angle + PI - PI / 3) * arrowSize,
                                           cos(angle + PI - PI / 3) * arrowSize);

    QPolygonF arrowHead;
	arrowHead.clear();
	arrowHead << pos << arrowP1 << arrowP2;
	painter->drawLine(pos, arrowP1);
	painter->drawLine(pos, arrowP2);
}

void QSysMLControlFlowConnector::updatePosition()
{
    prepareGeometryChange();
    m_breakPoints = breakPoints();
    calcBoundingRect();
    update();
}

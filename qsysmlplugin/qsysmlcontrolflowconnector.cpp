/******************************************************************************
** Copyright (C) 2013 Martin Hoppe martin@2x2hoppe.de
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
#include "qsysmlcontrolflowconnector.h"
#include "qsysmlplugin.h"

#include <qdiagramstylesheet.h>
#include <qdiagramtoolkit.h>
#include <qdiagramgraphicstextitem.h>

#include "qsysmlaction.h"
#include "qsysmlnode.h"

#define PI 3.14159265

class ConnectorPath
{
public:
	ConnectorPath(QDiagramToolkit::ConnectionPointOrientation orientation);
	~ConnectorPath();

	void build(const QLineF & line);

	QPainterPath painterPath() const;
private:
	void buildPainterPath();
	void next(QDiagramToolkit::ConnectionPointOrientation orientation);

	QLineF m_line;
	QDiagramToolkit::ConnectionPointOrientation m_orientation;
	QPainterPath m_path;
	qreal m_radius;
	QList<QLineF> m_sections;
};

ConnectorPath::ConnectorPath(QDiagramToolkit::ConnectionPointOrientation orientation)
{
	m_orientation = orientation;
	m_radius = 20;
}

ConnectorPath::~ConnectorPath()
{
}

void ConnectorPath::build(const QLineF & line)
{
	m_line = line;
	if (m_line.dx() == 0 || m_line.dy() == 0){
		m_sections << m_line;
	} else if (m_line.dx() > 0 && m_line.dy() > 0){
		if (m_orientation == QDiagramToolkit::South){
			m_sections << QLineF(m_line.p1(), QPointF(m_line.p1().x(), m_line.p1().y() + m_line.dy() / 2 - m_radius));
			next(QDiagramToolkit::East);
		}
	}
	buildPainterPath();
}

void ConnectorPath::buildPainterPath()
{
	m_path = QPainterPath();
	if (m_sections.isEmpty()){
		return;
	}
	m_path.moveTo(m_sections.first().p1());
	m_path.lineTo(m_sections.first().p2());
	for (int i = 1; i < m_sections.size(); i++){
		if (m_sections.at(i - 1).dx() == 0){
			if (m_sections.at(i).dy() == 0){
				m_path.arcTo(m_sections.at(i - 1).p2().x(), m_sections.at(i - 1).p2().y(), m_radius, m_radius, 180, 90);
			}
		} else if (m_sections.at(i - 1).dy() == 0){
			if (m_sections.at(i).dx() == 0){
				m_path.arcTo(m_sections.at(i - 1).p2().x(), m_sections.at(i - 1).p2().y(), m_radius, m_radius, 90, -90);
			}
		}
		m_path.lineTo(m_sections.at(i).p2());

		//QLineF l(m_sections.at(i - 1).p2(), m_sections.at(i).p1());
		//if (l.dx() > 0 && l.dy() > 0){
		//	m_path.arcTo(m_sections.at(i - 1).p2().x(), m_sections.at(i - 1).p2().y(), m_radius, m_radius, 180, 90);
		//}
		//m_path.lineTo(m_sections.at(i).p2());
	}
	//qDebug() << m_path;
}

void ConnectorPath::next(QDiagramToolkit::ConnectionPointOrientation orientation)
{
	QLineF line;
	QPointF p1;
	if (line.dx() == 0 && line.dy() == 0){
//		return;
	}
	if (orientation == QDiagramToolkit::East){
		if (m_sections.last().dx() == 0){
			// Last secion is heading south
			if (m_sections.last().dy() >= 0){
				p1 = QPointF(m_sections.last().p2().x() + m_radius, m_sections.last().p2().y() + m_radius);
			} else {
				//	line = QLineF(m_sections.last().p2().x() + m_radius, m_sections.last().p2().y() - m_radius);
			}
		} else if (m_sections.last().dy() == 0){
			// Last secion is heading east
			if (m_orientation == QDiagramToolkit::South){
				m_sections << QLineF(m_sections.last().p2(), m_line.p2());
			}
		}
		m_sections << QLineF(p1, QPointF(m_line.p2().x() - m_radius, p1.y()));
		next(m_orientation);
	} else if (orientation == QDiagramToolkit::South){
		QPointF p1(m_sections.last().p2().x() + m_radius, m_sections.last().p2().y() + m_radius);
		m_sections << QLineF(p1, m_line.p2());
	}
}

QPainterPath ConnectorPath::painterPath() const
{
	return m_path;
}

QSysMLControlFlowConnector::QSysMLControlFlowConnector(const QVariantMap & properties) :
    QSysMLFlowConnector("flow.control", properties)
{
	restoreProperties(properties);
}

QRectF QSysMLControlFlowConnector::boundingRect() const
{
	QRectF r = m_boundingRect.adjusted(-3, -3, 3, 3);
    return shape().boundingRect();
}

QList<QPointF> QSysMLControlFlowConnector::breakPoints() const
{
	QPointF sp = startPos();
	QPointF ep = endPos();

	if (ep.y() - 100 > sp.y()){
		if (ep.x() >= sp.x()){
			return trbConnector();
		} else {
			return  tlbConnector();
		}
	} else {
		if (ep.x() >= sp.x()){
			return btrConnector();
		} else {
			return btlConnector();
		}
	}
    return defaultConnector();
}

QList<QPointF> QSysMLControlFlowConnector::btlConnector() const
{
	QList<QPointF> l;

	l << startPos();
	l << QPointF(startPos().x(), startPos().y() - 100);
	l << QPointF(endPos().x(), startPos().y() - 100);
	l << endPos();
	return l;
}

QList<QPointF> QSysMLControlFlowConnector::btrConnector() const
{
	QList<QPointF> l;

	l << startPos();
		l << QPointF(startPos().x(), startPos().y() + 100);
		l << QPointF(endPos().x(), startPos().y() + 100);
	l << endPos();
	return l;
}

QList<QPointF> QSysMLControlFlowConnector::tlbConnector() const
{
	QList<QPointF> l;

	l << startPos();
	l << QPointF(startPos().x(), endPos().y());
	l << endPos();
	return l;
}

QList<QPointF> QSysMLControlFlowConnector::trbConnector() const
{
	QList<QPointF> lp;
	QLineF l(startPos(), endPos());
	lp << l.p1();
	if (orientationAtEnd() == QDiagramToolkit::ConnectionPointOrientationInvalid){
		lp << QPointF(startPos().x(), endPos().y());
	} else if (orientationAtEnd() == QDiagramToolkit::North){
		lp << QPointF(l.p1().x(), l.p1().y() + l.dy() / 2);
		lp << QPointF(l.p2().x(), l.p1().y() + l.dy() / 2);
	}
	lp << l.p2();

	return lp;
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
	qreal o = 0;
	p.moveTo(l.p1());
	if (l.dy() >= 0){
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
				p.arcTo(l.p1().x() + l.dx(), l.p1().y() - r, r, r, 270, -90);
			}
		}
	} else {
		if (l.dx() >= 0){
			p.arcTo(l.p1().x(), l.p1().y(), r, r, 90, 90);
		} else {
			if (abs(l.dx()) > r){
				p.lineTo(l.p2().x() + r, l.p1().y());
			}
			p.arcTo(l.p2().x(), l.p1().y() - r, r, r, 270, -90);
		}

	}
	p.lineTo(l.p2());
//	}
	return p;
}

void QSysMLControlFlowConnector::calcNextSection(int previous)
{
	if (previous == - 1){
		
	}
}

QPainterPath QSysMLControlFlowConnector::southTo() const
{
	QLineF l = line();
	QPainterPath p;
	int r = radius();
	int sy = l.dy() >= 0?1:-1;
	p.moveTo(l.p1());
	if (l.dy() >= r){
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
	} else {
		if (l.dx() >= 0){
			p.arcTo(l.p1().x(), l.p1().y(), r, r, 180, 90);
			p.lineTo(l.p2().x() - r, l.p1().y() + r);
			p.arcTo(l.p2().x() - r, l.p1().y(), r, r, 270, 90);
		} else {
			p.arcTo(l.p1().x() - r, l.p1().y(), r, r, 270, -90);
		}
	}
	p.lineTo(l.p2());
	return p;
}

void QSysMLControlFlowConnector::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
	painter->save();
	painter->setPen(diagram()->styleSheet()->lineStyle("ControlFlowLineStyle").pen());

	//if (orientationAtStart() == QDiagramToolkit::East){
	//	painter->drawPath(eastTo());
	//} else if (orientationAtStart() == QDiagramToolkit::West){
	//	painter->drawPath(westTo());
	//} else if (orientationAtStart() == QDiagramToolkit::South){
	//	painter->drawPath(southTo());
	//}
	painter->drawPath(m_polyline.painterPath(100));
//	drawPolyline(painter, m_breakPoints);
	//pen.setStyle(Qt::SolidLine);
	//pen.setJoinStyle(Qt::RoundJoin);
	//painter->setPen(pen);

	painter->restore();

	if (isSelected()){
		paintFocus(painter, option, widget);
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

QPainterPath QSysMLControlFlowConnector::shape() const
{
	QPainterPath p;
	QPainterPath tp = textItem()->shape();

	if (orientationAtStart() == QDiagramToolkit::East){
		p = eastTo();
	} else if (orientationAtStart() == QDiagramToolkit::West){
		p = westTo();
	} else if (orientationAtStart() == QDiagramToolkit::South){
		//ConnectorPath path(QDiagramToolkit::South);
		//path.build(line());
		//p = path.painterPath();
		p = southTo();
	}
	tp.translate(p.boundingRect().topLeft());
	tp.translate(0, -textItem()->boundingRect().height());
	p.connectPath(tp);
	return p;
}

void QSysMLControlFlowConnector::updatePosition()
{
    prepareGeometryChange();
    m_breakPoints = breakPoints();
    calcBoundingRect();

	QRectF boundary;

	QAbstractDiagramShape* s = 0;
	if (connectionPointAtStart()){
		s = connectionPointAtStart()->parentShape();
	}
	if (s){
		boundary = s->geometry().adjusted(-100, -100, 100, 100);
	}
	QLineF l(startPos(), endPos());
	QDiagramConnectorPolyline p(100);
	p.addPoint(l.p1());
	if (orientationAtEnd() == QDiagramToolkit::ConnectionPointOrientationInvalid){
		if (orientationAtStart() == QDiagramToolkit::East){
			p.addPoint(l.p2().x(), l.p1().y());
		} else if (orientationAtStart() == QDiagramToolkit::West){
			p.addPoint(l.p2().x(), l.p1().y());
		} else if (orientationAtStart() == QDiagramToolkit::South){
			if (endPos().y() - startPos().y() < 100){
				p.addPoint(QPointF(l.p1().x(), l.p1().y() + 100));
				p.addPoint(QPointF(endPos().x(), l.p1().y() + 100));

			} else {
				p.addPoint(QPointF(startPos().x(), endPos().y()));
			}
		}
	} else if (orientationAtEnd() == QDiagramToolkit::East){
		if (orientationAtStart() == QDiagramToolkit::South){
			p.addPoint(l.p1().x(), l.p2().y());
		}
	} else if (orientationAtEnd() == QDiagramToolkit::West){
		if (orientationAtStart() == QDiagramToolkit::South){
			p.addPoint(l.p1().x(), l.p2().y());
		}
	} else if (orientationAtEnd() == QDiagramToolkit::North){
		if (orientationAtStart() == QDiagramToolkit::East){
			p.addPoint(l.p2().x(), l.p1().y());
		} else if (orientationAtStart() == QDiagramToolkit::West){
			p.addPoint(l.p2().x(), l.p1().y());
		} else {
			p.addPoint(QPointF(l.p1().x(), l.p1().y() + l.dy() / 2));
			p.addPoint(QPointF(l.p2().x(), l.p1().y() + l.dy() / 2));
		}
	}
	p.addPoint(l.p2());
	m_polyline = p;
	
	
	update();
	updateTextItemPosition();
}

void QSysMLControlFlowConnector::updateTextItemPosition()
{
	if (orientationAtStart() == QDiagramToolkit::East){
		QPointF p = line().p1();
		p.setY(p.y() - textItem()->boundingRect().height());
		textItem()->setPos(p);
	} else if (orientationAtStart() == QDiagramToolkit::West){
		QPointF p = line().p1();
		p.setY(p.y() - textItem()->boundingRect().height());
		p.setX(p.x() - textItem()->boundingRect().width());
		textItem()->setPos(p);
	} else {
		textItem()->setPos(line().p1());
	}
}
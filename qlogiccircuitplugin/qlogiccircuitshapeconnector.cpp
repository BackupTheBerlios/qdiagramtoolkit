/******************************************************************************
** Copyright (C) 2011-2012 Martin Hoppe martin@2x2hoppe.de
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
#include "qlogiccircuitshapeconnector.h"

#include "qlogiccircuitplugin.h"

#include "qlogiccircuitfunctionshape.h"
#include "qlogiccircuitgateshape.h"
#include "qlogiccircuitinputshape.h"
#include "qlogiccircuitoutputshape.h"
#include "qlogiccircuitparametershape.h"
//												Shape at End
// Shape Start	Property Name	Property Value
// Input		Signal Type		Analog

// QMap<QString, QMultiMap<QString, QString> > 
QLogicCircuitShapeConnector::QLogicCircuitShapeConnector(const QVariantMap & properties) :
    QAbstractDiagramShapeConnector(QLogicCircuitPlugin::staticName(), "default", properties)
{
	addProperty("lineStyle", QDiagramToolkit::Pen, true, properties.value("lineStyle"));
	restoreProperties(properties);
}

QRectF QLogicCircuitShapeConnector::boundingRect() const
{
    return m_boundingRect.adjusted(-3, -3, 3, 3);
}

QList<QPointF> QLogicCircuitShapeConnector::breakPoints() const
{
    return defaultConnector();
}

void QLogicCircuitShapeConnector::calcBoundingRect()
{
    QPainterPath p;
    if (!m_breakPoints.isEmpty()){
        p.moveTo(m_breakPoints.first());
        for (int iPoints = 1; iPoints < m_breakPoints.size(); iPoints++){
            p.lineTo(m_breakPoints.at(iPoints));
        }
    }
    m_boundingRect = p.boundingRect();
}

bool QLogicCircuitShapeConnector::canConnect(QAbstractDiagramShapeConnectionPoint* startPoint, QAbstractDiagramShapeConnectionPoint* endPoint) const
{
    if (startPoint == 0 || endPoint == 0){
        return false;
    }
    if (startPoint == endPoint){
        return false;
    }
    //
    //
	if (startPoint->parentShape()->metaData()->itemClass() == QLogicCircuitGateShape::staticItemClass()){
		if (endPoint->parentShape()->metaData()->itemClass() == QLogicCircuitOutputShape::staticItemClass() && endPoint->connections().size() < endPoint->maxConnections()){
			return true;
		} else if (endPoint->parentShape()->metaData()->itemClass() == QLogicCircuitInputShape::staticItemClass()){
			return true;
		}
        // Gate to Gate
        if (startPoint->orientation() == QDiagramToolkit::East && endPoint->orientation() == QDiagramToolkit::West){
            if (endPoint->connections().size() < endPoint->maxConnections()){
                // Output to Input
                return true;
            }
        }
    } else if (startPoint->parentShape()->metaData()->itemClass() == QLogicCircuitFunctionShape::staticItemClass()){
        if (startPoint->parentShape()->property("function") == "comparator"){
			if (endPoint->parentShape()->metaData()->itemClass() == QLogicCircuitFunctionShape::staticItemClass()){
                if (endPoint->id() == "trigger"){
                    return true;
                }
			} else if (endPoint->parentShape()->metaData()->itemClass() == QLogicCircuitOutputShape::staticItemClass()){
                if (endPoint->parentShape()->property("signalType") == "digital"){
                    return true;
                }
			} else if (endPoint->parentShape()->metaData()->itemClass() == QLogicCircuitGateShape::staticItemClass()){
				return true;
			} else if (endPoint->parentShape()->metaData()->itemClass() == QLogicCircuitParameterShape::staticItemClass()){
                if (endPoint->parentShape()->property("signalType") == "analog"){
                    return true;
                }
            }
        } else if (startPoint->parentShape()->property("function") == "operatingHoursCounter"){
            if (endPoint->parentShape()->metaData()->itemClass() == QLogicCircuitOutputShape::staticItemClass()){
                if (endPoint->parentShape()->property("signalType") == "analog"){
                    return true;
                }
            }
        } else if (startPoint->parentShape()->property("function") == "timer"){
            if (endPoint->parentShape()->metaData()->itemClass() == QLogicCircuitOutputShape::staticItemClass()){
                if (endPoint->parentShape()->property("signalType") == "digital"){
                    return true;
                }
			} else if (endPoint->parentShape()->metaData()->itemClass() == QLogicCircuitGateShape::staticItemClass()){
				return true;
			}
        }
    } else if (startPoint->parentShape()->metaData()->itemClass() == QLogicCircuitInputShape::staticItemClass()){
        if (startPoint->parentShape()->property("signalType").toString() == "analog"){
            if (endPoint->parentShape()->metaData()->itemClass() == QLogicCircuitFunctionShape::staticItemClass()){
                if (endPoint->parentShape()->property("Function") == "comparator"){
                    return true;
                }
            }
        } else if (startPoint->parentShape()->property("signalType").toString() == "digital"){
            if (endPoint->parentShape()->metaData()->itemClass() == QLogicCircuitFunctionShape::staticItemClass()){
				if (endPoint->parentShape()->property("Function") == "timer"){
					if (endPoint->id() == "trigger"){
						return true;
					}
				}
			} else if (endPoint->parentShape()->metaData()->itemClass() == QLogicCircuitGateShape::staticItemClass()){
                return true;
            }
        }
	} else if (startPoint->parentShape()->metaData()->itemClass() == QLogicCircuitParameterShape::staticItemClass()){
        if (startPoint->parentShape()->property("signalType").toString() == "analog"){
			if (endPoint->parentShape()->metaData()->itemClass() == QLogicCircuitFunctionShape::staticItemClass()){
				if (endPoint->parentShape()->property("Function") == "comparator"){
					return true;
				} else if (endPoint->parentShape()->property("Function").toString() == "timer"){
					if (endPoint->id() == "parameter"){
						return true;
					}
				}
			}
		} else if (startPoint->parentShape()->property("signalType").toString() == "digital"){
	        if (endPoint->parentShape()->metaData()->itemClass() == QLogicCircuitGateShape::staticItemClass()){
				return true;
			}
		}
	} else if (startPoint->parentShape()->metaData()->itemClass() == "value"){
		if (endPoint->parentShape()->metaData()->itemClass() == QLogicCircuitFunctionShape::staticItemClass()){
			if (endPoint->parentShape()->property("Function") == "comparator"){
				return true;
			} else if (endPoint->parentShape()->property("Function") == "timer"){
				if (endPoint->id() == "parameter"){
					return true;
				}
			}
		}
    }
    return false;
}

QList<QPointF> QLogicCircuitShapeConnector::defaultConnector() const
{
    QList<QPointF> points;
    QList<QPointF> sections;
    QLineF line(startPos(), endPos());
    QLineF lineEnd;
    QLineF lineStart;

    line = QLineF(startPos(), endPos());

    lineStart.setP1(startPos());
    lineEnd.setP1(endPos());

    switch(orientationAtStart()){
    case QDiagramToolkit::East:
        if (line.dx() < 20){
            lineStart.setP2(startPos() + QPointF(20, 0));

            lineEnd.setP2(lineEnd.p1() - QPointF(20, 0));
            sections << QPointF(lineStart.p2().x(), lineStart.p1().y() + line.dy() / 2)
                          << QPointF(lineEnd.p2().x(), lineEnd.p2().y() - line.dy() / 2);
        } else {
            lineStart.setP2(lineStart.p1() + QPointF(line.dx() / 2 , 0));

            lineEnd.setP2(QPointF(lineStart.p2().x(), lineEnd.p1().y()));
        }
        break;
    case QDiagramToolkit::West:
        if (line.dx() > -20){
            lineStart.setP2(startPos() - QPointF(20, 0));

            lineEnd.setP2(lineEnd.p1() + QPointF(20, 0));
            sections << QPointF(lineStart.p2().x(), lineStart.p1().y() + line.dy() / 2)
                          << QPointF(lineEnd.p2().x(), lineEnd.p2().y() - line.dy() / 2);
        } else {
            lineStart.setP2(lineStart.p1() + QPointF(line.dx() / 2 , 0));

            lineEnd.setP2(QPointF(lineStart.p2().x(), lineEnd.p1().y()));
        }
        break;
    default:
        break;
    }


    points.append(lineStart.p1());
    points.append(lineStart.p2());
    QListIterator<QPointF> it(sections);
    while(it.hasNext()){
        points.append(it.next());
    }
    points.append(lineEnd.p2());
    points.append(lineEnd.p1());
    return points;
}

void QLogicCircuitShapeConnector::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if (connectionPointAtStart() && connectionPointAtStart()->parentShape()->property("state").toBool()){
        painter->setPen(Qt::green);
    }
	QPen pen(Qt::black);
	pen.setWidthF(5);
	painter->setPen(pen);
    drawPolyline(painter, m_breakPoints);
    if (isSelected()){
        paintBreakPoints(painter, m_breakPoints);
    }
}

void QLogicCircuitShapeConnector::paintBreakPoints(QPainter * painter, const QList<QPointF> & points)
{
    if (points.size() < 3){
        return;
    }
    painter->save();
    painter->setBrush(QBrush(Qt::red));
    for (int iPoints = 0; iPoints < points.size(); iPoints++){
		if (iPoints == 0){
			painter->drawRect(points.at(iPoints).x(), points.at(iPoints).y() - 15, 30, 30);
		} else if (iPoints == points.size() - 1){
			painter->drawRect(points.at(iPoints).x() - 30, points.at(iPoints).y() - 15, 30, 30);
		} else {
			painter->drawRect(points.at(iPoints).x() - 15, points.at(iPoints).y() - 15, 30, 30);
		}
    }
    painter->restore();
}

void QLogicCircuitShapeConnector::updatePosition()
{
    prepareGeometryChange();
    m_breakPoints = breakPoints();
    calcBoundingRect();
    update();
}

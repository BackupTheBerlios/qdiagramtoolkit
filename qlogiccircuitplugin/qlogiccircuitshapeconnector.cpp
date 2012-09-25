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

//												Shape at End
// Shape Start	Property Name	Property Value
// Input		Signal Type		Analog

// QMap<QString, QMultiMap<QString, QString> > 
QLogicCircuitShapeConnector::QLogicCircuitShapeConnector(const QVariantMap & properties) :
    QAbstractDiagramShapeConnector(properties)
{
	restoreProperties(properties);
}

//QLogicCircuitShapeConnector::QLogicCircuitShapeConnector(const QString & uuid, const QString & style) :
//    QAbstractDiagramShapeConnector(uuid, style)
//{
//    setFlag(QGraphicsItem::ItemIsSelectable);
//}

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
    QPainterPath mPath;
    if (!m_breakPoints.isEmpty()){
        mPath.moveTo(m_breakPoints.first());
        for (int iPoints = 1; iPoints < m_breakPoints.size(); iPoints++){
            mPath.lineTo(m_breakPoints.at(iPoints));
        }
    }
    m_boundingRect = mPath.boundingRect();
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
    if (startPoint->parentShape()->property("shape").toString() == "gate"){
		if (endPoint->parentShape()->property("shape") == "output" && endPoint->connections().size() < endPoint->maxConnections()){
			return true;
		} else if (endPoint->parentShape()->property("shape") == "input"){
			return true;
		}
        // Gate to gate
        if (startPoint->orientation() == QAbstractDiagramShapeConnectionPoint::East && endPoint->orientation() == QAbstractDiagramShapeConnectionPoint::West){
            if (endPoint->connections().size() < endPoint->maxConnections()){
                // Output to input
                return true;
            }
        }
    } else if (startPoint->parentShape()->property("shape").toString() == "function"){
        if (startPoint->parentShape()->property("function") == "comparator"){
            if (endPoint->parentShape()->property("shape") == "output"){
                if (endPoint->parentShape()->property("signalType") == "analog"){
                    return true;
                }
			} else if (endPoint->parentShape()->property("shape") == "gate"){
				return true;
			} else if (endPoint->parentShape()->property("shape") == "parameter"){
                if (endPoint->parentShape()->property("signalType") == "analog"){
                    return true;
                }
            }
        } else if (startPoint->parentShape()->property("function") == "operatingHoursCounter"){
            if (endPoint->parentShape()->property("shape") == "output"){
                if (endPoint->parentShape()->property("signalType") == "analog"){
                    return true;
                }
            }
        }
    } else if (startPoint->parentShape()->property("shape").toString() == "input"){
        if (startPoint->parentShape()->property("signalType").toString() == "analog"){
            if (endPoint->parentShape()->property("shape") == "function"){
                if (endPoint->parentShape()->property("function") == "comparator"){
                    return true;
                }
            }
        } else if (startPoint->parentShape()->property("signalType").toString() == "digital"){
            if (endPoint->parentShape()->property("shape").toString() == "gate"){
                return true;
			} else if (endPoint->parentShape()->property("shape").toString() == "output"){
                return true;
            }
        }
    } else if (startPoint->parentShape()->property("shape").toString() == "parameter"){
        if (startPoint->parentShape()->property("signalType").toString() == "analog"){
	        if (endPoint->parentShape()->property("shape").toString() == "function"){
				if (endPoint->parentShape()->property("function") == "comparator"){
					return true;
				}
			}
		}
    }
    return false;
}

QList<QPointF> QLogicCircuitShapeConnector::defaultConnector() const
{
    QList<QPointF> mPoints;
    QList<QPointF> mIntermediate;
    QLineF mLine(startPos(), endPos());
    QLineF mLineEnd;
    QLineF mLineStart;

    mLine = QLineF(startPos(), endPos());

    mLineStart.setP1(startPos());
    mLineEnd.setP1(endPos());

    switch(orientationAtStart()){
    case QAbstractDiagramShapeConnectionPoint::East:
        if (mLine.dx() < 20){
            mLineStart.setP2(startPos() + QPointF(20, 0));

            mLineEnd.setP2(mLineEnd.p1() - QPointF(20, 0));
            mIntermediate << QPointF(mLineStart.p2().x(), mLineStart.p1().y() + mLine.dy() / 2)
                          << QPointF(mLineEnd.p2().x(), mLineEnd.p2().y() - mLine.dy() / 2);
        } else {
            mLineStart.setP2(mLineStart.p1() + QPointF(mLine.dx() / 2 , 0));

            mLineEnd.setP2(QPointF(mLineStart.p2().x(), mLineEnd.p1().y()));
        }
        break;
    case QAbstractDiagramShapeConnectionPoint::West:
        if (mLine.dx() > -20){
            mLineStart.setP2(startPos() - QPointF(20, 0));

            mLineEnd.setP2(mLineEnd.p1() + QPointF(20, 0));
            mIntermediate << QPointF(mLineStart.p2().x(), mLineStart.p1().y() + mLine.dy() / 2)
                          << QPointF(mLineEnd.p2().x(), mLineEnd.p2().y() - mLine.dy() / 2);
        } else {
            mLineStart.setP2(mLineStart.p1() + QPointF(mLine.dx() / 2 , 0));

            mLineEnd.setP2(QPointF(mLineStart.p2().x(), mLineEnd.p1().y()));
        }
        break;
    default:
        break;
    }


    mPoints.append(mLineStart.p1());
    mPoints.append(mLineStart.p2());
    QListIterator<QPointF> mIt(mIntermediate);
    while(mIt.hasNext()){
        mPoints.append(mIt.next());
    }
    mPoints.append(mLineEnd.p2());
    mPoints.append(mLineEnd.p1());

    return mPoints;
}

void QLogicCircuitShapeConnector::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if (startConnectionPoint() && startConnectionPoint()->parentShape()->property("state").toBool()){
        painter->setPen(Qt::green);
    }
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
    for (int iPoints = 1; iPoints < points.size() - 1; iPoints++){
        painter->drawRect(points.at(iPoints).x() - 2, points.at(iPoints).y() - 2, 4, 4);
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

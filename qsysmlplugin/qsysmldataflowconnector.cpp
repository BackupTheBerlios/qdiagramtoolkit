#include "stdafx.h"
#include "qsysmldataflowconnector.h"
#include "qsysmlplugin.h"

QSysMLDataFlowConnector::QSysMLDataFlowConnector(const QVariantMap & properties) :
    QAbstractDiagramShapeConnector(QSysMLPlugin::staticName(), "default", properties)
{
	restoreProperties(properties);
}

QRectF QSysMLDataFlowConnector::boundingRect() const
{
    return m_boundingRect.adjusted(-3, -3, 3, 3);
}

QList<QPointF> QSysMLDataFlowConnector::breakPoints() const
{
    return defaultConnector();
}

void QSysMLDataFlowConnector::calcBoundingRect()
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

bool QSysMLDataFlowConnector::canConnect(QAbstractDiagramShapeConnectionPoint* startPoint, QAbstractDiagramShapeConnectionPoint* endPoint) const
{
    if (startPoint == 0 || endPoint == 0){
        return false;
    }
    if (startPoint == endPoint){
        return false;
    }
    //
	if (startPoint->id() == "controlIn" && endPoint->id() == "controlOut"){
		return true;
	}
	if (endPoint->id() == "controlIn" && startPoint->id() == "controlOut"){
		return true;
	}
    return false;
}

QList<QPointF> QSysMLDataFlowConnector::defaultConnector() const
{
    QList<QPointF> points;
    QList<QPointF> sections;
    QLineF line(startPos(), endPos());
    QLineF lineEnd;
    QLineF lineStart;

    line = QLineF(startPos(), endPos());

    lineStart.setP1(startPos());
    lineEnd.setP1(endPos());

    //switch(orientationAtStart()){
    //case QDiagramToolkit::East:
    //    if (line.dx() < 20){
    //        lineStart.setP2(startPos() + QPointF(20, 0));

    //        lineEnd.setP2(lineEnd.p1() - QPointF(20, 0));
    //        sections << QPointF(lineStart.p2().x(), lineStart.p1().y() + line.dy() / 2)
    //                      << QPointF(lineEnd.p2().x(), lineEnd.p2().y() - line.dy() / 2);
    //    } else {
    //        lineStart.setP2(lineStart.p1() + QPointF(line.dx() / 2 , 0));

    //        lineEnd.setP2(QPointF(lineStart.p2().x(), lineEnd.p1().y()));
    //    }
    //    break;
    //case QDiagramToolkit::West:
    //    if (line.dx() > -20){
    //        lineStart.setP2(startPos() - QPointF(20, 0));

    //        lineEnd.setP2(lineEnd.p1() + QPointF(20, 0));
    //        sections << QPointF(lineStart.p2().x(), lineStart.p1().y() + line.dy() / 2)
    //                      << QPointF(lineEnd.p2().x(), lineEnd.p2().y() - line.dy() / 2);
    //    } else {
    //        lineStart.setP2(lineStart.p1() + QPointF(line.dx() / 2 , 0));

    //        lineEnd.setP2(QPointF(lineStart.p2().x(), lineEnd.p1().y()));
    //    }
    //    break;
    //default:
    //    break;
    //}


    //points.append(lineStart.p1());
    //points.append(lineStart.p2());
    //QListIterator<QPointF> it(sections);
    //while(it.hasNext()){
    //    points.append(it.next());
    //}
    //points.append(lineEnd.p2());
    //points.append(lineEnd.p1());
    points << startPos() << endPos();

	return points;
}

void QSysMLDataFlowConnector::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

	painter->setPen(Qt::SolidLine);
    drawPolyline(painter, m_breakPoints);
    if (isSelected()){
        paintBreakPoints(painter, m_breakPoints);
    }
}

void QSysMLDataFlowConnector::paintBreakPoints(QPainter * painter, const QList<QPointF> & points)
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

void QSysMLDataFlowConnector::updatePosition()
{
    prepareGeometryChange();
    m_breakPoints = breakPoints();
    calcBoundingRect();
    update();
}


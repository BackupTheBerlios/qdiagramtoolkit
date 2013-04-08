/******************************************************************************
** Copyright (C) 2011 Martin Hoppe martin@2x2hoppe.de
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
#include "qdiagramshapeconnector.h"

#include <qabstractdiagramshapeconnectionpoint.h>

#include <qdiagram.h>
#include <qdiagramshape.h>

const qreal Pi = 3.14;

#include <math.h>
#include <cmath>

#define arrowSize 10

//QDiagramShapeConnector::QDiagramShapeConnector(const QString & uuid) :
//    QAbstractDiagramShapeConnector(uuid)
//{
//    setFlag(QGraphicsItem::ItemIsSelectable);
//}

//QDiagramShapeConnector::QDiagramShapeConnector(const QString & style, QAbstractDiagramShapeConnectionPoint* from) :
//    QAbstractDiagramShapeConnector(style, from)
//{
//    setProperty("style", style);
//    cTempEndPos = from->parentShape()->pos();
//}

//QDiagramShapeConnector::QDiagramShapeConnector(const QString & uuid, const QString & style, const QString & from, const QString & to) :
//    QAbstractDiagramShapeConnector(uuid)
//{
//    setProperty("fromUuid", from);
//    setProperty("toUuid", to);

//    setFlag(QGraphicsItem::ItemIsSelectable);
//}


//QDiagramShapeConnector::QDiagramShapeConnector(const QString & uuid, const QString & style, QAbstractDiagramShapeConnectionPoint* from, QAbstractDiagramShapeConnectionPoint* to) :
//    QAbstractDiagramShapeConnector(uuid, style, from, to)
//{
//}

QDiagramShapeConnector::~QDiagramShapeConnector()
{

}

QList<QPointF> QDiagramShapeConnector::breakPoints() const
{
    if (property("style").toString() == "line"){
        return lineConnector();
    } else if (property("style").toString() == "straight"){
        return straightConnector();
    }
    return defaultConnector();
}

void QDiagramShapeConnector::calcBoundingRect()
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

QList<QPointF> QDiagramShapeConnector::defaultConnector() const
{
    QList<QPointF> mPoints;
    QLineF mLineEnd;
    QLineF mLineStart;
    QLineF mLine;
    QList<QPointF> mIntermediate;

    mLineStart.setP1(QPointF(connectionPointAtEnd()->scenePos() + connectionPointAtEnd()->boundingRect().center()));

    mLine.setP1(mLineStart.p1());
    QDiagramToolkit::ConnectionPointOrientation mOrientation;    

    if (connectionPointAtStart() == 0){
        mOrientation = m_tempOrientationAtEnd;
        mLineEnd.setP1(m_tempEndPos);
        mLine.setP2(m_tempEndPos);
    } else {
        mOrientation = connectionPointAtStart()->orientation();
        mLineEnd.setP1(connectionPointAtStart()->scenePos() + connectionPointAtStart()->boundingRect().center());
        mLine.setP2(mLineEnd.p1());
    }
    if (mOrientation == QDiagramToolkit::ConnectionPointOrientationInvalid){

        qreal mStartDeltaX = 0;
        qreal mStartDeltaY = 0;

        qreal mEndDeltaX = 0;
        qreal mEndDeltaY = 0;

        switch(connectionPointAtEnd()->orientation()){
        case QDiagramToolkit::North:
            if (mLine.dy() < -40){
                mStartDeltaY =  mLine.dy() / 2;
                mEndDeltaY = mStartDeltaY;
            } else {
                mStartDeltaY = -20;
                mEndDeltaY = mLine.dy() + 20;
            }
            break;
        case QDiagramToolkit::East:
            if (mLine.dx() > 40){
                mStartDeltaX = mLine.dx() / 2;
                mEndDeltaX = mStartDeltaX;
            } else {
                mStartDeltaX = 20;
                mEndDeltaX = mLine.dx() - 20;
            }
            break;
        case QDiagramToolkit::South:
            if (mLine.dy() > 40){
                mStartDeltaY =  mLine.dy() / 2;
                mEndDeltaY = mStartDeltaY;
            } else {
                mStartDeltaY = 20;
                mEndDeltaY = mLine.dy() - 20;
            }
            break;
        case QDiagramToolkit::West:
            if (mLine.dx() < -40){
                mStartDeltaX = mLine.dx() / 2;
                mEndDeltaX = mStartDeltaX;
            } else {
                mStartDeltaX = -20;
                mEndDeltaX = mLine.dx() + 20;
            }
            break;
        }
        //        qDebug() << mLine.dx() << mStartDeltaX  << mEndDeltaX;
        mLineStart.setP2(QPointF(mLineStart.p1().x() + mStartDeltaX, mLineStart.p1().y() + mStartDeltaY));
        mLineEnd.setP2(QPointF(mLineEnd.p1().x() - mEndDeltaX, mLineEnd.p1().y() - mEndDeltaY));
    }  else if (connectionPointAtEnd()->orientation() == QDiagramToolkit::South){
        if (mOrientation == QDiagramToolkit::West){
            if (mLine.dx() < 20){
                mLineEnd.setP2(QPointF(mLineEnd.p1().x() - 20, mLineEnd.p1().y()));
                mLineStart.setP2(QPointF(mLineStart.p1().x(), mLineStart.p1().y() + mLine.dy() / 2));
                mIntermediate.append(QPointF(mLineEnd.p2().x(), mLineStart.p2().y()));
            } else {
                mLineStart.setP2(QPointF(mLineStart.p1().x(), mLineEnd.p1().y()));
                mLineEnd.setP2(mLineStart.p2());
            }
        } else if ( mOrientation == QDiagramToolkit::South){
            mLineEnd.setP2(QPointF(mLineEnd.p1().x(), mLineEnd.p1().y() + 20));
            mLineStart.setP2(QPointF(mLineStart.p1().x(), mLineEnd.p2().y()));
        }
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

QList<QPointF> QDiagramShapeConnector::lineConnector() const
{
    QList<QPointF> mPoints;
    QLineF mLineStart;
    QLineF mLineEnd;
    switch(connectionPointAtEnd()->orientation())
    {
    case QDiagramToolkit::North:
        mLineStart.setP1(QPointF(connectionPointAtEnd()->scenePos() + connectionPointAtEnd()->boundingRect().center()));
        mLineStart.setP2(QPointF(mLineStart.p1().x(), mLineStart.p1().y() - 20));
        break;
    case QDiagramToolkit::East:
        mLineStart.setP1(QPointF(connectionPointAtEnd()->scenePos() + connectionPointAtEnd()->boundingRect().center()));
        mLineStart.setP2(QPointF(mLineStart.p1().x()+ 20, mLineStart.p1().y()));
        break;
    case QDiagramToolkit::South:
        mLineStart.setP1(QPointF(connectionPointAtEnd()->scenePos() + connectionPointAtEnd()->boundingRect().center()));
        mLineStart.setP2(QPointF(mLineStart.p1().x(), mLineStart.p1().y() + 20));
        break;
    case QDiagramToolkit::West:
        mLineStart.setP1(QPointF(connectionPointAtEnd()->scenePos() + connectionPointAtEnd()->boundingRect().center()));
        mLineStart.setP2(QPointF(mLineStart.p1().x() - 20, mLineStart.p1().y()));
        break;
    default:
        mLineStart.setP1(QPointF(connectionPointAtEnd()->scenePos() + connectionPointAtEnd()->boundingRect().center()));
        mLineStart.setP2(QPointF(mLineStart.p1().x(), mLineStart.p1().y() - 20));
    }
    mPoints.append(intersectPoint(connectionPointAtEnd()->parentShape(), mLineStart));
    mPoints.append(mLineStart.p2());

    QDiagramToolkit::ConnectionPointOrientation mOrientation;
    if (connectionPointAtStart() == 0){
        mOrientation = m_tempOrientationAtEnd;
        mLineEnd.setP1(m_tempEndPos);
    } else {
        mOrientation = connectionPointAtStart()->orientation();
        mLineEnd.setP1(QPointF(connectionPointAtStart()->scenePos() + connectionPointAtStart()->boundingRect().center()));
    }
    switch(mOrientation)
    {
    case QDiagramToolkit::North:
        mLineEnd.setP2(QPointF(mLineEnd.p1().x(), mLineEnd.p1().y() - 20));
        break;
    case QDiagramToolkit::East:
        mLineEnd.setP2(QPointF(mLineEnd.p1().x() + 20, mLineEnd.p1().y()));
        break;
    case QDiagramToolkit::South:
        mLineEnd.setP2(QPointF(mLineEnd.p1().x(), mLineEnd.p1().y() + 20));
        break;
    case QDiagramToolkit::West:
        mLineEnd.setP2(QPointF(mLineEnd.p1().x() - 20, mLineEnd.p1().y()));
        break;
    default:
        mLineEnd.setP2(m_tempEndPos);
    }
    mPoints.append(mLineEnd.p2());
    if (connectionPointAtStart() == 0){
        mPoints.append(mLineEnd.p1());
    } else {
        mPoints.append(intersectPoint(connectionPointAtStart()->scenePos(), connectionPointAtStart()->shape().toFillPolygon(), mLineEnd));
    }

    return mPoints;
}

void QDiagramShapeConnector::paintArrows(const QLineF & line, QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    m_arrowFrom.clear();
    m_arrowTo.clear();

    double angle = ::acos(line.dx() / line.length());
    angle = Pi / 2 - angle + Pi / 2;
    if (line.dy() < 0){
        angle = (Pi * 2) - angle;
    }

    if (property("arrows").toString() == "both"){
        qreal mAngleFrom = angle + Pi;
        QPointF arrowP1 = line.p1() + QPointF(sin(mAngleFrom + Pi / 3) * arrowSize,
                                               cos(mAngleFrom + Pi / 3) * arrowSize);
        QPointF arrowP2 = line.p1() + QPointF(sin(mAngleFrom + Pi - Pi / 3) * arrowSize,
                                               cos(mAngleFrom + Pi - Pi / 3) * arrowSize);

        if (property("arrowStyleStart").toString() == "arrow"){
            m_arrowFrom << line.p1() << arrowP1 << arrowP2;
        } else if (property("arrowStyleStart").toString() == "line"){
            m_arrowFrom << line.p1() << arrowP1 << line.p1() << arrowP2 << line.p1();
        }
        painter->drawPolygon(m_arrowFrom);
    }
    if (property("arrows").toString() == "both" || property("arrows").toString() == "end"){
        QPointF arrowP1 = line.p2() + QPointF(sin(angle + Pi / 3) * arrowSize,
                                               cos(angle + Pi / 3) * arrowSize);
        QPointF arrowP2 = line.p2() + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
                                               cos(angle + Pi - Pi / 3) * arrowSize);

        if (property("arrowStyleEnd").toString() == "arrow"){
            m_arrowTo << line.p2() << arrowP1 << arrowP2;
        } else if (property("arrowStyleEnd").toString() == "line"){
            m_arrowTo << line.p2() << arrowP1 << line.p2() << arrowP2 << line.p2();
        }
        painter->drawPolygon(m_arrowTo);
    }
}

QRectF QDiagramShapeConnector::boundingRect() const
{
    return m_boundingRect.adjusted(-3, -3, 3, 3);
    return shape().boundingRect();
    if (property("style").toString() == "default"){
    } else {
        qreal extra = (/*pen().width() +*/ 20) / 2.0;
        const qreal x1 = m_line.p1().x();
        const qreal x2 = m_line.p2().x();
        const qreal y1 = m_line.p1().y();
        const qreal y2 = m_line.p2().y();
        qreal lx = qMin(x1, x2);
        qreal rx = qMax(x1, x2);
        qreal ty = qMin(y1, y2);
        qreal by = qMax(y1, y2);
        return QRectF(lx, ty, rx - lx, by - ty).normalized().adjusted(-extra, -extra, extra, extra);
    }
}

void QDiagramShapeConnector::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    drawPolyline(painter, m_breakPoints);
    if (isSelected()){
        paintBreakPoints(painter, m_breakPoints);
    }
}

void QDiagramShapeConnector::paintBreakPoints(QPainter * painter, const QList<QPointF> & points)
{
    if (points.size() < 3){
        return;
    }
    for (int iPoints = 1; iPoints < points.size() - 1; iPoints++){
        painter->drawRect(points.at(iPoints).x() - 2, points.at(iPoints).y() - 2, 4, 4);
    }
}


void QDiagramShapeConnector::rebuildConnection()
{
//    if (from()){

//    }
//    if (to()){

//    }
    if (property("from").toString().isEmpty() || property("to").toString().isEmpty()){
        return;
    }
    // @todo
//    setFrom(diagram()->item(property("from").toString()));
//    setTo(diagram()->item(property("to").toString()));
//    if (from() == 0|| to() == 0){
//        return;
//    }
//    from()->append(this, QDiagramGraphicsItem::OutgoingConnection);
//    to()->append(this, QDiagramGraphicsItem::IncomingConnection);

    updatePosition();
}

QPainterPath QDiagramShapeConnector::shape() const
{
    QPainterPath mPath;
    if (!m_breakPoints.isEmpty()){
        mPath.moveTo(m_breakPoints.first());
        for (int iPoints = 1; iPoints < m_breakPoints.size(); iPoints++){
            mPath.lineTo(m_breakPoints.at(iPoints));
        }
    }
    return mPath;
}

QPainterPath QDiagramShapeConnector::shapeDefaultConnector() const
{
    QPainterPath mPath;
    QLineF mLineEnd;
    QLineF mLineStart;
    QPointF mStart;
    QPointF mEnd;

    if (connectionPointAtEnd() == 0 && connectionPointAtStart() == 0){
        return mPath;
    }
    if (connectionPointAtEnd() != 0 && connectionPointAtStart() == 0){
        mEnd = m_tempEndPos;
    }
    mStart = connectionPointAtEnd()->scenePos();
    if (connectionPointAtEnd() != 0 && connectionPointAtStart() != 0){
        mEnd = connectionPointAtStart()->scenePos();

    }

    QLineF mTempLine(mStart, mEnd);

    if (mTempLine.dy() < 20){
        if (connectionPointAtEnd()->orientation() == QDiagramToolkit::North){
        } else if (connectionPointAtEnd()->orientation() == QDiagramToolkit::East){
        } else if (connectionPointAtEnd()->orientation() == QDiagramToolkit::South){
            mLineStart = QLineF(mStart, QPointF(mStart.x(), mStart.y() + 20));
            mLineEnd = QLineF(mEnd, QPointF(mEnd.x(), mEnd.y() - 20));

            mPath.moveTo(intersectPoint(connectionPointAtEnd()->parentShape(), mLineStart));
            mPath.lineTo(mLineStart.p2());
            if (mEnd.x() > mStart.x()){
                mPath.lineTo(mLineStart.p2().x() + abs(mTempLine.dx() /2), mLineStart.p2().y());
                mPath.lineTo(mLineStart.p2().x() + abs(mTempLine.dx() /2), mLineEnd.p2().y());
            } else {
                mPath.lineTo(mLineStart.p2().x() - abs(mTempLine.dx() /2), mLineStart.p2().y());
                mPath.lineTo(mLineStart.p2().x() - abs(mTempLine.dx() /2), mLineEnd.p2().y());
            }
            mPath.lineTo(mLineEnd.p2());

            if (connectionPointAtStart() == 0){
                mPath.lineTo(mEnd);
            } else {
                mPath.lineTo(intersectPoint(connectionPointAtStart()->parentShape(), mLineEnd));
            }

//            if (to() == 0){
//                mPath.moveTo(mEnd);
//            } else {
//                mPath.moveTo(intersectPoint(to()->shape(), mLineEnd));
//            }
//            mPath.lineTo(mLineEnd.p2());


        } else if (connectionPointAtEnd()->orientation() == QDiagramToolkit::West){
        }

    } else {
        if (connectionPointAtEnd()->orientation() == QDiagramToolkit::North){
            mLineStart = QLineF(mStart, QPointF(mStart.x(), mStart.y() - mTempLine.dy() / 2));
            mLineEnd = QLineF(mEnd, QPointF(mEnd.x(), mEnd.y() + mTempLine.dy() / 2));
        } else if (connectionPointAtEnd()->orientation() == QDiagramToolkit::East){
            mLineStart = QLineF(mStart, QPointF(mStart.x() + 10, mStart.y()));
            mLineEnd = QLineF(mEnd, QPointF(mEnd.x() - 10, mEnd.y()));
        } else if (connectionPointAtEnd()->orientation() == QDiagramToolkit::South){
            mLineStart = QLineF(mStart, QPointF(mStart.x(), mStart.y() + mTempLine.dy() / 2));
            mLineEnd = QLineF(mEnd, QPointF(mEnd.x(), mEnd.y() - mTempLine.dy() / 2));
        } else if (connectionPointAtEnd()->orientation() == QDiagramToolkit::West){
            mLineStart = QLineF(mStart, QPointF(mStart.x() - 10, mStart.y()));
            mLineEnd = QLineF(mEnd, QPointF(mEnd.x() + 10, mEnd.y()));
        }

        mPath.moveTo(intersectPoint(connectionPointAtEnd()->parentShape(), mLineStart));
        mPath.lineTo(mLineStart.p2());
        if (connectionPointAtStart() == 0){
            mPath.moveTo(mEnd);
        } else {
            mPath.moveTo(intersectPoint(connectionPointAtStart()->parentShape(), mLineEnd));
        }
        mPath.lineTo(mLineEnd.p2());
        mPath.lineTo(mLineStart.p2());
    }
    return mPath;
}

QPainterPath QDiagramShapeConnector::shapeLineConnector() const
{
    QPainterPath mPath;
    QLineF mLineEnd;
    QLineF mLineStart;
    QPointF mStart;
    QPointF mEnd;

    if (connectionPointAtEnd() == 0 && connectionPointAtStart() == 0){
        return mPath;
    }
    mStart = connectionPointAtEnd()->scenePos();
    if (connectionPointAtEnd() != 0 && connectionPointAtStart() == 0){
        mEnd = m_tempEndPos;
    }
    if (connectionPointAtEnd() != 0 && connectionPointAtStart() != 0){
        mEnd = connectionPointAtStart()->scenePos();
    }
    if (connectionPointAtEnd()->orientation() == QDiagramToolkit::North){
        mLineStart = QLineF(mStart, QPointF(mStart.x(), mStart.y() - 10));
        mLineEnd = QLineF(mEnd, QPointF(mEnd.x(), mEnd.y() + 10));
    } else if (connectionPointAtEnd()->orientation() == QDiagramToolkit::East){
        mLineStart = QLineF(mStart, QPointF(mStart.x() + 10, mStart.y()));
        mLineEnd = QLineF(mEnd, QPointF(mEnd.x() - 10, mEnd.y()));
    } else if (connectionPointAtEnd()->orientation() == QDiagramToolkit::South){
        mLineStart = QLineF(mStart, QPointF(mStart.x(), mStart.y() + 10));
        mLineEnd = QLineF(mEnd, QPointF(mEnd.x(), mEnd.y() - 10));
    } else if (connectionPointAtEnd()->orientation() == QDiagramToolkit::West){
        mLineStart = QLineF(mStart, QPointF(mStart.x() - 10, mStart.y()));
        mLineEnd = QLineF(mEnd, QPointF(mEnd.x() + 10, mEnd.y()));
    }

    mPath.moveTo(intersectPoint(connectionPointAtEnd()->parentShape(), mLineStart));
    mPath.lineTo(mLineStart.p2());
    if (connectionPointAtStart() == 0){
        mPath.moveTo(mEnd);
    } else {
        mPath.moveTo(intersectPoint(connectionPointAtStart()->parentShape(), mLineEnd));
    }
    mPath.lineTo(mLineEnd.p2());
    mPath.lineTo(mLineStart.p2());
    return mPath;
}

QPainterPath QDiagramShapeConnector::shapeStraightConnector() const
{
    QPainterPath mPath;
    QLineF mLine;
    if (connectionPointAtEnd() != 0 && connectionPointAtStart() == 0){
        mPath.moveTo(connectionPointAtEnd()->scenePos());
        mPath.lineTo(m_tempEndPos);
//        mLine.setP1(from()->scenePos());
//        mLine.setP2(cTempToPos);
    }
    if (connectionPointAtEnd() != 0 && connectionPointAtStart() != 0){
        mPath.moveTo(intersectPoint(connectionPointAtEnd()->pos(), connectionPointAtEnd()->shape().toFillPolygon(), m_line));
        mPath.lineTo(intersectPoint(connectionPointAtStart()->pos(), connectionPointAtStart()->shape().toFillPolygon(), m_line));
//        mLine.setP1(intersectPoint(from()->shape()->pos(), from()->shape()->shape().toFillPolygon(), cLine));
//        mLine.setP2(intersectPoint(to()->shape()->pos(), to()->shape()->shape().toFillPolygon(), cLine));
    }
    return mPath;
}

QList<QPointF> QDiagramShapeConnector::straightConnector() const
{
    QList<QPointF> mPoints;
    QLineF mLine;
    mLine.setP1(connectionPointAtEnd()->scenePos() + connectionPointAtEnd()->boundingRect().center());
    if (connectionPointAtStart() == 0){
        mLine.setP2(m_tempEndPos);
        mPoints.append(m_tempEndPos);
    } else {
        mLine.setP2(connectionPointAtStart()->scenePos());
        mPoints.append(intersectPoint(connectionPointAtStart()->scenePos(), connectionPointAtStart()->shape().toFillPolygon(), mLine));
    }
    mPoints.append(intersectPoint(connectionPointAtEnd()->scenePos(), connectionPointAtEnd()->shape().toFillPolygon(), mLine));
    return mPoints;
}

void QDiagramShapeConnector::temporaryPosition(const QPointF & pos, QDiagramToolkit::ConnectionPointOrientation orientation, const QRectF & rect)
{
//    if (property("style").toString() == "straight"){
        m_tempEndPos = pos;
        m_tempOrientationAtEnd = orientation;
        m_tempToRect = rect;
        updatePosition();
//    }
}

void QDiagramShapeConnector::updatePosition()
{
    if (connectionPointAtEnd() == 0 && connectionPointAtStart() == 0){
        return;
    }
    prepareGeometryChange();
    m_breakPoints = breakPoints();
    calcBoundingRect();
    if (connectionPointAtEnd() != 0 && connectionPointAtStart() == 0){
        m_line = QLineF(connectionPointAtEnd()->scenePos(), m_tempEndPos);
    }
    if (connectionPointAtEnd() != 0 && connectionPointAtStart() != 0){
//        cLine = QLineF(from()->pos() + from()->boundingRect().center(), to()->pos() + to()->boundingRect().center());
        m_line = QLineF(connectionPointAtEnd()->scenePos(), connectionPointAtStart()->scenePos());
    }
    update();
}

#include "stdafx.h"
#include "qstandardshapeconnector.h"

#include <qdiagramendoflinestyle.h>
#define PI 3.14159265

QStandardShapeConnector::QStandardShapeConnector(const QVariantMap &properties) :
    QAbstractDiagramShapeConnector(properties)
{
    setFlag(QGraphicsItem::ItemIsSelectable);
    addProperty("lineStyle", QDiagramGraphicsItemMetaProperty::LineStyle, false);
    addProperty("lineStart", QDiagramGraphicsItemMetaProperty::EndOfLineStyle, false);
    addProperty("lineEnd", QDiagramGraphicsItemMetaProperty::EndOfLineStyle, false);
    addProperty("text", QDiagramGraphicsItemMetaProperty::String, false);
}

//QStandardShapeConnector::QStandardShapeConnector(const QString &uuid, const QString &style) :
//    QAbstractDiagramShapeConnector(uuid, style)
//{
//    setFlag(QGraphicsItem::ItemIsSelectable);
//    addProperty("lineStyle", QDiagramGraphicsItemMetaProperty::LineStyle, false);
//    addProperty("lineStart", QDiagramGraphicsItemMetaProperty::EndOfLineStyle, false);
//    addProperty("lineEnd", QDiagramGraphicsItemMetaProperty::EndOfLineStyle, false);
//    addProperty("text", QDiagramGraphicsItemMetaProperty::String, false);
//}

QStandardShapeConnector::~QStandardShapeConnector()
{
}

QRectF QStandardShapeConnector::boundingRect() const
{
    QRectF b(m_boundingRect);
    qreal extra = (pen().width() + 20) / 2.0;
    return b.adjusted(-extra, -extra, extra, extra);

//    return m_boundingRect.adjusted(-3, -3, 3, 3);
}

QList<QPointF> QStandardShapeConnector::breakPoints() const
{
    return m_breakPoints;
}

void QStandardShapeConnector::calcBoundingRect()
{
    QPainterPath path;
    if (!m_breakPoints.isEmpty()){
        path.moveTo(m_breakPoints.first());
        for (int iPoints = 1; iPoints < m_breakPoints.size(); iPoints++){
            path.lineTo(m_breakPoints.at(iPoints));
        }
    }
    m_boundingRect = path.boundingRect();
}

QList<QPointF> QStandardShapeConnector::defaultConnector() const
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

QList<QPointF> QStandardShapeConnector::lineConnector() const
{
    QList<QPointF> p;
    p << startPos();
    p << endPos();
    return p;
}

bool QStandardShapeConnector::canConnect(QAbstractDiagramShapeConnectionPoint *start, QAbstractDiagramShapeConnectionPoint *end) const
{
    return true;
}

void QStandardShapeConnector::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    drawPolyline(painter, m_breakPoints);
    if (isSelected()){
        paintBreakPoints(painter, m_breakPoints);
    }
    paintEndOfLine(painter, m_breakPoints.last(), 0);
}

void QStandardShapeConnector::paintEndOfLine(QPainter* painter, const QPointF & pos, double angle)
{
    QDiagramEndOfLineStyle style = qvariant_cast<QDiagramEndOfLineStyle>(property("lineStart"));
    if (!style.isValid() || style.name() == "none"){
        return;
    }
    double arrowSize = 20;
//    double angle = ::acos(mLine.dx() / mLine.length());
//    if (line().dy() >= 0){
//        angle = (Pi * 2) - angle;
//    }

//        QPointF arrowP1 = mLine.p1() + QPointF(sin(angle + Pi / 3) * arrowSize,
//                                               cos(angle + Pi / 3) * arrowSize);
//        QPointF arrowP2 = mLine.p1() + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
//                                               cos(angle + Pi - Pi / 3) * arrowSize);
    QPointF mArrowP1 = pos - QPointF(sin(angle + PI / 3) * arrowSize,
                                           cos(angle + PI / 3) * arrowSize);
    QPointF mArrowP2 = pos - QPointF(sin(angle + PI - PI / 3) * arrowSize,
                                           cos(angle + PI - PI / 3) * arrowSize);

    QPolygonF arrowHead;
    arrowHead.clear();
       arrowHead << pos << mArrowP1 << mArrowP2;
        painter->drawLine(pos, mArrowP1);
        painter->drawLine(pos, mArrowP2);
}

void QStandardShapeConnector::paintBreakPoints(QPainter *painter, const QList<QPointF> &points)
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

void QStandardShapeConnector::updatePosition()
{
    prepareGeometryChange();
    if (property("style") == "line"){
        m_breakPoints = lineConnector();
    } else {
        m_breakPoints = defaultConnector();
    }
    calcBoundingRect();
    update();
}

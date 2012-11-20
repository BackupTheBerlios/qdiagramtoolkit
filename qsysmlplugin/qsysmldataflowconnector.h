#ifndef QSYSMLDATAFLOWCONNECTOR_H
#define QSYSMLDATAFLOWCONNECTOR_H

#include <qabstractdiagramshapeconnector.h>

class QSysMLDataFlowConnector : public QAbstractDiagramShapeConnector
{
public:
	QSysMLDataFlowConnector(const QVariantMap & properties);
    /**
      * Returns the outer bounds of the connector as a rectangle.
      */
    QRectF boundingRect() const;

    bool canConnect(QAbstractDiagramShapeConnectionPoint* start, QAbstractDiagramShapeConnectionPoint* end) const;

    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);

    void updatePosition();
private:
    QList<QPointF> breakPoints() const;

    void calcBoundingRect();

    QList<QPointF> defaultConnector() const;

    void paintBreakPoints(QPainter * painter, const QList<QPointF> & points);

    QRectF m_boundingRect;
    QList<QPointF> m_breakPoints;
    QLineF m_line;
    QPointF m_tempEndPos;
    QDiagramToolkit::ConnectionPointOrientation m_tempOrientationAtEnd;
    QRectF m_tempToRect;
};

#endif // QSYSMLDATAFLOWCONNECTOR_H

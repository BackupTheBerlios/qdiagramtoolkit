#ifndef QSYSMLCONTROLFLOWCONNECTOR_H
#define QSYSMLCONTROLFLOWCONNECTOR_H

#include "qsysmlflowconnector.h"

class QSysMLControlFlowConnector : public QSysMLFlowConnector
{
public:
	QSysMLControlFlowConnector(const QVariantMap & properties);
    /**
      * Returns the outer bounds of the connector as a rectangle.
      */
    QRectF boundingRect() const;

    bool canConnect(QAbstractDiagramShapeConnectionPoint* start, QAbstractDiagramShapeConnectionPoint* end) const;

	bool canStartWith(QAbstractDiagramShapeConnectionPoint* cp) const;

    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);

    void updatePosition();
private:
    QList<QPointF> breakPoints() const;

    void calcBoundingRect();

	QPainterPath eastTo() const;

	QPainterPath westTo() const;

	QPainterPath southTo() const;


    QList<QPointF> defaultConnector() const;

    void paintBreakPoints(QPainter * painter, const QList<QPointF> & points);
	void paintEndOfLine(QPainter* painter, const QPointF & pos, double angle);

    QRectF m_boundingRect;
    QList<QPointF> m_breakPoints;
    QLineF m_line;
	QPainterPath m_path;
    QPointF m_tempEndPos;
    QDiagramToolkit::ConnectionPointOrientation m_tempOrientationAtEnd;
    QRectF m_tempToRect;
};

#endif // QSYSMLCONTROLCONNECTOR_H

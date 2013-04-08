#ifndef QSYSMLCONTROLFLOWCONNECTOR_H
#define QSYSMLCONTROLFLOWCONNECTOR_H

#include <qdiagramconnectorpolyline.h>

#include "qsysmlflowconnector.h"

class QSysMLControlFlowConnector : public QSysMLFlowConnector
{
public:
	QDIAGRAM_DECLARE_CONNECTOR(QSysMLControlFlowConnector)

	QSysMLControlFlowConnector(const QVariantMap & properties);
    /**
      * Returns the outer bounds of the connector as a rectangle.
      */
    QRectF boundingRect() const;

	QList<QPointF> breakPoints() const;

    bool canConnect(QAbstractDiagramShapeConnectionPoint* start, QAbstractDiagramShapeConnectionPoint* end) const;

	bool canStartWith(QAbstractDiagramShapeConnectionPoint* cp) const;

    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);

	QPainterPath shape() const;

    void updatePosition();
protected:
	void updateTextItemPosition();
private:

    void calcBoundingRect();

	QPainterPath eastTo() const;

	QPainterPath westTo() const;

	QPainterPath southTo() const;

	void calcNextSection(int previous);

    QList<QPointF> defaultConnector() const;

	QList<QPointF> btlConnector() const;

	QList<QPointF> btrConnector() const;

	QList<QPointF> tlbConnector() const;

	QList<QPointF> trbConnector() const;

    void paintBreakPoints(QPainter * painter, const QList<QPointF> & points);
	void paintEndOfLine(QPainter* painter, const QPointF & pos, double angle);

	QDiagramConnectorPolyline m_polyline;

    QRectF m_boundingRect;
    QList<QPointF> m_breakPoints;
    QLineF m_line;
	QPainterPath m_path;
	QList<QLineF> m_sections;
    QPointF m_tempEndPos;
    QDiagramToolkit::ConnectionPointOrientation m_tempOrientationAtEnd;
    QRectF m_tempToRect;
};

#endif // QSYSMLCONTROLCONNECTOR_H

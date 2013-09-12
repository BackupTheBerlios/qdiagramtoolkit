#ifndef QLOGICCIRCUITFLIPFLOPSHAPE_H
#define QLOGICCIRCUITFLIPFLOPSHAPE_H

#include <qabstractdiagramshape.h>

class QLogicCircuitFlipFlopShape : public QAbstractDiagramShape
{
public:
	QDIAGRAM_DECLARE_SHAPE(QLogicCircuitFlipFlopShape, FlipFlop);

	QLogicCircuitFlipFlopShape(QGraphicsItem* parent = 0);
    QLogicCircuitFlipFlopShape(const QMap<QString,QVariant> & properties, QGraphicsItem* parent = 0);

    QRectF boundingRect() const;
	/**
	 * Returns the default properties the for shape specified by the given @p id;
	 */
	static QVariantMap defaultProperties(const QString & id);
	/**
	 * Returns the shape's hot spot.
	 */
	static QPointF hotSpot(const QString & id);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    QPainterPath shape() const;
};

#endif // QLOGICCIRCUITFLIPFLOPSHAPE_H

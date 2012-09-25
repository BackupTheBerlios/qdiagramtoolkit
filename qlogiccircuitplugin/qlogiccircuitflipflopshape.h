#ifndef QLOGICCIRCUITFLIPFLOPSHAPE_H
#define QLOGICCIRCUITFLIPFLOPSHAPE_H

#include <qabstractdiagramshape.h>

class QLogicCircuitFlipFlopShape : public QAbstractDiagramShape
{
public:
    QLogicCircuitFlipFlopShape(QGraphicsItem* parent = 0);
    QLogicCircuitFlipFlopShape(const QMap<QString,QVariant> & properties, QGraphicsItem* parent = 0);

    QRectF boundingRect() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    QPainterPath shape() const;
};

#endif // QLOGICCIRCUITFLIPFLOPSHAPE_H

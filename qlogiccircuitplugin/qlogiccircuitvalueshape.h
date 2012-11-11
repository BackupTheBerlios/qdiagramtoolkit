#ifndef QLOGICCIRCUITVALUESHAPE_H
#define QLOGICCIRCUITVALUESHAPE_H

#include <qabstractdiagramshape.h>

class QLogicCircuitValueShape : public QAbstractDiagramShape
{
public:
    QLogicCircuitValueShape(QGraphicsItem* parent = 0);

    QLogicCircuitValueShape(const QMap<QString,QVariant> & properties, QGraphicsItem* parent = 0);
    /**
      *
      */
    //explicit QLogicCircuitInputShape(const QString & uuid, const QString & type, const QString & plugin, QGraphicsItem* parent = 0);

    QRectF boundingRect() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    /**
      * Returns the shape of this item as a QPainterPath in local coordinates.
      */
    QPainterPath shape() const;
};

#endif // QLOGICCIRCUITVALUESHAPE_H

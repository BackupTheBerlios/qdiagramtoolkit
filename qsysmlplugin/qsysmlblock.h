#ifndef QSYSMLBLOCK_H
#define QSYSMLBLOCK_H

#include <qabstractdiagramshape.h>

class QSysMLBlock : public QAbstractDiagramShape
{
public:
    QSysMLBlock(const QMap<QString,QVariant> & properties, QGraphicsItem* parent = 0);

    QRectF boundingRect() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    /**
      * Returns the shape of this item as a QPainterPath in local coordinates.
      */
    QPainterPath shape() const;

};

#endif // QSYSMLBLOCK_H

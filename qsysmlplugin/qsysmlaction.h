#ifndef QSYSMLACTION_H
#define QSYSMLACTION_H

#include <qabstractdiagramshape.h>

class QSysMLAction : public QAbstractDiagramShape
{
public:
    QSysMLAction(const QMap<QString,QVariant> & properties, QGraphicsItem* parent = 0);

    QRectF boundingRect() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    /**
      * Returns the shape of this item as a QPainterPath in local coordinates.
      */
    QPainterPath shape() const;

};

#endif // QSYSMLACTION_H

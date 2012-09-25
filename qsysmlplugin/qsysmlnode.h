#ifndef QSYSMLNODE_H
#define QSYSMLNODE_H

#include <qabstractdiagramshape.h>

class QSysMLNode : public QAbstractDiagramShape
{
public:
    QSysMLNode(const QMap<QString,QVariant> & properties, QGraphicsItem* parent = 0);

    QRectF boundingRect() const;

    QString nodeType() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    /**
      * Returns the shape of this item as a QPainterPath in local coordinates.
      */
    QPainterPath shape() const;
};

#endif // QSYSMLNODE_H

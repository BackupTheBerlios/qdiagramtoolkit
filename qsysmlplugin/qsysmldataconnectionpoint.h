#ifndef QSYSMLDATACONNECTIONPOINT_H
#define QSYSMLDATACONNECTIONPOINT_H

#include <qabstractdiagramshapeconnectionpoint.h>

class QSysMLDataConnectionPointIn : public QAbstractDiagramShapeConnectionPoint
{
public:
    explicit QSysMLDataConnectionPointIn(QAbstractDiagramShape* shape, const QString & id, int index, int maxConnections);
    ~QSysMLDataConnectionPointIn();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void updatePosition();
private:
    int m_index;
};

class QSysMLDataConnectionPointOut : public QAbstractDiagramShapeConnectionPoint
{
public:
    explicit QSysMLDataConnectionPointOut(QAbstractDiagramShape* shape, const QString & id, int index, int maxConnections);
    ~QSysMLDataConnectionPointOut();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void updatePosition();
private:
    int m_index;
};

#endif // QSYSMLDATACONNECTIONPOINT_H

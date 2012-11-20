#ifndef QSYSMLCONTROLCONNECTIONPOINT_H
#define QSYSMLCONTROLCONNECTIONPOINT_H

#include <qabstractdiagramshapeconnectionpoint.h>
#include <qdiagramtoolkit.h>

class QSysMLControlConnectionPointIn : public QAbstractDiagramShapeConnectionPoint
{
public:
    explicit QSysMLControlConnectionPointIn(QAbstractDiagramShape* shape, const QString & id, int index, int maxConnections);
    ~QSysMLControlConnectionPointIn();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void updatePosition();
private:
    int m_index;
};

class QSysMLControlConnectionPointOut : public QAbstractDiagramShapeConnectionPoint
{
public:
    explicit QSysMLControlConnectionPointOut(QAbstractDiagramShape* shape, const QString & id, int index, int maxConnections);
	explicit QSysMLControlConnectionPointOut(QAbstractDiagramShape* shape, const QString & id, QDiagramToolkit::ConnectionPointOrientation orientation, int index, int maxConnections);
    ~QSysMLControlConnectionPointOut();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void updatePosition();
private:
    int m_index;
};

#endif // QSYSMLCONTROLCONNECTIONPOINT_H

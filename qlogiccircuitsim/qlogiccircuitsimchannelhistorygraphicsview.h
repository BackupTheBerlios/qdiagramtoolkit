#ifndef QLOGICCIRCUITSIMCHANNELHISTORYGRAPHICSVIEW_H
#define QLOGICCIRCUITSIMCHANNELHISTORYGRAPHICSVIEW_H

#include <QFrame>

#include "qabstractdiagramshape.h"

class QLogicCircuitSimChannelHistoryGraphicsView : public QFrame
{
    Q_OBJECT
public:
    explicit QLogicCircuitSimChannelHistoryGraphicsView(QWidget *parent = 0);
    
    void setShape(QAbstractDiagramShape* shape);
signals:
    
public slots:
    void reset();

    void sample();
protected:
    void paintEvent(QPaintEvent* event);
private:
    QList<qreal> m_values;
    QAbstractDiagramShape* m_shape;
};

#endif // QLOGICCIRCUITCHANNELHISTORYGRAPHICSVIEW_H

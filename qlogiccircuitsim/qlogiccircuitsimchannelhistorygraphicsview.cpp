#include "stdafx.h"
#include "qlogiccircuitsimchannelhistorygraphicsview.h"

QLogicCircuitSimChannelHistoryGraphicsView::QLogicCircuitSimChannelHistoryGraphicsView(QWidget *parent) :
    QFrame(parent)
{
    setFrameStyle(QFrame::Panel | QFrame::Sunken);
    m_shape = 0;
}

void QLogicCircuitSimChannelHistoryGraphicsView::setShape(QAbstractDiagramShape *shape)
{
    m_shape = shape;
}

void QLogicCircuitSimChannelHistoryGraphicsView::reset()
{
    m_values.clear();
}

void QLogicCircuitSimChannelHistoryGraphicsView::sample()
{
    if (m_values.size() == 100){
        m_values.removeAt(0);
    }
    m_values.append(m_shape->property("simulatedValue").toInt());
    update();
}

void QLogicCircuitSimChannelHistoryGraphicsView::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);

    QPainter p(this);
    qreal d = width() / 100;

    if (m_values.size() > 1){
        QPointF p1(width(), height() - 5 - (m_values.at(0) * (height() - 10)));
        QPointF p2;
        for (int i = 1; i < m_values.size(); i++){
            p2 = QPointF(width() - d *  i, height() - 5 - (m_values.at(i) * (height() - 10)));
            p.drawLine(p1, p2);
            p1 = p2;
        }
    }
}

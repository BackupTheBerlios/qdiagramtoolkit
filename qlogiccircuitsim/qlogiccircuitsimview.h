#ifndef QLOGICCIRCUITSIMVIEW_H
#define QLOGICCIRCUITSIMVIEW_H

#include <QWidget>

class QLogicCircuitSimEngine;

namespace Ui {
class QLogicCircuitSimView;
}

#include <qdiagram.h>
#include <qlogiccircuitsim_global.h>

class QLOGICCIRCUITSIMSHARED_EXPORT QLogicCircuitSimView : public QWidget
{
    Q_OBJECT
    
public:
    explicit QLogicCircuitSimView(QWidget *parent = 0);
    ~QLogicCircuitSimView();

    void setDiagram(QDiagram* diagram);
	void setEngine(QLogicCircuitSimEngine* engine);
public slots:
	void debug();

    void pause();

    void reset();

    void resume();

    void start();

    void stop();
private slots:
    void timerTick();
private:
    QDiagram* m_diagram;
	QLogicCircuitSimEngine* m_engine;
    QTimer* m_timer;
    Ui::QLogicCircuitSimView *ui;
};

#endif // QLOGICCIRCUITSIMVIEW_H

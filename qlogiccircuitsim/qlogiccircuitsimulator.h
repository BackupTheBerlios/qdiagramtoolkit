#ifndef QLOGICCIRCUITSIMULATOR_H
#define QLOGICCIRCUITSIMULATOR_H

#include <QDialog>

#include <qlogiccircuitsim_global.h>

namespace Ui {
class QLogicCircuitSimulator;
}

class QDiagram;
class QLogicCircuitSimEngine;

class QLOGICCIRCUITSIMSHARED_EXPORT QLogicCircuitSimulator : public QDialog
{
    Q_OBJECT
    
public:
    explicit QLogicCircuitSimulator(QWidget *parent = 0);
    ~QLogicCircuitSimulator();

    void setDiagram(QDiagram* diagram);
private:
    QLogicCircuitSimEngine* m_engine;

    Ui::QLogicCircuitSimulator *ui;
};

#endif // QLOGICCIRCUITSIMULATOR_H

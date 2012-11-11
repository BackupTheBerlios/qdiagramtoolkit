#include "qlogiccircuitsimulator.h"
#include "ui_qlogiccircuitsimulator.h"

#include "qlogiccircuitsimengine.h"

QLogicCircuitSimulator::QLogicCircuitSimulator(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QLogicCircuitSimulator)
{
    ui->setupUi(this);

    m_engine = new QLogicCircuitSimEngine(this);
}

QLogicCircuitSimulator::~QLogicCircuitSimulator()
{
    delete ui;
}

void QLogicCircuitSimulator::setDiagram(QDiagram *diagram)
{
    ui->simView->setDiagram(diagram);
	ui->simView->setEngine(m_engine);
    m_engine->setDiagram(diagram);
    ui->textEdit->setText(m_engine->script());

}

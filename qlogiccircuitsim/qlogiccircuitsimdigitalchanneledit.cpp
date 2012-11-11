#include "qlogiccircuitsimdigitalchanneledit.h"
#include "ui_qlogiccircuitsimdigitalchanneledit.h"

QLogicCircuitSimDigitalChannelEdit::QLogicCircuitSimDigitalChannelEdit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QLogicCircuitSimDigitalChannelEdit)
{
    ui->setupUi(this);
    m_shape = 0;
}

QLogicCircuitSimDigitalChannelEdit::~QLogicCircuitSimDigitalChannelEdit()
{
    delete ui;
}

void QLogicCircuitSimDigitalChannelEdit::setShape(QAbstractDiagramShape *shape)
{
    m_shape = shape;
    ui->nameLabel->setText(shape->property("name").toString());
}

void QLogicCircuitSimDigitalChannelEdit::toggle()
{
}

void QLogicCircuitSimDigitalChannelEdit::onRadioButtonToggled(bool on)
{
    if (m_shape){
        m_shape->setProperty("simulatedValue", on?1.0:0.0);
    }
}

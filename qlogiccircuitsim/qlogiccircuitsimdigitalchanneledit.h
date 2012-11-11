#ifndef QLOGICCIRCUITSIMDIGITALCHANNELEDIT_H
#define QLOGICCIRCUITSIMDIGITALCHANNELEDIT_H

#include <QWidget>

#include "qabstractdiagramshape.h"

namespace Ui {
class QLogicCircuitSimDigitalChannelEdit;
}

class QLogicCircuitSimDigitalChannelEdit : public QWidget
{
    Q_OBJECT
public:
    explicit QLogicCircuitSimDigitalChannelEdit(QWidget *parent = 0);
    ~QLogicCircuitSimDigitalChannelEdit();

    void setShape(QAbstractDiagramShape* shape);
public slots:
    void toggle();
private slots:
    void onRadioButtonToggled(bool on);
private:
    QAbstractDiagramShape* m_shape;
    Ui::QLogicCircuitSimDigitalChannelEdit *ui;
};

#endif // QLOGICCIRCUITSIMDIGITALCHANNELEDIT_H

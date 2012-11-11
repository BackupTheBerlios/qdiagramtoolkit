#include "stdafx.h"
#include "qlogiccircuitsimview.h"
#include "ui_qlogiccircuitsimview.h"

#include "qlogiccircuitsimdigitalchanneledit.h"
#include "qlogiccircuitsimchannelhistorygraphicsview.h"
#include "qlogiccircuitsimengine.h"

QLogicCircuitSimView::QLogicCircuitSimView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QLogicCircuitSimView)
{
    ui->setupUi(this);

    m_timer = new QTimer(this);
    m_timer->setInterval(1000);
    m_timer->start();

	m_engine = 0;
}

QLogicCircuitSimView::~QLogicCircuitSimView()
{
    m_timer->stop();
    delete ui;
}

void QLogicCircuitSimView::debug()
{
     QScriptEngineDebugger debugger;
	 debugger.attachTo(m_engine->scriptEngine());
	 debugger.action(QScriptEngineDebugger::InterruptAction)->trigger();
	 m_engine->scriptEngine()->evaluate(m_engine->script());
}

void QLogicCircuitSimView::pause()
{
}

void QLogicCircuitSimView::reset()
{
}

void QLogicCircuitSimView::resume()
{
}

void QLogicCircuitSimView::setDiagram(QDiagram *diagram)
{
    m_diagram = diagram;

    QLogicCircuitSimDigitalChannelEdit* e;
    QLogicCircuitSimChannelHistoryGraphicsView* v;
    int r = 0;
    ui->diagramGraphicsView->setScene(m_diagram->scene());
    Q_FOREACH(QAbstractDiagramGraphicsItem* i, m_diagram->items()){
        QAbstractDiagramShape* s = qgraphicsitem_cast<QAbstractDiagramShape*>(i);
        if (s){
            if (s->property("shape") == "input"){
                e = new QLogicCircuitSimDigitalChannelEdit(this);
                e->setShape(s);
                ui->inputsGridLayout->addWidget(e, r, 0);
                v = new QLogicCircuitSimChannelHistoryGraphicsView(this);
                v->setShape(s);
                connect(m_timer, SIGNAL(timeout()), v, SLOT(sample()));
                ui->inputsGridLayout->addWidget(v, r, 1);
            } else if (s->property("shape") == "output"){
//                ui->outputsScrollAreadVerticalLayout->addWidget(new QLogicCircuitSimChannel(this));
            }
            r++;
        }
    }
}

void QLogicCircuitSimView::setEngine(QLogicCircuitSimEngine* engine)
{
	m_engine = engine;
}

void QLogicCircuitSimView::start()
{
}

void QLogicCircuitSimView::stop()
{
}

void QLogicCircuitSimView::timerTick()
{
}

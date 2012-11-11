#include "StdAfx.h"
#include "qlogiccircuitdiagram.h"

#include <qabstractdiagramgraphicsitem.h>
#include <qabstractdiagramshapeconnector.h>
#include <qabstractdiagramshapeconnectionpoint.h>

#include <qlogiccircuitgateshape.h>
#include <qlogiccircuitoutputshape.h>

QLogicCircuitDiagram::QLogicCircuitDiagram(QObject *parent)
	: QDiagram(parent)
{
	addPlugin("Logic Circuit");

	connect(this, SIGNAL(contentsChanged()), this, SLOT(render()));
}

QLogicCircuitDiagram::~QLogicCircuitDiagram()
{

}

QString QLogicCircuitDiagram::plugin() const
{
    return QLatin1String("Logic Circuit");
}

void QLogicCircuitDiagram::render()
{
	qDebug() << toText();
}

QString QLogicCircuitDiagram::type() const
{
    return QLatin1String("Logic Circuit");
}

QString QLogicCircuitDiagram::toText() const
{
	QString t;
	Q_FOREACH(QAbstractDiagramGraphicsItem* i, items()){
		QAbstractDiagramShape* s = qgraphicsitem_cast<QAbstractDiagramShape*>(i);
		if (s && s->metaData()->itemClass() == "output"){
			t += toText(s);
		}
	}
	return t;
}

QString QLogicCircuitDiagram::toText(QAbstractDiagramShape* s) const
{
	QString t;
	if (s->metaData()->itemClass() == "output"){
		if (s->connectionPoints(QDiagramToolkit::West).size() == 1){
			QList<QAbstractDiagramShapeConnectionPoint::Connection> c = s->connectionPoints(QDiagramToolkit::West).at(0)->connections();
			if (c.size() == 1){
				if (s == c.at(0).connector->startConnectionPoint()->parentShape()){
					t = toText(c.at(0).connector->endConnectionPoint()->parentShape());
				} else {
					t = toText(c.at(0).connector->startConnectionPoint()->parentShape());
				}
			}
		}
	} else if (s->metaData()->itemClass() == "function"){
		if (s->property("function") == "timer"){
			t += QString("DELAY %1sec ").arg(s->property("time").toString());
			QAbstractDiagramShapeConnectionPoint* p = s->connectionPoint("trigger");
			if (p){
				QAbstractDiagramShape* cs;
				if (p->connections().size() == 1){
					if (p->connections().at(0).connector->startConnectionPoint()->parentShape() == s){
						cs = p->connections().at(0).connector->endConnectionPoint()->parentShape();
					} else {
						cs = p->connections().at(0).connector->startConnectionPoint()->parentShape();
					}
					t += toText(cs);
				}
			}
		}
	} else if (s->metaData()->itemClass() == "gate"){
		Q_FOREACH(QAbstractDiagramShapeConnectionPoint* p, s->connectionPoints(QDiagramToolkit::West)){
			Q_FOREACH(QAbstractDiagramShapeConnectionPoint::Connection c, p->connections()){
				if (s->property("gateType").toString() == "and"){
					if (s == c.connector->startConnectionPoint()->parentShape()){
						t += QString("AND %1").arg(toText(c.connector->endConnectionPoint()->parentShape()));
					} else {
						t += QString("AND %1").arg(toText(c.connector->startConnectionPoint()->parentShape()));
					}
				}
			}
		}
	} else if (s->metaData()->itemClass() == "input"){
		t = QString("%1 \n").arg(s->property("name").toString());
	}
	return t;
}

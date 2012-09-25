#include "StdAfx.h"
#include "qlogiccircuitdiagram.h"

QLogicCircuitDiagram::QLogicCircuitDiagram(QObject *parent)
	: QDiagram(parent)
{
	addPlugin("Logic Circuit");
}

QLogicCircuitDiagram::~QLogicCircuitDiagram()
{

}

QString QLogicCircuitDiagram::plugin() const
{
    return QLatin1String("Logic Circuit");
}

QString QLogicCircuitDiagram::type() const
{
    return QLatin1String("Logic Circuit");
}

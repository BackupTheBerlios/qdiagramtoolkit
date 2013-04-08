/******************************************************************************
** Copyright (C) 2011 Martin Hoppe martin@2x2hoppe.de
**
** This file is part of the QDiagram Toolkit (qdiagramlib)
**
** qdiagramlib is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as
** published by the Free Software Foundation, either version 3 of the
** License, or (at your option) any later version.
**
** qdiagramlib is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Leser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with qdialgramlib.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/
#include "stdafx.h"
#include "qlogiccircuitplugin.h"

#include <qabstractdiagram.h>
#include <qdiagram.h>
#include "qlogiccircuitdiagram.h"
#include "qlogiccircuitflipflopshape.h"
#include "qlogiccircuitfunctionshape.h"
#include "qlogiccircuitgateshape.h"
#include "qlogiccircuitinputshape.h"
#include "qlogiccircuitoutputshape.h"
#include "qlogiccircuitparametershape.h"
#include "qlogiccircuitshapeconnector.h"
#include "qlogiccircuitvalueshape.h"


//
//QMap<QString, QVariant> QLogicCircuitPlugin::defaultProperties(const QString &name) const
//{
//    QMap<QString, QVariant> properties;
//	QFont f("Arial");
//	f.setPointSizeF(4);
//	properties["textFont"] = QDiagramProperty::toMap(f);
//    if (name == "flipflop.asyncron"){
//        properties["function"] = "comparator";
//    } else if (name == "function.comparator"){
//        properties["function"] = "comparator";
//    } else if (name == "function.computation"){
//        properties["function"] = "computation";
//    } else if (name == "function.counter"){
//        properties["function"] = "counter";
//    } else if (name == "function.counter.operating_hours"){
//        properties["function"] = "operatingHoursCounter";
//    } else if (name == "function.timer"){
//        properties["function"] = "timer";
//        properties["mode"] = "delay";
//        properties["time"] = 5;
//    } else if (name == "gate.and"){
//        properties["gateType"] = "and";
//    } else if (name == "gate.or"){
//        properties["gateType"] = "or";
//    } else if (name == "gate.nand"){
//        properties["gateType"] = "nand";
//    } else if (name == "gate.nor"){
//        properties["gateType"] = "nor";
//    } else if (name == "gate.not"){
//        properties["gateType"] = "not";
//    } else if (name == "gate.xnor"){
//        properties["gateType"] = "xnor";
//    } else if (name == "gate.xor"){
//        properties["gateType"] = "xor";
//    } else if (name == "input.analog"){
//        properties["signalType"] = "analog";
//    } else if (name == "input.digital"){
//        properties["signalType"] = "digital";
//    } else if (name == "output.analog"){
//        properties["signalType"] = "analog";
//    } else if (name == "output.digital"){
//        properties["signalType"] = "digital";
//    } else if (name == "parameter.analog"){
//        properties["signalType"] = "analog";
//    } else if (name == "parameter.digital"){
//        properties["signalType"] = "digital";
//    } else if (name == "value.analog"){
//        properties["signalType"] = "analog";
//    }
//	properties["zlevel"] = 0;
//    return properties;
//}

//QDiagram* QLogicCircuitPlugin::diagram(const QString & type, QObject* parent) const
//{
//	QDiagram* d = new QLogicCircuitDiagram(parent);
//    d->addPlugin(name());
//    return d;
//}

QList<QDiagramEndOfLineStyle> QLogicCircuitPlugin::endOfLineStyles() const
{
    return QList<QDiagramEndOfLineStyle>();
}

//QPointF QLogicCircuitPlugin::hotSpot(const QVariantMap & metaData, const QVariantMap & properties) const
//{
//    QPointF p(0, 0);
//    if (properties.value("gateType") == "gate.and" || properties.value("gateType") == "gate.nand" || properties.value("gateType") == "gate.nor"
//            || properties.value("gateType") == "gate.or"
//            || properties.value("gateType") == "gate.xnor"
//            || properties.value("gateType") == "gate.xor"){
//        p = QPointF(0, -13);
//    } else if (properties.value("gateType") == "gate.not"){
//        p = QPointF(0, -26);
//    }
//    return p;
//}
//
//QIcon QLogicCircuitPlugin::icon(const QString & name) const
//{
//    if (name == "flipflop.asyncron"){
//        return QIcon(":/logiccircuit/flipflop.asyncron");
//    } else if (name == "function.comparator"){
//        return QIcon(":/logiccircuit/function.comparator");
//    } else if (name == "function.counter"){
//        return QIcon(":/logiccircuit/function.counter");
//    } else if (name == "function.counter.operating_hours"){
//        return QIcon(":/logiccircuit/function.counter.operating_hours");
//    } else if (name == "function.computation"){
//        return QIcon(":/logiccircuit/function.summation");
//    } else if (name == "function.timer"){
//        return QIcon(":/logiccircuit/function.timer");
//    } else if (name == "gate.and"){
//        return QIcon(":/logiccircuit/gate.and");
//    } else if (name == "gate.nand"){
//        return QIcon(":/logiccircuit/gate.nand");
//    } else if (name == "gate.nor"){
//        return QIcon(":/logiccircuit/gate.nor");
//    } else if (name == "gate.not"){
//        return QIcon(":/logiccircuit/gate.not");
//    } else if (name == "gate.or"){
//        return QIcon(":/logiccircuit/gate.or");
//    } else if (name == "gate.xnor"){
//        return QIcon(":/logiccircuit/gate.xnor");
//    } else if (name == "gate.xor"){
//        return QIcon(":/logiccircuit/gate.xor");
//    } else if (name == "input.digital"){
//        return QIcon(":/logiccircuit/input.digital");
//    } else if (name == "input.analog"){
//        return QIcon(":/logiccircuit/input.analog");
//    } else if (name == "output.digital"){
//        return QIcon(":/logiccircuit/output.digital");
//    } else if (name == "output.analog"){
//        return QIcon(":/logiccircuit/output.analog");
//    } else if (name == "parameter.digital"){
//        return QIcon(":/logiccircuit/parameter.digital");
//    } else if (name == "parameter.analog"){
//        return QIcon(":/logiccircuit/parameter.analog");
//    } else if (name == "value.analog"){
//        return QIcon(":/logiccircuit/value.analog");
//    }
//    return QIcon();
//}

void QLogicCircuitPlugin::initialize()
{
	registerConnector<QLogicCircuitShapeConnector>("default", tr("Default"), QIcon(":/logiccircuit/connector.default"));

	registerDiagram<QLogicCircuitDiagram>(tr("Logic Circuit Diagram"), QIcon());

	registerShape<QLogicCircuitGateShape>("gate.and", tr("Gates"), tr("AND"),QIcon(":/logiccircuit/gate.and"));
	registerShape<QLogicCircuitGateShape>("gate.nand", tr("Gates"), tr("NAND"),QIcon(":/logiccircuit/gate.nand"));
	registerShape<QLogicCircuitGateShape>("gate.nor", tr("Gates"), tr("NOR"),QIcon(":/logiccircuit/gate.nor"));
	registerShape<QLogicCircuitGateShape>("gate.not", tr("Gates"), tr("NOT"),QIcon(":/logiccircuit/gate.not"));
	registerShape<QLogicCircuitGateShape>("gate.xnor", tr("Gates"), tr("XNOR"),QIcon(":/logiccircuit/gate.xnor"));
	registerShape<QLogicCircuitGateShape>("gate.xor", tr("Gates"), tr("XOR"),QIcon(":/logiccircuit/gate.xor"));

	registerShape<QLogicCircuitFunctionShape>("function.timer", tr("Functions"), tr("Timer"),QIcon(":/logiccircuit/function.timer"));
	registerShape<QLogicCircuitFunctionShape>("function.comparator", tr("Functions"), tr("Comparator"),QIcon(":/logiccircuit/function.comparator"));
	registerShape<QLogicCircuitFunctionShape>("function.counter", tr("Functions"), tr("Counter"),QIcon(":/logiccircuit/function.counter"));
	registerShape<QLogicCircuitFunctionShape>("function.counter.oph", tr("Functions"), tr("Operating Hours Counter"),QIcon(":/logiccircuit/function.counter.operating_hours"));

	registerShape<QLogicCircuitInputShape>("input.analog", tr("Inputs / Outputs"), tr("Input (analog)"),QIcon(":/logiccircuit/input.analog"));
	registerShape<QLogicCircuitInputShape>("input.digital", tr("Inputs / Outputs"), tr("Input (digital)"),QIcon(":/logiccircuit/input.digital"));

	registerShape<QLogicCircuitOutputShape>("output.analog", tr("Inputs / Outputs"), tr("Output (analog)"),QIcon(":/logiccircuit/output.analog"));
	registerShape<QLogicCircuitOutputShape>("output.digital", tr("Inputs / Outputs"), tr("Output (digital)"),QIcon(":/logiccircuit/output.digital"));

	registerShape<QLogicCircuitParameterShape>("parameter.analog", tr("Inputs / Outputs"), tr("Parameter (analog)"),QIcon(":/logiccircuit/parameter.analog"));
	registerShape<QLogicCircuitParameterShape>("parameter.digital", tr("Inputs / Outputs"), tr("Parameter (digital)"),QIcon(":/logiccircuit/parameter.digital"));

	registerShape<QLogicCircuitValueShape>("value.analog", tr("Inputs / Outputs"), tr("Value (analog)"),QIcon(":/logiccircuit/value.analog"));
}

QList<QDiagramLineStyle> QLogicCircuitPlugin::lineStyles() const
{
    return QList<QDiagramLineStyle>();
}

QList<QAction *> QLogicCircuitPlugin::shapeActions(const QString & style) const
{
    QList<QAction*> a;
    if (style == "function.comparator"){

    }
    return a;
}

QString QLogicCircuitPlugin::toolTip(const QString & shape) const
{
    Q_UNUSED(shape);
    return QString::null;
}

Q_EXPORT_PLUGIN2(qlogiccircuitplugin, QLogicCircuitPlugin)

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

QList<QDiagramConnectorStyle> QLogicCircuitPlugin::connectors() const
{
    QList<QDiagramConnectorStyle> mStyles;

    mStyles << QDiagramConnectorStyle(name(), QIcon(":/logiccircuit/connector.default"), "Default", "default");
    return mStyles;
}

QAbstractDiagramGraphicsItem* QLogicCircuitPlugin::createItem(const QMap<QString,QVariant> & properties, QGraphicsScene* scene)
{
    QAbstractDiagramGraphicsItem* item = 0;
    if (!properties.contains("uuid")){
        return 0;
    }
    if (properties.value("itemType") == "shape"){
        if (properties.value("shape") == "flipflop"){
            item = new QLogicCircuitFlipFlopShape(properties);
        } else if (properties.value("shape") == "function"){
            item = new QLogicCircuitFunctionShape(properties);
        } else if (properties.value("shape") == "gate"){
            item = new QLogicCircuitGateShape(properties);
        } else if (properties.value("shape") == "input"){
            item = new QLogicCircuitInputShape(properties);
        } else if (properties.value("shape") == "output"){
            item = new QLogicCircuitOutputShape(properties);
        } else if (properties.value("shape") == "parameter"){
            item = new QLogicCircuitParameterShape(properties);
        }
    } else if (properties.value("itemType") == "connector"){
        item = new QLogicCircuitShapeConnector(properties);
    }
    if (item && scene){
        scene->addItem(item);
    }
    return item;
}

QMap<QString, QVariant> QLogicCircuitPlugin::defaultProperties(const QString &name) const
{
    QMap<QString, QVariant> properties;
    properties["plugin"] = this->name();
    properties["itemType"] = "shape";
    if (name == "flipflop.asyncron"){
        properties["shape"] = "flipflop";
        properties["function"] = "comparator";
		QVariantMap p;
		p["x"] = 0;
		p["y"] = 0;
		p["width"] = 78;
		p["height"] = 52;
        properties["geometry"] = p;
    } else if (name == "function.comparator"){
        properties["shape"] = "function";
        properties["function"] = "comparator";
        properties["equal"] = "equal";
		QVariantMap p;
		p["x"] = 0;
		p["y"] = 0;
		p["width"] = 78;
		p["height"] = 52;
        properties["geometry"] = p;
    } else if (name == "function.computation"){
        properties["shape"] = "function";
        properties["function"] = "computation";
		QVariantMap p;
		p["x"] = 0;
		p["y"] = 0;
		p["width"] = 78;
		p["height"] = 52;
        properties["geometry"] = p;
    } else if (name == "function.counter"){
        properties["shape"] = "function";
        properties["function"] = "counter";
        QVariantMap p;
        p["x"] = 0;
        p["y"] = 0;
        p["width"] = 78;
        p["height"] = 52;
        properties["geometry"] = p;
    } else if (name == "function.counter.operating_hours"){
        properties["shape"] = "function";
        properties["function"] = "operatingHoursCounter";
        properties["digits"] = 5;
        QVariantMap p;
        p["x"] = 0;
        p["y"] = 0;
        p["width"] = 78;
        p["height"] = 52;
        properties["geometry"] = p;
    } else if (name == "gate.and"){
        properties["shape"] = "gate";
        properties["gateType"] = "and";
		QVariantMap p;
		p["x"] = 0;
		p["y"] = 0;
		p["width"] = 78;
		p["height"] = 52;
        properties["geometry"] = p;
    } else if (name == "gate.or"){
        properties["shape"] = "gate";
        properties["gateType"] = "or";
		QVariantMap p;
		p["x"] = 0;
		p["y"] = 0;
		p["width"] = 78;
		p["height"] = 52;
        properties["geometry"] = p;
    } else if (name == "gate.nand"){
        properties["shape"] = "gate";
        properties["gateType"] = "nand";
		QVariantMap p;
		p["x"] = 0;
		p["y"] = 0;
		p["width"] = 78;
		p["height"] = 52;
        properties["geometry"] = p;
    } else if (name == "gate.nor"){
        properties["shape"] = "gate";
        properties["gateType"] = "nor";
		QVariantMap p;
		p["x"] = 0;
		p["y"] = 0;
		p["width"] = 78;
		p["height"] = 52;
        properties["geometry"] = p;
    } else if (name == "gate.not"){
        properties["shape"] = "gate";
        properties["gateType"] = "not";
		QVariantMap p;
		p["x"] = 0;
		p["y"] = 0;
		p["width"] = 78;
		p["height"] = 52;
        properties["geometry"] = p;
    } else if (name == "gate.xnor"){
        properties["shape"] = "gate";
        properties["gateType"] = "xnor";
		QVariantMap p;
		p["x"] = 0;
		p["y"] = 0;
		p["width"] = 78;
		p["height"] = 52;
        properties["geometry"] = p;
    } else if (name == "gate.xor"){
        properties["shape"] = "gate";
        properties["gateType"] = "xor";
		QVariantMap p;
		p["x"] = 0;
		p["y"] = 0;
		p["width"] = 78;
		p["height"] = 52;
        properties["geometry"] = p;
    } else if (name == "input.analog"){
        properties["shape"] = "input";
        properties["signalType"] = "analog";
		QVariantMap p;
		p["x"] = 0;
		p["y"] = 0;
		p["width"] = 182;
		p["height"] = 26;
        properties["geometry"] = p;
    } else if (name == "input.digital"){
        properties["shape"] = "input";
        properties["signalType"] = "digital";
		QVariantMap p;
		p["x"] = 0;
		p["y"] = 0;
		p["width"] = 182;
		p["height"] = 26;
        properties["geometry"] = p;
    } else if (name == "output.analog"){
        properties["shape"] = "output";
        properties["signalType"] = "analog";
		QVariantMap p;
		p["x"] = 0;
		p["y"] = 0;
		p["width"] = 182;
		p["height"] = 26;
        properties["geometry"] = p;
    } else if (name == "output.digital"){
        properties["shape"] = "output";
        properties["signalType"] = "digital";
		QVariantMap p;
		p["x"] = 0;
		p["y"] = 0;
		p["width"] = 182;
		p["height"] = 26;
        properties["geometry"] = p;
    } else if (name == "parameter.analog"){
        properties["shape"] = "parameter";
        properties["signalType"] = "analog";
		QVariantMap p;
		p["x"] = 0;
		p["y"] = 0;
		p["width"] = 182;
		p["height"] = 26;
        properties["geometry"] = p;
    } else if (name == "parameter.digital"){
        properties["shape"] = "parameter";
        properties["signalType"] = "digital";
		QVariantMap p;
		p["x"] = 0;
		p["y"] = 0;
		p["width"] = 182;
		p["height"] = 26;
        properties["geometry"] = p;
    }
    return properties;
}

QDiagram* QLogicCircuitPlugin::diagram(const QString & type, QObject* parent) const
{
	QDiagram* d = new QLogicCircuitDiagram(parent);
    d->addPlugin(name());
    return d;
}

QStringList QLogicCircuitPlugin::diagrams() const
{
    QStringList n;
    n << "Logic Circuit";
    return n;
}

QList<QDiagramEndOfLineStyle> QLogicCircuitPlugin::endOfLineStyles() const
{
    return QList<QDiagramEndOfLineStyle>();
}

QStringList QLogicCircuitPlugin::groups(QAbstractDiagram *diagram) const
{
    QStringList l;
    if (diagram == 0){
        l << QObject::tr("Gates") << QObject::tr("Inputs/Outputs") << QObject::tr("Functions");
    }
    return l;
}

QPointF QLogicCircuitPlugin::hotSpot(const QString &name) const
{
    QPointF p(0, 0);
    if (name == "gate.and" || name == "gate.nand" || name == "gate.nor"
            || name == "gate.or"
            || name == "gate.xnor"
            || name == "gate.xor"){
        p = QPointF(0, -13);
    } else if (name == "gate.not"){
        p = QPointF(0, -26);
    }
    return p;
}

QIcon QLogicCircuitPlugin::icon(const QString & name) const
{
    if (name == "flipflop.asyncron"){
        return QIcon(":/logiccircuit/flipflop.asyncron");
    } else if (name == "function.comparator"){
        return QIcon(":/logiccircuit/function.comparator");
    } else if (name == "function.counter"){
        return QIcon(":/logiccircuit/function.counter");
    } else if (name == "function.counter.operating_hours"){
        return QIcon(":/logiccircuit/function.counter.operating_hours");
    } else if (name == "function.computation"){
        return QIcon(":/logiccircuit/function.summation");
    } else if (name == "gate.and"){
        return QIcon(":/logiccircuit/gate.and");
    } else if (name == "gate.nand"){
        return QIcon(":/logiccircuit/gate.nand");
    } else if (name == "gate.nor"){
        return QIcon(":/logiccircuit/gate.nor");
    } else if (name == "gate.not"){
        return QIcon(":/logiccircuit/gate.not");
    } else if (name == "gate.or"){
        return QIcon(":/logiccircuit/gate.or");
    } else if (name == "gate.xnor"){
        return QIcon(":/logiccircuit/gate.xnor");
    } else if (name == "gate.xor"){
        return QIcon(":/logiccircuit/gate.xor");
    } else if (name == "input.digital"){
        return QIcon(":/logiccircuit/input.digital");
    } else if (name == "input.analog"){
        return QIcon(":/logiccircuit/input.analog");
    } else if (name == "output.digital"){
        return QIcon(":/logiccircuit/output.digital");
    } else if (name == "output.analog"){
        return QIcon(":/logiccircuit/output.analog");
    } else if (name == "parameter.digital"){
        return QIcon(":/logiccircuit/parameter.digital");
    } else if (name == "parameter.analog"){
        return QIcon(":/logiccircuit/parameter.analog");
    }
    return QIcon();
}

QList<QDiagramLineStyle> QLogicCircuitPlugin::lineStyles() const
{
    return QList<QDiagramLineStyle>();
}

QString QLogicCircuitPlugin::name() const
{
    return "Logic Circuit";
}

QAbstractDiagramShape* QLogicCircuitPlugin::restoreShape(const QString & uuid, const QString & style, const QMap<QString,QVariant> & properties, QGraphicsScene* scene)
{
    return 0;
}

QList<QAction *> QLogicCircuitPlugin::shapeActions(const QString & style) const
{
    QList<QAction*> a;
    if (style == "function.comparator"){

    }
    return a;
}

QStringList QLogicCircuitPlugin::shapes(const QString & group, QAbstractDiagram *diagram) const
{    
    QStringList functions;
    QStringList gates;
    QStringList inputsOutputs;

    functions << "function.comparator"
               << "function.counter"
               << "function.counter.operating_hours"
               << "function.computation";
    gates << "gate.and"
           << "gate.or"
           << "gate.nand"
           << "gate.nor"
           << "gate.not"
           << "gate.xnor"
           << "gate.xor";
//    mGates << "flipflop.asyncron";

    inputsOutputs << "input.digital"
                   << "output.digital"
                   << "input.analog"
                   << "output.analog"
				   << "parameter.analog"
				   << "parameter.digital";

    if (group == QObject::tr("Functions")){
        return functions;
    }
    if (group == QObject::tr("Gates")){
        return gates;
    }
    if (group == QObject::tr("Inputs/Outputs")){
        return inputsOutputs;
    }
    return functions + gates + inputsOutputs;
}

QString QLogicCircuitPlugin::title(const QString & name) const
{
    if (name == "flipflop.asyncron"){
        return QObject::tr("SR Latch");
    } else if (name == "function.comparator"){
        return QObject::tr("Comparator");
    } else if (name == "function.counter"){
        return QObject::tr("Counter");
    } else if (name == "function.counter.operating_hours"){
        return QObject::tr("Operating Hours Counter");
    } else if (name == "function.computation"){
        return QObject::tr("Computation");
    } else if (name == "gate.and"){
        return QObject::tr("AND");
    } else if (name == "gate.nand"){
        return QObject::tr("NAND");
    } else if (name == "gate.nor"){
        return QObject::tr("NOR");
    } else if (name == "gate.not"){
        return QObject::tr("NOT");
    } else if (name == "gate.or"){
        return QObject::tr("OR");
    } else if (name == "gate.xnor"){
        return QObject::tr("XNOR");
    } else if (name == "gate.xor"){
        return QObject::tr("XOR");
    } else if (name == "input.digital"){
        return QObject::tr("Input (digital)");
    } else if (name == "input.analog"){
        return QObject::tr("Input (analog)");
    } else if (name == "output.digital"){
        return QObject::tr("Output (digital)");
    } else if (name == "output.analog"){
        return QObject::tr("Output (analog)");
    } else if (name == "parameter.analog"){
        return QObject::tr("Parameter (analog)");
    } else if (name == "parameter.digital"){
        return QObject::tr("Parameter (digital)");
    }
    return name;
}

QString QLogicCircuitPlugin::toolTip(const QString & shape) const
{
    Q_UNUSED(shape);
    return QString::null;
}

Q_EXPORT_PLUGIN2(qlogiccircuitplugin, QLogicCircuitPlugin)

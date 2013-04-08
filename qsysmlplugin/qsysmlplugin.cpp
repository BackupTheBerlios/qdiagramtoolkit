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
#include "qsysmlplugin.h"

#include <qabstractdiagram.h>
#include <qdiagram.h>
#include <qdiagramlinestyle.h>

#include "qsysmlaction.h"
#include "qsysmlblock.h"
#include "qsysmlblockdiagram.h"
#include "qsysmldiagram.h"
#include "qsysmlcontrolflowconnector.h"
#include "qsysmldataflowconnector.h"
#include "qsysmlnode.h"
#include "qsysmlport.h"

//QDiagram *QSysMLPlugin::diagram(const QString & type, QObject *parent) const
//{
//    QSysMLDiagram* d = new QSysMLDiagram(parent);
//    d->addPlugin(name());
//    return d;
//}
//
//QStringList QSysMLPlugin::diagrams() const
//{
//    QStringList n;
//    n << "Activity Diagram";
//    return n;
//}

QList<QDiagramEndOfLineStyle> QSysMLPlugin::endOfLineStyles() const
{
    QList<QDiagramEndOfLineStyle> s;

    s << QDiagramEndOfLineStyle(tr("- none -"), "none", name());
    s << QDiagramEndOfLineStyle(QIcon(":/qstandardformsplugin/arrows/start.default"), tr("Arrow"), "arrow", name());
    s << QDiagramEndOfLineStyle(QIcon(":/qstandardformsplugin/arrows/start.small"), tr("Arrow (small)"), "arrow.small", name());
    s << QDiagramEndOfLineStyle(QIcon(":/qstandardformsplugin/arrows/start.double"), tr("Double Arrow"), "arrow.double", name());
    s << QDiagramEndOfLineStyle(QIcon(":/qstandardformsplugin/eol/start.square"), tr("Square"), "square", name());
    s << QDiagramEndOfLineStyle(QIcon(":/qstandardformsplugin/eol/start.circle"), tr("Circle"), "circle", name());
    s << QDiagramEndOfLineStyle(QIcon(":/qstandardformsplugin/eol/start.diamond"), tr("Diamond"), "diamond", name());
    return s;
}

//
//QIcon QSysMLPlugin::icon(const QString &name) const
//{
//    if (name == "action"){
//        return QIcon(":/qsysmlplugin/action");
//    } else if (name == "action.accept_event"){
//        return QIcon(":/qsysmlplugin/action.accept_event");
//    } else if (name == "action.send_signal"){
//        return QIcon(":/qsysmlplugin/action.send_signal");
//    } else if (name == "block"){
//        return QIcon(":/qsysmlplugin/block");
//    } else if (name == "node.initial"){
//        return QIcon(":/qsysmlplugin/node.initial");
//    } else if (name == "node.final.activity"){
//        return QIcon(":/qsysmlplugin/node.final.activity");
//    } else if (name == "node.final.flow"){
//        return QIcon(":/qsysmlplugin/node.final.flow");
//    } else if (name == "node.fork.horizontal"){
//        return QIcon(":/qsysmlplugin/node.fork_horizontal");
//    } else if (name == "node.fork"){
//        return QIcon(":/qsysmlplugin/node.fork");
//    } else if (name == "node.join"){
//        return QIcon(":/qsysmlplugin/node.join");
//    } else if (name == "node.decision"){
//        return QIcon(":/qsysmlplugin/node.decision");
//    } else if (name == "node.merge"){
//        return QIcon(":/qsysmlplugin/node.merge");
//    } else if (name == "node.transition"){
//        return QIcon(":/qsysmlplugin/node.transition");
//    } else if (name == "port.input"){
//        return QIcon(":/qsysmlplugin/port.input");
//    } else if (name == "port.output"){
//        return QIcon(":/qsysmlplugin/port.output");
//    }
//    return QIcon();
//}

void QSysMLPlugin::initialize()
{
	registerConnector<QSysMLControlFlowConnector>("flow.control", "Control Flow", QIcon(":/emcsplugin/flow.control"));
	registerConnector<QSysMLDataFlowConnector>("flow.data", "data Flow", QIcon(":/emcsplugin/flow.data"));

	registerDiagram<QSysMLDiagram>(tr("Activity Diagram"), QIcon());
	registerDiagram<QSysMLBlockDiagram>(tr("Block Diagram"), QIcon());

	registerShape<QSysMLAction>("action.default", tr("Actions"), tr("Action"), QIcon(":/qsysmlplugin/action"));
	registerShape<QSysMLAction>("action.accept_event", tr("Actions"), tr("Accept Event Action"), QIcon(":/qsysmlplugin/action.accept_event"));
	registerShape<QSysMLAction>("action.send_signal", tr("Actions"), tr("Send Signal Action"), QIcon(":/qsysmlplugin/action.send_signal"));

	registerShape<QSysMLBlock>("block", tr("Block"), tr("Block"), QIcon(":/qsysmlplugin/block"));

	registerShape<QSysMLNode>("node.decision", tr("Nodes"), tr("Decision Node"), QIcon(":/qsysmlplugin/node.decision"));
	registerShape<QSysMLNode>("node.initial", tr("Nodes"), tr("Initial Node"), QIcon(":/qsysmlplugin/node.initial"));
	registerShape<QSysMLNode>("node.final.activity", tr("Nodes"), tr("Final Activity Node"), QIcon(":/qsysmlplugin/node.final.activity"));
	registerShape<QSysMLNode>("node.final.flow", tr("Nodes"), tr("Final Flow Node"), QIcon(":/qsysmlplugin/node.final.flow"));
	registerShape<QSysMLNode>("node.fork", tr("Nodes"), tr("Fork"), QIcon(":/qsysmlplugin/node.fork"));
	registerShape<QSysMLNode>("node.merge", tr("Nodes"), tr("Merge Node"), QIcon(":/qsysmlplugin/node.merge"));

	registerShape<QSysMLPort>("port.input", tr("Block"), tr("Input"),QIcon(":/qsysmlplugin/port.input"));
	registerShape<QSysMLPort>("port.output", tr("Block"), tr("Output"),QIcon(":/qsysmlplugin/port.output"));
}

QList<QDiagramLineStyle> QSysMLPlugin::lineStyles() const
{
    QList<QDiagramLineStyle> l;
	QPen pen(Qt::black);
	pen.setStyle(Qt::SolidLine);
	pen.setWidthF(10.0);
	l << QDiagramLineStyle(name(), "ControlFlowLineStyle", tr("Control Flow"), pen);

	pen = QPen(Qt::black);
	pen.setStyle(Qt::SolidLine);
	pen.setWidthF(10.0);
	l << QDiagramLineStyle(name(), "DataFlowLineStyle", tr("Data Flow"), pen);
    return l;
}


//QMap<QString, QVariant> QSysMLPlugin::defaultProperties(const QString & name) const
//{
//    QMap<QString, QVariant> properties;
//    if (name == "action"){
//        properties["actionType"] = "default";
//		properties["textFont"] = QDiagramProperty::toMap(QFont("Arial", 5));
//    } else if (name == "action.accept_event"){
//        properties["actionType"] = "event";
//		properties["textFont"] = QFont("Arial", 5);
//	} else if (name == "action.send_signal"){
//        properties["actionType"] = "sendSignal";
//		properties["textFont"] = QFont("Arial", 5);
//    } else if (name == "block"){
//        properties["blockType"] = "default";
//	} else if (name == "flow.control"){
//		properties["font"] = QDiagramProperty::toMap(QFont("Arial", 10));
//		QPen pen(Qt::black);
//		pen.setWidthF(2.0);
//		pen.setStyle(Qt::DashLine);
//		properties["lineColor"] = QDiagramProperty::toMap(pen);
//	} else if (name == "node.decision"){
//        properties["nodeType"] = "decision";
//		properties["font"] = QDiagramProperty::toMap(QFont("Arial", 10));
//		QPen pen(Qt::black);
//		pen.setWidthF(2.0);
//		pen.setStyle(Qt::SolidLine);
//		properties["lineColor"] = QDiagramProperty::toMap(pen);
//    } else if (name == "node.initial"){
//		QBrush b(Qt::black);
//		properties["background"] = QDiagramProperty::toMap(b);
//        properties["nodeType"] = "initial";
//    } else if (name == "node.final.activity"){
//        properties["nodeType"] = "final.activity";
//    } else if (name == "node.final.flow"){
//        properties["nodeType"] = "final.flow";
//    } else if (name == "node.fork.horizontal"){
//        properties["nodeType"] = "fork";
//        properties["alignment"] = Qt::Horizontal;
//    } else if (name == "node.fork"){
//        properties["nodeType"] = "fork";
//        properties["alignment"] = Qt::Horizontal;
//    } else if (name == "node.join"){
//        properties["nodeType"] = "join";
//        properties["alignment"] = Qt::Horizontal;
//    } else if (name == "node.transition"){
//        properties["nodeType"] = "transition";
//		properties["font"] = QDiagramProperty::toMap(QFont("Arial", 10));
//		QPen pen(Qt::black);
//		pen.setWidthF(2.0);
//		properties["lineColor"] = QDiagramProperty::toMap(pen);
//    } else if (name == "port.input"){
//		properties = Port::defaultProperties();
//        properties["portType"] = "input";
//    } else if (name == "port.output"){
//		properties = Port::defaultProperties();
//        properties["portType"] = "output";
//    }
//    return properties;
//}


//QString QSysMLPlugin::title(const QString & name) const
//{
//    if (name == "block"){
//        return tr("Block");
//    } else if (name == "node.initial"){
//        return tr("Initial Node");
//    } else if (name == "node.final.activity"){
//        return tr("Final Activity Node");
//    } else if (name == "node.final.flow"){
//        return tr("Final Flow Node");
//    } else if (name == "node.fork.horizontal"){
//        return tr("Fork Node (horizontal)");
//    } else if (name == "node.fork"){
//        return tr("Fork Node");
//    } else if (name == "node.join"){
//        return tr("Join Node");
//    } else if (name == "node.decision"){
//        return tr("Decision Node");
//    } else if (name == "parallelogramm"){
//        return tr("Parallelogramm");
//    } else if (name == "node.merge"){
//        return tr("Merge Node");
//    } else if (name == "node.transition"){
//        return tr("Transition Node");
//    } else if (name == "action"){
//        return tr("Action");
//    } else if (name == "action.send_signal"){
//        return tr("Send Signal Action");
//    } else if (name == "action.accept_event"){
//        return tr("Accept Event Action");
//    } else if (name == "port.input"){
//        return tr("Input");
//    } else if (name == "port.output"){
//        return tr("Output");
//    }
//    return QString::null;
//}

QString QSysMLPlugin::toolTip(const QString & shape) const
{
    return QString::null;
}

Q_EXPORT_PLUGIN2(qsysmlplugin, QSysMLPlugin)

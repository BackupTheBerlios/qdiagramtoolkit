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
#include "qsysmlcontrolflowconnector.h"
#include "qsysmldataflowconnector.h"
#include "qsysmlnode.h"

QString QSysMLPlugin::name() const
{
	return QSysMLPlugin::staticName();
}

QList<QDiagramConnectorStyle> QSysMLPlugin::connectors() const
{
    QList<QDiagramConnectorStyle> l;

    l << QDiagramConnectorStyle(name(), QIcon(":/qsysmlplugin/flow.control"), "Control Flow", "flow.control");
    l << QDiagramConnectorStyle(name(), QIcon(":/qsysmlplugin/flow.data"), "Data Flow", "flow.data");
    return l;
}

QAbstractDiagramGraphicsItem *QSysMLPlugin::createItem(const QMap<QString,QVariant> & metaData, const QMap<QString, QVariant> &properties, QGraphicsScene *scene)
{
    QAbstractDiagramGraphicsItem* item = 0;
    if (!properties.contains("uuid")){
        return 0;
    }
    if (metaData.value("itemType") == "connector"){
		if (metaData.value("itemClass") == "flow.control"){
			item = new QSysMLControlFlowConnector(properties);
		} else if (metaData.value("itemClass") == "flow.data"){
			item = new QSysMLDataFlowConnector(properties);
		}
    } else if (metaData.value("itemType") == "shape"){
        if (metaData.value("itemClass") == "action"){
            item = new QSysMLAction(properties);
        } else if (metaData.value("itemClass") == "block"){
            item = new QSysMLBlock(properties);
        } else if (metaData.value("itemClass") == "node"){
            item = new QSysMLNode(properties);
        }
    }
    return item;
}

QDiagram *QSysMLPlugin::diagram(const QString & type, QObject *parent) const
{
    QDiagram* d = new QDiagram(parent);
    d->addPlugin(name());
    return d;
}

QStringList QSysMLPlugin::diagrams() const
{
    QStringList n;
    n << "Activity Diagram";
    return n;
}

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

QStringList QSysMLPlugin::groups(QAbstractDiagram *diagram) const
{
    QStringList g;
    g  << tr("Blocks")
       << tr("Nodes")
       << "Actions";
    return g;
}

QIcon QSysMLPlugin::icon(const QString &name) const
{
    if (name == "action"){
        return QIcon(":/qsysmlplugin/action");
    } else if (name == "action.accept_event"){
        return QIcon(":/qsysmlplugin/action.accept_event");
    } else if (name == "action.send_signal"){
        return QIcon(":/qsysmlplugin/action.send_signal");
    } else if (name == "block"){
        return QIcon(":/qsysmlplugin/block");
    } else if (name == "node.initial"){
        return QIcon(":/qsysmlplugin/node.initial");
    } else if (name == "node.final.activity"){
        return QIcon(":/qsysmlplugin/node.final.activity");
    } else if (name == "node.final.flow"){
        return QIcon(":/qsysmlplugin/node.final.flow");
    } else if (name == "node.fork.horizontal"){
        return QIcon(":/qsysmlplugin/node.fork_horizontal");
    } else if (name == "node.fork"){
        return QIcon(":/qsysmlplugin/node.fork");
    } else if (name == "node.join"){
        return QIcon(":/qsysmlplugin/node.join");
    } else if (name == "node.decision"){
        return QIcon(":/qsysmlplugin/node.decision");
    } else if (name == "node.merge"){
        return QIcon(":/qsysmlplugin/node.merge");
    } else if (name == "node.transition"){
        return QIcon(":/qsysmlplugin/node.transition");
    }
    return QIcon();
}

QList<QDiagramLineStyle> QSysMLPlugin::lineStyles() const
{
    QList<QDiagramLineStyle> styles;

    return styles;
}

QAbstractDiagramShape *QSysMLPlugin::restoreShape(const QString &uuid, const QString &style, const QMap<QString, QVariant> &properties, QGraphicsScene *scene)
{
    // TODO obsolete
    return 0;
}

QVariantMap QSysMLPlugin::metaData(const QString & name) const
{
	QVariantMap m;
	m["plugin"] = QSysMLPlugin::staticName();
    if (name == "action"){
        m["itemType"] = "shape";
        m["itemClass"] = "action";
	} else if (name == "action.accept_event"){
        m["itemType"] = "shape";
        m["itemClass"] = "action";
	} else if (name == "action.send_signal"){
        m["itemType"] = "shape";
        m["itemClass"] = "action";
    } else if (name == "block"){
        m["itemType"] = "shape";
        m["itemClass"] = "block";
    } else if (name == "node.decision"){
        m["itemType"] = "shape";
        m["itemClass"] = "node";
    } else if (name == "node.initial"){
        m["itemType"] = "shape";
        m["itemClass"] = "node";
    } else if (name == "node.final.activity"){
        m["itemType"] = "shape";
        m["itemClass"] = "node";
    } else if (name == "node.final.flow"){
        m["itemType"] = "shape";
        m["itemClass"] = "node";
    } else if (name == "node.fork.horizontal"){
        m["itemType"] = "shape";
        m["itemClass"] = "node";
    } else if (name == "node.fork"){
        m["itemType"] = "shape";
        m["itemClass"] = "node";
    } else if (name == "node.join"){
        m["itemType"] = "shape";
        m["itemClass"] = "node";
    } else if (name == "node.transition"){
        m["itemType"] = "shape";
        m["itemClass"] = "node";
    }

	return m;
}

QMap<QString, QVariant> QSysMLPlugin::defaultProperties(const QString & name) const
{
    QMap<QString, QVariant> properties;
    if (name == "action"){
        properties["actionType"] = "default";
	} else if (name == "action.send_signal"){
        properties["actionType"] = "sendSignal";
    } else if (name == "block"){
        properties["blockType"] = "default";
	} else if (name == "flow.control"){
		properties["font"] = QDiagramProperty::toMap(QFont("Arial", 10));
		QPen pen(Qt::black);
		pen.setWidthF(2.0);
		pen.setStyle(Qt::DashLine);
		properties["lineColor"] = QDiagramProperty::toMap(pen);
	} else if (name == "node.decision"){
        properties["nodeType"] = "decision";
		properties["font"] = QDiagramProperty::toMap(QFont("Arial", 10));
		QPen pen(Qt::black);
		pen.setWidthF(2.0);
		pen.setStyle(Qt::SolidLine);
		properties["lineColor"] = QDiagramProperty::toMap(pen);
    } else if (name == "node.initial"){
        properties["nodeType"] = "initial";
    } else if (name == "node.final.activity"){
        properties["nodeType"] = "final.activity";
    } else if (name == "node.final.flow"){
        properties["nodeType"] = "final.flow";
    } else if (name == "node.fork.horizontal"){
        properties["nodeType"] = "fork";
        properties["alignment"] = Qt::Horizontal;
    } else if (name == "node.fork"){
        properties["nodeType"] = "fork";
        properties["alignment"] = Qt::Horizontal;
    } else if (name == "node.join"){
        properties["nodeType"] = "join";
        properties["alignment"] = Qt::Horizontal;
    } else if (name == "node.transition"){
        properties["nodeType"] = "transition";
		properties["font"] = QDiagramProperty::toMap(QFont("Arial", 10));
		QPen pen(Qt::black);
		pen.setWidthF(2.0);
		properties["lineColor"] = QDiagramProperty::toMap(pen);
    }
    return properties;
}

QStringList QSysMLPlugin::shapes(const QString & group, QAbstractDiagram *diagram) const
{
    QStringList s;
    QStringList ad;
    QStringList ac;
    QStringList bl;
    if (group == tr("Blocks")){
        bl << "block";
        return bl;
    } else if (group == tr("Nodes")){
        ad << "node.initial"
           << "node.final.flow"
           << "node.final.activity"
           << "node.fork.horizontal"
           << "node.fork"
           << "node.join"
           << "node.decision"
           << "node.merge"
           << "node.transition";
        return ad;
    } else if (group == tr("Actions")){
        ac << "action"
          << "action.send_signal"
          << "action.accept_event";
        return ac;
    } else if (group.isEmpty()){
        s.append(ad);
        s.append(ac);
        s.append(bl);
    }
    return s;
}

QString QSysMLPlugin::staticName()
{
	return "SysML";
}

QString QSysMLPlugin::title(const QString & name) const
{
    if (name == "block"){
        return tr("Block");
    } else if (name == "node.initial"){
        return tr("Initial Node");
    } else if (name == "node.final.activity"){
        return tr("Final Activity Node");
    } else if (name == "node.final.flow"){
        return tr("Final Flow Node");
    } else if (name == "node.fork.horizontal"){
        return tr("Fork Node (horizontal)");
    } else if (name == "node.fork"){
        return tr("Fork Node");
    } else if (name == "node.join"){
        return tr("Join Node");
    } else if (name == "node.decision"){
        return tr("Decision Node");
    } else if (name == "parallelogramm"){
        return tr("Parallelogramm");
    } else if (name == "node.merge"){
        return tr("Merge Node");
    } else if (name == "node.transition"){
        return tr("Transition Node");
    } else if (name == "action"){
        return tr("Action");
    } else if (name == "action.send_signal"){
        return tr("Send Signal Action");
    } else if (name == "action.accept_event"){
        return tr("Accept Event Action");
    }
    return QString::null;
}

QString QSysMLPlugin::toolTip(const QString & shape) const
{
    return QString::null;
}

Q_EXPORT_PLUGIN2(qsysmlplugin, QSysMLPlugin)

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
#include "qsysmlnode.h"

QString QSysMLPlugin::name() const
{
	return QSysMLPlugin::staticName();
}

QList<QDiagramConnectorStyle> QSysMLPlugin::connectors() const
{
    QList<QDiagramConnectorStyle> mStyles;

//    mStyles << QDiagramConnectorStyle(name(), QIcon(":/qstandardformsplugin/connector.default"), "Default", "default");
//    mStyles << QDiagramConnectorStyle(name(), QIcon(":/qstandardformsplugin/connector.line"), "Line", "line");
//    mStyles << QDiagramConnectorStyle(name(), QIcon(":/qstandardformsplugin/connector.line.ae"), "Line Arrow End", "line.ae");
    return mStyles;
}

QAbstractDiagramGraphicsItem *QSysMLPlugin::createItem(const QMap<QString,QVariant> & metaData, const QMap<QString, QVariant> &properties, QGraphicsScene *scene)
{
    QAbstractDiagramGraphicsItem* item = 0;
    if (!properties.contains("uuid")){
        return 0;
    }
    if (metaData.value("itemType") == "connector"){
//        item = new QStandardShapeConnector(properties);
    } else if (metaData.value("itemType") == "line"){
//        props["p1"] = QPointF(properties.value("x").toDouble(), properties.value("y").toDouble());
//        props["p2"] = props.value("p1").toPointF() + QPointF(100, 100);
//        item = new QStandardLine(props);
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

QDiagram *QSysMLPlugin::diagram(const QString &type, QObject *parent) const
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


QPointF QSysMLPlugin::hotSpot(const QString &name) const
{
    QPointF p(0, 0);
    if (name == "node.initial" || name == "gate.nand" || name == "gate.nor"){
        p = QPointF(-19.5, -19.5);
    } else if (name == "gate.not"){
        p = QPointF(0, -26);
    }
    return p;
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
    }
    return QIcon();
}

QList<QDiagramLineStyle> QSysMLPlugin::lineStyles() const
{
    QList<QDiagramLineStyle> styles;
//    QPen p;

//    p = QPen(Qt::black);
//    p.setStyle(Qt::DashLine);
//    styles.append(QDiagramLineStyle(name(), "dashed", tr("Dashed"), p, QIcon(":/qstandardformsplugin/line.dashed")));

//    p = QPen(Qt::black);
//    p.setStyle(Qt::DashDotDotLine);
//    styles.append(QDiagramLineStyle(name(), "dash_dot_dot", tr("Dash Dot Dot"), p, QIcon(":/qstandardformsplugin/line.dash_dot_dot")));

//    p = QPen(Qt::black);
//    p.setStyle(Qt::DashDotLine);
//    styles.append(QDiagramLineStyle(name(), "dash_dot", tr("Dash Dot"), p, QIcon(":/qstandardformsplugin/line.dash_dot")));

//    p = QPen(Qt::DotLine);
//    p.setColor(QColor(Qt::black));
//    styles.append(QDiagramLineStyle(name(), "dotted", tr("Dotted"), p, QIcon(":/qstandardformsplugin/line.dotted")));

//    p = QPen(Qt::black);
//    p.setStyle(Qt::SolidLine);
//    styles.append(QDiagramLineStyle(name(), "solid", tr("Solid"), p, QIcon(":/qstandardformsplugin/line.solid")));

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
    } else if (name == "node.fork.horizontal"){
        m["itemType"] = "shape";
        m["itemClass"] = "node";
    } else if (name == "node.fork"){
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
    } else if (name == "block"){
        properties["blockType"] = "default";
    } else if (name == "node.decision"){
        properties["nodeType"] = "decision";
    } else if (name == "node.initial"){
        properties["nodeType"] = "initial";
    } else if (name == "node.final.activity"){
        properties["nodeType"] = "final.activity";
    } else if (name == "node.fork.horizontal"){
        properties["nodeType"] = "fork";
        properties["alignment"] = Qt::Horizontal;
    } else if (name == "node.fork"){
        properties["nodeType"] = "fork";
        properties["alignment"] = Qt::Horizontal;
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
           << "node.merge";
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
    } else if (name == "action"){
        return tr("Action");
    } else if (name == "action.send_signal"){
        return tr("Send Signal Action");
    } else if (name == "action.accept_event"){
        return tr("Accept Event Action");
//    } else if (name == "trapezoid"){
//        return tr("Trapezoid");
//    } else if (name == "triangle.right_angle"){
//        return tr("Right Angle Triangle ");
//    } else if (name == "triangle.isosceles"){
//        return tr(" Isosceles Triangle");
    }
    return QString::null;
}

QString QSysMLPlugin::toolTip(const QString & shape) const
{
    return QString::null;
}

Q_EXPORT_PLUGIN2(qsysmlplugin, QSysMLPlugin)

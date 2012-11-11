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
#include "qstandardformsplugin.h"

#include <qabstractdiagram.h>
#include <qdiagram.h>
#include <qdiagramlinestyle.h>

#include "qstandardblockshape.h"
#include "qstandardline.h"
#include "qstandardshapeconnector.h"

QString QStandardFormsPlugin::name() const
{
	return QStandardFormsPlugin::staticName();
}

QList<QDiagramConnectorStyle> QStandardFormsPlugin::connectors() const
{
    QList<QDiagramConnectorStyle> l;

    l << QDiagramConnectorStyle(name(), QIcon(":/qstandardformsplugin/connector.default"), "Default", "default");
    l << QDiagramConnectorStyle(name(), QIcon(":/qstandardformsplugin/connector.line"), "Line", "line");
    l << QDiagramConnectorStyle(name(), QIcon(":/qstandardformsplugin/connector.line.ae"), "Line Arrow End", "line.ae");
    return l;
}

QAbstractDiagramGraphicsItem *QStandardFormsPlugin::createItem(const QMap<QString,QVariant> & metaData, const QMap<QString, QVariant> &properties, QGraphicsScene *scene)
{
    QVariantMap props(properties);
    QAbstractDiagramGraphicsItem* item = 0;
    if (!properties.contains("uuid")){
        return 0;
    }
    if (metaData.value("itemType") == "connector"){
        item = new QStandardShapeConnector(properties);
    } else if (metaData.value("itemType") == "line"){
        props["p1"] = QPointF(properties.value("x").toDouble(), properties.value("y").toDouble());
        props["p2"] = props.value("p1").toPointF() + QPointF(100, 100);
        item = new QStandardLine(props);
    } else if (metaData.value("itemType") == "shape"){
		item = new QStandardBlockShape(metaData.value("itemClass").toString(), properties);
    }
    return item;
}

QDiagram *QStandardFormsPlugin::diagram(const QString &type, QObject *parent) const
{
    QDiagram* d = new QDiagram(parent);
    d->addPlugin(name());
    return d;
}

QStringList QStandardFormsPlugin::diagrams() const
{
    QStringList n;
    n << "Standard Diagram";
    return n;
}

QList<QDiagramEndOfLineStyle> QStandardFormsPlugin::endOfLineStyles() const
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

QStringList QStandardFormsPlugin::groups(QAbstractDiagram *diagram) const
{
    Q_UNUSED(diagram);
    QStringList g;
    g  << tr("Blocks") << tr("Lines");
    return g;
}

QIcon QStandardFormsPlugin::icon(const QString &name) const
{
    if (name == "circle"){
        return QIcon(":/qstandardformsplugin/circle");
    } else if (name == "cross"){
        return QIcon(":/qstandardformsplugin/cross");
    } else if (name == "diamond"){
        return QIcon(":/qstandardformsplugin/diamond");
    } else if (name == "hexagon"){
        return QIcon(":/qstandardformsplugin/hexagon");
    } else if (name == "lines.line"){
        return QIcon(":/qstandardformsplugin/lines.line");
    } else if (name == "octagon"){
        return QIcon(":/qstandardformsplugin/octagon");
    } else if (name == "parallelogramm"){
        return QIcon(":/qstandardformsplugin/parallelogramm");
    } else if (name == "rectangle"){
        return QIcon(":/qstandardformsplugin/rectangle");
    } else if (name == "rectangle.rounded"){
        return QIcon(":/qstandardformsplugin/rectangle.rounded");
    } else if (name == "square"){
        return QIcon(":/qstandardformsplugin/square");
    } else if (name == "square.rounded"){
        return QIcon(":/qstandardformsplugin/square.rounded");
    } else if (name == "trapezoid"){
        return QIcon(":/qstandardformsplugin/trapezoid");
    } else if (name == "triangle.right_angle"){
        return QIcon(":/qstandardformsplugin/triangle.right_angle");
    } else if (name == "triangle.isosceles"){
        return QIcon(":/qstandardformsplugin/triangle.isosceles");
    }
    return QIcon();
}

void QStandardFormsPlugin::initializeProperties(QAbstractDiagramGraphicsItem *item)
{
    if (item == 0){
        return;
    }
    if (item->property("itemType") == "line" || item->property("itemType") == "shape"){
        item->setProperty("lineStyle", QVariant::fromValue(QDiagram::lineStyle("solid@"+this->name())));
    }
}

QList<QDiagramLineStyle> QStandardFormsPlugin::lineStyles() const
{
    QList<QDiagramLineStyle> styles;
    QPen p;

    p = QPen(Qt::black);
    p.setStyle(Qt::DashLine);
    styles.append(QDiagramLineStyle(name(), "dashed", tr("Dashed"), p, QIcon(":/qstandardformsplugin/line.dashed")));

    p = QPen(Qt::black);
    p.setStyle(Qt::DashDotDotLine);
    styles.append(QDiagramLineStyle(name(), "dash_dot_dot", tr("Dash Dot Dot"), p, QIcon(":/qstandardformsplugin/line.dash_dot_dot")));

    p = QPen(Qt::black);
    p.setStyle(Qt::DashDotLine);
    styles.append(QDiagramLineStyle(name(), "dash_dot", tr("Dash Dot"), p, QIcon(":/qstandardformsplugin/line.dash_dot")));

    p = QPen(Qt::DotLine);
    p.setColor(QColor(Qt::black));
    styles.append(QDiagramLineStyle(name(), "dotted", tr("Dotted"), p, QIcon(":/qstandardformsplugin/line.dotted")));

    p = QPen(Qt::black);
    p.setStyle(Qt::SolidLine);
    styles.append(QDiagramLineStyle(name(), "solid", tr("Solid"), p, QIcon(":/qstandardformsplugin/line.solid")));

    return styles;
}

QAbstractDiagramShape *QStandardFormsPlugin::restoreShape(const QString &uuid, const QString &style, const QMap<QString, QVariant> &properties, QGraphicsScene *scene)
{
    // TODO obsolete
    return 0;
}

QMap<QString, QVariant> QStandardFormsPlugin::defaultProperties(const QString & name) const
{
    QMap<QString, QVariant> properties;
    QVariantMap p;
    properties["rotation"] = 0.0;
    if (name == "hexagon"){
        properties["angle"] = 30.0;
    }
    if (name == "circle"){
        properties["thickness"] = 50.0;
    }
    if (name == "cross" || name == "octagon"){
        properties["ratio"] = 50.0;
    }
    if (name == "lines.line"){
        QRectF r;
        r.setWidth(100.0);
        r.setHeight(100.0);
        properties["geometry"] = r;
    }
    if (name == "parallelogramm" || name == "trapeziod"){
        properties["angle"] = 10.0;
    }
    if (name == "square.rounded" || name == "rectangle.rounded"){
        properties["radius"] = 10.0;
    }
	properties["zlevel"] = 50;
    return properties;
}

QVariantMap QStandardFormsPlugin::metaData(const QString & name) const
{
	QVariantMap m;
	m["plugin"] = QStandardFormsPlugin::staticName();
	if (name.startsWith("lines.")){
		m["itemType"] = "line";
	} else {
		m["itemType"] = "shape";
	}
	m["itemClass"] = name;

	return m;
}

QStringList QStandardFormsPlugin::shapes(const QString & group, QAbstractDiagram *diagram) const
{
    QStringList s;

    if (group == "Lines"){
        s << "lines.line";
    } else if (group == tr("Blocks")){
        s << "rectangle" << "rectangle.rounded" << "square" << "square.rounded" << "diamond"
          << "parallelogramm"
          << "trapezoid"
          << "triangle.isosceles"
          << "triangle.right_angle"
          << "hexagon" << "octagon"
          << "cross" << "circle";
    }
    return s;
}

QString QStandardFormsPlugin::staticName()
{
    return "Standard Forms";
}

QString QStandardFormsPlugin::title(const QString & name) const
{
    if (name == "circle"){
        return tr("Circle");
    } else if (name == "cross"){
        return tr("Cross");
    } else if (name == "diamond"){
        return tr("Diamond");
    } else if (name == "hexagon"){
        return tr("Hexagon");
    } else if (name == "lines.line"){
        return tr("Line");
    } else if (name == "octagon"){
        return tr("Octagon");
    } else if (name == "parallelogramm"){
        return tr("Parallelogramm");
    } else if (name == "rectangle"){
        return tr("Rectangle");
    } else if (name == "rectangle.rounded"){
        return tr("Rounded Rectangle");
    } else if (name == "square"){
        return tr("Square");
    } else if (name == "square.rounded"){
        return tr("Rounded Square");
    } else if (name == "trapezoid"){
        return tr("Trapezoid");
    } else if (name == "triangle.right_angle"){
        return tr("Right Angle Triangle ");
    } else if (name == "triangle.isosceles"){
        return tr(" Isosceles Triangle");
    }
    return QString::null;
}

QString QStandardFormsPlugin::toolTip(const QString &shape) const
{
    return QString::null;
}

Q_EXPORT_PLUGIN2(qstandardformsplugin, QStandardFormsPlugin)

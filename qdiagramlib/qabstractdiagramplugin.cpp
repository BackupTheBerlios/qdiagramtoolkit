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
#include "qabstractdiagramplugin.h"

#include "qabstractdiagramgraphicsitem.h"
#include "qdiagram.h"

QAbstractDiagramPlugin::~QAbstractDiagramPlugin()
{
}

QList<QDiagramConnectorStyle> QAbstractDiagramPlugin::connectors() const
{
    QList<QDiagramConnectorStyle> l;
	QMapIterator<QString, ShapeConnectorClass> it(m_connectors);
	while(it.hasNext()){
		it.next();
		l << QDiagramConnectorStyle(name(), it.value().icon, it.value().title, it.key());
	}
    return l;
}

QAbstractDiagramGraphicsItem* QAbstractDiagramPlugin::createItem(const QMap<QString,QVariant> & metaData, const QMap<QString, QVariant> &properties)
{
    if (!properties.contains("uuid")){
        return 0;
    }
    if (metaData.value("itemType") == "Connector"){
		if (m_connectors.contains(metaData.value("itemClass").toString())){
			ShapeConnectorClass c = m_connectors.value(metaData.value("itemClass").toString());
			return reinterpret_cast<QAbstractDiagramGraphicsItem*>(c.creator(properties));
		}
    } else if (metaData.value("itemType") == "Shape"){
		if (m_shapes.contains(metaData.value("itemClass").toString())){
			ShapeClass c = m_shapes.value(metaData.value("itemClass").toString());
			return c.creator(properties);
		}
    }
    return 0;
}

QMap<QString,QVariant> QAbstractDiagramPlugin::defaultProperties(const QString & id) const
{
	QMapIterator<QString, ShapeClass> itShapes(registeredShapes());
	while(itShapes.hasNext()){
		itShapes.next();
		QMapIterator<QString, ShapeData> itData(itShapes.value().shapeData);
		while(itData.hasNext()){
			itData.next();
			if (itData.key() == id){
				ShapeClass c = itShapes.value();
				return c.defaultProperties(id);
			}
		}
	}
	return QVariantMap();
}

QAbstractDiagram* QAbstractDiagramPlugin::diagram(const QString & id, QObject* parent) const
{
	QAbstractDiagram* d = 0;
	if (m_diagrams.contains(id)){
		PluginDiagramData pdd = m_diagrams.value(id);
		d = (QAbstractDiagram*)pdd.creator(name(), parent);
	}
	if (d == 0){
		d = new QDiagram(parent);
	}
	d->addPlugin(name());
    return d;
}

QStringList QAbstractDiagramPlugin::diagrams() const
{
	return m_diagrams.keys();
}

QList<QDiagramEndOfLineStyle> QAbstractDiagramPlugin::endOfLineStyles() const
{
	return QList<QDiagramEndOfLineStyle>();
}

QStringList QAbstractDiagramPlugin::groups(QAbstractDiagram *diagram) const
{
    QStringList g;
	QMapIterator<QString, ShapeClass> itShapes(m_shapes);
	while(itShapes.hasNext()){
		itShapes.next();
		QMapIterator<QString, ShapeData> itData(itShapes.value().shapeData);
		while(itData.hasNext()){
			itData.next();
			if (!g.contains(itData.value().group)){
				g << itData.value().group;
			}
		}
	}
    return g;
}

QPointF QAbstractDiagramPlugin::hotSpot(const QString & id) const
{
	QMapIterator<QString, ShapeClass> itShapes(registeredShapes());
	while(itShapes.hasNext()){
		itShapes.next();
		QMapIterator<QString, ShapeData> itData(itShapes.value().shapeData);
		while(itData.hasNext()){
			itData.next();
			if (itData.key() == id){
				ShapeClass c = itShapes.value();
				return c.hotSpot(id);
			}
		}
	}
    return QPointF(0, 0);
}

//QPointF QAbstractDiagramPlugin::hotSpot(const QVariantMap & metaData, const QVariantMap & properties) const
//{
//	if (metaData.value("itemType") == "Shape"){
//		if (m_shapes.contains(metaData.value("itemClass").toString())){
//			ShapeClass c = m_shapes.value(metaData.value("itemClass").toString());
//			return c.hotSpot(properties);
//		}
//	}
//    return QPointF(0, 0);
//}

QIcon QAbstractDiagramPlugin::icon(const QString & name) const
{
	QMapIterator<QString, ShapeClass> itShapes(registeredShapes());
	while(itShapes.hasNext()){
		itShapes.next();
		QMapIterator<QString, ShapeData> itData(itShapes.value().shapeData);
		while(itData.hasNext()){
			itData.next();
			if (itData.key() == name){
				return itData.value().icon;
			}
		}
	}
	return QIcon();
}

void QAbstractDiagramPlugin::initialize()
{
}

QString QAbstractDiagramPlugin::itemClass(const QString & id) const
{
	QMapIterator<QString, ShapeClass> itShapes(registeredShapes());
	while(itShapes.hasNext()){
		itShapes.next();
		QMapIterator<QString, ShapeData> itData(itShapes.value().shapeData);
		while(itData.hasNext()){
			itData.next();
			if (itData.key() == id){
				return itShapes.key();
			}
		}
	}
	return QString::null;
}

QVariantMap QAbstractDiagramPlugin::metaData(const QString & id) const
{
	QMapIterator<QString, ShapeClass> itShapes(registeredShapes());
	while(itShapes.hasNext()){
		itShapes.next();
		QMapIterator<QString, ShapeData> itData(itShapes.value().shapeData);
		while(itData.hasNext()){
			itData.next();
			if (itData.key() == id){
				ShapeClass c = itShapes.value();
				return c.staticMetaData();
			}
		}
	}
	return QVariantMap();
}

QList<QAction*> QAbstractDiagramPlugin::shapeActions(const QString &style) const
{
    return QList<QAction*>();
}

QStringList QAbstractDiagramPlugin::shapes(const QString & group, QAbstractDiagram* diagram) const
{
	QStringList l;
	QMapIterator<QString, ShapeClass> itShapes(m_shapes);
	while(itShapes.hasNext()){
		itShapes.next();
		QMapIterator<QString, ShapeData> itData(itShapes.value().shapeData);
		while(itData.hasNext()){
			itData.next();
			if (itData.value().group == group || group.isEmpty()){
				l << itData.key();
			}
		}
	}
	return l;
}

QString QAbstractDiagramPlugin::title(const QString & id) const
{
	QMapIterator<QString, PluginDiagramData> itDiagrams(m_diagrams);
	while(itDiagrams.hasNext()){
		itDiagrams.next();
		if (itDiagrams.key() == id){
			return itDiagrams.value().title;
		}
	}

	QMapIterator<QString, ShapeClass> itShapes(m_shapes);
	while(itShapes.hasNext()){
		itShapes.next();
		QMapIterator<QString, ShapeData> itData(itShapes.value().shapeData);
		while(itData.hasNext()){
			itData.next();
			if (itData.key() == id){
				return itData.value().title;
			}
		}
	}
	return QString::null;
}

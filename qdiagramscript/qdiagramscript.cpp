/******************************************************************************
** Copyright (C) 2011 - 2012 Martin Hoppe martin@2x2hoppe.de
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
#include "qdiagramscript.h"

#include "qdiagramtoolkit.h"

#include "qabstractdiagramgraphicsitemobject.h"
#include "qabstractdiagramshapeconnectionpointobject.h"
#include "qabstractdiagramshapeconnectorobject.h"
#include "qabstractdiagramshapeobject.h"
#include "qdiagramconnectorstyleobject.h"
#include "qdiagramlayerobject.h"
#include "qdiagramlayersobject.h"
#include "qdiagrammetadataobject.h"
#include "qdiagrammetapropertyobject.h"
#include "qdiagramobject.h"
#include "qdiagrampluginloaderobject.h"
#include "qdiagrampropertyobject.h"
#include "qdiagramreaderobject.h"
#include "qdiagramstylesheetobject.h"
#include "qdiagramviewobject.h"
#include "qdiagramwriterobject.h"

QDiagramScriptExtensionPlugin::QDiagramScriptExtensionPlugin(QObject* parent ) :
QScriptExtensionPlugin(parent)
{

}

QDiagramScriptExtensionPlugin::~QDiagramScriptExtensionPlugin()
{

}

void QDiagramScriptExtensionPlugin::initialize(const QString & key, QScriptEngine* engine)
{
	if (key != "qdiagramtoolkit"){
		return;
	}
	//QAbstractDiagramShapeClass* adsc = new QAbstractDiagramShapeClass(engine);
	//engine->globalObject().setProperty("QAbstractDiagramShape", adsc->constructor());
	//QAbstractDiagramGraphicsItemClass* adgic = new QAbstractDiagramGraphicsItemClass(engine);
	//engine->globalObject().setProperty("QAbstractDiagramGraphicsItem", adgic->constructor());

	QDiagramPluginLoaderClass* dplc = new QDiagramPluginLoaderClass(engine);
	engine->globalObject().setProperty("QDiagramPluginLoader", dplc->constructor());

	QScriptValue qdtk = engine->newQMetaObject(&QDiagramToolkit::staticMetaObject);
	engine->globalObject().setProperty("QDiagramToolkit", qdtk);

	initAbstractDiagramGraphicsItem(engine);
	initAbstractDiagramShape(engine);
	initAbstractDiagramShapeConnectionPoint(engine);
	initAbstractDiagramShapeConnector(engine);
	initDiagram(engine);
	initDiagramConnectorStyle(engine);
	initDiagramLayer(engine);
	initDiagramLayers(engine);
	initDiagramMetaData(engine);
	initDiagramMetaProperty(engine);
	initDiagramProperty(engine);
	initDiagramReader(engine);
	initDiagramStyleSheet(engine);
	initDiagramWriter(engine);
	initDiagramView(engine);
}

QStringList QDiagramScriptExtensionPlugin::keys() const
{
	QStringList l;
	l << "qdiagramtoolkit";
	return l;
}

Q_EXPORT_STATIC_PLUGIN(QDiagramScriptExtensionPlugin)
Q_EXPORT_PLUGIN2(qdiagramscriptextensionplugin, QDiagramScriptExtensionPlugin)

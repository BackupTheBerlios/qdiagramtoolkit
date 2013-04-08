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
#include "qabstractdiagramshapeconnectorobject.h"
#include "qabstractdiagramshapeconnectorobject_p.h"
#include "qdiagramscriptutils.h"

#include "qabstractdiagramshapeconnector.h"

static QScriptValue ctor(QScriptContext* context, QScriptEngine* engine)
{
	QAbstractDiagramShapeConnectorDummy* i = new QAbstractDiagramShapeConnectorDummy();
	return engine->newVariant(qVariantFromValue(i));
}

DEF_PROPERTY_GETTER(QAbstractDiagramShapeConnector, end, connectionPointAtEnd);
DEF_PROPERTY_SETTER_READONLY(QAbstractDiagramShapeConnector, end);

DEF_PROPERTY_GETTER(QAbstractDiagramShapeConnector, start, connectionPointAtStart);
DEF_PROPERTY_SETTER_READONLY(QAbstractDiagramShapeConnector, start);

//static QScriptValue get_end(QScriptContext* context, QScriptEngine* engine)
//{
//	DECLARE_SELF(QDiagramMetaData, indexOfProperty);
//	QScriptValue obj = engine->newObject();
//
//	return obj;
//}
//
//static QScriptValue get_start(QScriptContext* context, QScriptEngine* engine)
//{
//	DECLARE_SELF(QDiagramMetaData, indexOfProperty);
//	QScriptValue obj = engine->newObject();
//
//	return obj;
//}

void initAbstractDiagramShapeConnector(QScriptEngine* engine)
{
	QAbstractDiagramShapeConnectorDummy* i = new QAbstractDiagramShapeConnectorDummy();
	QScriptValue proto = engine->newVariant(qVariantFromValue(i));
	ADD_PROPERTY(proto, end);
	ADD_PROPERTY(proto, start);

	engine->setDefaultPrototype(qMetaTypeId<QAbstractDiagramShapeConnector*>(), proto );
	qScriptRegisterSequenceMetaType<QList<QAbstractDiagramShapeConnector*> >(engine);

	QScriptValue f = engine->newFunction(ctor);
	engine->globalObject().setProperty("QAbstractDiagramShapeConnector", f);
}

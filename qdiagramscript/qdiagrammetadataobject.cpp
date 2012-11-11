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
#include "qdiagrammetadataobject.h"
#include "qdiagramscriptutils.h"

#include <qdiagrammetadata.h>

static QScriptValue ctor(QScriptContext* context, QScriptEngine* engine)
{
	return engine->newVariant(qVariantFromValue(new QDiagramMetaData()));
}

DEF_PROPERTY_GETTER(QDiagramMetaData, itemClass, itemClass);
DEF_PROPERTY_SETTER_READONLY(QDiagramMetaData, itemClass);

DEF_PROPERTY_GETTER(QDiagramMetaData, itemType, itemType);
DEF_PROPERTY_SETTER_READONLY(QDiagramMetaData, itemType);

DEF_PROPERTY_GETTER(QDiagramMetaData, pluginName, pluginName);
DEF_PROPERTY_SETTER_READONLY(QDiagramMetaData, pluginName);

DEF_PROPERTY_GETTER(QDiagramMetaData, propertyCount, propertyCount);
DEF_PROPERTY_SETTER_READONLY(QDiagramMetaData, propertyCount);

static QScriptValue indexOfProperty(QScriptContext* context, QScriptEngine* engine)
{
	DECLARE_SELF(QDiagramMetaData, indexOfProperty);
	CHECK_ARG_COUNT(1)
	return qScriptValueFromValue(engine, self->indexOfProperty(context->argument(0).toString()));
}

static QScriptValue property(QScriptContext* context, QScriptEngine* engine)
{
	DECLARE_SELF(QDiagramMetaData, property);
	CHECK_ARG_COUNT(1)
	return qScriptValueFromValue(engine, self->property(context->argument(0).toInt32()));
}

void initDiagramMetaData(QScriptEngine* engine)
{
	QScriptValue proto = engine->newVariant(qVariantFromValue(new QDiagramMetaData()));
	ADD_METHOD(proto, indexOfProperty);
	ADD_METHOD(proto, property);
	ADD_PROPERTY(proto, itemClass);
	ADD_PROPERTY(proto, itemType);
	ADD_PROPERTY(proto, pluginName);
	ADD_PROPERTY(proto, propertyCount);

	engine->setDefaultPrototype(qMetaTypeId<QDiagramMetaData*>(), proto);

	QScriptValue f = engine->newFunction(ctor);
	engine->globalObject().setProperty("QDiagramMetaData", f);
}

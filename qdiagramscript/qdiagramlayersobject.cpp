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
#include "qdiagramlayersobject.h"
#include "qdiagramscriptutils.h"

#include <qdiagramlayers.h>

static QScriptValue ctor(QScriptContext *context, QScriptEngine *engine)
{
	return engine->newVariant(qVariantFromValue(new QDiagramLayers()));
}

static QScriptValue add(QScriptContext* context, QScriptEngine* engine)
{
	DECLARE_SELF(QDiagramLayers, indexOf);
	CHECK_ARG_COUNT(1);
	return qScriptValueFromValue(engine, self->add(context->argument(0).toString()));
}

static QScriptValue indexOf(QScriptContext* context, QScriptEngine* engine)
{
	DECLARE_SELF(QDiagramLayers, indexOf);
	CHECK_ARG_COUNT(1);
	return qScriptValueFromValue(engine, self->indexOf(context->argument(0).toString()));
}

static QScriptValue isLocked(QScriptContext* context, QScriptEngine* engine)
{
	DECLARE_SELF(QDiagramLayers, isLocked);
	CHECK_ARG_COUNT(1);
	QAbstractDiagramGraphicsItem* i = qscriptvalue_cast<QAbstractDiagramGraphicsItem*>(context->argument(0));
	if (i){
		return qScriptValueFromValue(engine, self->isLocked(i));
	}
	return true;
}

static QScriptValue layer(QScriptContext* context, QScriptEngine* engine)
{
	DECLARE_SELF(QDiagramLayers, layer);
	CHECK_ARG_COUNT(1);
	if (context->argument(0).isNumber()){
		return qScriptValueFromValue(engine, self->layer(context->argument(0).toInt32()));
	}
	if (context->argument(0).isString()){
		return qScriptValueFromValue(engine, self->layer(context->argument(0).toString()));
	}
	return engine->undefinedValue();
}

static QScriptValue layers(QScriptContext* context, QScriptEngine* engine)
{
	DECLARE_SELF(QDiagramLayers, isLocked);
	CHECK_ARG_COUNT(1);
	QAbstractDiagramGraphicsItem* i = qscriptvalue_cast<QAbstractDiagramGraphicsItem*>(context->argument(0));
	if (i){
		return qScriptValueFromValue(engine, self->layers(i));
	}
	return engine->undefinedValue();
}

static QScriptValue setActiveLayer(QScriptContext* context, QScriptEngine* engine)
{
	DECLARE_SELF(QDiagramLayers, setActiveLayer);
	CHECK_ARG_COUNT(1);
	self->setActiveLayer(context->argument(0).toString());
	return engine->undefinedValue();
}

DEF_PROPERTY_GETTER(QDiagramLayers, activeLayer, activeLayer)
DEF_PROPERTY_SETTER_READONLY(QDiagramLayers, activeLayer)

DEF_PROPERTY_GETTER(QDiagramLayers, count, count)
DEF_PROPERTY_SETTER_READONLY(QDiagramLayers, count)

void initDiagramLayers(QScriptEngine* engine)
{
	QScriptValue proto = engine->newVariant(qVariantFromValue(new QDiagramLayers()));
	ADD_PROPERTY(proto, activeLayer);
	ADD_PROPERTY(proto, count);

	ADD_METHOD(proto, add);
	ADD_METHOD(proto, indexOf);
	ADD_METHOD(proto, isLocked);
	ADD_METHOD(proto, layer);
	ADD_METHOD(proto, layers);

	engine->setDefaultPrototype(qMetaTypeId<QDiagramLayers*>(), proto);

	QScriptValue f = engine->newFunction(ctor);
	engine->globalObject().setProperty("QDiagramLayers", f);
}

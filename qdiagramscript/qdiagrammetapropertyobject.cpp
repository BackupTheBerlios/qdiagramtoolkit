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
#include "qdiagrammetapropertyobject.h"
#include "qdiagramscriptutils.h"

#include <qdiagramtoolkit.h>
#include <qdiagrammetaproperty.h>

static QScriptValue ctor(QScriptContext* context, QScriptEngine* engine)
{
	return engine->newVariant(qVariantFromValue(QDiagramMetaProperty()));
}

DEF_PROPERTY_GETTER(QDiagramMetaProperty, name, name)
DEF_PROPERTY_SETTER_READONLY(QDiagramMetaProperty, name)

DEF_PROPERTY_GETTER(QDiagramMetaProperty, propertyCount, propertyCount)
DEF_PROPERTY_SETTER_READONLY(QDiagramMetaProperty, propertyCount)

DEF_PROPERTY_GETTER(QDiagramMetaProperty, type, type)
DEF_PROPERTY_SETTER_READONLY(QDiagramMetaProperty, type)

DEF_PROPERTY_GETTER(QDiagramMetaProperty, dynamic, isDynamic)
DEF_PROPERTY_SETTER_READONLY(QDiagramMetaProperty, dynamic)

DEF_PROPERTY_GETTER(QDiagramMetaProperty, readOnly, isReadOnly)
DEF_PROPERTY_SETTER_READONLY(QDiagramMetaProperty, readOnly)

DEF_PROPERTY_GETTER(QDiagramMetaProperty, valid, isValid)
DEF_PROPERTY_SETTER_READONLY(QDiagramMetaProperty, valid)

static QScriptValue indexOf(QScriptContext* context, QScriptEngine* engine)
{
	DECLARE_SELF(QDiagramMetaProperty, indexOf);
	CHECK_ARG_COUNT(1)
	return qScriptValueFromValue(engine, self->indexOf(context->argument(0).toString()));
}

static QScriptValue property(QScriptContext* context, QScriptEngine* engine)
{
	DECLARE_SELF(QDiagramMetaProperty, property);
	CHECK_ARG_COUNT(1)
	return qScriptValueFromValue(engine, self->property(context->argument(0).toInt32()));
}

void initDiagramMetaProperty(QScriptEngine* engine)
{
	QScriptValue proto = engine->newVariant(qVariantFromValue(QDiagramMetaProperty()));
	ADD_PROPERTY(proto, dynamic);
	ADD_PROPERTY(proto, name);
	ADD_PROPERTY(proto, propertyCount);
	ADD_PROPERTY(proto, type);
	ADD_PROPERTY(proto, readOnly);
	ADD_PROPERTY(proto, valid);

	ADD_METHOD(proto, indexOf);
	ADD_METHOD(proto, property);

	engine->setDefaultPrototype(qMetaTypeId<QDiagramMetaProperty>(), proto);

	QScriptValue f = engine->newFunction(ctor);
	engine->globalObject().setProperty("QDiagramMetaProperty", f);
}

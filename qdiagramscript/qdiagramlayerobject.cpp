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
#include "qdiagramlayerobject.h"
#include "qdiagramscriptutils.h"

#include <qdiagramlayer.h>

static QScriptValue ctor(QScriptContext *context, QScriptEngine *engine)
{
	return engine->newVariant(qVariantFromValue(new QDiagramLayer()));
}

static QScriptValue add(QScriptContext* context, QScriptEngine* engine)
{
	DECLARE_SELF(QDiagramLayer, add);
	CHECK_ARG_COUNT(1);
	QAbstractDiagramGraphicsItem* i = qscriptvalue_cast<QAbstractDiagramGraphicsItem*>(context->argument(0));
	if (i){
		self->add(i);
	}
	return engine->undefinedValue();
}

static QScriptValue contains(QScriptContext* context, QScriptEngine* engine)
{
	DECLARE_SELF(QDiagramLayer, contains);
	CHECK_ARG_COUNT(1);
	QAbstractDiagramGraphicsItem* i = qscriptvalue_cast<QAbstractDiagramGraphicsItem*>(context->argument(0));
	if (i){
		return qScriptValueFromValue(engine, self->contains(i));
	}
	return qScriptValueFromValue(engine, false);
}

static QScriptValue remove(QScriptContext* context, QScriptEngine* engine)
{
	DECLARE_SELF(QDiagramLayer, remove);
	CHECK_ARG_COUNT(1);
	QAbstractDiagramGraphicsItem* i = qscriptvalue_cast<QAbstractDiagramGraphicsItem*>(context->argument(0));
	if (i){
		self->remove(i);
	}
	return engine->undefinedValue();
}

DEF_PROPERTY_GETTER(QDiagramLayer, items, items)
DEF_PROPERTY_SETTER_READONLY(QDiagramLayer, items)

DEF_PROPERTY_GETTER(QDiagramLayer, locked, isLocked)
DEF_PROPERTY_SETTER_CAST(QDiagramLayer, locked, setLocked, toBool())

DEF_PROPERTY_GETTER(QDiagramLayer, name, name)
DEF_PROPERTY_SETTER_CAST(QDiagramLayer, name, setName, toString())

DEF_PROPERTY_GETTER(QDiagramLayer, visible, isVisible)
DEF_PROPERTY_SETTER_CAST(QDiagramLayer, visible, setVisible, toBool())

void initDiagramLayer(QScriptEngine* engine)
{
	QScriptValue proto = engine->newVariant(qVariantFromValue(new QDiagramLayer()));
	ADD_PROPERTY(proto, items);
	ADD_PROPERTY(proto, locked);
	ADD_PROPERTY(proto, name);
	ADD_PROPERTY(proto, visible);
	ADD_METHOD(proto, add);
	ADD_METHOD(proto, contains);
	ADD_METHOD(proto, remove);

	engine->setDefaultPrototype(qMetaTypeId<QDiagramLayer*>(), proto);

	QScriptValue f = engine->newFunction(ctor);
	engine->globalObject().setProperty("QDiagramLayer", f);
}

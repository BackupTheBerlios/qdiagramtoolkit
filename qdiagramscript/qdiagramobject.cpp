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
#include "qdiagramobject.h"
#include "qdiagramscriptutils.h"

#include <qdiagram.h>
#include <qdiagramstylesheet.h>
#include <qdiagramconnectorstyle.h>

static QScriptValue ctor(QScriptContext *context, QScriptEngine *engine)
{
	return engine->newQObject(new QDiagram());
}

static QScriptValue addPlugin(QScriptContext* context, QScriptEngine* engine)
{
	DECLARE_SELF(QDiagram, addPlugin);
	CHECK_ARG_COUNT(1);
	self->addPlugin(context->argument(0).toString());
	return engine->undefinedValue();
}

static QScriptValue addShape(QScriptContext* context, QScriptEngine* engine)
{
	DECLARE_SELF(QDiagram, addShape);
	CHECK_ARG_COUNT(4);
	QString p = context->argument(0).toString();
	QString i = context->argument(1).toString();
	qreal x = context->argument(2).toNumber();
	qreal y = context->argument(3).toNumber();

	return qScriptValueFromValue(engine, self->addShape(p, i, QPointF(x, y)));
}

static QScriptValue clearSelection(QScriptContext* context, QScriptEngine* engine)
{
	DECLARE_SELF(QDiagram, clearSelection);
	self->clearSelection();
	return engine->undefinedValue();
}

static QScriptValue connectShapes(QScriptContext* context, QScriptEngine* engine)
{
	DECLARE_SELF(QDiagram, connectShapes);
	if (context->argument(0).isString()){
		QDiagramConnectorStyle s = qscriptvalue_cast<QDiagramConnectorStyle>(context->argument(4));
		return qScriptValueFromValue(engine, self->connectShapes(context->argument(0).toString(), context->argument(1).toString(), context->argument(2).toString(), context->argument(3).toString(), s));
	}
	return engine->undefinedValue();
}

static QScriptValue findItemByUuid(QScriptContext* context, QScriptEngine* engine)
{
	DECLARE_SELF(QDiagram, findItemByUuid);
	CHECK_ARG_COUNT(1);
	return qScriptValueFromValue(engine, self->findItemByUuid(context->argument(0).toString()));
}

static QScriptValue removeItem(QScriptContext* context, QScriptEngine* engine)
{
	DECLARE_SELF(QDiagram, removeItem);
	CHECK_ARG_COUNT(1);
	return qScriptValueFromValue(engine, self->removeItem(context->argument(0).toString()));
}

static QScriptValue select(QScriptContext* context, QScriptEngine* engine)
{
	DECLARE_SELF(QDiagram, select);
	CHECK_ARG_COUNT(1);
	self->select(context->argument(0).toString());
	return engine->undefinedValue();
}

static QScriptValue selectAll(QScriptContext* context, QScriptEngine* engine)
{
	DECLARE_SELF(QDiagram, selectAll);
	self->selectAll();
	return engine->undefinedValue();
}

DEF_PROPERTY_GETTER(QDiagram, author, author)
DEF_PROPERTY_SETTER_CAST(QDiagram, author, setAuthor, toString())

DEF_PROPERTY_GETTER(QDiagram, modified, isModified)
DEF_PROPERTY_SETTER_READONLY(QDiagram, modified)

DEF_PROPERTY_GETTER(QDiagram, items, items)
DEF_PROPERTY_SETTER_READONLY(QDiagram, items)

DEF_PROPERTY_GETTER(QDiagram, pluginName, pluginName)
DEF_PROPERTY_SETTER_READONLY(QDiagram, pluginName)

DEF_PROPERTY_GETTER(QDiagram, plugins, plugins)
DEF_PROPERTY_SETTER_READONLY(QDiagram, plugins)

DEF_PROPERTY_GETTER(QDiagram, selectedItems, selectedItems)
DEF_PROPERTY_SETTER_READONLY(QDiagram, selectedItems)

DEF_PROPERTY_GETTER(QDiagram, shapes, shapes)
DEF_PROPERTY_SETTER_READONLY(QDiagram, shapes)

DEF_PROPERTY_GETTER(QDiagram, styleSheet, styleSheet)
DEF_PROPERTY_SETTER_READONLY(QDiagram, styleSheet)

DEF_PROPERTY_GETTER(QDiagram, title, title)
DEF_PROPERTY_SETTER_CAST(QDiagram, title, setTitle, toString())

void initDiagram(QScriptEngine* engine)
{
	QScriptValue proto = engine->newVariant(qVariantFromValue(new QDiagram()));
	ADD_METHOD(proto, addPlugin);
	ADD_METHOD(proto, addShape);
	ADD_METHOD(proto, clearSelection);
	ADD_METHOD(proto, connectShapes);
	ADD_METHOD(proto, findItemByUuid);
	ADD_METHOD(proto, removeItem);
	ADD_METHOD(proto, select);
	ADD_METHOD(proto, selectAll);
	ADD_PROPERTY(proto, author);
	ADD_PROPERTY(proto, items);
	ADD_PROPERTY(proto, modified);
	ADD_PROPERTY(proto, pluginName);
	ADD_PROPERTY(proto, plugins);
	ADD_PROPERTY(proto, selectedItems);
	ADD_PROPERTY(proto, shapes);
	ADD_PROPERTY(proto, styleSheet);
	ADD_PROPERTY(proto, title);

	engine->setDefaultPrototype(qMetaTypeId<QAbstractDiagram*>(), proto);

	QScriptValue f = engine->newFunction(ctor);
	engine->globalObject().setProperty("QAbstractDiagram", f);
}

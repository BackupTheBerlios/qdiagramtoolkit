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
#include "qabstractdiagramshapeobject.h"
#include "qabstractdiagramshapeobject_p.h"

#include "qdiagramscriptutils.h"

static QScriptValue ctor(QScriptContext* context, QScriptEngine* engine)
{
	QAbstractDiagramShapeDummy* i = new QAbstractDiagramShapeDummy(0);
	return engine->newVariant(qVariantFromValue(i));
}

DEF_PROPERTY_GETTER(QAbstractDiagramShape, connectionPoints, connectionPoints)
DEF_PROPERTY_SETTER_READONLY(QAbstractDiagramShape, connectionPoints)

DEF_PROPERTY_GETTER(QAbstractDiagramShape, metaData, metaData)
DEF_PROPERTY_SETTER_READONLY(QAbstractDiagramShape, metaData)

DEF_PROPERTY_GETTER(QAbstractDiagramShape, uuid, uuid)
DEF_PROPERTY_SETTER_READONLY(QAbstractDiagramShape, uuid)

static QScriptValue connectionPoint(QScriptContext* context, QScriptEngine* engine)
{
	DECLARE_SELF(QAbstractDiagramShape, connectionPoint);
	CHECK_ARG_COUNT(1);
	QAbstractDiagramShapeConnectionPoint* cp = self->connectionPoint(context->argument(0).toString());
	if (cp){
		return qScriptValueFromValue(engine, cp);
	}
	return engine->undefinedValue();
}

static QScriptValue property(QScriptContext* context, QScriptEngine* engine)
{
	DECLARE_SELF(QAbstractDiagramShape, property);
	CHECK_ARG_COUNT(1);
	return qScriptValueFromValue(engine, self->property(context->argument(0).toString()));
}

static QScriptValue moveBy(QScriptContext* context, QScriptEngine* engine)
{
	DECLARE_SELF(QAbstractDiagramShape, moveBy);
	CHECK_ARG_COUNT(2);
	self->moveBy(context->argument(0).toNumber(), context->argument(1).toNumber());
	return engine->undefinedValue();
}

void initAbstractDiagramShape(QScriptEngine* engine)
{
	QAbstractDiagramGraphicsItem* i = new QAbstractDiagramShapeDummy();
	QScriptValue proto = engine->newVariant(qVariantFromValue(i));
	ADD_PROPERTY(proto, connectionPoints);
	ADD_PROPERTY(proto, metaData);
	ADD_PROPERTY(proto, uuid);
	ADD_METHOD(proto, connectionPoint);
	ADD_METHOD(proto, moveBy);
	ADD_METHOD(proto, property);

	engine->setDefaultPrototype(qMetaTypeId<QAbstractDiagramShape*>(), proto );
	qScriptRegisterSequenceMetaType<QList<QAbstractDiagramShape*> >(engine);

	QScriptValue f = engine->newFunction(ctor);
	engine->globalObject().setProperty("QAbstractDiagramShape", f);
}

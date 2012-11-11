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
#include "qdiagrampropertyobject.h"
#include "qdiagramproperty.h"

#include "qdiagramscriptutils.h"

static QScriptValue ctor(QScriptContext* context, QScriptEngine* engine)
{
	return engine->newVariant(qVariantFromValue(QDiagramProperty()));
}

static QScriptValue value(QScriptContext* context, QScriptEngine* engine)
{
	DECLARE_SELF(QDiagramProperty, value);
	return qScriptValueFromValue(engine, self->value());
}

DEF_PROPERTY_GETTER(QDiagramProperty, value, value)
DEF_PROPERTY_SETTER(QDiagramProperty, value, setValue)

void initDiagramProperty(QScriptEngine* engine)
{
	QScriptValue proto = engine->newVariant(qVariantFromValue(QDiagramProperty()));
	ADD_PROPERTY(proto, value);
	//ADD_METHOD(proto, setFileName);
	//ADD_METHOD(proto, write);

	engine->setDefaultPrototype(qMetaTypeId<QDiagramProperty>(), proto );

	QScriptValue f = engine->newFunction(ctor);
    engine->globalObject().setProperty("QDiagramProperty", f);
}

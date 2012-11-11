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
#include "qdiagramstylesheetobject.h"
#include "qdiagramscriptutils.h"

#include <qdiagramstylesheet.h>

static QScriptValue ctor(QScriptContext* context, QScriptEngine* engine)
{
	//if (context->argumentCount() == 1 ){
	//	return engine->newVariant(qVariantFromValue(new QXmlStreamReader(context->argument(0).toString())));
	//}
	return engine->newVariant(qVariantFromValue(new QDiagramStyleSheet()));
}

static QScriptValue connector(QScriptContext* context, QScriptEngine* engine)
{
	DECLARE_SELF(QDiagramStyleSheet, connector);
	CHECK_ARG_COUNT(2)
	return qScriptValueFromValue(engine, self->connector(context->argument(0).toString(), context->argument(1).toString()));
}

static QScriptValue connectors(QScriptContext* context, QScriptEngine* engine)
{
	DECLARE_SELF(QDiagramStyleSheet, connectors);
	return qScriptValueFromValue(engine, self->connectors());
}

void initDiagramStyleSheet(QScriptEngine* engine)
{
	QScriptValue proto = engine->newVariant(qVariantFromValue(new QDiagramStyleSheet()));
	ADD_METHOD(proto, connector);
	ADD_METHOD(proto, connectors);
	//ADD_METHOD(proto, findItemByUuid);

	engine->setDefaultPrototype(qMetaTypeId<QDiagramStyleSheet*>(), proto);

	QScriptValue f = engine->newFunction(ctor);
	engine->globalObject().setProperty("QDiagramStyleSheet", f);
}

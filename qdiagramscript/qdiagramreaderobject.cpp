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
#include "qdiagramreaderobject.h"
#include "qdiagramscriptutils.h"

#include <qabstractdiagram.h>
#include "qdiagram.h"
#include "qdiagramreader.h"

static QScriptValue ctor(QScriptContext* context, QScriptEngine* engine)
{
	if (context->argumentCount() == 1){
		if (context->argument(0).isString()){
			return engine->newVariant(qVariantFromValue(new QDiagramReader(context->argument(0).toString())));
		}
	}
	return engine->newVariant(qVariantFromValue(new QDiagramReader()));
}

static QScriptValue read(QScriptContext* context, QScriptEngine* engine)
{
	DECLARE_SELF(QDiagramReader, read);
	return qScriptValueFromValue(engine, self->read());
}

DEF_PROPERTY_GETTER(QDiagramReader, errorString, errorString)
DEF_PROPERTY_SETTER_READONLY(QDiagramReader, errorString)

void initDiagramReader(QScriptEngine* engine)
{
	QDiagramReader* i = new QDiagramReader();
	QScriptValue proto = engine->newVariant(qVariantFromValue(i));
	ADD_PROPERTY(proto, errorString)	
	ADD_METHOD(proto, read);

	engine->setDefaultPrototype(qMetaTypeId<QDiagramReader*>(), proto );

	QScriptValue f = engine->newFunction(ctor);
	engine->globalObject().setProperty("QDiagramReader", f);
}

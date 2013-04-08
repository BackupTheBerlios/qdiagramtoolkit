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
#include "qdiagramviewobject.h"
#include "qdiagram.h"
#include "qdiagramview.h"

#include "qdiagramscriptutils.h"

Q_DECLARE_METATYPE(QDiagramView*)

static QScriptValue ctor(QScriptContext* context, QScriptEngine* engine)
{
	//if (context->argumentCount() == 1 ){
	//	return engine->newVariant(qVariantFromValue(new QXmlStreamReader(context->argument(0).toString())));
	//}
	return engine->newVariant(qVariantFromValue(new QDiagramView()));
}

static QScriptValue setDiagram(QScriptContext* context, QScriptEngine* engine)
{
	DECLARE_SELF(QDiagramView, setDiagram);
	if (context->argumentCount() == 1){
		QAbstractDiagram* d = qscriptvalue_cast<QAbstractDiagram*>(context->argument(0));
		if (d){
			self->setDiagram(d);
		}
	}
	return engine->undefinedValue();
}

void initDiagramView(QScriptEngine* engine)
{
	QScriptValue proto = engine->newVariant(qVariantFromValue(new QDiagramView()));
	ADD_METHOD(proto, setDiagram);
	//ADD_METHOD(proto, setFileName);
	//ADD_METHOD(proto, write);

	engine->setDefaultPrototype(qMetaTypeId<QDiagramView*>(), proto);

	QScriptValue f = engine->newFunction(ctor);
    engine->globalObject().setProperty("QDiagramView", f);
}

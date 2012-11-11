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
#include "qdiagramconnectorstyleobject.h"
#include "qdiagramscriptutils.h"

#include <qdiagramconnectorstyle.h>

static QScriptValue ctor(QScriptContext* context, QScriptEngine* engine)
{
	return engine->newVariant(qVariantFromValue(QDiagramConnectorStyle()));
}

DEF_PROPERTY_GETTER(QDiagramConnectorStyle, name, name)
DEF_PROPERTY_SETTER_READONLY(QDiagramConnectorStyle,name)

DEF_PROPERTY_GETTER(QDiagramConnectorStyle, plugin, plugin)
DEF_PROPERTY_SETTER_READONLY(QDiagramConnectorStyle, plugin)

DEF_PROPERTY_GETTER(QDiagramConnectorStyle, valid, isValid)
DEF_PROPERTY_SETTER_READONLY(QDiagramConnectorStyle, valid)

void initDiagramConnectorStyle(QScriptEngine* engine)
{
	QScriptValue proto = engine->newVariant(qVariantFromValue(QDiagramConnectorStyle()));
	ADD_PROPERTY(proto, plugin)
	ADD_PROPERTY(proto, name)
	ADD_PROPERTY(proto, valid)

	engine->setDefaultPrototype(qMetaTypeId<QDiagramConnectorStyle>(), proto);
    qScriptRegisterSequenceMetaType<QList<QDiagramConnectorStyle> >(engine);

	QScriptValue f = engine->newFunction(ctor);
	engine->globalObject().setProperty("QDiagramConnectorStyle", f);
}

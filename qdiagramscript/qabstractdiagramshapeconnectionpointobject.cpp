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
#include "qabstractdiagramshapeconnectionpointobject.h"
#include "qabstractdiagramshapeconnectionpointobject_p.h"
#include "qdiagramscriptutils.h"

#include "qabstractdiagramshape.h"
#include "qabstractdiagramshapeconnectionpoint.h"
#include "qabstractdiagramshapeconnector.h"

static QScriptValue ctor(QScriptContext* context, QScriptEngine* engine)
{
	QAbstractDiagramShapeConnectionPointDummy* i = new QAbstractDiagramShapeConnectionPointDummy();
	return engine->newVariant(qVariantFromValue(i));
}

DEF_PROPERTY_GETTER(QAbstractDiagramShapeConnectionPoint, count, count)
DEF_PROPERTY_SETTER_READONLY(QAbstractDiagramShapeConnectionPoint, count)

DEF_PROPERTY_GETTER(QAbstractDiagramShapeConnectionPoint, connectors, connectors)
DEF_PROPERTY_SETTER_READONLY(QAbstractDiagramShapeConnectionPoint, connectors)

DEF_PROPERTY_GETTER_CAST(QAbstractDiagramShapeConnectionPoint, orientation, orientation, int)
DEF_PROPERTY_SETTER_READONLY(QAbstractDiagramShapeConnectionPoint, orientation)

DEF_PROPERTY_GETTER(QAbstractDiagramShapeConnectionPoint, shape, parentShape)
DEF_PROPERTY_SETTER_READONLY(QAbstractDiagramShapeConnectionPoint, shape)

void initAbstractDiagramShapeConnectionPoint(QScriptEngine* engine)
{
	QAbstractDiagramShapeConnectionPoint* i = new QAbstractDiagramShapeConnectionPointDummy();
	QScriptValue proto = engine->newVariant(qVariantFromValue(i));
	ADD_PROPERTY(proto, count);
	ADD_PROPERTY(proto, connectors);
	ADD_PROPERTY(proto, orientation);
	ADD_PROPERTY(proto, shape);
	//ADD_METHOD(proto, moveBy);
	//ADD_METHOD(proto, property);

	engine->setDefaultPrototype(qMetaTypeId<QAbstractDiagramShapeConnectionPoint*>(), proto );
	qScriptRegisterSequenceMetaType<QList<QAbstractDiagramShapeConnectionPoint*> >(engine);

	QScriptValue f = engine->newFunction(ctor);
	engine->globalObject().setProperty("QAbstractDiagramShapeConnectionPoint", f);
}

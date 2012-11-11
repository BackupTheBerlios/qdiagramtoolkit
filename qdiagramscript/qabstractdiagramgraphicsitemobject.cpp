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
#include "qabstractdiagramgraphicsitemobject.h"
#include "qdiagramscriptutils.h"

#include <qabstractdiagramgraphicsitem.h>
#include <qdiagrammetadata.h>

QAbstractDiagramGraphicsItemObject::QAbstractDiagramGraphicsItemObject(const QString & uuid, const QString & pluginName, const QString & itemType, const QString & itemClass, QGraphicsItem* parent) :
QAbstractDiagramGraphicsItem(uuid, pluginName, itemType, itemClass, parent)
{
}

QAbstractDiagramGraphicsItemObject::~QAbstractDiagramGraphicsItemObject()
{
}

QRectF QAbstractDiagramGraphicsItemObject::boundingRect() const
{
	return QRectF();
}

void QAbstractDiagramGraphicsItemObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
}

static QScriptValue ctor(QScriptContext* context, QScriptEngine* engine)
{
	QAbstractDiagramGraphicsItem* i = new QAbstractDiagramGraphicsItemObject("", "", "", "");
	return engine->newVariant(qVariantFromValue(i));
}

DEF_PROPERTY_GETTER(QAbstractDiagramGraphicsItem, metaData, metaData)
DEF_PROPERTY_SETTER_READONLY(QAbstractDiagramGraphicsItem,metaData)

DEF_PROPERTY_GETTER(QAbstractDiagramGraphicsItem, uuid, uuid)
DEF_PROPERTY_SETTER_READONLY(QAbstractDiagramGraphicsItem,uuid)

static QScriptValue property(QScriptContext* context, QScriptEngine* engine)
{
	DECLARE_SELF(QAbstractDiagramGraphicsItem, property);
	CHECK_ARG_COUNT(1);
	return qScriptValueFromValue(engine, self->property(context->argument(0).toString()));
}

static QScriptValue moveBy(QScriptContext* context, QScriptEngine* engine)
{
	DECLARE_SELF(QAbstractDiagramGraphicsItem, moveBy);
	CHECK_ARG_COUNT(2);
	self->moveBy(context->argument(0).toNumber(), context->argument(1).toNumber());
	return engine->undefinedValue();
}

void initAbstractDiagramGraphicsItem(QScriptEngine* engine)
{
	QAbstractDiagramGraphicsItem* i = new QAbstractDiagramGraphicsItemObject("", "", "", "");
	QScriptValue proto = engine->newVariant(qVariantFromValue(i));
	ADD_PROPERTY(proto,metaData);
	ADD_PROPERTY(proto,uuid);
	ADD_METHOD(proto, moveBy);
	ADD_METHOD(proto, property);

	engine->setDefaultPrototype(qMetaTypeId<QAbstractDiagramGraphicsItem*>(), proto );
	qScriptRegisterSequenceMetaType<QList<QAbstractDiagramGraphicsItem*> >(engine);

	QScriptValue f = engine->newFunction(ctor);
	engine->globalObject().setProperty("QAbstractDiagramGraphicsItem", f);
}

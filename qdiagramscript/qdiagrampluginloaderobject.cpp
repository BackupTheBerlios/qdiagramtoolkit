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
#include "qdiagrampluginloaderobject.h"

#include <qdiagrampluginloader.h>

Q_DECLARE_METATYPE(QDiagram*)
Q_DECLARE_METATYPE(QDiagramPluginLoader)
Q_DECLARE_METATYPE(QDiagramPluginLoader*)
Q_DECLARE_METATYPE(QDiagramPluginLoaderClass*)

QDiagramPluginLoaderPrototype::QDiagramPluginLoaderPrototype(QObject *parent) :
QObject(parent)
{
}

QDiagramPluginLoaderPrototype::~QDiagramPluginLoaderPrototype()
{
}

QAbstractDiagram* QDiagramPluginLoaderPrototype::diagram(const QString & plugin, const QString & type)
{
	return QDiagramPluginLoader::diagram(plugin, type);
}

QStringList QDiagramPluginLoaderPrototype::plugins() const
{
	return QDiagramPluginLoader::plugins();
}

QDiagramPluginLoader* QDiagramPluginLoaderPrototype::thisClass() const
{
	return qscriptvalue_cast<QDiagramPluginLoader*>(thisObject().data());
}

QDiagramPluginLoaderClass::QDiagramPluginLoaderClass(QScriptEngine *engine) :
QObject(engine), QScriptClass(engine)
{
	qScriptRegisterMetaType<QDiagramPluginLoader>(engine, toScriptValue, fromScriptValue);

	m_proto = engine->newQObject(new QDiagramPluginLoaderPrototype(this),
		QScriptEngine::QtOwnership,
		QScriptEngine::SkipMethodsInEnumeration
		| QScriptEngine::ExcludeSuperClassMethods
		| QScriptEngine::ExcludeSuperClassProperties);
	QScriptValue global = engine->globalObject();
	m_proto.setPrototype(global.property("Object").property("prototype"));

	m_ctor = engine->newFunction(construct);
	m_ctor.setData(qScriptValueFromValue(engine, this));
}

QDiagramPluginLoaderClass::~QDiagramPluginLoaderClass()
{
}

QScriptValue QDiagramPluginLoaderClass::construct(QScriptContext* context, QScriptEngine* engine)
{
	QDiagramPluginLoaderClass* c = qscriptvalue_cast<QDiagramPluginLoaderClass*>(context->callee().data());
	if (!c){
		return QScriptValue();
	}
	return c->newInstance();
}

QScriptValue QDiagramPluginLoaderClass::constructor()
{
	return m_ctor;
}

QString QDiagramPluginLoaderClass::name() const
{
	return QLatin1String("QDiagramPluginLoader");
}

QScriptValue QDiagramPluginLoaderClass::newInstance()
{
	QScriptValue d = engine()->newVariant(qVariantFromValue(QDiagramPluginLoader()));
	return engine()->newObject(this, d);
}

QScriptValue QDiagramPluginLoaderClass::newInstance(const QDiagramPluginLoader & other)
{
	QScriptValue d = engine()->newVariant(qVariantFromValue(other));
	return engine()->newObject(this, d);
}

QScriptValue QDiagramPluginLoaderClass::property(const QScriptValue &object, const QScriptString & name, uint id)
{
	QDiagramPluginLoader* c = qscriptvalue_cast<QDiagramPluginLoader*>(object.data());
	if (!c){
		return QScriptValue();
	}
	return QScriptValue();
}

QScriptValue::PropertyFlags QDiagramPluginLoaderClass::propertyFlags( const QScriptValue & object , const QScriptString & name, uint id )
{
	//if (name == cPropertyCurrentEngineer) {
	//	return QScriptValue::Undeletable
	//		| QScriptValue::SkipInEnumeration 
	//		| QScriptValue::ReadOnly;
	//}
	return QScriptValue::Undeletable;
}

QScriptValue QDiagramPluginLoaderClass::prototype() const
{
	return m_proto;
}

QScriptClass::QueryFlags QDiagramPluginLoaderClass::queryProperty( const QScriptValue & object, const QScriptString & name, QScriptClass::QueryFlags flags, uint* id )
{
	QDiagramPluginLoader* c = qscriptvalue_cast<QDiagramPluginLoader*>(object.data());
	if (!c){
		return 0;
	}
	//if (name == cPropertyCurrentEngineer){
	//	return flags;
	//}
	return 0;
}

void QDiagramPluginLoaderClass::setProperty( QScriptValue & object, const QScriptString &name, uint id, const QScriptValue & value )
{
	QDiagramPluginLoader *c= qscriptvalue_cast<QDiagramPluginLoader*>(object.data());
	if (!c){
		return;
	}
}
QScriptValue QDiagramPluginLoaderClass::toScriptValue(QScriptEngine* engine, const QDiagramPluginLoader & o)
{
	QScriptValue ctor = engine->globalObject().property("QDiagramPluginLoader");
	QDiagramPluginLoaderClass* c = qscriptvalue_cast<QDiagramPluginLoaderClass*>(ctor.data());
	if (!c){
		return engine->newVariant(qVariantFromValue(c));
	}
	return c->newInstance(o);
}

void QDiagramPluginLoaderClass::fromScriptValue(const QScriptValue & value, QDiagramPluginLoader & o)
{
	o = qscriptvalue_cast<QDiagramPluginLoader>(value.data());
}

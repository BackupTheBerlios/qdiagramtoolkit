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
#include "qdiagramwriterobject.h"

#include <qdiagram.h>
#include <qdiagramwriter.h>

#include "qdiagramscriptutils.h"

Q_DECLARE_METATYPE(QDiagramWriter*);

static QScriptValue ctor(QScriptContext* context, QScriptEngine* engine)
{
	//if (context->argumentCount() == 1 ){
	//	return engine->newVariant(qVariantFromValue(new QXmlStreamReader(context->argument(0).toString())));
	//}
	return engine->newVariant(qVariantFromValue(new QDiagramWriter()));
}

//static QScriptValue errorString(QScriptContext* context, QScriptEngine* engine)
//{
//	DECLARE_SELF(QDiagramWriter, setFileName);
//	return qScriptValueFromValue(engine, self->errorString());
//}

static QScriptValue setFileName(QScriptContext* context, QScriptEngine* engine)
{
	DECLARE_SELF(QDiagramWriter, setFileName);
	if (context->argumentCount() == 1){
		self->setFileName(context->argument(0).toString());
	}
	return engine->undefinedValue();
}

static QScriptValue write(QScriptContext* context, QScriptEngine* engine)
{
	DECLARE_SELF(QDiagramWriter, write);
	if (context->argumentCount() == 1){
		QDiagram* d = qscriptvalue_cast<QDiagram*>(context->argument(0));
		if (d){
			return qScriptValueFromValue(engine, self->write(d));
		}
	}
	return engine->undefinedValue();
}

DEF_PROPERTY_GETTER(QDiagramWriter, errorString, errorString)
DEF_PROPERTY_SETTER_READONLY(QDiagramWriter, errorString)

void initDiagramWriter(QScriptEngine* engine)
{
	QScriptValue proto = engine->newVariant(qVariantFromValue(new QDiagramWriter()));
	ADD_PROPERTY(proto, errorString)
	ADD_METHOD(proto, setFileName);
	ADD_METHOD(proto, write);

	engine->setDefaultPrototype( qMetaTypeId<QDiagramWriter*>(), proto );

	QScriptValue f = engine->newFunction(ctor);
    engine->globalObject().setProperty("QDiagramWriter", f);
}

//Q_DECLARE_METATYPE(QDiagramWriter)
//Q_DECLARE_METATYPE(QDiagramWriter*)
//Q_DECLARE_METATYPE(QDiagramWriterClass*)
//
//QDiagramWriterPrototype::QDiagramWriterPrototype(QObject *parent) :
//QObject(parent)
//{
//}
//
//QDiagramWriterPrototype::~QDiagramWriterPrototype()
//{
//}
//
//void QDiagramWriterPrototype::setFileName(const QString & fileName)
//{
//	thisClass()->setFileName(fileName);
//}
//
//QStringList QDiagramWriterPrototype::supportedDiagramFormats() const
//{
//	QStringList l;
//	Q_FOREACH(QByteArray d, QDiagramWriter::supportedDiagramFormats()){
//		l << QString(d);
//	}
//	return l;
//}
//
//QDiagramWriter* QDiagramWriterPrototype::thisClass() const
//{
//	return qscriptvalue_cast<QDiagramWriter*>(thisObject().data());
//}
//
//bool QDiagramWriterPrototype::write(QAbstractDiagram* diagram)
//{
//	return thisClass()->write(diagram);
//}
//
//QDiagramWriterClass::QDiagramWriterClass(QScriptEngine *engine) :
//QObject(engine), QScriptClass(engine)
//{
//	qScriptRegisterMetaType<QDiagramWriter>(engine, toScriptValue, fromScriptValue);
//
//	m_proto = engine->newQObject(new QDiagramWriterPrototype(this),
//		QScriptEngine::QtOwnership,
//		QScriptEngine::SkipMethodsInEnumeration
//		| QScriptEngine::ExcludeSuperClassMethods
//		| QScriptEngine::ExcludeSuperClassProperties);
//	QScriptValue global = engine->globalObject();
//	m_proto.setPrototype(global.property("Object").property("prototype"));
//
//	m_ctor = engine->newFunction(construct);
//	m_ctor.setData(qScriptValueFromValue(engine, this));
//}
//
//QDiagramWriterClass::~QDiagramWriterClass()
//{
//}
//
//QScriptValue QDiagramWriterClass::construct(QScriptContext* context, QScriptEngine* engine)
//{
//	QDiagramWriterClass* c = qscriptvalue_cast<QDiagramWriterClass*>(context->callee().data());
//	if (!c){
//		return QScriptValue();
//	}
//	return c->newInstance();
//}
//
//QScriptValue QDiagramWriterClass::constructor()
//{
//	return m_ctor;
//}
//
//QString QDiagramWriterClass::name() const
//{
//	return QLatin1String("QDiagramWriter");
//}
//
//QScriptValue QDiagramWriterClass::newInstance()
//{
//	QScriptValue d = engine()->newVariant(qVariantFromValue(QDiagramWriter()));
//	return engine()->newObject(this, d);
//}
//
//QScriptValue QDiagramWriterClass::newInstance(const QDiagramWriter & other)
//{
//	QScriptValue d = engine()->newVariant(qVariantFromValue(other));
//	return engine()->newObject(this, d);
//}
//
//QScriptValue QDiagramWriterClass::property(const QScriptValue &object, const QScriptString & name, uint id)
//{
//	QDiagramWriter* c = qscriptvalue_cast<QDiagramWriter*>(object.data());
//	if (!c){
//		return QScriptValue();
//	}
//	return QScriptValue();
//}
//
//QScriptValue::PropertyFlags QDiagramWriterClass::propertyFlags( const QScriptValue & object , const QScriptString & name, uint id )
//{
//	//if (name == cPropertyCurrentEngineer) {
//	//	return QScriptValue::Undeletable
//	//		| QScriptValue::SkipInEnumeration 
//	//		| QScriptValue::ReadOnly;
//	//}
//	return QScriptValue::Undeletable;
//}
//
//QScriptValue QDiagramWriterClass::prototype() const
//{
//	return m_proto;
//}
//
//QScriptClass::QueryFlags QDiagramWriterClass::queryProperty( const QScriptValue & object, const QScriptString & name, QScriptClass::QueryFlags flags, uint* id )
//{
//	QDiagramWriter* c = qscriptvalue_cast<QDiagramWriter*>(object.data());
//	if (!c){
//		return 0;
//	}
//	//if (name == cPropertyCurrentEngineer){
//	//	return flags;
//	//}
//	return 0;
//}
//
//void QDiagramWriterClass::setProperty( QScriptValue & object, const QScriptString & name, uint id, const QScriptValue & value )
//{
//	QDiagramWriter *c= qscriptvalue_cast<QDiagramWriter*>(object.data());
//	if (!c){
//		return;
//	}
//}
//QScriptValue QDiagramWriterClass::toScriptValue(QScriptEngine* engine, const QDiagramWriter & o)
//{
//	QScriptValue ctor = engine->globalObject().property("QDiagramWriter");
//	QDiagramWriterClass* c = qscriptvalue_cast<QDiagramWriterClass*>(ctor.data());
//	if (!c){
//		return engine->newVariant(qVariantFromValue(c));
//	}
//	return c->newInstance(o);
//}
//
//void QDiagramWriterClass::fromScriptValue(const QScriptValue & value, QDiagramWriter & o)
//{
//	o = qscriptvalue_cast<QDiagramWriter>(value.data());
//}

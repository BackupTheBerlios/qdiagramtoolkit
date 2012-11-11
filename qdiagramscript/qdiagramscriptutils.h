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
#ifndef QDIAGRAMSCRIPTUTILS_H
#define QDIAGRAMSCRIPTUTILS_H

#define ADD_METHOD(__p__, __f__) \
	__p__.setProperty(#__f__, __p__.engine()->newFunction(__f__))	

#define DECLARE_SELF( Class, __fn__ ) Class* self = qscriptvalue_cast<Class*>(context->thisObject()); \
    if (!self) { \
        return context->throwError(QScriptContext::TypeError, \
            QString::fromLatin1("%0.prototype.%1: this object is not a %0") \
            .arg(#Class).arg(#__fn__)); \
    }

#define THROW_TYPE_ERROR(_n_,_t_) context->throwError( \
	QScriptContext::TypeError, \
	QString("arg #%1: %2 expected").arg(_n_).arg(#_t_));

#define CHECK_ARG_COUNT(__c__) if (context->argumentCount() == 0){ \
		return context->throwError(QScriptContext::SyntaxError, QObject::tr("argument missing")); \
	} else if (context->argumentCount() < __c__){ \
		return context->throwError(QScriptContext::SyntaxError, QObject::tr("too few arguments")); \
	} else if (context->argumentCount() > __c__){ \
		return context->throwError(QScriptContext::SyntaxError, QObject::tr("too many arguments")); \
	}

#define ADD_PROPERTY(__p__, __n__) ADD_PROPERTY_GETTER(__p__,__n__) \
ADD_PROPERTY_SETTER(__p__,__n__)

#define ADD_PROPERTY_GETTER(__p__, __n__) \
	__p__.setProperty(#__n__, __p__.engine()->newFunction(get_##__n__), QScriptValue::PropertyGetter);

#define ADD_PROPERTY_SETTER(__p__, __n__) \
	__p__.setProperty(#__n__, __p__.engine()->newFunction(set_##__n__), QScriptValue::PropertySetter);

#define DEF_PROPERTY_GETTER(__c__,__n__,__f__) static QScriptValue get_##__n__(QScriptContext* context, QScriptEngine* engine) \
{ \
	__c__* self = qscriptvalue_cast<__c__*>(context->thisObject()); \
    if (!self) { \
        return context->throwError(QScriptContext::TypeError, \
            QString::fromLatin1("%0.prototype.%1: this object is not a %0") \
            .arg(#__c__).arg(#__n__)); \
    }\
	return qScriptValueFromValue(engine, self->##__f__()); \
}

#define DEF_PROPERTY_GETTER_CAST(__c__,__n__,__f__,__t__) static QScriptValue get_##__n__(QScriptContext* context, QScriptEngine* engine) \
{ \
	__c__* self = qscriptvalue_cast<__c__*>(context->thisObject()); \
    if (!self) { \
        return context->throwError(QScriptContext::TypeError, \
            QString::fromLatin1("%0.prototype.%1: this object is not a %0") \
            .arg(#__c__).arg(#__n__)); \
    }\
	return qScriptValueFromValue(engine, (##__t__)	self->##__f__()); \
}

#define DEF_PROPERTY_SETTER(__c__,__n__,__f__) static QScriptValue set_##__n__(QScriptContext* context, QScriptEngine* engine) \
{ \
	__c__* self = qscriptvalue_cast<__c__*>(context->thisObject()); \
    if (!self) { \
        return context->throwError(QScriptContext::TypeError, \
            QString::fromLatin1("%0.prototype.%1: this object is not a %0") \
            .arg(#__c__).arg(#__n__)); \
    }\
	CHECK_ARG_COUNT(1); \
	self->##__f__(context->argument(0).toVariant()); \
	return engine->undefinedValue(); \
}

#define DEF_PROPERTY_SETTER_CAST(__c__,__n__,__f__,__t__) static QScriptValue set_##__n__(QScriptContext* context, QScriptEngine* engine) \
{ \
	__c__* self = qscriptvalue_cast<__c__*>(context->thisObject()); \
    if (!self) { \
        return context->throwError(QScriptContext::TypeError, \
            QString::fromLatin1("%0.prototype.%1: this object is not a %0") \
            .arg(#__c__).arg(#__n__)); \
    }\
	CHECK_ARG_COUNT(1); \
	self->##__f__(context->argument(0).toVariant().##__t__); \
	return engine->undefinedValue(); \
}

#define DEF_PROPERTY_SETTER_READONLY(__c__,__n__) static QScriptValue set_##__n__(QScriptContext* context, QScriptEngine* engine) \
{ \
	__c__* self = qscriptvalue_cast<__c__*>(context->thisObject()); \
    if (!self) { \
        return context->throwError(QScriptContext::TypeError, \
            QString::fromLatin1("%0.prototype.%1: this object is not a %0") \
            .arg(#__c__).arg(#__n__)); \
    }\
	return engine->undefinedValue(); \
}

#endif // QDIAGRAMSCRIPTUTILS_H

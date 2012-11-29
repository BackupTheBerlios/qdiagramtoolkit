#include "stdafx.h"
#include "qabstractdiagramobject.h"

#include <qdiagram.h>

Q_DECLARE_METATYPE(QDiagram*)

static QScriptValue ctor(QScriptContext *context, QScriptEngine *engine)
{
	QDiagram* d = new QDiagram(engine);
	return engine->newQObject(d);
}

QScriptValue toScriptValue(QScriptEngine* engine, QDiagram* const & in)
{
	return engine->newQObject(in);
}

void fromScriptValue(const QScriptValue & object, QDiagram* & out)
{
	out = qobject_cast<QDiagram*>(object.toQObject());	
}

void addQAbstractDiagram(QScriptEngine* engine )
{
	qScriptRegisterMetaType(engine, toScriptValue, fromScriptValue);

    engine->globalObject().setProperty("QDiagram", engine->newFunction(ctor));
}

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

class QDiagram;
#include <qdiagrampluginloader.h>

class QDiagramPluginLoaderPrototype : public QObject, public QScriptable
{
	Q_OBJECT
public:
	QDiagramPluginLoaderPrototype(QObject *parent = 0);
	~QDiagramPluginLoaderPrototype();
public slots:
	QAbstractDiagram* diagram(const QString & plugin, const QString & type);
	QStringList plugins() const;
private:
	QDiagramPluginLoader *thisClass() const;
};

class QDiagramPluginLoaderClass : public QObject, public QScriptClass
{
public:
    QDiagramPluginLoaderClass(QScriptEngine *engine);
    ~QDiagramPluginLoaderClass();

	QScriptValue constructor();
	QString name() const;
	QScriptValue newInstance();
	QScriptValue newInstance(const QDiagramPluginLoader & other);

	QScriptValue property( const QScriptValue &object, const QScriptString & name, uint id );
	QScriptValue::PropertyFlags propertyFlags( const QScriptValue & object , const QScriptString & name, uint id );
	QScriptValue prototype() const;

	QScriptClass::QueryFlags queryProperty( const QScriptValue & object, const QScriptString & name, QScriptClass::QueryFlags flags, uint* id );
	
	void setProperty( QScriptValue & object, const QScriptString &name, uint id, const QScriptValue & value );
private:
	static QScriptValue construct( QScriptContext* context, QScriptEngine* engine );
	static QScriptValue toScriptValue( QScriptEngine* engine, const QDiagramPluginLoader & o);
	static void fromScriptValue(const QScriptValue & value, QDiagramPluginLoader & o);

	QScriptValue m_proto;
	QScriptValue m_ctor;
};

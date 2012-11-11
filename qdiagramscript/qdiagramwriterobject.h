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
class QScriptEngine;

void initDiagramWriter(QScriptEngine* engine);

//#include <qdiagramwriter.h>
//
//class QDiagramWriterPrototype : public QObject, public QScriptable
//{
//	Q_OBJECT
//public:
//	QDiagramWriterPrototype(QObject *parent = 0);
//	~QDiagramWriterPrototype();
//public slots:
//	void setFileName(const QString & fileName);
//
//	QStringList supportedDiagramFormats() const;
//
//	bool write(QAbstractDiagram* diagram);
//private:
//	QDiagramWriter* thisClass() const;
//};
//
//class QDiagramWriterClass : public QObject, public QScriptClass
//{
//public:
//    QDiagramWriterClass(QScriptEngine *engine);
//    ~QDiagramWriterClass();
//
//	QScriptValue constructor();
//	QString name() const;
//	QScriptValue newInstance();
//	QScriptValue newInstance(const QDiagramWriter & other);
//
//	QScriptValue property( const QScriptValue & object, const QScriptString & name, uint id );
//	QScriptValue::PropertyFlags propertyFlags( const QScriptValue & object , const QScriptString & name, uint id );
//	QScriptValue prototype() const;
//
//	QScriptClass::QueryFlags queryProperty( const QScriptValue & object, const QScriptString & name, QScriptClass::QueryFlags flags, uint* id );
//	
//	void setProperty( QScriptValue & object, const QScriptString &name, uint id, const QScriptValue & value );
//private:
//	static QScriptValue construct(QScriptContext* context, QScriptEngine* engine );
//	static QScriptValue toScriptValue(QScriptEngine* engine, const QDiagramWriter & o);
//	static void fromScriptValue(const QScriptValue & value, QDiagramWriter & o);
//
//	QScriptValue m_proto;
//	QScriptValue m_ctor;
//};

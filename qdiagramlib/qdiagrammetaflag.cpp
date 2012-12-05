/******************************************************************************
** Copyright (C) 2011 Martin Hoppe martin@2x2hoppe.de
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
#include "qdiagrammetaflag.h"

#define ADD_FLAG(__m__,__d__,__n__) __m__[__d__::__n__] = QObject::tr(#__n__)

QMap<int, QDiagramMetaFlag> s_flags;

QDiagramMetaFlag::QDiagramMetaFlag()
{
}

QDiagramMetaFlag::QDiagramMetaFlag(const QMap<int, QString> & flags)
{
    m_flags = flags;
}

QDiagramMetaFlag QDiagramMetaFlag::defaultFlag(QDiagramToolkit::PropertyType type)
{
	QMap<int, QDiagramMetaFlag>::iterator it = s_flags.find(type);
	if (it != s_flags.end()){
		return it.value();
	}
	QDiagramMetaFlag f;
	if (type == QDiagramToolkit::Alignment){
		QMap<int, QString> m;
		ADD_FLAG(m, Qt, AlignLeft);
		ADD_FLAG(m, Qt, AlignRight);
		ADD_FLAG(m, Qt, AlignHCenter);
		ADD_FLAG(m, Qt, AlignJustify);
		ADD_FLAG(m, Qt, AlignTop);
		ADD_FLAG(m, Qt, AlignBottom);
		ADD_FLAG(m, Qt, AlignVCenter);
		ADD_FLAG(m, Qt, AlignAbsolute);
		return (*s_flags.insert(type, QDiagramMetaFlag(m)));
	}
	return f;
}

int QDiagramMetaFlag::keyCount() const
{
    return m_flags.keys().size();
}

QString QDiagramMetaFlag::key(int index) const
{
    if (index < m_flags.values().size()){
        return m_flags.values().at(index);
    }
    return QString::null;
}

bool QDiagramMetaFlag::isValid() const
{
    return !m_flags.isEmpty();
}

QMap<QString, int> QDiagramMetaFlag::masks(QDiagramToolkit::PropertyType type)
{
	QMap<QString, int>  m;
	if (type == QDiagramToolkit::Alignment){
		m["horizontal"] = Qt::AlignHorizontal_Mask;
		m["vertical"] = Qt::AlignVertical_Mask;
	}
	return m;
}

QStringList QDiagramMetaFlag::matchingKeys(int value) const
{
	QStringList l;
	for (int i = 0; i < m_flags.keys().size(); i++){
		if (value & m_flags.keys().at(i)){
			l << m_flags.values().at(i);
		}
	}
	return l;
}

int QDiagramMetaFlag::value(int index) const
{
    if (index < m_flags.keys().size()){
        return m_flags.keys().at(index);
    }
    return 0;
}

int QDiagramMetaFlag::value(const QString & key) const
{
	for (int i = 0; i < m_flags.values().size(); i++){
		if (m_flags.values().at(i) == key){
			return m_flags.keys().at(i);
		}
	}
	return 0;
}

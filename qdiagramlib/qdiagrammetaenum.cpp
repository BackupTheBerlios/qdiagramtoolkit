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
#include "qdiagrammetaenum.h"

#define ADD_ENUM(__m__,__d__,__n__) __m__[__d__::__n__] = QObject::tr(#__n__)

QMap<int, QDiagramMetaEnum> s_enums;

QDiagramMetaEnum::QDiagramMetaEnum()
{
}

QDiagramMetaEnum::QDiagramMetaEnum(const QMap<int, QString> pairs, const QMap<int,QIcon> icons)
{
    m_pairs = pairs;
	m_icons = icons;
}

QDiagramMetaEnum QDiagramMetaEnum::defaultEnum(QDiagramToolkit::PropertyType type)
{
	QMap<int, QDiagramMetaEnum>::iterator it = s_enums.find(type);
	if (it != s_enums.end()){
		return it.value();
	}
	QDiagramMetaEnum e;
	if (type == QDiagramToolkit::BrushStyle){
		QMap<int, QString> m;
		ADD_ENUM(m, Qt, NoBrush);
		ADD_ENUM(m, Qt, SolidPattern);
		ADD_ENUM(m, Qt, Dense1Pattern);
		ADD_ENUM(m, Qt, Dense2Pattern);
		ADD_ENUM(m, Qt, Dense3Pattern);
		ADD_ENUM(m, Qt, Dense4Pattern);
		ADD_ENUM(m, Qt, Dense5Pattern);
		ADD_ENUM(m, Qt, Dense6Pattern);
		ADD_ENUM(m, Qt, Dense7Pattern);
		ADD_ENUM(m, Qt, HorPattern);
		ADD_ENUM(m, Qt, VerPattern);
		ADD_ENUM(m, Qt, CrossPattern);
		ADD_ENUM(m, Qt, BDiagPattern);
		ADD_ENUM(m, Qt, FDiagPattern);
		ADD_ENUM(m, Qt, DiagCrossPattern);
		ADD_ENUM(m, Qt, LinearGradientPattern);
		ADD_ENUM(m, Qt, ConicalGradientPattern);
		ADD_ENUM(m, Qt, RadialGradientPattern);
		ADD_ENUM(m, Qt, TexturePattern);
		return (*s_enums.insert(type, QDiagramMetaEnum(m)));
	} else if (type == QDiagramToolkit::Orientation){
		QMap<int, QString> m;
		ADD_ENUM(m, Qt, Horizontal);
		ADD_ENUM(m, Qt, Vertical);
		return (*s_enums.insert(type, QDiagramMetaEnum(m)));
	} else if (type == QDiagramToolkit::PenJoinStyle){
		QMap<int, QString> m;
		ADD_ENUM(m, Qt, MiterJoin);
		ADD_ENUM(m, Qt, BevelJoin);
		ADD_ENUM(m, Qt, RoundJoin);
		return (*s_enums.insert(type, QDiagramMetaEnum(m)));
	}
	return e;
}

int QDiagramMetaEnum::fromString(QDiagramToolkit::PropertyType type, const QString & text)
{
	QDiagramMetaEnum e = defaultEnum(type);
	if (e.isValid()){
		for (int i = 0; i < e.keyCount(); i++){
			if (e.key(i) == text){
				return e.value(i);
			}
		}
	}
	return 0;
}

QIcon QDiagramMetaEnum::icon(int index) const
{
	return m_icons.value(index);
}

int QDiagramMetaEnum::indexOf(int value) const
{
	return m_pairs.keys().indexOf(value);
}

bool QDiagramMetaEnum::isValid() const
{
    return !m_pairs.isEmpty();
}

QString QDiagramMetaEnum::key(int index) const
{
    if (index != -1 && index < m_pairs.size()){
        return m_pairs.values().at(index);
    }
    return QString::null;
}

int QDiagramMetaEnum::keyCount() const
{
    return m_pairs.keys().size();
}

QString QDiagramMetaEnum::toString(QDiagramToolkit::PropertyType type, int value)
{
	QDiagramMetaEnum e = defaultEnum(type);
	if (e.isValid()){
		for (int i = 0; i < e.keyCount(); i++){
			if (e.value(i) == value){
				return e.key(i);
			}
		}
	}
	return QString::null;
}

int QDiagramMetaEnum::value(int index) const
{
    if (index < m_pairs.size()){
        return m_pairs.keys().at(index);
    }
    return -1;
}

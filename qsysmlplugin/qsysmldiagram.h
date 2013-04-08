/******************************************************************************
** Copyright (C) 2013 Martin Hoppe martin@2x2hoppe.de
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
#ifndef QSYSMLDIAGRAM_H
#define QSYSMLDIAGRAM_H

#include <qabstractdiagram.h>

class QSysMLDiagram : public QAbstractDiagram
{
	Q_OBJECT

public:
	QDIAGRAM_DECLARE_DIAGRAM(QSysMLDiagram, "sysml.diagram");

	QSysMLDiagram(const QString & pluginName, QObject *parent);
	~QSysMLDiagram();
protected:
	void drawDiagramFrame(QPainter* painter, QDiagramSheet* page);
private:
	
};

#endif // QSYSMLDIAGRAM_H

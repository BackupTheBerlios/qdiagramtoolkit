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
#include "qlogiccircuitdiagram.h"

#include <qabstractdiagramgraphicsitem.h>
#include <qabstractdiagramshapeconnector.h>
#include <qabstractdiagramshapeconnectionpoint.h>
#include <qdiagramsheet.h>

QLogicCircuitDiagram::QLogicCircuitDiagram(const QString & pluginName, QObject *parent)
	: QAbstractDiagram(pluginName, parent)
{
	addPlugin("Logic Circuit");
	sheet(0)->setPaperSize(QDiagramToolkit::A3);
	sheet(0)->setPaperOrientation(QDiagramToolkit::Landscape);

	connect(this, SIGNAL(contentsChanged()), this, SLOT(render()));
}

QLogicCircuitDiagram::~QLogicCircuitDiagram()
{

}


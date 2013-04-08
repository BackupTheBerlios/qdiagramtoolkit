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
#include "stdafx.h"
#include "qsysmldiagram.h"

#include <qdiagramsheet.h>

QSysMLDiagram::QSysMLDiagram(const QString & pluginName, QObject *parent)
	: QAbstractDiagram(pluginName, parent)
{
	currentSheet()->setPaperSize(QDiagramToolkit::A3);
	currentSheet()->setPaperOrientation(QDiagramToolkit::Landscape);
}

QSysMLDiagram::~QSysMLDiagram()
{

}

void QSysMLDiagram::drawDiagramFrame(QPainter* painter, QDiagramSheet* page)
{
	painter->save();
	painter->drawRect(page->sceneRect());
	QFont f("Arial");
	f.setPixelSize(60);
	f.setBold(true);
	painter->setFont(f);
	QFontMetricsF fm(f);
	QString n;
	QRectF r = fm.boundingRect(tr("act") + " " + n);
	
	painter->drawText(QPointF(50, r.height()), tr("act") + " " + n);

	QPen pen(Qt::black);
	pen.setWidthF(5);

	QPainterPath p;
	p.moveTo(0, r.height() + 20);
	p.lineTo(r.width() + 50, r.height() + 20);
	p.lineTo(r.width() + 50 + (r.height() + 20) / 2, r.height() - 30/*r.height() - (r.height() + 20) / 2*/);
	p.lineTo(r.width() + 50 + (r.height() + 20) / 2, 0);
	painter->drawPath(p);
	painter->restore();
}

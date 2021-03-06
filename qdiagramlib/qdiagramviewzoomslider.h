/******************************************************************************
** Copyright (C) 2012 Martin Hoppe martin@2x2hoppe.de
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
#ifndef QDIAGRAMVIEWZOOMSLIDER_H
#define QDIAGRAMVIEWZOOMSLIDER_H

#include <QWidget>

#include "qdiagramlib_global.h"

namespace Ui { class QDiagramViewZoomSlider; };

class QDIAGRAMLIBSHARED_EXPORT QDiagramViewZoomSlider : public QWidget
{
	Q_OBJECT

public:
	QDiagramViewZoomSlider(QWidget *parent = 0);
	~QDiagramViewZoomSlider();
public slots:
	void setZoom(int percent);
signals:
	void zoomChanged(int percent);
private slots:
	void sliderValueChanged(int value);
	void zoomInToolButtonClicked();
	void zoomOutToolButtonClicked();
private:
	Ui::QDiagramViewZoomSlider* ui;
};

#endif // QDIAGRAMVIEWZOOMSLIDER_H

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
#include "stdafx.h"
#include "qdiagramviewzoomslider.h"
#include "ui_qdiagramviewzoomslider.h"

QDiagramViewZoomSlider::QDiagramViewZoomSlider(QWidget *parent)
: QWidget(parent), ui(new Ui::QDiagramViewZoomSlider)
{
	ui->setupUi(this);

	ui->zoomLabel->setFixedWidth(QFontMetrics(QApplication::font(ui->zoomLabel)).width("100%"));
}

QDiagramViewZoomSlider::~QDiagramViewZoomSlider()
{
	delete ui;
}

void QDiagramViewZoomSlider::setZoom(int percent)
{
	ui->zoomSlider->setValue(percent);
}

void QDiagramViewZoomSlider::sliderValueChanged(int value)
{
	ui->zoomLabel->setText(tr("%1 %").arg(value));
	emit zoomChanged(ui->zoomSlider->value());
}

void QDiagramViewZoomSlider::zoomInToolButtonClicked()
{
	if (ui->zoomSlider->value() >= 100){
		ui->zoomSlider->setValue(ui->zoomSlider->value() + 25);
	} else {
		ui->zoomSlider->setValue(ui->zoomSlider->value() + 10);
	}
}

void QDiagramViewZoomSlider::zoomOutToolButtonClicked()
{
	if (ui->zoomSlider->value() >= 100){
		ui->zoomSlider->setValue(ui->zoomSlider->value() - 25);
	} else {
		ui->zoomSlider->setValue(ui->zoomSlider->value() - 10);
	}
}

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
#include "qdiagramviewcontrolpanel.h"
#include "ui_qdiagramviewcontrolpanel.h"

QDiagramViewControlPanel::QDiagramViewControlPanel(QWidget *parent)
	: QWidget(parent)
{
	ui = new Ui::QDiagramViewControlPanel();
	ui->setupUi(this);

	m_snapToGrid = false;
	updateIcons();

	QButtonGroup* g = new QButtonGroup(this);
    g->addButton(ui->showGridToolButton);
    g->setId(ui->showGridToolButton, 0);
    ui->showGridToolButton->setChecked(true);
    g->addButton(ui->hideGridToolButton);
    g->setId(ui->hideGridToolButton, 1);
    g->setExclusive(true);

    connect(g, SIGNAL(buttonClicked(int)), SLOT(gridToolButtonChanged(int)));

    ui->mousePositionLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ui->mousePositionLabel->setFixedWidth(QFontMetrics(QApplication::font(ui->mousePositionLabel)).width("000,000"));
    ui->mousePositionLabel->setFixedWidth(QFontMetrics(QApplication::font(ui->mousePositionLabel)).width("000,000"));

	connect(ui->zoomSlider, SIGNAL(zoomChanged(int)), this, SIGNAL(zoomChanged(int)));
}

QDiagramViewControlPanel::~QDiagramViewControlPanel()
{
	delete ui;
}

void QDiagramViewControlPanel::gridToolButtonChanged(int id)
{
	emit showGridToogled(id == 0 ? true : false);
}

void QDiagramViewControlPanel::setGridVisible(bool visible)
{
	if (visible){
		ui->showGridToolButton->setChecked(true);
	} else {
		ui->hideGridToolButton->setChecked(true);
	}
}

void QDiagramViewControlPanel::setMousePosition(const QPointF & pos)
{
    ui->mousePositionLabel->setText(QString("%1,%2").arg(pos.toPoint().x()).arg(pos.toPoint().y()));
}

void QDiagramViewControlPanel::setSnapToGridEnabled(bool on)
{
	if (m_snapToGrid != on){
		m_snapToGrid = on;
		updateIcons();
		emit snapToGridToogled(m_snapToGrid);
	}
}

void QDiagramViewControlPanel::setZoom(int percent)
{
	ui->zoomSlider->setZoom(percent);
}

bool QDiagramViewControlPanel::snapToGridEnabled() const
{
	return m_snapToGrid;
}

void QDiagramViewControlPanel::snapToolButtonClicked()
{
	m_snapToGrid = !m_snapToGrid;
	updateIcons();
	emit snapToGridToogled(m_snapToGrid);
}

void QDiagramViewControlPanel::updateIcons()
{
	if (m_snapToGrid){
		ui->snapToolButton->setIcon(QIcon(":/qdiagram/snap.enabled"));
	} else {
		ui->snapToolButton->setIcon(QIcon(":/qdiagram/snap.disabled"));
	}
}
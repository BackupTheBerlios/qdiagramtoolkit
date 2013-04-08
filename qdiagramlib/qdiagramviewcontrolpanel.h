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
#ifndef QDIAGRAMVIEWCONTROLPANEL_H
#define QDIAGRAMVIEWCONTROLPANEL_H

#include "qdiagramlib_global.h"

#include <QWidget>
namespace Ui {class QDiagramViewControlPanel;};

//! The QDiagramViewControlPanel provides a control panel widget to control properties of a QDiagramView.
/**
 * @ingroup widgets
 */
class QDIAGRAMLIBSHARED_EXPORT QDiagramViewControlPanel : public QWidget
{
	Q_OBJECT
	//! @property(snapToGrid)
	/**
	 * This property holds whether the snap to grid is enabled.
	 */
	Q_PROPERTY(bool snap READ snapEnabled WRITE setSnapEnabled)
public:
	/**
	 * Constructs a QDiagramViewControlPanel with the specified @p parent.
	 */
	QDiagramViewControlPanel(QWidget *parent = 0);
	/**
	 * Destroys the control panel.
	 */
	~QDiagramViewControlPanel();
	/**
	 * Returns the if snap to grid is enabled.
	 */
	bool snapEnabled() const;
public slots:
	void setGridVisible(bool visible);
	/**
	 * Sets the display of the mouse position to the coordinates given in @p pos.
	 */
	void setMousePosition(const QPointF & pos);
	
	void setSnapEnabled(bool on);
	void setZoom(int percent);
signals:
	void showGridToogled(bool visible);
	/**
	 * This signal is emitted whenever the snap to grid button has changed it's status. This can be the result of a user interaction, 
	 * or because setSnapToGrid() was called.
	 */
	void snapToogled(bool on);
	/**
	 * This signal is emitted whenever the zoom level has changed. This can be the result of a user interaction (moving the slider or
	 * pressing zoom in/out) or because setZoom() was called.
	 */
	void zoomChanged(int percent);
private slots:
    void gridToolButtonChanged(int id);
	void snapToolButtonClicked();
private:
	void updateIcons();
	bool m_snapToGrid;
	Ui::QDiagramViewControlPanel *ui;
};

#endif // QDIAGRAMVIEWCONTROLPANEL_H

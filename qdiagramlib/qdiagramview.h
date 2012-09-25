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
#ifndef QDIAGRAMVIEW_H
#define QDIAGRAMVIEW_H

#include "qdiagramlib_global.h"

#include <QWidget>

#include <qdiagram.h>
#include <qdiagramconnectorstyle.h>
#include <qdiagramgraphicsview.h>
#include <qdiagramshapetoolbox.h>

namespace Ui {
class QDiagramView;
}

//! The QDiagramView class provides a widget for displaying the contents of a QDiagram.
class QDIAGRAMLIBSHARED_EXPORT QDiagramView : public QWidget
{
    Q_OBJECT
	Q_PROPERTY(Qt::Alignment alignment READ alignment WRITE setAlignment)
//    Q_PROPERTY(Qt::ContextMenuPolicy )
    //! @property(interactive)
    /**
      * This property holds whether the view allowed scene interaction.
      * If enabled, this view is set to allow scene interaction. Otherwise, this view will not allow interaction, and any mouse or key events are ignored (i.e., it will act as a read-only view).
      * By default, this property is true.
      */
	Q_PROPERTY(bool interactive READ isInteractive WRITE setInteractive)

    Q_PROPERTY(bool snapToGrid READ snapToGrid WRITE setSnapToGrid)

    Q_PROPERTY(QSizeF snapSize READ snapSize WRITE setSnapSize)
public:
    /**
      * Constructs a QDiagramView. @p parent is passed to QWidget's constructor.
      */
    explicit QDiagramView(QWidget *parent = 0);
    /**
      * Destructs the QDiagramView object.
      */
    ~QDiagramView();

    void addConnector(const QDiagramConnectorStyle & style);
    void addConnector(const QIcon & icon, const QString & text, const QString & shape);
	/**
	 *
	 */
	Qt::Alignment alignment() const;
    /**
      * Returns whether text can be pasted from the clipboard into the diagram view.
      */
    bool canPaste();
	/**
     * Returns a pointer to the diagram that is currently visualized in the view. If no diagram is currently visualized, 0 is returned.
     * @see setDiagram()
	 */
	QDiagram* diagram() const;
    /**
      * Returns the diagram graphics view.
      */
    QDiagramGraphicsView* graphicsView() const;
	/**
     * Return true if the view allows interaction with the scene.
	 */
	bool isInteractive() const;
    /**
      * Maps the point @p point, which is in this scene's coordinate system, to the scene's grid coordinate system, and returns the mapped coordinate.
      */
    QPointF mapToGrid(const QPointF & point);
    /**
	 *
	 */
	void setAlignment(Qt::Alignment alignment);
    /**
      * Sets the current diagram to @p diagram. If @p diagram is already being viewed, this function does nothing.
      * @see diagram()
      */
    void setDiagram(QDiagram* diagram);

	void setInteractive(bool allowed);
    /**
      * Sets the snap grid @p size.
      * @see snapGridSize()
      */
    void setSnapSize(const QSizeF & size);
    /**
      * If @p on is true, snap to grid is enabled.
      */
    void setSnapToGrid(bool on);
    /**
      * Returns the snap grid size.
      * @see setSnapSize()
      */
    QSizeF snapSize() const;
    /**
      * Returns true if snap to grid is enabled. Otherwise false.
      * @see setSnapToGrid()
      */
    bool snapToGrid() const;
signals:
    void graphicsViewContextMenuRequested(const QPoint & screenPos, const QPointF & scenePos);
public slots:
    /**
      * Copies any selected items to the clipboard.
      */
	void copy();
    /**
      * Copies the selected items to the clipboard and deletes them from the diagram.
      * If there is no selected text nothing happens.
      * @see copy() paste()
      */
    void cut();
    /**
      * Pastes the mimedata from the clipboard into the diagram view at the current cursor position.
      * If there is no text in the clipboard nothing happens.
      * To change the behavior of this function, i.e. to modify what QDiagramView can paste and how it is being pasted, reimplement the virtual canInsertFromMimeData() and insertFromMimeData() functions.
      * @see cut(), copy()
      */
	void paste();
    void setMode(QDiagramGraphicsView::Mode mode);
protected:
    /**
      * This function returns true if the contents of the MIME data object, specified by source, can be decoded and inserted into the diagram view.
      */
    bool canInsertFromMimeData(const QMimeData* source) const;
    /**
      * This function inserts the contents of the MIME data object, specified by source, into the text edit at the current cursor position.
      * It is called whenever shapes are inserted as the result of a clipboard paste operation, or when the diagram view accepts data from a drag and drop operation.
      */
    void insertFromMimeData(const QMimeData* source, const QPointF & scenePos);
private slots:
    void graphicsViewContextMenuRequestHandler(const QPoint & pos);
    void gridToolButtonChanged(int id);
	void itemRestored(QAbstractDiagramGraphicsItem* item);
    void modeMenuActionTriggered();
    void mouseScenePositionChanged(const QPointF & pos);
    void zoomChanged(int percent);
    void zoomSliderValueChanged(int value);
private:
    QDiagram* m_diagram;
    bool m_snapToGrid;
    QSizeF m_snapSize;

    Ui::QDiagramView *ui;
};

#endif // QDIAGRAMVIEW_H

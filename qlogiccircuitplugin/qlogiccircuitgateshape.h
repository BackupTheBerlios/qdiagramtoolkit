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
#ifndef QLOGICGATESHAPE_H
#define QLOGICGATESHAPE_H

#include "qlogiccircuitplugin_global.h"

#include <qabstractdiagramshape.h>

class QAbstractDiagramShapeConnector;

//! The QLogicCircui	tGateShape provides a shape showing logic gates.
/**
 * @section AND-Gate
 * @subsection Shape
 * <table>
 * <tr><td>@image html lc_gate_and.png</td></tr>
 * </table>
 * @subsection Properties
 * <table>
 * <tr>
 * <th>Name</th><th>Type</th><th>R/W</th><th>Default</th><th>Remarks</th>
 * </tr>
 * <tr><td>gateType</td><td>String</td><td>ReadOnly</td><td>and</td><td></td>
 * <tr><td>inputs</td><td>Integer</td><td>ReadWrite</td><td>2</td><td>min. value: 2</td>
 * </tr>
 * </table>
 *
 * @section NAND-Gate
 * @subsection Shape
 * <table>
 * <tr><td>@image html lc_gate_nand.png</td></tr>
 * </table>
 * @subsection Properties
 * <table>
 * <tr>
 * <th>Name</th><th>Type</th><th>R/W</th><th>Default</th><th>Remarks</th>
 * </tr>
 * <tr><td>gateType</td><td>String</td><td>ReadOnly</td><td>nand</td><td></td>
 * <tr><td>inputs</td><td>Integer</td><td>ReadWrite</td><td>2</td><td>min. value: 2</td>
 * </tr>
 * </table>
 *
 * @section NOR-Gate
 * @subsection Shape
 * <table>
 * <tr><td>@image html lc_gate_nor.png</td></tr>
 * </table>
 * @subsection Properties
 * <table>
 * <tr>
 * <th>Name</th><th>Type</th><th>R/W</th><th>Default</th><th>Remarks</th>
 * </tr>
 * <tr><td>gateType</td><td>String</td><td>ReadOnly</td><td>nor</td><td></td>
 * <tr><td>inputs</td><td>Integer</td><td>ReadWrite</td><td>2</td><td>min. value: 2</td>
 * </tr>
 * </table>
 *
 * @section NOT-Gate
 * @subsection Shape
 * <table>
 * <tr><td>@image html lc_gate_not.png</td></tr>
 * </table>
 * @subsection Properties
 * <table>
 * <tr>
 * <th>Name</th><th>Type</th><th>R/W</th><th>Default</th><th>Remarks</th>
 * </tr>
 * <tr><td>gateType</td><td>String</td><td>ReadOnly</td><td>not</td><td></td>
 * <tr><td>inputs</td><td>Integer</td><td>ReadOnly</td><td>1</td><td></td>
 * </tr>
 *
 * @section OR-Gate
 * @subsection Shape
 * <table>
 * <tr><td>@image html lc_gate_or.png</td></tr>
 * </table>
 * @subsection Properties
 * <table>
 * <tr>
 * <th>Name</th><th>Type</th><th>R/W</th><th>Default</th><th>Remarks</th>
 * </tr>
 * <tr><td>gateType</td><td>String</td><td>ReadOnly</td><td>or</td><td></td>
 * <tr><td>inputs</td><td>Integer</td><td>ReadOnly</td><td>1</td><td></td>
 * </tr>
 *
 * @section XNOR-Gate
 * @subsection Shape
 * <table>
 * <tr><td>@image html lc_gate_xnor.png</td></tr>
 * </table>
 * @subsection Properties
 * <table>
 * <tr>
 * <th>Name</th><th>Type</th><th>R/W</th><th>Default</th><th>Remarks</th>
 * </tr>
 * <tr><td>gateType</td><td>String</td><td>ReadOnly</td><td>xnor</td><td></td>
 * <tr><td>inputs</td><td>Integer</td><td>ReadOnly</td><td>1</td><td></td>
 *
 * @section XOR-Gate
 * @subsection Shape
 * <table>
 * <tr><td>@image html lc_gate_xor.png</td></tr>
 * </table>
 * @subsection Properties
 * <table>
 * <tr>
 * <th>Name</th><th>Type</th><th>R/W</th><th>Default</th><th>Remarks</th>
 * </tr>
 * <tr><td>gateType</td><td>String</td><td>ReadOnly</td><td>xor</td><td></td>
 * </tr>
 * <tr><td>inputs</td><td>Integer</td><td>ReadOnly</td><td>1</td><td></td>
 * </tr>
 * </table>
 */
class QLogicCircuitGateShape : public QAbstractDiagramShape
{
public:
	QDIAGRAM_DECLARE_SHAPE(QLogicCircuitGateShape, Gate)

	explicit QLogicCircuitGateShape(QGraphicsItem* parent = 0);

    explicit QLogicCircuitGateShape(const QMap<QString,QVariant> & properties, QGraphicsItem* parent = 0);

    QRectF boundingRect() const;

	QList<QAction*> createActions(QWidget* parent);
	/**
	 * Returns the default properties the for shape specified by the given @p id;
	 */
	static QVariantMap defaultProperties(const QString & id);
	/**
	 * Returns the shape's hot spot.
	 */
	static QPointF hotSpot(const QString & id);

	QList<QAbstractDiagramShapeConnector*> inputs() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    /**
      * Returns the shape of this item as a QPainterPath in local coordinates.
      */
    QPainterPath shape() const;

	void triggerAction(const QString & name, const QVariant & data);
protected:
    QVariant itemPropertyChange( const QString & name, const QVariant & value);
    QVariant itemPropertyHasChanged( const QString & name, const QVariant & value);
    void paintState(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
private:
};

#endif // QLOGICGATESHAPE_H

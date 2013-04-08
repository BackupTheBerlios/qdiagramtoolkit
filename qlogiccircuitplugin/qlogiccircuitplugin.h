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
#ifndef QLOGICCIRCUITPLUGIN_H
#define QLOGICCIRCUITPLUGIN_H

#include <qabstractdiagramplugin.h>
#include <qdiagramtoolkit.h>
//! @ingroup plugins The QLogicCircuitPlugin provides a set of shapes for logic circuits.
/**
  * @section lcshape Shapes
  * <table>
  * <tr>
  * <th>Shape</th>
  * <th>Description</th>
  * <th>Id</th>
  * <th>Class</th>
  * </tr>
  * <tr>
  * <td>@image html lc_gate_and.png</td>
  * <td><b>AND-gate</b></td>
  * <td>gate.and</td>
  * <td>QLogicCircuitGateShape</td>
  * </tr>
  * <tr>
  * <td>@image html lc_gate_nand.png</td>
  * <td><b>NAND-gate</b></td>
  * <td>gate.nand</td>
  * <td>QLogicCircuitGateShape</td>
  * </tr>
  * <tr>
  * <td>@image html lc_gate_nor.png</td>
  * <td><b>NOR-gate</b></td>
  * <td>gate.nor</td>
  * <td>QLogicCircuitGateShape</td>
  * </tr>
  * <tr>
  * <td>@image html lc_gate_not.png</td>
  * <td><b>NOT-gate</b><br/>Inverts the input signal</td>
  * <td>gate.not</td>
  * <td>QLogicCircuitGateShape</td>
  * </tr>
  * <tr>
  * <td>@image html lc_gate_xor.png</td>
  * <td><b>XOR-gate</b></td>
  * <td>gate.xor</td>
  * <td>QLogicCircuitGateShape</td>
  * </tr>
  * <tr>
  * <td>@image html lc_gate_xnor.png</td>
  * <td><b>XOR-gate</b></td>
  * <td>gate.xnor</td>
  * <td>QLogicCircuitGateShape</td>
  * </tr>
  * <tr>
  * <td>@image html lc_func_comp.png</td>
  * <td><b>Comparator</b><br/>Compares analog input X with Y</td>
  * <td>function.comparator</td>
  * <td>QLogicCircuitFunctionShape</td>
  * </tr>
  * <tr>
  * <td>@image html lc_func_counter.png</td>
  * <td><b>Counter</b></td>
  * <td>function.counter</td>
  * <td>QLogicCircuitFunctionShape</td>
  * </tr>
  * <tr>
  * <td>@image html lc_func_oph_counter.png</td>
  * <td><b>Operating Hours Counter</b></td>
  * <td>function.counter.oph</td>
  * <td>QLogicCircuitFunctionShape</td>
  * </tr>
  * <tr>
  * <td>@image html lc_func_timer.png</td>
  * <td><b>Timer</b></td>
  * <td>function.timer</td>
  * <td>QLogicCircuitFunctionShape</td>
  * </tr>
  * </table>
  */
class QLogicCircuitPlugin : public QObject, public QAbstractDiagramPlugin
{
    Q_OBJECT
    Q_INTERFACES(QAbstractDiagramPlugin)
public:
	QDIAGRAM_DECLARE_PLUGIN(QLogicCircuitPlugin, "Logic Circuit");

    QAbstractDiagramShapeConnector* createConnection(const QString & uuid, QAbstractDiagramShapeConnectionPoint* from, QAbstractDiagramShapeConnectionPoint* to, const QString & style);

    QList<QDiagramEndOfLineStyle> endOfLineStyles() const;

	void initialize();

    QList<QDiagramLineStyle> lineStyles() const;
    /**
      * Returns a list of actions defined for the shape specified by the given @p style.
      */
    QList<QAction*> shapeActions(const QString & style) const;
    /**
      * Returns the (user-readable) title of the connector or shape specified by the given @p name.
      */
    //QString title(const QString & name) const;
    /**
      * Returns the @p shape's tool tip.
      */
    QString toolTip(const QString & shape) const;
};

#endif // QLOGICCIRCUITPLUGIN_H

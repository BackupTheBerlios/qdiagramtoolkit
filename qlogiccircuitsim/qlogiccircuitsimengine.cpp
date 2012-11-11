#include "stdafx.h"
#include "qlogiccircuitsimengine.h"

#include <qabstractdiagramshapeconnectionpoint.h>
#include <qabstractdiagramshapeconnector.h>
#include "qdiagram.h"

#include "qdiagramshapeconnector.h"

QLogicCircuitSimEngine::QLogicCircuitSimEngine(QObject *parent) :
    QThread(parent)
{
    m_engine = new QScriptEngine(this);
}

QString QLogicCircuitSimEngine::script() const
{
    return m_script;
}

void QLogicCircuitSimEngine::setDiagram(QDiagram *diagram)
{
    QStringList inputs;
    QStringList outputs;
    QStringList gates;
    int gcount = 1;
    int icount = 1;
    int ocount = 1;
	QMap<QString,int> execLevelMap;
    QMap<QString,QString> uuidMap;

	QMap<QString, QString> ops;

    Q_FOREACH(QAbstractDiagramGraphicsItem* i, diagram->items()){
        QAbstractDiagramShape* s = qgraphicsitem_cast<QAbstractDiagramShape*>(i);
        if (s){
			QString key;
            if (s->property("shape") == "input"){
				key = QString("i%1").arg(icount++);
				inputs << QString("\"%1\": 0").arg(key);
				uuidMap[s->uuid()] = key;
            } else if (s->property("shape") == "gate"){
				key = QString("g%1").arg(gcount++);
				gates << QString("\"%1\": 0").arg(key);
				uuidMap[s->uuid()] = key;
            } else if (s->property("shape") == "output"){
				key = QString("o%1").arg(ocount++);
				outputs << QString("\"%1\": 0").arg(key);
				uuidMap[s->uuid()] = key;
            }
		}
	}
	
	m_script.clear();
	m_script += "var inputs = {" + inputs.join(", ") + "};\n";
	m_script += "var outputs = {" + outputs.join(", ") + "};\n";
	m_script += "var gates = {" + gates.join(", ") + "};\n";

    Q_FOREACH(QAbstractDiagramGraphicsItem* i, diagram->items()){
        QAbstractDiagramShape* s = qgraphicsitem_cast<QAbstractDiagramShape*>(i);
        if (s && s->property("shape") == "gate"){
			execLevelMap[s->uuid()] = level(s, 0);
			QStringList g;
			Q_FOREACH(QAbstractDiagramShapeConnectionPoint* cp, s->connectionPoints(QDiagramToolkit::West)){
				Q_FOREACH(QAbstractDiagramShapeConnectionPoint::Connection c, cp->connections()){
					if (c.connector->startConnectionPoint()->parentShape() == s){
						if (c.connector->endConnectionPoint()->parentShape()->property("shape") == "input"){
							g << "inputs['" + uuidMap.value(c.connector->endConnectionPoint()->parentShape()->uuid()) + "']";
						} else if (c.connector->endConnectionPoint()->parentShape()->property("shape") == "gate"){
							g << "gate['" + uuidMap.value(c.connector->endConnectionPoint()->parentShape()->uuid()) + "']";
						}
					} else {
						if (c.connector->startConnectionPoint()->parentShape()->property("shape") == "input"){
							g << "inputs['" + uuidMap.value(c.connector->startConnectionPoint()->parentShape()->uuid()) + "']";
						} else if (c.connector->startConnectionPoint()->parentShape()->property("shape") == "gate"){
							g << "gate['" + uuidMap.value(c.connector->startConnectionPoint()->parentShape()->uuid()) + "']";
						}
					}
				}
			}
			QString op;
			op += "gates['" + uuidMap.value(s->uuid()) + "'] = ";
			if (s->property("gateType") == "and"){
				op += g.join(" && ") + ";\n";
			} else if (s->property("gateType") == "nand"){
				op += "!(" + g.join(" && ") + ");\n";
			} else if (s->property("gateType") == "nor"){
				op += "!(" + g.join(" || ") + ");\n";
			} else if (s->property("gateType") == "not"){
				op += "!(" + g.join(" || ") + ");\n";
			} else if (s->property("gateType") == "or"){
				op += "(" + g.join(" || ") + ");\n";
			}
			ops[s->uuid()] = op;
		}
	}
	int i = 1;

	m_script += "true != false;\n// EOF\n";
//    QStringList inputs;
//    QStringList outputs;
//    QStringList functions;
//    QStringList gates;
//    QStringList vars;
//    QMap<QString,QString> uuidMap;
//    QMap<QString,int> execLevelMap;
//    int r = 0;
//    int fcount = 1;
//    int gcount = 1;
//    int icount = 1;
//    int ocount = 1;
//    Q_FOREACH(QAbstractDiagramGraphicsItem* i, diagram->items()){
//        QAbstractDiagramShape* s = qgraphicsitem_cast<QAbstractDiagramShape*>(i);
//        if (s){
//            if (s->property("shape") == "function"){
//                uuidMap[s->uuid()] = QString("f%1").arg(fcount);
//                fcount++;
//            } else if (s->property("shape") == "gate"){
//                uuidMap[s->uuid()] = QString("g%1").arg(gcount);
//                gcount++;
//            } else if (s->property("shape") == "input"){
//                uuidMap[s->uuid()] = QString("i%1").arg(icount);
//                icount++;
//            } else if (s->property("shape") == "output"){
//                uuidMap[s->uuid()] = QString("o%1").arg(ocount);
//                ocount++;
//            }
//        }
//    }
//
//    Q_FOREACH(QAbstractDiagramGraphicsItem* i, diagram->items()){
//        QAbstractDiagramShape* s = qgraphicsitem_cast<QAbstractDiagramShape*>(i);
//        if (s){
//            if (s->property("shape") == "gate"){
//                QString c;
//                c += QString("function %1(){\n").arg(uuidMap.value(s->uuid()));
//                c += "// " + s->uuid() + "\n";
//                QStringList logic;
//                Q_FOREACH(QAbstractDiagramShapeConnectionPoint* cp, s->connectionPoints()){
//                    if (cp->direction() == QAbstractDiagramShapeConnectionPoint::In){
//                        Q_FOREACH(QAbstractDiagramShapeConnectionPoint::Connection c, cp->connections()){
//                            qDebug() << c.connector->startConnectionPoint()->parentShape()->uuid()
//                                     << c.connector->endConnectionPoint()->parentShape()->uuid();
//                            logic << uuidMap.value(c.connector->endConnectionPoint()->parentShape()->uuid());
//                        }
//                    }
//                }
//                if (logic.isEmpty()){
//                    c += " return 0\n";
//                } else {
//                    if (logic.size() == 1){
//                        logic << "0"
//;                    }
//                    if (s->property("gateType") == "and"){
//                        c += " return " + logic.join(" && ") + ";\n";
//                    } else if (s->property("gateType") == "nand"){
//                    } else if (s->property("gateType") == "or"){
//                        c += " return " + logic.join(" || ") + ";\n";
//                    }
//                }
//                c += "};\n";
//                gates << c;
//
//                execLevelMap[uuidMap.value(s->uuid())] = level(s, 0);
//
//                gcount++;
//            } else if (s->property("shape") == "input"){
//                inputs << QString("\"%1\": 0").arg(s->property("name").toString());
//            } else if (s->property("shape") == "output"){
//                outputs << QString("\"%1\": 0").arg(s->property("name").toString());
//            }
//            r++;
//        }
//    }
//    QString s;
//    QMapIterator<QString, QString> it_uuid(uuidMap);
//    while(it_uuid.hasNext()){
//        it_uuid.next();
//        s += "// " + it_uuid.value() + ": " + it_uuid.key() + "\n";
//    }
//    s += "var inputs = {";
//    s += inputs.join(", ");
//    s += "};\n";
//
//    s += "var outputs = {";
//    s += outputs.join(", ");
//    s += "};\n";
//
//
//    QMap<int, QStringList> e;
//    QMapIterator<QString,int> it(execLevelMap);
//    while(it.hasNext()){
//        it.next();
//        if (e.contains(it.value())){
//            e[it.value()].append(it.key());
//        } else {
//            QStringList l;
//            l << it.key();
//            e[it.value()] = l;
//        }
//    }
//
//    QMapIterator<int,QStringList> it_e(e);
//	s += "var state = {";
//	while(it_e.hasNext()){
//		it_e.next();
//		for(int i = 0; i < it_e.value().size(); i++){
//			if (i > 0){
//				s += ", ";
//			}
//			s += QString("\"%1\": 0").arg(it_e.value().at(i));
//		}
//	}
//	s += "};\n";
//
//    s += gates.join("\n");
//
//	it_e = e;
//    while(it_e.hasNext()){
//        it_e.next();
//        Q_FOREACH(QString f, it_e.value()){
//            s += f + "();\n";
//        }
//    }
//
//    m_script += s;
}

void QLogicCircuitSimEngine::pause()
{
}

void QLogicCircuitSimEngine::reset()
{
}

void QLogicCircuitSimEngine::resume()
{
}

void QLogicCircuitSimEngine::stop()
{
}

int QLogicCircuitSimEngine::level(QAbstractDiagramShape *shape, int previous)
{
    int p = previous;
    p++;
    Q_FOREACH(QAbstractDiagramShapeConnectionPoint* cp, shape->connectionPoints()){
        if (cp->direction() == QAbstractDiagramShapeConnectionPoint::Out){
            Q_FOREACH(QAbstractDiagramShapeConnectionPoint::Connection c, cp->connections()){
                if (c.connector->endConnectionPoint()->direction() == QAbstractDiagramShapeConnectionPoint::In){
                    p = level(c.connector->endConnectionPoint()->parentShape(), p);
                }
            }
        }
    }
    return p;
}

QScriptEngine* QLogicCircuitSimEngine::scriptEngine() const
{
	return m_engine;
}

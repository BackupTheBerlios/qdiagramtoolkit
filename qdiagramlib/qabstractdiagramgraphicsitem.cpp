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
#include "qabstractdiagramgraphicsitem.h"

#include "qabstractdiagramshapeconnector.h"
#include "qdiagram.h"
#include "qdiagramlayers.h"
#include "qdiagrammetadata.h"
#include "qdiagramundocommand.h"
#include "qdiagramchangepropertycommand.h"

#include <json.h>

QAbstractDiagramGraphicsItem::QAbstractDiagramGraphicsItem(QGraphicsItem* parent) :
    QGraphicsItem(parent)
{
    m_blockUndoCommands = false;
    initMetaData("invalid", "invalid", "invalid", "invalid");
}

QAbstractDiagramGraphicsItem::QAbstractDiagramGraphicsItem(const QString & uuid, const QString & pluginName, const QString & itemType, const QString & itemClass, QGraphicsItem* parent) :
    QGraphicsItem(parent)
{
    m_blockUndoCommands = false;
    initMetaData(uuid, pluginName, itemType, itemClass);
}

QAbstractDiagramGraphicsItem::~QAbstractDiagramGraphicsItem()
{
    delete m_metadata;
}

QList<QAction *> QAbstractDiagramGraphicsItem::actions() const
{
    return m_actions;
}

void QAbstractDiagramGraphicsItem::addAction(QAction *action)
{
    m_actions.append(action);
}

bool QAbstractDiagramGraphicsItem::blockUndoCommands(bool block)
{
    if (m_blockUndoCommands != block){
        m_blockUndoCommands = block;
        return !m_blockUndoCommands;
    }
    return m_blockUndoCommands;
}

void QAbstractDiagramGraphicsItem::bringForward()
{
	if (zValue() <= 100){
		setZValue(zValue() + 10);
	}
}

void QAbstractDiagramGraphicsItem::bringToFront()
{
	setZValue(100);
}

void QAbstractDiagramGraphicsItem::addDynamicProperty(const QString & name, const QVariant & value)
{
	m_metadata->addProperty(name, QDiagramToolkit::Dynamic, false);
	m_properties[name] = value;
}

void QAbstractDiagramGraphicsItem::addProperty(const QString & name, QDiagramToolkit::PropertyType type, bool readOnly, const QVariant & value)
{
    m_metadata->addProperty(name, type, readOnly);
	m_properties[name] = QDiagramProperty::fromVariant(type, value);
}

void QAbstractDiagramGraphicsItem::addProperty(const QString &name, QDiagramToolkit::PropertyType type, const QMap<int, QString> &pairs, const QVariant &value)
{
    m_metadata->addProperty(name, type, pairs);
	m_properties[name] = QDiagramProperty::fromVariant(type, value);
//	m_values[m_metadata->indexOfProperty(name)] = QDiagramPropertyValue(type, value);
}

QBrush QAbstractDiagramGraphicsItem::brush() const
{
    return m_brush;
}

void QAbstractDiagramGraphicsItem::changeGeometry(const QRectF & r)
{
	QVariantMap m = QDiagramProperty::toMap(r);
	m_properties["geometry"] = m;
	prepareGeometryChange();
}

bool QAbstractDiagramGraphicsItem::changeProperty(const QString & name, const QVariant & value)
{
    QDiagramMetaProperty metaProperty = m_metadata->property(m_metadata->indexOfProperty(name));
	// Check if property is dynamic
	if (!metaProperty.isValid()){
		// Check if dynamic property has changed
		if (m_dynamicProperties.value(name) != value){
			m_dynamicProperties[name] = value;
			itemPropertyHasChanged(name, value);
		}
		return false;
	}
	// Check if static property is read only
    if (metaProperty.isReadOnly()){
		// Property is read only
        return false;
    }
    QVariant currentValue = m_properties.value(name);
    if (currentValue != value){
        if (name == "brush"){
            m_brush = brush(value.toString());
        } else if (name == "geometry"){
			if (value.canConvert(QVariant::RectF)){
//                blockUndoCommands(true);
	            prepareGeometryChange();
                setPos(value.toRectF().x(), value.toRectF().y());
                //blockUndoCommands(false);
			} else if (value.canConvert(QVariant::Map)){
                //blockUndoCommands(true);
                prepareGeometryChange();
		        setPos(value.toMap().value("x").toDouble(), value.toMap().value("y").toDouble());
                //blockUndoCommands(false);
            }
			itemGeometryHasChanged(value);
        }
		m_properties[name] = value;
		itemPropertyHasChanged(name, value);
    }
    return true;
}

void QAbstractDiagramGraphicsItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QMenu menu;

    if (!isSelected()){
        setSelected(true);
    }
    if (!m_actions.isEmpty()){
        menu.addActions(m_actions);
        menu.addSeparator();
    }
    if (diagram() != 0){
        menu.addActions(diagram()->standardItemContextMenuActions());
    }
    menu.exec(event->screenPos());
}

QList<QAction*> QAbstractDiagramGraphicsItem::createActions(QWidget* parent)
{
	return QList<QAction*>();
}

QBrush QAbstractDiagramGraphicsItem::brush(const QString & attrs)
{
    QBrush mBrush;
    QStringList mValues = attrs.split(";");
    for(int iValues = 0; iValues < mValues.size(); iValues++){
        QString mValue = mValues.at(iValues).section(":", 1);
        if (mValues.at(iValues).section(":",0, 0) == "color"){
            mBrush.setColor(QColor(mValue));
        } else if (mValues.at(iValues) == "style"){
            if (mValue == "nobrush"){
                mBrush.setStyle(Qt::NoBrush);
            } else if (mValue == "solid"){
                mBrush.setStyle(Qt::SolidPattern);
            } else if (mValue == "dense1"){
                mBrush.setStyle(Qt::Dense1Pattern);
            } else if (mValue == "dense2"){
                mBrush.setStyle(Qt::Dense2Pattern);
            } else if (mValue == "dense3"){
                mBrush.setStyle(Qt::Dense3Pattern);
            } else if (mValue == "dense4"){
                mBrush.setStyle(Qt::Dense4Pattern);
            } else if (mValue == "dense5"){
                mBrush.setStyle(Qt::Dense5Pattern);
            } else if (mValue == "dense6"){
                mBrush.setStyle(Qt::Dense6Pattern);
            } else if (mValue == "dense7"){
                mBrush.setStyle(Qt::Dense7Pattern);
            } else if (mValue == "hor"){
                mBrush.setStyle(Qt::HorPattern);
            } else if (mValue == "ver"){
                mBrush.setStyle(Qt::VerPattern);
            } else if (mValue == "cross"){
                mBrush.setStyle(Qt::CrossPattern);
            } else if (mValue == "bdiag"){
                mBrush.setStyle(Qt::BDiagPattern);
            } else if (mValue == "fdiag"){
                mBrush.setStyle(Qt::FDiagPattern);
            } else if (mValue == "diagcross"){
                mBrush.setStyle(Qt::DiagCrossPattern);
            } else {
                mBrush.setStyle(Qt::SolidPattern);
            }
        }
    }
    return mBrush;
}

QAbstractDiagram* QAbstractDiagramGraphicsItem::diagram() const
{
    QAbstractDiagramScene* mScene = qobject_cast<QAbstractDiagramScene*>(scene());
    if (mScene){
        return mScene->diagram();
    }
    return 0;
}

QRectF QAbstractDiagramGraphicsItem::geometry() const
{
	QRectF r;
	QVariantMap p = m_properties.value("geometry").toMap();
	r.setX(p.value("x").toDouble());
	r.setY(p.value("y").toDouble());
	r.setWidth(p.value("width").toDouble());
	r.setHeight(p.value("height").toDouble());
    return r;
}

bool QAbstractDiagramGraphicsItem::hasProperty(const QString & name) const
{
    return m_properties.contains(name);
}

void QAbstractDiagramGraphicsItem::initGeometry(qreal width, qreal height)
{
	prepareGeometryChange();
	QVariantMap g;
	g["x"] = 0;
	g["y"] = 0;
	g["width"] = width;
	g["height"] = height;
	m_properties["geometry"] = g;
}

void QAbstractDiagramGraphicsItem::initMetaData(const QString & uuid, const QString & pluginName, const QString & itemType, const QString & itemClass)
{
    m_metadata = new QDiagramMetaData(pluginName, itemType, itemClass);

    m_metadata->addProperty("uuid", QDiagramToolkit::UUID, true);
    m_properties["uuid"] = uuid;
//	m_values[m_metadata->indexOfProperty("uuid")] = QDiagramPropertyValue(QDiagramToolkit::UUID, uuid);

 //   m_metadata->addProperty("itemType", QDiagramToolkit::String, true);
 //   m_properties["itemType"] = type;
	//m_values[m_metadata->indexOfProperty("itemType")] = QDiagramPropertyValue(QDiagramToolkit::String, type);

    m_metadata->addProperty("geometry", QDiagramToolkit::Rect, false);
//	m_values[m_metadata->indexOfProperty("geometry")] = QDiagramPropertyValue(QDiagramToolkit::Rect);

//    m_metadata->addProperty("pen", QDiagramToolkit::Pen, false);
//	m_values[m_metadata->indexOfProperty("pen")] = QDiagramPropertyValue(QDiagramToolkit::Pen);

	//QVariantMap m;

	//QPen p;
	//p.setColor(Qt::black);
	//p.setWidthF(1.0);
	//p.setStyle(Qt::SolidLine);
	//m_properties["pen"] = QDiagramProperty::toMap(p);

    m_metadata->addProperty("zlevel", QDiagramToolkit::Int, false);
//	m_values[m_metadata->indexOfProperty("zlevel")] = QDiagramPropertyValue(QDiagramToolkit::Int);
}

QVariant QAbstractDiagramGraphicsItem::itemChange(GraphicsItemChange change, const QVariant & value)
{
    QVariant mValue(value);
    if (change == QGraphicsItem::ItemPositionChange){
		return itemPositionChange(value);
    } else if (change == QGraphicsItem::ItemPositionHasChanged){
        return itemPositionHasChanged(value);
    } else if (change == QGraphicsItem::ItemSceneHasChanged){
        return itemSceneHasChanged(value);
    } else if (change == QGraphicsItem::ItemSelectedChange){
        return itemSelectedChange(value);
    } else if (change == QGraphicsItem::ItemSelectedHasChanged){
        return itemSelectedHasChanged(value);
	} else if (change == QGraphicsItem::ItemZValueChange){
    } else if (change == QGraphicsItem::ItemZValueHasChanged){
        return itemZValueHasChanged(value);
    }
    return QGraphicsItem::itemChange(change, value);
}

QVariant QAbstractDiagramGraphicsItem::itemGeometryHasChanged(const QVariant & value)
{
	return value;
}

QVariant QAbstractDiagramGraphicsItem::itemPositionChange(const QVariant & value)
{
	QPointF p = value.toPointF();
	if (p.x() < 0){
		p.setX(0);
	}
	if (p.y() <0){
		p.setY(0);
	}
	if (diagram()){
		if (diagram()->layers()->isLocked(this)){
			return pos();
		}
		if (diagram()->isSnapToGridEnabled()){
			p = diagram()->gridPos(p);
		}
	}
	return p;
}

QVariant QAbstractDiagramGraphicsItem::itemPositionHasChanged(const QVariant & value)
{
    QVariantMap m = m_properties.value("geometry").toMap();
    m["x"] = value.toPointF().x();
    m["y"] = value.toPointF().y();
    m_properties["geometry"] = m;
    return value;
}

QVariant QAbstractDiagramGraphicsItem::itemPropertyChange( const QString & name, const QVariant & value)
{
    Q_UNUSED(name);
    update();
    return value;
}

QVariant QAbstractDiagramGraphicsItem::itemPropertyHasChanged( const QString & name, const QVariant & value)
{
    Q_UNUSED(name);
    update();
    if (name == "geometry"){
        updateSizeGripHandles();
    }
    return value;
}

QVariant QAbstractDiagramGraphicsItem::itemSceneHasChanged(const QVariant & value)
{
	return value;
}

QVariant QAbstractDiagramGraphicsItem::itemSelectedChange( const QVariant & value )
{
    return value;
}

QVariant QAbstractDiagramGraphicsItem::itemSelectedHasChanged( const QVariant & value )
{
    update();
    return value;
}

QVariant QAbstractDiagramGraphicsItem::itemZValueHasChanged(const QVariant &value)
{
    setProperty("zlevel", value);
    return value;
}

QDiagramMetaData* QAbstractDiagramGraphicsItem::metaData() const
{
    return m_metadata;
}

bool QAbstractDiagramGraphicsItem::parse(const QString &data)
{
    bool success;
    QVariant p = QtJson::Json::parse(data, success);
    if (success){
        QMapIterator<QString,QVariant> it(p.toMap());
        while(it.hasNext()){
            int i = metaData()->indexOfProperty(it.key());
            if (i > -1){
                if (metaData()->property(i).type() == QDiagramToolkit::Rect){
                    //QVariantMap m = it.value().toMap();
                    //QRectF r;
                    //r.setX(m.value("x").toDouble());
                    //r.setY(m.value("y").toDouble());
                    //r.setWidth(m.value("with").toDouble());
                    //r.setHeight(m.value("height").toDouble());
                    m_properties[metaData()->property(i).name()] = it.value().toMap();
                } else {
                    m_properties[metaData()->property(i).name()] = it.value();
                }
            }
            it.next();
        }
    }
    return true;
}

QPen QAbstractDiagramGraphicsItem::pen() const
{
    return m_pen;
}

QPen QAbstractDiagramGraphicsItem::pen(const QString & attrs)
{
    QPen mPen;
    // color:<name or #xxxxxx;width:x;style:x
    QStringList mValues = attrs.split(";");
    for(int iValues = 0; iValues < mValues.size(); iValues++){
        QString mValue = mValues.at(iValues).section(":", 1);
        if (mValues.at(iValues).section(":",0, 0) == "color"){
            mPen.setColor(QColor(mValue));
        } else if (mValues.at(iValues).section(":",0, 0) == "width"){
            mPen.setWidth(mValues.at(iValues).section(":", 1).toInt());
        } else if (mValues.at(iValues).section(":",0, 0) == "style"){
            QString mStyle = mValues.at(iValues).section(":", 1);
            if (mStyle == "solid"){
                mPen.setStyle(Qt::SolidLine);
            } else if (mStyle == "dash"){
                mPen.setStyle(Qt::DashLine);
            } else if (mStyle == "dash"){
                mPen.setStyle(Qt::DashLine);
            } else if (mStyle == "dot"){
                mPen.setStyle(Qt::DotLine);
            } else if (mStyle == "dashdot"){
                mPen.setStyle(Qt::DashDotLine);
            } else if (mStyle == "dashdotdot"){
                mPen.setStyle(Qt::DashDotDotLine);
            } else {
                mPen.setStyle(Qt::SolidLine);
            }
        }
    }
    return mPen;
}

QMap<QString,QVariant> QAbstractDiagramGraphicsItem::properties() const
{
	QVariantMap p(m_properties);
	QMapIterator<QString,QVariant> it(m_dynamicProperties);
	while(it.hasNext()){
		it.next();
		p[it.key()] = it.value();
	}
    return p;
}

QDiagramProperty QAbstractDiagramGraphicsItem::property( const QString & name ) const
{
	QDiagramMetaProperty m = m_metadata->property(m_metadata->indexOfProperty(name));
	if (m.isValid()){
		return QDiagramProperty(this, m_metadata->indexOfProperty(name));
	}
	return QDiagramProperty();
}

QVariant QAbstractDiagramGraphicsItem::propertyValue(int index) const
{
	QDiagramMetaProperty m = m_metadata->property(index);
	QVariant v;
	if (m.isValid()){
		v = m_properties.value(m.name());
		if (v.isNull()){
			QAbstractDiagram* d = diagram();
			if (d){
				return d->defaultValue(m.type());
			} else {
				return QDiagramProperty::defaultValue(m.type());
			}
		}
	}
	return v;
}

void QAbstractDiagramGraphicsItem::restoreProperties(const QVariantMap & p)
{
	blockUndoCommands(true);
	QMapIterator<QString,QVariant> it(p);
	int index;
	while(it.hasNext()){
		it.next();
		index = metaData()->indexOfProperty(it.key());
		if (index > -1){
			if (metaData()->property(index).type() == QDiagramToolkit::Color){
				m_properties[it.key()] = QColor(it.value().toString());
			} else if (metaData()->property(index).type() == QDiagramToolkit::Rect){
				if (it.value().canConvert(QVariant::Map)){
					if (it.key() == "geometry"){
						QVariantMap m = m_properties.value("geometry").toMap();
						m["width"] = it.value().toMap().value("width", m.value("width"));
						m["height"] = it.value().toMap().value("height", m.value("height"));
						m["x"] = it.value().toMap().value("x").toDouble();
						m["y"] = it.value().toMap().value("y").toDouble();
						setPos(m.value("x").toDouble(), m.value("y").toDouble());
						m_properties[it.key()] = m;
					} else {
						m_properties[it.key()] = it.value().toMap();
					}
				}
			} else {
				m_properties[it.key()] = it.value();
			}
			if (it.key() == "zlevel"){
				setZValue(it.value().toDouble());
			}
		} else {
			addDynamicProperty(it.key(), it.value());
			m_properties[it.key()] = it.value();
		}
	}
	blockUndoCommands(false);
}

void QAbstractDiagramGraphicsItem::restoreFromProperties(const QVariantMap & properties)
{
}

QByteArray QAbstractDiagramGraphicsItem::serialize() const
{
//	QVariantMap m;
//	for (int i = 0; i < metaData()->propertyCount(); i++){
//		if (metaData()->property(i).type() == QDiagramToolkit::Brush){
//		} else if (metaData()->property(i).type() == QDiagramToolkit::Color){
//		} else if (metaData()->property(i).type() == QDiagramToolkit::EndOfLineStyle){
//		} else if (metaData()->property(i).type() == QDiagramToolkit::LineStyle){
//		} else if (metaData()->property(i).type() == QDiagramToolkit::Point){
//		//} else if (metaData()->property(i).type() == QDiagramToolkit::Rect){
//		//	QVariantMap r;
//		//	r["x"] = property(metaData()->property(i).name()).toRectF().x();
//		//	r["y"] = property(metaData()->property(i).name()).toRectF().y();
//		//	r["width"] = property(metaData()->property(i).name()).toRectF().width();
//		//	r["height"] = property(metaData()->property(i).name()).toRectF().height();
//  //          m[metaData()->property(i).name()] = r;
//		} else if (metaData()->property(i).type() == QDiagramToolkit::TextStyle){
//		} else {
//			m[metaData()->property(i).name()] = property(metaData()->property(i).name());
//		}
//	}
    return QtJson::Json::serialize(m_properties);
}

QColor QAbstractDiagramGraphicsItem::selectionColor() const
{
    if (diagram() !=0){
        return diagram()->selectionColor();
    }
    return QColor(Qt::magenta);
}

bool QAbstractDiagramGraphicsItem::setProperty(const QString & name, const QVariant & value)
{
	if (diagram() && diagram()->layers()->isLocked(this)){
		return false;
	}
	QDiagramMetaProperty metaProperty = m_metadata->property(m_metadata->indexOfProperty(name));
	// Check if property is dynamic
	if (!metaProperty.isValid()){
		QVariant currentValue = m_dynamicProperties.value(name);
		QVariant newValue = itemPropertyChange(name, value);
		addDynamicProperty(name, newValue);
		if (currentValue != newValue){
			QAbstractDiagram* d = diagram();
			if (d && !m_blockUndoCommands){
				d->undoStack()->push(new QDiagramChangePropertyCommand(d, (QDiagramShape*)this, name, currentValue, newValue));
			}
		}
		return false;
	}
    if (metaProperty.isReadOnly()){
		// Property is read only
        return false;
    }
	QVariant currentValue = m_properties.value(name);
	QVariant newValue = itemPropertyChange(name, QDiagramProperty::fromVariant(metaProperty.type(), value));
	if (currentValue != newValue){
		QAbstractDiagram* d = diagram();
		if (d && !m_blockUndoCommands){
			d->undoStack()->push(new QDiagramChangePropertyCommand(d, (QDiagramShape*)this, name, currentValue, newValue));
			return true;
		}
	}
	return false;
}

bool QAbstractDiagramGraphicsItem::undoCommandsBlocked() const
{
    return m_blockUndoCommands;
}

void QAbstractDiagramGraphicsItem::sendBackward()
{
	if (zValue() > 10){
		setZValue(zValue() - 10);
	}
}

void QAbstractDiagramGraphicsItem::sendToBack()
{
	setZValue(0);
}

void QAbstractDiagramGraphicsItem::setBrush( const QBrush & brush )
{
    m_brush = brush;
    QStringList mAttrs;
    mAttrs << QString("color:#%1").arg(brush.color().rgb(),3, 16);
    switch(brush.style()){
    case Qt::NoBrush:
        mAttrs << "style:nobrush";
        break;
    case Qt::SolidPattern:
        mAttrs << "style:solid";
        break;
    case Qt::Dense1Pattern:
        mAttrs << "style:dense1";
        break;
    case Qt::Dense2Pattern:
        mAttrs << "style:dense2";
        break;
    case Qt::Dense3Pattern:
        mAttrs << "style:dense3";
        break;
    case Qt::Dense4Pattern:
        mAttrs << "style:dense4";
        break;
    case Qt::Dense5Pattern:
        mAttrs << "style:dense5";
        break;
    case Qt::Dense6Pattern:
        mAttrs << "style:dense6";
        break;
    case Qt::Dense7Pattern:
        mAttrs << "style:dense7";
        break;
    case Qt::HorPattern:
        mAttrs << "style:hor";
        break;
    case Qt::VerPattern:
        mAttrs << "style:ver";
        break;
    case Qt::CrossPattern:
        mAttrs << "style:cross";
        break;
    case Qt::BDiagPattern:
        mAttrs << "style:bdiag";
        break;
    case Qt::FDiagPattern:
        mAttrs << "style:fdiag";
        break;
    case Qt::DiagCrossPattern:
        mAttrs << "style:diagcross";
        break;
    default:
        mAttrs << "style:solid";
    }
    setProperty("brush", mAttrs.join(";"));
}

void QAbstractDiagramGraphicsItem::triggerAction(QAction* action)
{
	Q_UNUSED(action);
}

void QAbstractDiagramGraphicsItem::triggerAction(const QString & name, const QVariant & data)
{
	Q_UNUSED(name);
	Q_UNUSED(data);
}

void QAbstractDiagramGraphicsItem::updateSizeGripHandles()
{
}

void QAbstractDiagramGraphicsItem::setPen(const QPen & pen)
{
    QStringList mAttrs;
    mAttrs << QString("color:#%1").arg(pen.color().rgb(),3, 16)
           << QString("width:%1").arg(pen.width());
    switch(pen.style()){
    case Qt::SolidLine:
        mAttrs << "style::solid";
        break;
    case Qt::DashLine:
        mAttrs << "style::dash";
        break;
    case Qt::DotLine:
        mAttrs << "style::dor";
        break;
    case Qt::DashDotLine:
        mAttrs << "style::dashdot";
        break;
    case Qt::DashDotDotLine:
        mAttrs << "style::dashdotdot";
        break;
    default:
        mAttrs << "style::solid";
        break;
    }
    setProperty("pen", mAttrs.join(";"));
}

QString QAbstractDiagramGraphicsItem::uuid() const
{
    return m_properties.value("uuid", "invalid").toString();
}

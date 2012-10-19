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
#include "qdiagramgraphicsitemmetadata.h"
#include "qdiagramundocommand.h"

#include <json.h>

QAbstractDiagramGraphicsItem::QAbstractDiagramGraphicsItem(QGraphicsItem* parent) :
    QGraphicsItem(parent)
{
    m_blockUndoCommands = false;
//    cName = "invalid";
    initMetaData("invalid", "invalid");
}

QAbstractDiagramGraphicsItem::QAbstractDiagramGraphicsItem(const QString & uuid, const QString & type, QGraphicsItem* parent) :
    QGraphicsItem(parent)
{
    m_blockUndoCommands = false;
    initMetaData(uuid, type);
//    setProperty("pen", "color:black;width:1;style:solid");
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

void QAbstractDiagramGraphicsItem::addProperty(const QString & name, QDiagramGraphicsItemMetaProperty::Type type, bool readOnly, const QVariant & value)
{
    m_metadata->addProperty(name, type, readOnly);
    if (!value.isNull()){
        m_properties[name] = value;
    }
}

void QAbstractDiagramGraphicsItem::addProperty(const QString &name, QDiagramGraphicsItemMetaProperty::Type type, const QMap<int, QString> &pairs, const QVariant &value)
{
    m_metadata->addProperty(name, type, pairs);
    m_properties[name] = value;
}

QBrush QAbstractDiagramGraphicsItem::brush() const
{
    return m_brush;
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

void QAbstractDiagramGraphicsItem::initMetaData(const QString & uuid, const QString & type)
{
    m_metadata = new QDiagramGraphicsItemMetaData(type);

    m_metadata->addProperty("uuid", QDiagramGraphicsItemMetaProperty::UUID, true);
    m_properties["uuid"] = uuid;

    m_metadata->addProperty("itemType", QDiagramGraphicsItemMetaProperty::String, true);
    m_properties["itemType"] = type;

    m_metadata->addProperty("geometry", QDiagramGraphicsItemMetaProperty::Rect, false);

    m_metadata->addProperty("pen", QDiagramGraphicsItemMetaProperty::Pen, false);
    QVariantMap m;

    m["color"] = Qt::black;
    m["width"] = 1;
    m["style"] = Qt::SolidLine;
    m_properties["pen"] = m;

    m_metadata->addProperty("zlevel", QDiagramGraphicsItemMetaProperty::Int, false);
}

QVariant QAbstractDiagramGraphicsItem::itemChange(GraphicsItemChange change, const QVariant & value)
{
    QVariant mValue(value);
    if (change == QGraphicsItem::ItemPositionChange){
        QPointF mPoint = itemPositionChange(value).toPointF();
        if (mPoint.x() < 0){
            mPoint.setX(0);
        }
        if (mPoint.y() <0){
            mPoint.setY(0);
        }
        mValue = mPoint;
    } else if (change == QGraphicsItem::ItemPositionHasChanged){
        return itemPositionHasChanged(value);
    } else if (change == QGraphicsItem::ItemSceneHasChanged){
        return itemSceneHasChanged(value);
    } else if (change == QGraphicsItem::ItemSelectedChange){
        return itemSelectedChange(value);
    } else if (change == QGraphicsItem::ItemSelectedHasChanged){
        return itemSelectedHasChanged(value);
    } else if (change == QGraphicsItem::ItemZValueHasChanged){
        return itemZValueHasChanged(value);
    }
    return QGraphicsItem::itemChange(change, value);
}

QVariant QAbstractDiagramGraphicsItem::itemPositionChange( const QVariant & value )
{
    return value;
}

QVariant QAbstractDiagramGraphicsItem::itemPositionHasChanged( const QVariant & value )
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

QDiagramGraphicsItemMetaData* QAbstractDiagramGraphicsItem::metaData() const
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
                if (metaData()->property(i).type() == QDiagramGraphicsItemMetaProperty::Rect){
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
//    QVariantMap m;
//    for (int i = 0; i < metaData()->propertyCount(); i++){
//        if (metaData()->property(i).type() == QDiagramGraphicsItemMetaProperty::Brush){
//        } else if (metaData()->property(i).type() == QDiagramGraphicsItemMetaProperty::Color){
//            m[metaData()->property(i).name()] = qvariant_cast<QColor>(property(metaData()->property(i).name())).name();
//        } else if (metaData()->property(i).type() == QDiagramGraphicsItemMetaProperty::EndOfLineStyle){
//        } else if (metaData()->property(i).type() == QDiagramGraphicsItemMetaProperty::LineStyle){
//        } else if (metaData()->property(i).type() == QDiagramGraphicsItemMetaProperty::Point){
//        } else if (metaData()->property(i).type() == QDiagramGraphicsItemMetaProperty::Rect){
//            QVariantMap r;
//            r["x"] = property(metaData()->property(i).name()).toRectF().x();
//            r["y"] = property(metaData()->property(i).name()).toRectF().y();
//            r["width"] = property(metaData()->property(i).name()).toRectF().width();
//            r["height"] = property(metaData()->property(i).name()).toRectF().height();
//            m[metaData()->property(i).name()] = r;
//        } else if (metaData()->property(i).type() == QDiagramGraphicsItemMetaProperty::TextStyle){
//        } else {
//            m[metaData()->property(i).name()] = property(metaData()->property(i).name());
//        }
//    }
//    return m;
    return m_properties;
}

QVariant QAbstractDiagramGraphicsItem::property( const QString & name ) const
{
    return m_properties.value(name, QVariant());
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
			if (metaData()->property(index).type() == QDiagramGraphicsItemMetaProperty::Rect){
				if (it.value().canConvert(QVariant::Map)){
					m_properties[it.key()] = it.value().toMap();
					setPos(it.value().toMap().value("x").toDouble(), it.value().toMap().value("y").toDouble());
				}
			} else {
				m_properties[it.key()] = it.value();
			}
		} else {
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
//		if (metaData()->property(i).type() == QDiagramGraphicsItemMetaProperty::Brush){
//		} else if (metaData()->property(i).type() == QDiagramGraphicsItemMetaProperty::Color){
//		} else if (metaData()->property(i).type() == QDiagramGraphicsItemMetaProperty::EndOfLineStyle){
//		} else if (metaData()->property(i).type() == QDiagramGraphicsItemMetaProperty::LineStyle){
//		} else if (metaData()->property(i).type() == QDiagramGraphicsItemMetaProperty::Point){
//		//} else if (metaData()->property(i).type() == QDiagramGraphicsItemMetaProperty::Rect){
//		//	QVariantMap r;
//		//	r["x"] = property(metaData()->property(i).name()).toRectF().x();
//		//	r["y"] = property(metaData()->property(i).name()).toRectF().y();
//		//	r["width"] = property(metaData()->property(i).name()).toRectF().width();
//		//	r["height"] = property(metaData()->property(i).name()).toRectF().height();
//  //          m[metaData()->property(i).name()] = r;
//		} else if (metaData()->property(i).type() == QDiagramGraphicsItemMetaProperty::TextStyle){
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
    QDiagramGraphicsItemMetaProperty mMetaProperty = m_metadata->property(m_metadata->indexOfProperty(name));
    if (mMetaProperty.isValid() && mMetaProperty.isReadOnly()){
        return false;
    }
    QVariant mOldValue = m_properties.value(name);
    if (mOldValue != value){
        QVariant newValue = itemPropertyChange(name, value);
        if (name == "brush"){
            m_brush = brush(newValue.toString());
            m_properties[name] = newValue;
        } else if (name == "geometry"){
			if (value.canConvert(QVariant::RectF)){
				QVariantMap m;
				m["x"] = value.toRectF().x();
				m["y"] = value.toRectF().y();
				m["width"] = value.toRectF().width();
				m["height"] = value.toRectF().height();
                newValue = m;
				m_properties[name] = m;
                blockUndoCommands(true);
	            prepareGeometryChange();
                setPos(value.toRectF().x(), value.toRectF().y());
                blockUndoCommands(false);
			} else if (value.canConvert(QVariant::Map)){
                blockUndoCommands(true);
                prepareGeometryChange();
		        setPos(newValue.toMap().value("x").toDouble(), newValue.toMap().value("y").toDouble());
                blockUndoCommands(false);
            }
        } else if (name == "pen"){
            if (value.canConvert(QVariant::Map)){

            } else {
                m_pen = pen(newValue.toString());
            }
            m_properties[name] = newValue;
        } else {
            m_properties[name] = newValue;
        }
        itemPropertyHasChanged(name, m_properties.value(name));
        QAbstractDiagram* mDiagram = diagram();
        if (mDiagram && !m_blockUndoCommands){
            mDiagram->undoStack()->push(new QDiagramChangePropertyCommand(mDiagram, (QDiagramShape*)this, name, mOldValue, newValue));
        }
    }
    return true;
}

bool QAbstractDiagramGraphicsItem::undoCommandsBlocked() const
{
    return m_blockUndoCommands;
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

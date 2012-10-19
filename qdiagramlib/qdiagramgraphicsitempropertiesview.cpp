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
#include "qdiagramgraphicsitempropertiesview.h"
#include "qdiagramgraphicsitempropertiesview_p.h"
#include "ui_qdiagramgraphicsitempropertiesview.h"

#include "qdiagram.h"
#include "qdiagramgraphicsitemmetadata.h"
#include "qdiagramendoflinestyle.h"
#include "qdiagramlinestyle.h"
#include "qdiagramshadowstyle.h"
#include "qdiagramtextstyle.h"

QMap<QString,QColor> QDiagramColorComboBox::sColorNameMap;

QPropertiesModelItem::QPropertiesModelItem(QAbstractDiagramGraphicsItem* item)
{
    m_index = -1;
    m_item = item;
    m_parent = 0;
}

QPropertiesModelItem::QPropertiesModelItem(QAbstractDiagramGraphicsItem* item, int index, QPropertiesModelItem *parent)
{
    m_index = index;
    m_item = item;
    m_parent = parent;
    if (m_parent != 0){
        m_parent->m_children.append(this);
    }
    QStringList colorNames = QColor::colorNames();
    for (int i = 0; i < colorNames.size(); ++i) {
        m_colorNameMap[colorNames.at(i)] = QColor(colorNames.at(i));
    }
}

QPropertiesModelItem::QPropertiesModelItem(const QString &name, QPropertiesModelItem *parent)
{
    m_index = -1;
    m_item = 0;
    m_name = name;
    m_parent = parent;
    if (m_parent != 0){
        m_parent->m_children.append(this);
    }
}

QPropertiesModelItem::~QPropertiesModelItem()
{
    qDeleteAll(m_children);
}

QPropertiesModelItem* QPropertiesModelItem::child(int index) const
{
    if (index < m_children.size()){
        return m_children.at(index);
    }
    return 0;
}

int QPropertiesModelItem::childCount() const
{
    return m_children.size();
}

QWidget* QPropertiesModelItem::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex & index, const QStyledItemDelegate* receiver) const
{
    if (type() == QDiagramGraphicsItemMetaProperty::Angle){
        QDoubleSpinBox* spinBox = new QDoubleSpinBox(parent);
        spinBox->setDecimals(2);
        spinBox->setMaximum(0.00);
        spinBox->setMaximum(359.99);
        spinBox->setSuffix(QObject::tr(" deg"));
        return spinBox;
    } else if (type() == QDiagramGraphicsItemMetaProperty::EndOfLineStyle){
        if (name() == "style"){
            return new QDiagramEndOfLineStyleComboBox(graphicsItem()->diagram()->endOfLineStyles(), parent);
        } else if (name() == "width"){
            QDoubleSpinBox* spinBox = new QDoubleSpinBox(parent);
            spinBox->setDecimals(2);
            spinBox->setMaximum(0.00);
            spinBox->setMaximum(99.00);
            return spinBox;
        }
    } else if (type() == QDiagramGraphicsItemMetaProperty::Color){
        QComboBox* cb= new QComboBox(parent);
        QStringList colorNames = QColor::colorNames();

        for (int i = 0; i < colorNames.size(); ++i) {
            QColor color(colorNames[i]);

            cb->insertItem(i, colorNames[i]);
            cb->setItemData(i, color, Qt::DecorationRole);
        }
		QObject::connect(cb, SIGNAL(activated(int)), receiver, SLOT(commitAndClose()));
        return cb;
    } else if (type() == QDiagramGraphicsItemMetaProperty::Enumeration){
		QComboBox* cb = new QComboBox(parent);
		QObject::connect(cb, SIGNAL(activated(int)), receiver, SLOT(commitAndClose()));
		return cb;
    } else if (type() == QDiagramGraphicsItemMetaProperty::Percent){
        QSpinBox* spinBox = new QSpinBox(parent);
        spinBox->setMaximum(0);
        spinBox->setMaximum(100);
        spinBox->setSuffix(QObject::tr(" %"));
        return spinBox;
    } else if (type() == QDiagramGraphicsItemMetaProperty::LineStyle){
        if (m_item == 0){
            if (m_index == 0){
                QComboBox* cb = new QComboBox(parent);
                QStringList colorNames = QColor::colorNames();

                for (int i = 0; i < colorNames.size(); ++i) {
                    QColor color(colorNames[i]);
                    cb->insertItem(i, colorNames[i]);
                    cb->setItemData(i, color, Qt::DecorationRole);
                }
				QObject::connect(cb, SIGNAL(activated(int)), receiver, SLOT(commitAndClose()));
                return cb;
            } else if (m_index == 1){
                QDoubleSpinBox* spinBox = new QDoubleSpinBox(parent);
                spinBox->setDecimals(2);
                spinBox->setMaximum(0.00);
                spinBox->setMaximum(99.00);
                return spinBox;
            } else if (m_index == 2){
                QComboBox* cb = new QComboBox(parent);
                Q_FOREACH(QDiagramLineStyle style, QDiagram::linesStyles()){
                    cb->addItem(style.icon(), style.name(), style.id());
                }
				QObject::connect(cb, SIGNAL(activated(int)), receiver, SLOT(commitAndClose()));
                return cb;
            }
        }
    } else if (type() == QDiagramGraphicsItemMetaProperty::Shadow){
		if (name() == "blurRadius"){
			QDoubleSpinBox* sb = new QDoubleSpinBox(parent);
			sb->setDecimals(0);
			QObject::connect(sb, SIGNAL(editingFinished()), receiver, SLOT(commitAndClose()));
			return sb;
        } else if (name() == "color"){
			QDiagramColorComboBox* cb = new QDiagramColorComboBox(parent);
			QObject::connect(cb, SIGNAL(activated(int)), receiver, SLOT(commitAndClose()));
			return cb;
        } else if (name() == "offsetX"){
			QDoubleSpinBox* sb = new QDoubleSpinBox(parent);
			sb->setDecimals(0);
			QObject::connect(sb, SIGNAL(editingFinished()), receiver, SLOT(commitAndClose()));
			return sb;
        } else if (name() == "offsetY"){
			QDoubleSpinBox* sb = new QDoubleSpinBox(parent);
			sb->setDecimals(0);
			QObject::connect(sb, SIGNAL(editingFinished()), receiver, SLOT(commitAndClose()));
			return sb;
		} else if (name() == "visible"){
            QCheckBox* cb =  new QCheckBox(parent);
            cb->setAutoFillBackground(true);
			QObject::connect(cb, SIGNAL(stateChanged(int)), receiver, SLOT(commitAndClose()));
            return cb;
		}
    } else if (type() == QDiagramGraphicsItemMetaProperty::TextStyle){
        if (name() == "bold" || name() == "italic" || name() == "underline" || name() == "strikeOut"){
            QCheckBox* cb =  new QCheckBox(parent);
            cb->setAutoFillBackground(true);
            return cb;
        } else if (name() == "color"){
			QDiagramColorComboBox* cb = new QDiagramColorComboBox(parent);
			QObject::connect(cb, SIGNAL(activated(int)), receiver, SLOT(commitAndClose()));
			return cb;
        } else if (name() == "family"){
			QComboBox* cb = new QComboBox(parent);
			QObject::connect(cb, SIGNAL(activated(int)), receiver, SLOT(commitAndClose()));
			return cb;
        } else if (name() == "size"){
            return new QSpinBox(parent);
        }
    }
    return 0;
}

QVariant QPropertiesModelItem::data(const QModelIndex & index, int role) const
{
    if (role == Qt::DisplayRole){
        if (index.column() == 0){
            if (m_item == 0){
                if (type() == QDiagramGraphicsItemMetaProperty::TextStyle){
                    return name();
                } else if (type() == QDiagramGraphicsItemMetaProperty::LineStyle){
                    if (index.row() == 0){
                        return QObject::tr("color");
                    } else if (index.row() == 1){
                        return QObject::tr("width");
                    } else if (index.row() == 2){
                        return QObject::tr("itemType");
                    }
                } else if (type() == QDiagramGraphicsItemMetaProperty::EndOfLineStyle){
                    return name();
                } else if (m_parent->graphicsItem()->metaData()->property(m_parent->index()).type() == QDiagramGraphicsItemMetaProperty::Rect){
                    if (index.row() == 0){
                        return QObject::tr("x");
                    } else if (index.row() == 1){
                        return QObject::tr("y");
                    } else if (index.row() == 2){
                        return QObject::tr("width");
                    } else if (index.row() == 3){
                        return QObject::tr("height");
                    }
                } else if (m_parent->graphicsItem()->metaData()->property(m_parent->index()).type() == QDiagramGraphicsItemMetaProperty::Point){
                    if (index.row() == 0){
                        return QObject::tr("x");
                    } else if (index.row() == 1){
                        return QObject::tr("y");
                    }
                } else if (m_parent->graphicsItem()->metaData()->property(m_parent->index()).type() == QDiagramGraphicsItemMetaProperty::Shadow){
					return name();
                }
            } else {
                return m_item->metaData()->property(m_index).name();
            }
        } else if (index.column() == 1){
            if (m_item == 0){
                if (type() == QDiagramGraphicsItemMetaProperty::TextStyle){
                    QDiagramTextStyle style = qvariant_cast<QDiagramTextStyle>(value());
                    if (name() == "color"){
                        return QDiagramColorComboBox::colorName(style.color());
                    }
                    return style.value(index.row());
                } else if (type() == QDiagramGraphicsItemMetaProperty::EndOfLineStyle){
                    QDiagramEndOfLineStyle style = qvariant_cast<QDiagramEndOfLineStyle>(value());
                    if (index.row() == 0){
                        return style.caption();
                    } else if (index.row() == 1){
                        return style.width();
                    }
                } else if (type() == QDiagramGraphicsItemMetaProperty::LineStyle){
                    QDiagramLineStyle style = qvariant_cast<QDiagramLineStyle>(value());
                    if (index.row() == 0){
                        return m_colorNameMap.key(style.color().name());
                    } else if (index.row() == 1){
                        return style.width();
                    } else if (index.row() == 2){
                        return style.title();
                    }
                } else if (m_parent->graphicsItem()->metaData()->property(m_parent->m_index).type() == QDiagramGraphicsItemMetaProperty::Point){
                    if (index.row() == 0){
                        return m_parent->m_item->property(m_parent->m_item->metaData()->property(m_parent->m_index).name()).toPointF().x();
                    } else if (index.row() == 1){
                        return m_parent->m_item->property(m_parent->m_item->metaData()->property(m_parent->m_index).name()).toPointF().y();
                    }
                } else if (m_parent->graphicsItem()->metaData()->property(m_parent->m_index).type() == QDiagramGraphicsItemMetaProperty::Rect){
                    return value().toMap().value(name());
                } else if (type() == QDiagramGraphicsItemMetaProperty::Shadow){
					QDiagramShadowStyle shadow = qvariant_cast<QDiagramShadowStyle>(value());
                    if (name() == "color"){
                        return QDiagramColorComboBox::colorName(shadow.color());
                    }
                    return shadow.value(index.row());
                }
            } else {
                if (type() == QDiagramGraphicsItemMetaProperty::Angle){
                    return QString(QObject::tr("%1 deg")).arg(value().toDouble());
                } else if (type() == QDiagramGraphicsItemMetaProperty::Color){
                    QColor color = qvariant_cast<QColor>(value());
                    return m_colorNameMap.key(color, QObject::tr("<invalid>"));
                } else if (type() == QDiagramGraphicsItemMetaProperty::Enumeration){
                    return m_item->metaData()->property(m_index).enumerator().key(value().toInt());
                } else if (type() == QDiagramGraphicsItemMetaProperty::EndOfLineStyle){
                    QDiagramEndOfLineStyle style = qvariant_cast<QDiagramEndOfLineStyle>(value());
                    return QString("%1, %2").arg(style.caption()).arg(style.width());
                } else if (type() == QDiagramGraphicsItemMetaProperty::LineStyle){
                    QDiagramLineStyle style = qvariant_cast<QDiagramLineStyle>(value());
                    return QString("%1, %2, %3").arg(m_colorNameMap.key(style.color()))
                            .arg(style.width())
                            .arg(style.title());
                } else if (type() == QDiagramGraphicsItemMetaProperty::Percent){
                        return QString("%1 %").arg(value().toDouble());
                } else if (m_item->metaData()->property(m_index).type() == QDiagramGraphicsItemMetaProperty::Point){
                    return QString("(%1, %2)").arg(m_item->property(m_item->metaData()->property(m_index).name()).toPointF().x()).arg(m_item->property(m_item->metaData()->property(m_index).name()).toPointF().y());
                } else if (m_item->metaData()->property(m_index).type() == QDiagramGraphicsItemMetaProperty::Rect){
                    return QString("(%1, %2) %3 x %4")
                            .arg(value().toMap().value("x").toInt())
                            .arg(value().toMap().value("y").toInt())
                            .arg(value().toMap().value("width").toInt())
                            .arg(value().toMap().value("height").toInt());
                } else if (type() == QDiagramGraphicsItemMetaProperty::Shadow){
					return QString("%1").arg(value().toMap().value("visible").toString());
                } else if (type() == QDiagramGraphicsItemMetaProperty::TextStyle){
                    QDiagramTextStyle s = qvariant_cast<QDiagramTextStyle>(value());
                    return QString("%1, %2, %3").arg(s.family()).arg(s.size()).arg(QDiagramColorComboBox::colorName(s.color()));
                } else {
                    return m_item->property(m_item->metaData()->property(m_index).name());
                }
            }
        }
    } else if (role == Qt::DecorationRole){
        if (index.column() == 1){
            if (m_item == 0){
                if (type() == QDiagramGraphicsItemMetaProperty::LineStyle){
                    QDiagramLineStyle style = qvariant_cast<QDiagramLineStyle>(value());
                    if (index.row() == 0){
                        return style.color();
                    } else if (index.row() == 2){
                        return style.icon();
                    }
                } else if (type() == QDiagramGraphicsItemMetaProperty::Shadow){
					QDiagramShadowStyle s = qvariant_cast<QDiagramShadowStyle>(value());
                    if (name() == "color"){
                        return s.color();
                    }
                } else if (type() == QDiagramGraphicsItemMetaProperty::TextStyle){
                    QDiagramTextStyle style = qvariant_cast<QDiagramTextStyle>(value());
                    if (name() == "color"){
                        return style.color();
                    }
                }
            } else {
                if (type() == QDiagramGraphicsItemMetaProperty::Color){
                    return qvariant_cast<QColor>(value());
                } else if (type() == QDiagramGraphicsItemMetaProperty::EndOfLineStyle){
                        QDiagramEndOfLineStyle s = qvariant_cast<QDiagramEndOfLineStyle>(value());
                        return s.icon();
                } else if (type() == QDiagramGraphicsItemMetaProperty::LineStyle){
//                    QDiagramLineStyle style = qvariant_cast<QDiagramLineStyle>(value());
//                    if (style.isValid()){
//                        return style.color();
//                    }
                }
            }
        }
    } else if (role == Qt::EditRole){
        if (index.column() == 1){
            if (m_item == 0){
                if (type() == QDiagramGraphicsItemMetaProperty::TextStyle){
                    QDiagramTextStyle style = qvariant_cast<QDiagramTextStyle>(value());
                    return style.value(index.row());
                } else if (type() == QDiagramGraphicsItemMetaProperty::EndOfLineStyle){
                    QDiagramEndOfLineStyle s = qvariant_cast<QDiagramEndOfLineStyle>(value());
                    return s.value(name());
                } else if (type() == QDiagramGraphicsItemMetaProperty::LineStyle){
                    return m_parent->m_item->property(m_parent->m_item->metaData()->property(m_parent->m_index).name());
                } else if (type() == QDiagramGraphicsItemMetaProperty::Rect){
                    QRectF r = m_parent->m_item->property(m_parent->m_item->metaData()->property(m_parent->m_index).name()).toRectF();
                    if (index.row() == 0){
                        return r.left();
                    } else if (index.row() == 1){
                        return r.top();
                    } else if (index.row() == 2){
                        return r.width();
                    } else if (index.row() == 3){
                        return r.height();
                    }
                } else if (type() == QDiagramGraphicsItemMetaProperty::Shadow){
					QDiagramShadowStyle s = qvariant_cast<QDiagramShadowStyle>(value());
                    return s.value(name());
                }
            } else {
                return m_item->property(m_item->metaData()->property(m_index).name());
            }
        }
    } else if (role == QPropertiesModel::MetaTypeRole){
        if (m_item == 0){
            return m_parent->graphicsItem()->metaData()->property(m_parent->index()).type();
        } else {
            return m_item->metaData()->property(m_index).type();
        }
    }
    return QVariant();
}

Qt::ItemFlags QPropertiesModelItem::flags() const
{
    Qt::ItemFlags f;
    if (type() == QDiagramGraphicsItemMetaProperty::LineStyle){
        if (m_item == 0){

        }
    }
    return f;
}

QAbstractDiagramGraphicsItem* QPropertiesModelItem::graphicsItem() const
{
    if (m_parent == 0){
        return m_item;
    }
    return m_parent->graphicsItem();
}

int QPropertiesModelItem::index() const
{
    return m_index;
}

QString QPropertiesModelItem::name() const
{
    if (m_index == -1){
        return  m_name;
    }
    return graphicsItem()->metaData()->property(m_index).name();
}

bool QPropertiesModelItem::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index, const QStyleOptionViewItemV4 &opt4)
{
    QStyleOptionViewItemV4 o(opt4);
    if (index.column() == 1){
        if (index.data(QPropertiesModel::MetaTypeRole).toInt() == QDiagramGraphicsItemMetaProperty::Angle){
//            o.text = "";
//            QStyle *style = o.widget ? o.widget->style() : QApplication::style();
//            style->drawControl(QStyle::CE_ItemViewItem, &o, painter, o.widget);
//            if (o.state & QStyle::State_Selected){
////                painter->fillRect(o.rect, o.palette.highlight());
//            }
//            QPalette::ColorGroup cg = o.state & QStyle::State_Enabled ? QPalette::Normal : QPalette::Disabled;
//            if (cg == QPalette::Normal && !(o.state & QStyle::State_Active))
//                cg = QPalette::Inactive;

//            // set pen color
////            if (opt.state & QStyle::State_Selected)
////                painter->setPen(opt.palette.color(cg, QPalette::HighlightedText));
////            else
////                painter->setPen(opt.palette.color(cg, QPalette::Text));

//            painter->drawText(o.rect, o.displayAlignment, QString(QObject::tr("%1 deg")).arg(index.data().toDouble()));
//            return true;
        } else if (index.data(QPropertiesModel::MetaTypeRole).toInt() == QDiagramGraphicsItemMetaProperty::Percent){
//            o.text = "";
//            QStyle *style = o.widget ? o.widget->style() : QApplication::style();
//            style->drawControl(QStyle::CE_ItemViewItem, &o, painter, o.widget);
//            if (o.state & QStyle::State_Selected){
////                painter->fillRect(o.rect, o.palette.highlight());
//            }
//            QRect rect = o.rect;
//            QPalette::ColorGroup cg = o.state & QStyle::State_Enabled ? QPalette::Normal : QPalette::Disabled;
//            if (cg == QPalette::Normal && !(o.state & QStyle::State_Active))
//                cg = QPalette::Inactive;

//            // set pen color
////            if (opt.state & QStyle::State_Selected)
////                painter->setPen(opt.palette.color(cg, QPalette::HighlightedText));
////            else
////                painter->setPen(opt.palette.color(cg, QPalette::Text));

//            painter->drawText(o.rect, o.displayAlignment, QString("%1 %").arg(index.data().toDouble()));
//            return true;
        }
    }
    return false;
}

QPropertiesModelItem* QPropertiesModelItem::parent() const
{
    return m_parent;
}

int QPropertiesModelItem::row() const
{
    if (m_parent){
        return m_parent->m_children.indexOf(const_cast<QPropertiesModelItem*>(this));
    }
    return -1;
}

bool QPropertiesModelItem::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if (role == Qt::EditRole){
        if (index.column() == 1){
            if (m_item == 0){
                return m_parent->m_item->setProperty(m_parent->m_item->metaData()->property(m_parent->m_index).name(), value);
            } else {
                return m_item->setProperty(m_item->metaData()->property(m_index).name(), value);
            }
        }
    }
    return false;
}

bool QPropertiesModelItem::setEditorData(QWidget* editor, const QModelIndex & index) const
{
    if (type() == QDiagramGraphicsItemMetaProperty::Enumeration){
        QComboBox* cb = qobject_cast<QComboBox*>(editor);
        for (int i = 0; i < m_item->metaData()->property(m_index).enumerator().keys(); i++){
            cb->addItem(m_item->metaData()->property(m_index).enumerator().key(i), m_item->metaData()->property(m_index).enumerator().value(i));
        }
        cb->setCurrentIndex(cb->findData(value().toInt()));
    } else if (index.data(QPropertiesModel::MetaTypeRole).toInt() == QDiagramGraphicsItemMetaProperty::Angle){
        QDoubleSpinBox* spinBox = qobject_cast<QDoubleSpinBox*>(editor);
        spinBox->setValue(value().toDouble());
        return true;
    } else if (index.data(QPropertiesModel::MetaTypeRole).toInt() == QDiagramGraphicsItemMetaProperty::Color){
        QComboBox* comboBox = qobject_cast<QComboBox*>(editor);
        QColor color = qvariant_cast<QColor>(value());
        comboBox->setCurrentIndex(comboBox->findData(color, int(Qt::DecorationRole)));
        return true;
    } else if (type() == QDiagramGraphicsItemMetaProperty::EndOfLineStyle){
        QDiagramEndOfLineStyle s = qvariant_cast<QDiagramEndOfLineStyle>(value());
        if (name() == "style"){
            QDiagramEndOfLineStyleComboBox* cb = qobject_cast<QDiagramEndOfLineStyleComboBox*>(editor);
            cb->setCurrentIndex(cb->findData(s.id()));
        } else if (name() == "width"){
            QDoubleSpinBox* sb = qobject_cast<QDoubleSpinBox*>(editor);
            sb->setValue(s.width());
        }
        return true;
    } else if (type() == QDiagramGraphicsItemMetaProperty::LineStyle){
        QDiagramLineStyle style = qvariant_cast<QDiagramLineStyle>(value());
        if (m_index == 0){
            QComboBox* comboBox = qobject_cast<QComboBox*>(editor);
            comboBox->setCurrentIndex(comboBox->findData(style.color(), int(Qt::DecorationRole)));
            return true;
        } else if (m_index == 1){
            QDoubleSpinBox* spinBox = qobject_cast<QDoubleSpinBox*>(editor);
            spinBox->setValue(style.width());
            return true;
        } else if (m_index == 2){
            QComboBox* comboBox = qobject_cast<QComboBox*>(editor);
            comboBox->setCurrentIndex(comboBox->findData(style.id()));
        }
    } else if (index.data(QPropertiesModel::MetaTypeRole).toInt() == QDiagramGraphicsItemMetaProperty::Percent){
        QSpinBox* spinBox = qobject_cast<QSpinBox*>(editor);
        spinBox->setValue(value().toDouble());
        return true;
    } else if (type() == QDiagramGraphicsItemMetaProperty::Shadow){
		QDiagramShadowStyle s = qvariant_cast<QDiagramShadowStyle>(value());
        if (name() == "color"){
            QDiagramColorComboBox* comboBox = qobject_cast<QDiagramColorComboBox*>(editor);
            QColor color = qvariant_cast<QColor>(s.color());
            comboBox->setCurrentIndex(comboBox->findData(color, int(Qt::DecorationRole)));
            return true;
		} else if (name() == "blurRadius"){
			QDoubleSpinBox* w = qobject_cast<QDoubleSpinBox*>(editor);
			w->setValue(s.blurRadius());
		} else if (name() == "offsetX"){
			QDoubleSpinBox* w = qobject_cast<QDoubleSpinBox*>(editor);
			w->setValue(s.offsetX());
		} else if (name() == "offsetY"){
			QDoubleSpinBox* w = qobject_cast<QDoubleSpinBox*>(editor);
			w->setValue(s.offsetY());
		} else if (name() == "visible"){
            QCheckBox* checkBox = qobject_cast<QCheckBox*>(editor);
            checkBox->setChecked(s.isVisible());
            return true;
		}
    } else if (type() == QDiagramGraphicsItemMetaProperty::TextStyle){
        QDiagramTextStyle s = qvariant_cast<QDiagramTextStyle>(value());
        if (name() == "bold"){
            QCheckBox* checkBox = qobject_cast<QCheckBox*>(editor);
            checkBox->setChecked(s.bold());
            return true;
        } else if (name() == "color"){
            QDiagramColorComboBox* comboBox = qobject_cast<QDiagramColorComboBox*>(editor);
            QColor color = qvariant_cast<QColor>(s.color());
            comboBox->setCurrentIndex(comboBox->findData(color, int(Qt::DecorationRole)));
            return true;
        } else if (name() == "family"){
            QComboBox* comboBox = qobject_cast<QComboBox*>(editor);
            QFontDatabase d;
            comboBox->addItems(d.families());
            comboBox->setCurrentIndex(comboBox->findText(s.value("family").toString()));
            return true;
        } else if (name() == "italic"){
            QCheckBox* checkBox = qobject_cast<QCheckBox*>(editor);
            checkBox->setChecked(s.italic());
            return true;
        } else if (name() == "size"){
            QSpinBox* sb = qobject_cast<QSpinBox*>(editor);
            sb->setValue(s.size());
            return true;
        } else if (name() == "strikeOut"){
            QCheckBox* checkBox = qobject_cast<QCheckBox*>(editor);
            checkBox->setChecked(s.strikeOut());
            return true;
        } else if (name() == "underline"){
            QCheckBox* checkBox = qobject_cast<QCheckBox*>(editor);
            checkBox->setChecked(s.underline());
            return true;
        }
    }
    return false;
}

bool QPropertiesModelItem::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if (index.data(QPropertiesModel::MetaTypeRole).toInt() == QDiagramGraphicsItemMetaProperty::Angle){
        QDoubleSpinBox* spinBox = qobject_cast<QDoubleSpinBox*>(editor);
        model->setData(index, spinBox->value());
        return true;
    } else if (index.data(QPropertiesModel::MetaTypeRole).toInt() == QDiagramGraphicsItemMetaProperty::Color){
        QComboBox* comboBox = qobject_cast<QComboBox*>(editor);
        model->setData(index, qvariant_cast<QColor>(comboBox->itemData(comboBox->currentIndex(), Qt::DecorationRole)));
        return true;
    } else if (type() == QDiagramGraphicsItemMetaProperty::EndOfLineStyle){
        QDiagramEndOfLineStyleComboBox* cb = qobject_cast<QDiagramEndOfLineStyleComboBox*>(editor);
        QDiagramEndOfLineStyle currentStyle = qvariant_cast<QDiagramEndOfLineStyle>(value());
        if (name() == "style"){
            QDiagramEndOfLineStyle newStyle = graphicsItem()->diagram()->endOfLineStyle(cb->itemData(cb->currentIndex()).toString());
            newStyle.setWidth(currentStyle.width());
            model->setData(index, QVariant::fromValue(newStyle));
        } else if (name() == "width"){
            QDoubleSpinBox* sb = qobject_cast<QDoubleSpinBox*>(editor);
            currentStyle.setWidth(sb->value());
            model->setData(index, QVariant::fromValue(currentStyle));
        }
        return true;
    } else if (type() == QDiagramGraphicsItemMetaProperty::Enumeration){
        QComboBox* cb = qobject_cast<QComboBox*>(editor);
        model->setData(index, cb->itemData(cb->currentIndex()));
        return true;
    } else if (type() == QDiagramGraphicsItemMetaProperty::LineStyle){
        QDiagramLineStyle style = qvariant_cast<QDiagramLineStyle>(value());
        if (m_index == 0){
            QComboBox* comboBox = qobject_cast<QComboBox*>(editor);
            style.setColor(qvariant_cast<QColor>(comboBox->itemData(comboBox->currentIndex(), Qt::DecorationRole)));
            model->setData(index, QVariant::fromValue(style));
        } else if (m_index == 1){
            QDoubleSpinBox* spinBox = qobject_cast<QDoubleSpinBox*>(editor);
            style.setWidth(spinBox->value());
            model->setData(index, QVariant::fromValue(style));
        } else if (m_index == 2){
            QComboBox* comboBox = qobject_cast<QComboBox*>(editor);
            QDiagramLineStyle newStyle = QDiagram::lineStyle(comboBox->itemData(comboBox->currentIndex()).toString());
            if (newStyle.isValid()){
                newStyle.setColor(style.color());
                newStyle.setWidth(style.width());
                model->setData(index, QVariant::fromValue(newStyle));
            }
        }
        return true;
    } else if (index.data(QPropertiesModel::MetaTypeRole).toInt() == QDiagramGraphicsItemMetaProperty::Percent){
        QSpinBox* spinBox = qobject_cast<QSpinBox*>(editor);
        model->setData(index, spinBox->value());
        return true;
    } else if (index.data(QPropertiesModel::MetaTypeRole).toInt() == QDiagramGraphicsItemMetaProperty::Shadow){
        QDiagramShadowStyle s = qvariant_cast<QDiagramShadowStyle>(value());
		if (name() == "blurRadius"){
			QDoubleSpinBox* w = qobject_cast<QDoubleSpinBox*>(editor);
			s.setBlurRadius(w->value());
            model->setData(index, QVariant::fromValue(s));
		} else if (name() == "color"){
            QDiagramColorComboBox* comboBox = qobject_cast<QDiagramColorComboBox*>(editor);
            s.setColor(qvariant_cast<QColor>(comboBox->currentColor()));
            model->setData(index, QVariant::fromValue(s));
		} else if (name() == "offsetX"){
			QDoubleSpinBox* w = qobject_cast<QDoubleSpinBox*>(editor);
			s.setOffsetX(w->value());
            model->setData(index, QVariant::fromValue(s));
		} else if (name() == "offsetY"){
			QDoubleSpinBox* w = qobject_cast<QDoubleSpinBox*>(editor);
			s.setOffsetY(w->value());
            model->setData(index, QVariant::fromValue(s));
		} else if (name() == "visible"){
            QCheckBox* cb = qobject_cast<QCheckBox*>(editor);
			s.setVisible(cb->isChecked());
			model->setData(index, QVariant::fromValue(s));
		}
        return true;
    } else if (index.data(QPropertiesModel::MetaTypeRole).toInt() == QDiagramGraphicsItemMetaProperty::TextStyle){
        QDiagramTextStyle s = qvariant_cast<QDiagramTextStyle>(value());
        if (name() == "bold"){
            QCheckBox* cb = qobject_cast<QCheckBox*>(editor);
            s.setBold(cb->isChecked());
            model->setData(index, QVariant::fromValue(s));
        } else if (name() == "color"){
            QDiagramColorComboBox* comboBox = qobject_cast<QDiagramColorComboBox*>(editor);
            s.setColor(qvariant_cast<QColor>(comboBox->currentColor()));
            model->setData(index, QVariant::fromValue(s));
        } else if (name() == "family"){
            QComboBox* cb = qobject_cast<QComboBox*>(editor);
            s.setFamily(cb->currentText());
            model->setData(index, QVariant::fromValue(s));
        } else if (name() == "italic"){
            QCheckBox* cb = qobject_cast<QCheckBox*>(editor);
            s.setItalic(cb->isChecked());
            model->setData(index, QVariant::fromValue(s));
        } else if (name() == "size"){
            QSpinBox* sb = qobject_cast<QSpinBox*>(editor);
            s.setSize(sb->value());
            model->setData(index, QVariant::fromValue(s));
        } else if (name() == "strikeOut"){
            QCheckBox* cb = qobject_cast<QCheckBox*>(editor);
            s.setStrikeOut(cb->isChecked());
            model->setData(index, QVariant::fromValue(s));
        } else if (name() == "underline"){
            QCheckBox* cb = qobject_cast<QCheckBox*>(editor);
            s.setUnderline(cb->isChecked());
            model->setData(index, QVariant::fromValue(s));
        }
        return true;
    }
    return false;
}

void QPropertiesModelItem::setValue(const QVariant &value)
{
    if (m_item == 0){
        m_parent->setValue(value);
    } else {
        m_item->setProperty(m_item->metaData()->property(m_index).name(), value);
    }
}

QDiagramGraphicsItemMetaProperty::Type QPropertiesModelItem::type() const
{
    if (m_item == 0){
        return m_parent->m_item->metaData()->property(m_parent->m_index).type();
    } else {
        return m_item->metaData()->property(m_index).type();
    }
}

QVariant QPropertiesModelItem::value() const
{
    if (m_item == 0){
        return m_parent->value();
    }    
    return m_item->property(m_item->metaData()->property(m_index).name());
}


QPropertiesModel::QPropertiesModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    r = new QPropertiesModelItem(0);
}

QPropertiesModel::QPropertiesModel(QAbstractDiagramGraphicsItem* item, QObject* parent) :
    QAbstractItemModel(parent)
{
    r = new QPropertiesModelItem(0);
    buildModel(item);
}

QPropertiesModel::~QPropertiesModel()
{
    delete r;
}

void QPropertiesModel::buildModel(QAbstractDiagramGraphicsItem* item)
{
    beginResetModel();
    delete r;
    r = new QPropertiesModelItem(item);
    QPropertiesModelItem* p;
    for(int iProperty = 0; iProperty < item->metaData()->propertyCount(); iProperty++){
        p = new QPropertiesModelItem(item, iProperty, r);
        if (item->metaData()->property(iProperty).type() == QDiagramGraphicsItemMetaProperty::TextStyle){
            QDiagramTextStyle s;
            for (int i = 0; i < s.propertyCount(); i++){
                new QPropertiesModelItem(s.key(i), p);
            }
        } else if (item->metaData()->property(iProperty).type() == QDiagramGraphicsItemMetaProperty::EndOfLineStyle){
            new QPropertiesModelItem("style", p);
            new QPropertiesModelItem("width", p);
        } else if (item->metaData()->property(iProperty).type() == QDiagramGraphicsItemMetaProperty::LineStyle){
            new QPropertiesModelItem(0, 0, p);
            new QPropertiesModelItem(0, 1, p);
            new QPropertiesModelItem(0, 2, p);
        } else if (item->metaData()->property(iProperty).type() == QDiagramGraphicsItemMetaProperty::Rect){
            new QPropertiesModelItem("x", p);
            new QPropertiesModelItem("y", p);
            new QPropertiesModelItem("width", p);
            new QPropertiesModelItem("height", p);
        } else if (item->metaData()->property(iProperty).type() == QDiagramGraphicsItemMetaProperty::Point){
            new QPropertiesModelItem(0, 0, p);
            new QPropertiesModelItem(0, 1, p);
        } else if (item->metaData()->property(iProperty).type() == QDiagramGraphicsItemMetaProperty::Shadow){
            QDiagramShadowStyle s;
            for (int i = 0; i < s.propertyCount(); i++){
                new QPropertiesModelItem(s.key(i), p);
            }
        }
    }
    endResetModel();
}

void QPropertiesModel::reset()
{
    beginResetModel();
    delete r;
    r = new QPropertiesModelItem(0);

    endResetModel();
}

int QPropertiesModel::columnCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return 2;
}

QVariant QPropertiesModel::data(const QModelIndex & index, int role) const
{
    if (!index.isValid()){
        return QVariant();
    }

    QPropertiesModelItem* propertyItem = static_cast<QPropertiesModelItem*>(index.internalPointer());
    if (propertyItem){
        return propertyItem->data(index, role);
    }
    return QVariant();
}

Qt::ItemFlags QPropertiesModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags;
    if (index.isValid()){
        flags |= Qt::ItemIsSelectable;
        flags |= Qt::ItemIsEnabled;
        if (index.column() == 1){
            QPropertiesModelItem* modelItem = static_cast<QPropertiesModelItem*>(index.internalPointer());
            if (modelItem->parent() == r){
                if (modelItem->graphicsItem()->metaData() && !modelItem->graphicsItem()->metaData()->property(modelItem->index()).isReadOnly()){
                    flags |= Qt::ItemIsEditable;
                }
            } else {
                if (modelItem->type() == QDiagramGraphicsItemMetaProperty::EndOfLineStyle){
                    flags |= Qt::ItemIsEditable;
                } else if (modelItem->type() == QDiagramGraphicsItemMetaProperty::LineStyle){
                    flags |= Qt::ItemIsEditable;
                } else if (modelItem->type() == QDiagramGraphicsItemMetaProperty::Shadow){
                    flags |= Qt::ItemIsEditable;
                } else if (modelItem->type() == QDiagramGraphicsItemMetaProperty::TextStyle){
                    flags |= Qt::ItemIsEditable;
                }
            }
        }
    }
    return flags;
}

QVariant QPropertiesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole){
        if (orientation == Qt::Horizontal){
            if (section == 0){
                return tr("Name");
            } else if (section == 1){
                return tr("Value");
            }
        }
    }
    return QVariant();
}

QModelIndex QPropertiesModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    QPropertiesModelItem* mParent;

    if (!parent.isValid()){
        mParent = r;
    } else {
        mParent = static_cast<QPropertiesModelItem*>(parent.internalPointer());
    }

    QPropertiesModelItem* mChild = mParent->child(row);
    if (mChild){
        return createIndex(row, column, mChild);
    } else {
        return QModelIndex();
    }
}

QModelIndex QPropertiesModel::parent(const QModelIndex &index) const
{
    if (!index.isValid()){
        return QModelIndex();
    }

    QPropertiesModelItem* mChild = static_cast<QPropertiesModelItem*>(index.internalPointer());
    QPropertiesModelItem* mParent = mChild->parent();

    if (mParent == r){
        return QModelIndex();
    }

    return createIndex(mParent->row(), 0, mParent);
}

int QPropertiesModel::rowCount(const QModelIndex & parent) const
{
    QPropertiesModelItem* mParent;
    if (parent.column() > 0){
        return 0;
    }

    if (!parent.isValid()){
        mParent = r;
    } else {
        mParent = static_cast<QPropertiesModelItem*>(parent.internalPointer());
    }
    return mParent->childCount();
}

bool QPropertiesModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if (role == Qt::EditRole){
        QPropertiesModelItem* propertyItem = static_cast<QPropertiesModelItem*>(index.internalPointer());
        if (propertyItem){
            propertyItem->setValue(value);
            return true;
        }
    }
    return false;
}

QDiagramGraphicsItemPropertiesView::QDiagramGraphicsItemPropertiesView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QDiagramGraphicsItemPropertiesView)
{
    ui->setupUi(this);

    ui->propertyView->setItemDelegate(new QDiagramGraphicsItemDelegate(this));
    m_model = new QPropertiesModel(this);
    ui->propertyView->setModel(m_model);
}

QDiagramGraphicsItemPropertiesView::~QDiagramGraphicsItemPropertiesView()
{
    delete ui;
}

void QDiagramGraphicsItemPropertiesView::clear()
{
    m_model->reset();
}

void QDiagramGraphicsItemPropertiesView::showProperties(QList<QAbstractDiagramGraphicsItem*> items)
{
    if (items.size() == 1){
        m_model->buildModel(items.first());
    } else {

    }
}

QDiagramGraphicsItemDelegate::QDiagramGraphicsItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
    QStringList colorNames = QColor::colorNames();
    for (int i = 0; i < colorNames.size(); ++i) {
        m_colorNameMap[QColor(colorNames[i]).name()] = colorNames[i];
    }
}

QWidget *QDiagramGraphicsItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    QPropertiesModelItem* item = static_cast<QPropertiesModelItem*>(index.internalPointer());
    if (item){
        QWidget* w = item->createEditor(parent, option, index, this);
        if (w){
            return w;
        }
    }
    return QStyledItemDelegate::createEditor(parent, option, index);
}

void QDiagramGraphicsItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QPropertiesModelItem* item = static_cast<QPropertiesModelItem*>(index.internalPointer());
    if (item){
        QStyleOptionViewItemV4 opt = option;
        initStyleOption(&opt, index);
        if (!item->paint(painter, opt, index, opt)){
            QStyledItemDelegate::paint(painter, option, index);
        }
    } else {
        QStyledItemDelegate::paint(painter, option, index);
    }
}

void QDiagramGraphicsItemDelegate::setEditorData(QWidget* editor, const QModelIndex & index) const
{
    QPropertiesModelItem* item = static_cast<QPropertiesModelItem*>(index.internalPointer());
    if (item){
        if (!item->setEditorData(editor, index)){
            QStyledItemDelegate::setEditorData(editor, index);
        }
    } else {
        QStyledItemDelegate::setEditorData(editor, index);
    }
}

void QDiagramGraphicsItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QPropertiesModelItem* item = static_cast<QPropertiesModelItem*>(index.internalPointer());
    if (item){
        if (!item->setModelData(editor, model, index)){
            QStyledItemDelegate::setModelData(editor, model, index);
        }
    } else {
        QStyledItemDelegate::setModelData(editor, model, index);
    }
}

QSize QDiagramGraphicsItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize s = QStyledItemDelegate::sizeHint(option, index);
    s.setHeight(s.height() + 2);
    return s;
}

void QDiagramGraphicsItemDelegate::commitAndClose()
{
    QWidget* w = qobject_cast<QWidget*>(sender());
    if (w){
        emit commitData(w);
		emit closeEditor(w);
    }
}

void QDiagramGraphicsItemDelegate::editingFinished()
{
    QWidget* w = qobject_cast<QWidget*>(sender());
    if (w){
        emit commitData(w);
    }
}

QDiagramColorComboBox::QDiagramColorComboBox(QWidget *parent) :
    QComboBox(parent)
{
    QStringList colorNames = QColor::colorNames();

    for (int i = 0; i < colorNames.size(); ++i) {
        QColor color(colorNames[i]);
        insertItem(i, colorNames[i]);
        setItemData(i, color, Qt::DecorationRole);
    }
}

QString QDiagramColorComboBox::colorName(const QColor &color)
{
    initColorMap();
    return sColorNameMap.key(color, QObject::tr("<invalid>"));
}

QStringList QDiagramColorComboBox::colorNames()
{
    initColorMap();
    return sColorNameMap.keys();
}

QColor QDiagramColorComboBox::currentColor() const
{
    if (currentIndex() == -1){
        return QColor();
    }
    return qvariant_cast<QColor>(itemData(currentIndex(), Qt::DecorationRole));
}

void QDiagramColorComboBox::initColorMap()
{
    if (sColorNameMap.isEmpty()){
        QStringList colorNames = QColor::colorNames();
        for (int i = 0; i < colorNames.size(); ++i) {
            sColorNameMap[colorNames.at(i)] = QColor(colorNames.at(i));
        }
    }
}

QDiagramEndOfLineStyleComboBox::QDiagramEndOfLineStyleComboBox(const QList<QDiagramEndOfLineStyle> & styles, QWidget *parent) :
    QComboBox(parent)
{
    Q_FOREACH(QDiagramEndOfLineStyle style, styles){
        addItem(style.icon(), style.caption(), style.id());
    }
}

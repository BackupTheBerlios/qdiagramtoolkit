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
#include "qdiagramconnectionpoint.h"
#include "qdiagrammetadata.h"
#include "qdiagramendoflinestyle.h"
#include "qdiagramlinestyle.h"
#include "qdiagramgraphicsitemshadow.h"
#include "qdiagramtextstyle.h"

QMap<QString,QColor> QDiagramColorComboBox::sColorNameMap;

QPropertiesModelItem::QPropertiesModelItem(QAbstractDiagramGraphicsItem* item)
{
	m_flag = -1;
    m_index = -1;
    m_item = item;
    m_parent = 0;
}

QPropertiesModelItem::QPropertiesModelItem(QAbstractDiagramGraphicsItem* item, int index, QPropertiesModelItem *parent)
{
	m_flag = -1;
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
	if (item){
		if (item->metaData()->property(index).flag().isValid()){
			for (int i = 0; i < item->metaData()->property(index).flag().keyCount(); i++){
				new QPropertiesModelItem(item->metaData()->property(index).name(), i, this);
			}
		} else {
			for (int i = 0; i < item->metaData()->property(index).propertyCount(); i++){
				new QPropertiesModelItem(item->metaData()->property(index).property(i).name(), item->metaData()->property(index).property(i).type(), this);
			}
		}
	}
}

QPropertiesModelItem::QPropertiesModelItem(const QString & name, int flag, QPropertiesModelItem* parent)
{
	m_flag = flag;
    m_index = -1;
    m_item = 0;
    m_name = name;
    m_parent = parent;
    if (m_parent != 0){
        m_parent->m_children.append(this);
    }
}

QPropertiesModelItem::QPropertiesModelItem(const QString &name, QPropertiesModelItem *parent)
{
	m_flag = -1;
    m_index = -1;
    m_item = 0;
    m_name = name;
    m_parent = parent;
    if (m_parent != 0){
        m_parent->m_children.append(this);
    }
}

QPropertiesModelItem::QPropertiesModelItem(const QString & name, QDiagramToolkit::PropertyType type, QPropertiesModelItem* parent)
{
	m_flag = -1;
	m_index = -1;
    m_item = 0;
    m_name = name;
    m_parent = parent;
    if (m_parent != 0){
        m_parent->m_children.append(this);
    }
	m_type = type;
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

QWidget* QPropertiesModelItem::createEditor(QWidget* parent, const QStyleOptionViewItem & option, const QModelIndex & index, const QStyledItemDelegate* receiver) const
{
	if (metaEnumeration().isValid()){
		QComboBox* cb = new QComboBox(parent);
		for (int i = 0; i < metaEnumeration().keyCount(); i++){
			cb->addItem(metaEnumeration().key(i), metaEnumeration().value(i));
		}
		QObject::connect(cb, SIGNAL(activated(int)), receiver, SLOT(commitAndClose()));
		return cb;
	} else if (metaFlag().isValid()){
		QCheckBox* cb =  new QCheckBox(parent);
		cb->setAutoFillBackground(true);
		QObject::connect(cb, SIGNAL(toggled(bool)), receiver, SLOT(commitAndClose()));
		return cb;
	} else if (type() == QDiagramToolkit::Angle){
        QDoubleSpinBox* sb = new QDoubleSpinBox(parent);
        sb->setDecimals(1);
        sb->setMaximum(0.00);
        sb->setMaximum(359.9);
        sb->setSuffix(QObject::tr(" deg"));
        return sb;
	} else if (type() == QDiagramToolkit::Bool){
		QCheckBox* cb =  new QCheckBox(parent);
		cb->setAutoFillBackground(true);
		QObject::connect(cb, SIGNAL(toggled(bool)), receiver, SLOT(commitAndClose()));
		return cb;
	} else if (type() == QDiagramToolkit::BrushStyle){
		QComboBox* cb = new QComboBox(parent);
		QObject::connect(cb, SIGNAL(activated(int)), receiver, SLOT(commitAndClose()));
		return cb;
    } else if (type() == QDiagramToolkit::Color){
		QDiagramColorEditor* ce = new QDiagramColorEditor(parent);
		QObject::connect(ce, SIGNAL(colorEdited()), receiver, SLOT(commitAndClose()));
		return ce;
    } else if (type() == QDiagramToolkit::Double){
		QDoubleSpinBox* sb = new QDoubleSpinBox(parent);
        sb->setMaximum(99999.99);
		QObject::connect(sb, SIGNAL(editingFinished()), receiver, SLOT(commitAndClose()));
		return sb;
	} else if (type() == QDiagramToolkit::EndOfLineStyle){
        if (name() == "style"){
            return new QDiagramEndOfLineStyleComboBox(graphicsItem()->diagram()->endOfLineStyles(), parent);
        } else if (name() == "width"){
            QDoubleSpinBox* spinBox = new QDoubleSpinBox(parent);
            spinBox->setDecimals(2);
            spinBox->setMaximum(0.00);
            spinBox->setMaximum(99.00);
            return spinBox;
        }
    } else if (type() == QDiagramToolkit::Enumeration){
		QComboBox* cb = new QComboBox(parent);
		QObject::connect(cb, SIGNAL(activated(int)), receiver, SLOT(commitAndClose()));
		return cb;
    } else if (type() == QDiagramToolkit::Font){
    } else if (type() == QDiagramToolkit::FontFamily){
		QFontComboBox* cb = new QFontComboBox(parent);
		QObject::connect(cb, SIGNAL(activated(int)), receiver, SLOT(commitAndClose()));
		return cb;
    } else if (type() == QDiagramToolkit::Int){
        QSpinBox* sb = new QSpinBox(parent);
		QObject::connect(sb, SIGNAL(editingFinished()), receiver, SLOT(commitAndClose()));
		return sb;
    } else if (type() == QDiagramToolkit::Pen){
		QDiagramColorComboBox* cb = new QDiagramColorComboBox(parent);
		QObject::connect(cb, SIGNAL(activated(int)), receiver, SLOT(commitAndClose()));
		return cb;
    } else if (type() == QDiagramToolkit::PenStyle){
		QComboBox* cb = new QComboBox(parent);
		QObject::connect(cb, SIGNAL(activated(int)), receiver, SLOT(commitAndClose()));
		return cb;
    } else if (type() == QDiagramToolkit::Percent){
        QSpinBox* spinBox = new QSpinBox(parent);
        spinBox->setMaximum(0);
        spinBox->setMaximum(100);
        spinBox->setSuffix(QObject::tr(" %"));
        return spinBox;
    } else if (type() == QDiagramToolkit::LineStyle){
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
    } else if (type() == QDiagramToolkit::Shadow){
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
    } else if (type() == QDiagramToolkit::String){
		QLineEdit* w = new QLineEdit(parent);
		QObject::connect(w, SIGNAL(editingFinished()), receiver, SLOT(commitAndClose()));
		QObject::connect(w, SIGNAL(textEdited(QString)), receiver, SLOT(editingFinished()));
		return w;
    } else if (type() == QDiagramToolkit::Text){
		QLineEdit* w = new QLineEdit(parent);
		QObject::connect(w, SIGNAL(editingFinished()), receiver, SLOT(commitAndClose()));
		QObject::connect(w, SIGNAL(textEdited(QString)), receiver, SLOT(editingFinished()));
		return w;
    } else if (type() == QDiagramToolkit::TextStyle){
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
			return name();
        } else if (index.column() == 1){
			if (metaEnumeration().isValid()){
				return metaEnumeration().key(metaEnumeration().indexOf(property().toInt()));
			} else if (metaFlag().isValid()/*name() == "alignment"*/){
				return metaFlag().matchingKeys(value().toInt()).join("|");
			} else if (type() == QDiagramToolkit::Angle){
				return value().toString();
			} else if (type() == QDiagramToolkit::Bool){
				return value().toBool()?QObject::tr("enabled"):QObject::tr("disabled");
			} else if (type() == QDiagramToolkit::Brush){
				return QDiagramProperty::toString(qdiagramproperty_cast<QBrush>(property()).style());
			} else if (type() == QDiagramToolkit::BrushStyle){
				return property().toString();
			} else if (type() == QDiagramToolkit::Color){
				QColor c = qdiagramproperty_cast<QColor>(property());
				return QDiagramColorComboBox::colorName(c);
			} else if (type() == QDiagramToolkit::ConnectionPoint){
				QDiagramConnectionPoint cp = qdiagramproperty_cast<QDiagramConnectionPoint>(property());
				return QString("%1@%2").arg(cp.id()).arg(cp.uuid());
			} else if (type() == QDiagramToolkit::Double){
				return value().toDouble();
			} else if (type() == QDiagramToolkit::Dynamic){
				return value().toString();
			} else if (type() == QDiagramToolkit::Enumeration){
				return graphicsItem()->metaData()->property(m_index).enumerator().key(value().toInt());
			} else if (type() == QDiagramToolkit::Font){				
				// TODO QFont f = QDiagramMetaData::toFont(value(true).toMap());
				QFont f = qdiagramproperty_cast<QFont>(property());
				return QString("%1").arg(f.family());
			} else if (type() == QDiagramToolkit::FontFamily){
				return value().toString();
			} else if (type() == QDiagramToolkit::Int){
				return value().toString();
			} else if (type() == QDiagramToolkit::LineStyle){
				QDiagramLineStyle s = qdiagramproperty_cast<QDiagramLineStyle>(property());
				return QString("%1 %2").arg(QDiagramColorComboBox::colorName(s.color())).arg(s.width());
			} else if (type() == QDiagramToolkit::Point){
				return QString("%1, %2")
					.arg(value(true).toMap().value("x").toInt())
					.arg(value(true).toMap().value("y").toInt());
			} else if (type() == QDiagramToolkit::PenStyle){
				if (property().toPenStyle() == Qt::SolidLine){
					return QObject::tr("solid");
				} else if (property().toPenStyle() == Qt::DashDotDotLine){
					return QObject::tr("dash dot dot");
				} else if (property().toPenStyle() == Qt::DashDotLine){
					return QObject::tr("dash dot");
				} else if (property().toPenStyle() == Qt::DashLine){
					return QObject::tr("dashed");
				}
				return QObject::tr("none");
			} else if (type() == QDiagramToolkit::Percent){
				return QString("%1%").arg(value().toInt());
			} else if (type() == QDiagramToolkit::Rect){
				return QString("(%1, %2) %3 x %4")
					.arg(value(true).toMap().value("x").toInt())
					.arg(value(true).toMap().value("y").toInt())
					.arg(value(true).toMap().value("width").toInt())
					.arg(value(true).toMap().value("height").toInt());
			} else if (type() == QDiagramToolkit::String){
				return value().toString();
			} else if (type() == QDiagramToolkit::Text){
				return value().toString();
			} else if (type() == QDiagramToolkit::UUID){
				//return graphicsItem()->uuid();
				return value().toString();
			}
		}
    } else if (role == Qt::DecorationRole){
        if (index.column() == 1){
			if (type() == QDiagramToolkit::Brush){
				return qdiagramproperty_cast<QBrush>(property()).color();
			} else if (type() == QDiagramToolkit::Color){
				return QColor(value().toString());
			} else if(type() == QDiagramToolkit::LineStyle){
				if (property().toPenStyle() == Qt::SolidLine){
					return QIcon(":/qdiagram/line.solid");
				} else if (property().toPenStyle() == Qt::DashDotDotLine){
					return QIcon(":/qdiagram/line.dash_dot_dot");
				} else if (property().toPenStyle() == Qt::DashDotLine){
					return QIcon(":/qdiagram/line.dash_dot");
				} else if (property().toPenStyle() == Qt::DashLine){
					return QIcon(":/qdiagram/line.dashed");
				}
			} else if(type() == QDiagramToolkit::PenStyle){
				if (property().toPenStyle() == Qt::SolidLine){
					return QIcon(":/qdiagram/line.solid");
				} else if (property().toPenStyle() == Qt::DashDotDotLine){
					return QIcon(":/qdiagram/line.dash_dot_dot");
				} else if (property().toPenStyle() == Qt::DashDotLine){
					return QIcon(":/qdiagram/line.dash_dot");
				} else if (property().toPenStyle() == Qt::DashLine){
					return QIcon(":/qdiagram/line.dashed");
				}
			}
            if (m_item == 0){
                if (type() == QDiagramToolkit::LineStyle){
                    QDiagramLineStyle style = qvariant_cast<QDiagramLineStyle>(value());
                    if (index.row() == 0){
                        return style.color();
                    } else if (index.row() == 2){
                        return style.icon();
                    }
				} else if (type() == QDiagramToolkit::Pen){
					QPen p = qdiagramproperty_cast<QPen>(graphicsItem()->property(m_parent->name()));
                    if (name() == "color"){
                        return p.color();
                    }
                } else if (type() == QDiagramToolkit::Shadow){
					QDiagramGraphicsItemShadow s = qvariant_cast<QDiagramGraphicsItemShadow>(value());
                    if (name() == "color"){
                        return s.color();
                    }
                } else if (type() == QDiagramToolkit::TextStyle){
                    QDiagramTextStyle style = qvariant_cast<QDiagramTextStyle>(value());
                    if (name() == "color"){
                        return style.color();
                    }
                }
            } else {
                if (type() == QDiagramToolkit::Color){
                    return qvariant_cast<QColor>(value());
                } else if (type() == QDiagramToolkit::EndOfLineStyle){
                        QDiagramEndOfLineStyle s = qvariant_cast<QDiagramEndOfLineStyle>(value());
                        return s.icon();
                } else if (type() == QDiagramToolkit::LineStyle){
//                    QDiagramLineStyle style = qvariant_cast<QDiagramLineStyle>(value());
//                    if (style.isValid()){
//                        return style.color();
//                    }
                }
            }
        }
    } else if (role == Qt::EditRole){
        if (index.column() == 1){
			if (metaFlag().isValid()){
				return value().toBool();
			} else if (type() == QDiagramToolkit::Bool){
				return value().toBool();
			} else if (type() == QDiagramToolkit::Color){
				return qvariant_cast<QColor>(value());
			} else if (type() == QDiagramToolkit::Dynamic){
				return value();
			} else if (type() == QDiagramToolkit::Int){
				return value().toInt();
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

QDiagramMetaEnum QPropertiesModelItem::metaEnumeration() const
{
	if (m_flag != -1){
		return QDiagramMetaEnum();
	}
	if (m_item == 0 && m_flag == -1){
		QDiagramMetaProperty p = m_parent->graphicsItem()->metaData()->property(m_parent->index());
		return p.property(p.indexOf(m_name)).enumerator();
	}
	return m_item->metaData()->property(m_index).enumerator();
}

QDiagramMetaFlag QPropertiesModelItem::metaFlag() const
{
	//if (m_flag == -1){
	//	return QDiagramMetaFlag();
	//}
	if (m_item == 0){
		QDiagramMetaProperty p = m_parent->graphicsItem()->metaData()->property(m_parent->index());
		return p.flag();
	}
	return m_item->metaData()->property(m_index).flag();
}

Qt::ItemFlags QPropertiesModelItem::flags() const
{
    Qt::ItemFlags f;
	f |= Qt::ItemIsSelectable;
	f |= Qt::ItemIsEnabled;
	if (m_item == 0){
		if (m_parent->graphicsItem()->metaData()->property(m_parent->index()).isReadOnly()){
			return f;
		}
	} else {
		if (m_item->metaData()->property(m_index).isReadOnly()){
			return f;
		}
	}
	if (m_flag != -1){
		f |= Qt::ItemIsEditable;
	} else if (type() == QDiagramToolkit::Angle){
		f |= Qt::ItemIsEditable;
	} else if (type() == QDiagramToolkit::Bool){
		f |= Qt::ItemIsEditable;
	} else if (type() == QDiagramToolkit::BrushStyle){
		f |= Qt::ItemIsEditable;
	} else if (type() == QDiagramToolkit::Color){
		f |= Qt::ItemIsEditable;
	} else if (type() == QDiagramToolkit::Double){
		f |= Qt::ItemIsEditable;
	} else if (type() == QDiagramToolkit::Dynamic){
		f |= Qt::ItemIsEditable;
	} else if (type() == QDiagramToolkit::Enumeration){
		f |= Qt::ItemIsEditable;
	} else if (type() == QDiagramToolkit::FontFamily){
		f |= Qt::ItemIsEditable;
	} else if (type() == QDiagramToolkit::Int){
		f |= Qt::ItemIsEditable;
	} else if (type() == QDiagramToolkit::LineStyle){
		f |= Qt::ItemIsEditable;
	} else if (type() == QDiagramToolkit::Orientation){
		f |= Qt::ItemIsEditable;
	} else if (type() == QDiagramToolkit::PenJoinStyle){
		f |= Qt::ItemIsEditable;
	} else if (type() == QDiagramToolkit::PenStyle){
		f |= Qt::ItemIsEditable;
	} else if (type() == QDiagramToolkit::Percent){
		f |= Qt::ItemIsEditable;
	} else if (type() == QDiagramToolkit::String){
		f |= Qt::ItemIsEditable;
	} else if (type() == QDiagramToolkit::Text){
		f |= Qt::ItemIsEditable;
	} else if (type() == QDiagramToolkit::TextStyle){
		f |= Qt::ItemIsEditable;
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

bool QPropertiesModelItem::isChild() const
{
	return m_index == -1;
}

QString QPropertiesModelItem::name() const
{
	if (m_flag != -1){
		return metaFlag().key(m_flag);
	}
    if (m_index == -1){
        return  m_name;
    }
    return graphicsItem()->metaData()->property(m_index).name();
}

bool QPropertiesModelItem::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index, const QStyleOptionViewItemV4 &opt4)
{
    if (index.column() == 1){
		if (metaFlag().isValid() && m_flag != -1){
	        QStyle* style = opt4.widget ? opt4.widget->style() : QApplication::style();

		    style->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter, opt4.widget);
			QStyleOptionButton so;
			so.rect = opt4.rect;
			so.state |= index.data(Qt::EditRole).toBool() ? QStyle::State_On : QStyle::State_Off;
			so.state |= QStyle::State_Enabled;
			style->drawControl(QStyle::CE_CheckBox, &so, painter);
  			return true;
		} else if (type() == QDiagramToolkit::Bool){
	        QStyle* style = opt4.widget ? opt4.widget->style() : QApplication::style();

		    style->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter, opt4.widget);
			QStyleOptionButton so;
			so.rect = opt4.rect;
			so.state |= index.data(Qt::EditRole).toBool() ? QStyle::State_On : QStyle::State_Off;
			so.state |= QStyle::State_Enabled;
			style->drawControl(QStyle::CE_CheckBox, &so, painter);
  			return true;
		}
        if (index.data(QPropertiesModel::MetaTypeRole).toInt() == QDiagramToolkit::Angle){
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
        } else if (index.data(QPropertiesModel::MetaTypeRole).toInt() == QDiagramToolkit::Percent){
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

QDiagramProperty QPropertiesModelItem::property() const
{
	if (m_item){
	    return m_item->property(m_item->metaData()->property(m_index).name());
	}
	QDiagramProperty p = m_parent->m_item->property(m_parent->m_item->metaData()->property(m_parent->m_index).name());
	return p.property(m_name);
	//return QDiagramProperty();
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
	if (metaEnumeration().isValid()){
		QComboBox* cb = qobject_cast<QComboBox*>(editor);
		cb->setCurrentIndex(cb->findData(property().toInt()));
		return true;
	} else if (metaFlag().isValid()){
		QCheckBox* cb = qobject_cast<QCheckBox*>(editor);
		cb->setChecked(!value().toBool());
		return true;
	} else if (type() == QDiagramToolkit::Angle){
        QDoubleSpinBox* spinBox = qobject_cast<QDoubleSpinBox*>(editor);
		spinBox->setValue(qdiagramproperty_cast<double>(property()));
        return true;
	} else if (type() == QDiagramToolkit::Bool){
		QCheckBox* cb = qobject_cast<QCheckBox*>(editor);
		cb->setChecked(!value().toBool());
		return true;
	} else if (type() == QDiagramToolkit::BrushStyle){
		QComboBox* cb = qobject_cast<QComboBox*>(editor);
		QDiagramPropertyEnumMap::const_iterator it;
		QDiagramPropertyEnumMap m = QDiagramProperty::enumValues(QDiagramToolkit::BrushStyle);
		for (it = m.begin(); it != m.end(); ++it){
			cb->addItem(it.value(), it.key());
		}
		cb->setCurrentIndex(cb->findData(property().toBrushStyle()));
		return true;
	} else if (type() == QDiagramToolkit::Color){
		QDiagramColorEditor* ce = qobject_cast<QDiagramColorEditor*>(editor);
		ce->setColor(qvariant_cast<QColor>(value()));
		return true;
	} else if (type() == QDiagramToolkit::Double){
        QDoubleSpinBox* spinBox = qobject_cast<QDoubleSpinBox*>(editor);
		spinBox->setValue(qdiagramproperty_cast<double>(property()));
        return true;
	} else if (type() == QDiagramToolkit::Enumeration){
        QComboBox* cb = qobject_cast<QComboBox*>(editor);
        for (int i = 0; i < m_item->metaData()->property(m_index).enumerator().keyCount(); i++){
            cb->addItem(m_item->metaData()->property(m_index).enumerator().key(i), m_item->metaData()->property(m_index).enumerator().value(i));
        }
        cb->setCurrentIndex(cb->findData(value().toInt()));
	} else if (type() == QDiagramToolkit::FontFamily){
		QFontComboBox* cb = qobject_cast<QFontComboBox*>(editor);
// TODO		cb->setCurrentFont(QDiagramMetaData::toFont(value(true).toMap()));
		cb->setCurrentFont(qdiagramproperty_cast<QFont>(property()));
		return true;
	} else if (type() == QDiagramToolkit::Int){
		QSpinBox* sb = qobject_cast<QSpinBox*>(editor);
		sb->setValue(value().toInt());
		return true;
    } else if (index.data(QPropertiesModel::MetaTypeRole).toInt() == QDiagramToolkit::Angle){
        QDoubleSpinBox* spinBox = qobject_cast<QDoubleSpinBox*>(editor);
        spinBox->setValue(value().toDouble());
        return true;
    } else if (type() == QDiagramToolkit::EndOfLineStyle){
        QDiagramEndOfLineStyle s = qvariant_cast<QDiagramEndOfLineStyle>(value());
        if (name() == "style"){
            QDiagramEndOfLineStyleComboBox* cb = qobject_cast<QDiagramEndOfLineStyleComboBox*>(editor);
            cb->setCurrentIndex(cb->findData(s.id()));
        } else if (name() == "width"){
            QDoubleSpinBox* sb = qobject_cast<QDoubleSpinBox*>(editor);
            sb->setValue(s.width());
        }
        return true;
	} else if (type() == QDiagramToolkit::Enumeration){
		QComboBox* cb = qobject_cast<QComboBox*>(editor);
		cb->setCurrentIndex(cb->findData(value()));
    } else if (type() == QDiagramToolkit::LineStyle){
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
	} else if (type() == QDiagramToolkit::PenStyle){
		QComboBox* cb = qobject_cast<QComboBox*>(editor);
		cb->addItem(QIcon(":/qdiagram/line.solid"), QObject::tr("solid"), Qt::SolidLine);
		cb->addItem(QIcon(":/qdiagram/line.dash_dot_dot"), QObject::tr("dash dot dot"), Qt::DashDotDotLine);
		cb->addItem(QIcon(":/qdiagram/line.dash_dot"), QObject::tr("dash dot"), Qt::DashDotLine);
		cb->addItem(QIcon(":/qdiagram/line.dashed"), QObject::tr("dashed"), Qt::DashLine);
		cb->addItem(QObject::tr("none"), Qt::NoPen);
		cb->setCurrentIndex(cb->findData(property().toPenStyle()));
		return true;
    } else if (type() == QDiagramToolkit::Percent){
        QSpinBox* spinBox = qobject_cast<QSpinBox*>(editor);
        spinBox->setValue(value().toDouble());
        return true;
    } else if (type() == QDiagramToolkit::Shadow){
		QDiagramGraphicsItemShadow s = qvariant_cast<QDiagramGraphicsItemShadow>(value());
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
    } else if (type() == QDiagramToolkit::String){
		QLineEdit* le = qobject_cast<QLineEdit*>(editor);
		le->setText(property().toString());
		return true;
    } else if (type() == QDiagramToolkit::TextStyle){
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

bool QPropertiesModelItem::setModelData(QWidget* editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	if (metaEnumeration().isValid()){
		QComboBox* cb = qobject_cast<QComboBox*>(editor);
		model->setData(index, cb->itemData(cb->currentIndex()));
		return true;
	} else if (metaFlag().isValid()){
		QCheckBox* cb = qobject_cast<QCheckBox*>(editor);
        model->setData(index, cb->isChecked());
        return true;
	} else if (type() == QDiagramToolkit::Angle){
        QDoubleSpinBox* spinBox = qobject_cast<QDoubleSpinBox*>(editor);
        model->setData(index, spinBox->value());
        return true;
	} else if (type() == QDiagramToolkit::Bool){
		QCheckBox* cb = qobject_cast<QCheckBox*>(editor);
        model->setData(index, cb->isChecked());
        return true;
	} else if (type() == QDiagramToolkit::BrushStyle){
		QComboBox* cb = qobject_cast<QComboBox*>(editor);
		model->setData(index, static_cast<Qt::BrushStyle>(cb->itemData(cb->currentIndex()).toInt()));
		return true;
    } else if (type() == QDiagramToolkit::Color){
		QDiagramColorEditor* ce = qobject_cast<QDiagramColorEditor*>(editor);
        model->setData(index, ce->color());
        return true;
	} else if (index.data(QPropertiesModel::MetaTypeRole).toInt() == QDiagramToolkit::Double){
        QDoubleSpinBox* spinBox = qobject_cast<QDoubleSpinBox*>(editor);
        model->setData(index, spinBox->value());
        return true;
    } else if (type() == QDiagramToolkit::EndOfLineStyle){
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
    } else if (type() == QDiagramToolkit::Enumeration){
        QComboBox* cb = qobject_cast<QComboBox*>(editor);
        model->setData(index, cb->itemData(cb->currentIndex()));
        return true;
    } else if (type() == QDiagramToolkit::FontFamily){
		QFontComboBox* cb = qobject_cast<QFontComboBox*>(editor);
		model->setData(index, cb->currentFont().family());
		return true;
	} else if (type() == QDiagramToolkit::Int){
		QSpinBox* sb = qobject_cast<QSpinBox*>(editor);
        model->setData(index, sb->value());
        return true;
    } else if (type() == QDiagramToolkit::LineStyle){
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
	} else if (type() == QDiagramToolkit::PenStyle){
		QComboBox* cb = qobject_cast<QComboBox*>(editor);
		model->setData(index, static_cast<Qt::PenStyle>(cb->itemData(cb->currentIndex()).toInt()));
		return true;
    } else if (type() == QDiagramToolkit::Percent){
        QSpinBox* spinBox = qobject_cast<QSpinBox*>(editor);
        model->setData(index, spinBox->value());
        return true;
    } else if (index.data(QPropertiesModel::MetaTypeRole).toInt() == QDiagramToolkit::Shadow){
        QDiagramGraphicsItemShadow s = qvariant_cast<QDiagramGraphicsItemShadow>(value());
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
	} else if (index.data(QPropertiesModel::MetaTypeRole).toInt() == QDiagramToolkit::String){
		QLineEdit* w = qobject_cast<QLineEdit*>(editor);
		model->setData(index, w->text());
        return true;
	} else if (index.data(QPropertiesModel::MetaTypeRole).toInt() == QDiagramToolkit::Text){
		QLineEdit* w = qobject_cast<QLineEdit*>(editor);
		model->setData(index, w->text());
        return true;
    } else if (index.data(QPropertiesModel::MetaTypeRole).toInt() == QDiagramToolkit::TextStyle){
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

void QPropertiesModelItem::setValue(const QVariant & value)
{
    if (m_item == 0){
		if (m_flag != -1){
			int v = this->value(true).toInt();
			if (value.toBool()){
				v |= metaFlag().value(m_flag);
			} else {
				v &= ~metaFlag().value(m_flag);
			}
			graphicsItem()->setProperty(graphicsItem()->metaData()->property(m_parent->m_index).name(), v);
		} else {
			QVariantMap m = graphicsItem()->property(graphicsItem()->metaData()->property(m_parent->m_index).name()).toMap();
			if (m.isEmpty()){
				graphicsItem()->setProperty(graphicsItem()->metaData()->property(m_parent->m_index).name(), value);
			} else {
				property().setValue(value);
			}
		}
    } else {
        m_item->setProperty(m_item->metaData()->property(m_index).name(), value);
    }
}

QDiagramToolkit::PropertyType QPropertiesModelItem::type() const
{
    if (m_item == 0){
		return m_type;
        return m_parent->m_item->metaData()->property(m_parent->m_index).type();
    } else {
        return m_item->metaData()->property(m_index).type();
    }
}

QVariant QPropertiesModelItem::value(bool parent) const
{
	if (m_flag != -1){
		int v = graphicsItem()->property(graphicsItem()->metaData()->property(m_parent->m_index).name()).toInt();
		if (parent){
			return v;
		}
		return v & metaFlag().value(m_flag);
	}
	if (m_item == 0){
		if (!parent){
			QVariantMap m = graphicsItem()->property(graphicsItem()->metaData()->property(m_parent->m_index).name()).toMap();
			return m.value(m_name);
		} else {
			return graphicsItem()->property(m_parent->name()).value();
		}
    }    
    return m_item->property(m_item->metaData()->property(m_index).name()).value();
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
    Qt::ItemFlags f;
    if (index.isValid()){
        f |= Qt::ItemIsSelectable;
        f |= Qt::ItemIsEnabled;
        if (index.column() == 1){
            QPropertiesModelItem* i = static_cast<QPropertiesModelItem*>(index.internalPointer());
			if (i){
				return i->flags();
			}
        }
    }
    return f;
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
    QPropertiesModelItem* i;
    if (parent.column() > 0){
        return 0;
    }

    if (!parent.isValid()){
        i = r;
    } else {
        i = static_cast<QPropertiesModelItem*>(parent.internalPointer());
    }
    return i->childCount();
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

void QDiagramGraphicsItemPropertiesView::propertyViewClicked(const QModelIndex & index)
{
	if (index.column() == 1){
		ui->propertyView->edit(index);
	}
}

void QDiagramGraphicsItemPropertiesView::showProperties(QList<QAbstractDiagramGraphicsItem*> items)
{
    if (items.size() == 1){
		ui->typeLineEdit->setText(items.first()->metaData()->itemType());
		ui->classLineEdit->setText(items.first()->metaData()->itemClass());
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
	if (!color.isValid()){
		return QObject::tr("<invalid>");
	}
    initColorMap();
	QMap<QString,QColor>::Iterator it;
	QList<QString> l = sColorNameMap.keys(color);
	if (l.size() > 0){
		return l.at(0);
	}
    return QObject::tr("<custom>");
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

QDiagramColorEditor::QDiagramColorEditor(QWidget* parent) :
	QWidget(parent)
{
	QHBoxLayout* l = new QHBoxLayout();
	m_comboBox = new QComboBox(this);
    QStringList colorNames = QColor::colorNames();

    for (int i = 0; i < colorNames.size(); ++i) {
        QColor color(colorNames[i]);
        m_comboBox->insertItem(i, colorNames[i]);
        m_comboBox->setItemData(i, color, Qt::DecorationRole);
    }
	int i = m_comboBox->count();
	m_comboBox->insertItem(i, tr("<custom>"));
	connect(m_comboBox, SIGNAL(activated(int)), this, SLOT(comboBoxActivated(int)));

	m_toolButton = new QPushButton(this);
	m_toolButton->setIcon(QIcon(":/qdiagram/color.select"));
	m_toolButton->setFlat(true);
	connect(m_toolButton, SIGNAL(clicked()), this, SLOT(toolButtonClicked()));
	l->addWidget(m_comboBox);
	l->addWidget(m_toolButton);
	l->setContentsMargins(0, 0, 0, 0);
	l->setSpacing(0);
	setLayout(l);

	setFocusPolicy(Qt::StrongFocus);
}

QDiagramColorEditor::~QDiagramColorEditor()
{
}

QColor QDiagramColorEditor::color() const
{
	return m_color;
//	return qvariant_cast<QColor>(m_comboBox->itemData(comboBox->currentIndex(), Qt::DecorationRole));
}

void QDiagramColorEditor::comboBoxActivated(int index)
{
	m_color = qvariant_cast<QColor>(m_comboBox->itemData(index, Qt::DecorationRole));
	emit colorEdited();
}

void QDiagramColorEditor::setColor(const QColor & color)
{
	m_color = color;
	int i = m_comboBox->findData(color, int(Qt::DecorationRole));
	if (i == -1){
		i = m_comboBox->findText(tr("<custom>"));
	}
	m_comboBox->setCurrentIndex(i);
}

void QDiagramColorEditor::toolButtonClicked()
{
	QColor c = qvariant_cast<QColor>(m_comboBox->itemData(m_comboBox->currentIndex(), Qt::DecorationRole));
	QColor newColor = QColorDialog::getColor(c.isValid()?c:m_color, this);
	if (newColor.isValid()){
		m_color = newColor;
		setColor(newColor);
	}
	emit colorEdited();
}

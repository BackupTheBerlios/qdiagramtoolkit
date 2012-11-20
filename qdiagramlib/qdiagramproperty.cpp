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
#include "qdiagramproperty.h"
#include "qdiagramproperty_p.h"

#include <qabstractdiagram.h>
#include <qabstractdiagramgraphicsitem.h>

sdDiagramProperty::sdDiagramProperty()
{
	diagram = 0;
	index = -1;
	uuid = "";
	parent = -1;
	type = QDiagramToolkit::PropertyTypeInvalid;
}

sdDiagramProperty::sdDiagramProperty(QAbstractDiagram* d, int idx)
{
	diagram = d;
	index = idx;
	uuid = "";
	parent = -1;
	type = QDiagramToolkit::PropertyTypeInvalid;
}

sdDiagramProperty::sdDiagramProperty(const QAbstractDiagramGraphicsItem* i, int idx)
{
	diagram = i->diagram();
	index = idx;
	uuid = i->uuid();
	parent = -1;
	type = QDiagramToolkit::PropertyTypeInvalid;
}

sdDiagramProperty::sdDiagramProperty(const QAbstractDiagramGraphicsItem* i, int p, int c)
{
	diagram = i->diagram();
	index = c;
	uuid = i->uuid();
	parent = p;
	type = QDiagramToolkit::PropertyTypeInvalid;
}

sdDiagramProperty::sdDiagramProperty( const sdDiagramProperty & other ) :
QSharedData(other)
{
	diagram = other.diagram;
	index = other.index;
	uuid = other.uuid;
	parent = other.parent;
	value = other.value;
	type = other.type;
}

sdDiagramProperty::~sdDiagramProperty()
{
}

QDiagramProperty::QDiagramProperty()
{
	d = new sdDiagramProperty();
}

QDiagramProperty::QDiagramProperty(QDiagramToolkit::PropertyType type, const QVariant & value)
{
	d = new sdDiagramProperty();
	d->type = type;
	d->value = QDiagramProperty::fromVariant(type, value);
}

QDiagramProperty::QDiagramProperty(const QAbstractDiagramGraphicsItem* item, int index)
{
	d = new sdDiagramProperty(item, index);
}

QDiagramProperty::QDiagramProperty(QAbstractDiagramGraphicsItem* i, int p, int c)
{
	d = new sdDiagramProperty(i, p, c);
}

QDiagramProperty::QDiagramProperty(const QDiagramProperty & other) :
	d(other.d)
{
}

QDiagramProperty::QDiagramProperty(sdDiagramProperty* other) :
	d(other)
{
}

QDiagramProperty::~QDiagramProperty()
{
}

bool QDiagramProperty::cast_helper(const QDiagramProperty & p, QDiagramToolkit::PropertyType t, void* ptr)
{
	return cast_helper(p.value(), t, ptr);
}

bool QDiagramProperty::cast_helper(const QVariant & v, QDiagramToolkit::PropertyType t, void* ptr)
{
	if (ptr == 0){
		return  false;
	}
	if (t == QDiagramToolkit::PropertyTypeInvalid){
		return false;
	}
	if (t == QDiagramToolkit::Bool){
		if (!v.canConvert(QVariant::Bool)){
			return false;
		}
		bool* b = reinterpret_cast<bool*>(ptr);
		*b = v.toBool();
		return true;
	} else if (t == QDiagramToolkit::Brush){
		QBrush* b = reinterpret_cast<QBrush*>(ptr);
		QColor c = QColor(v.toMap().value("color").toString());
		c.setAlpha(v.toMap().value("transparent", 100).toInt() * 255 / 100);
		b->setColor(c);
		b->setStyle(toBrushStyle(v.toMap().value("style").toString()));
		if (b->style() == Qt::LinearGradientPattern){
			if (b->gradient()){
				QGradientStops stops = b->gradient()->stops();
			}
		}
		return true;
	} else if (t == QDiagramToolkit::Color){
		if (!v.canConvert(QVariant::String)){
			return false;
		}
		QColor* c = reinterpret_cast<QColor*>(ptr);
		c->setNamedColor(v.toString());
		return true;
	} else if (t == QDiagramToolkit::ConnectionPoint){
		QDiagramConnectionPoint* c = reinterpret_cast<QDiagramConnectionPoint*>(ptr);
		c->setUUID(v.toMap().value("uuid").toString());
		c->setId(v.toMap().value("id").toString());
		return true;
	} else if (t == QDiagramToolkit::Double){
		if (!v.canConvert(QVariant::Bool)){
			return false;
		}
		double* d = reinterpret_cast<double*>(ptr);
		*d = v.toDouble();
		return true;
	} else if (t == QDiagramToolkit::Font){
		if (!v.canConvert(QVariant::Map)){
			return false;
		}
		QFont *f = reinterpret_cast<QFont*>(ptr);
		f->setFamily(v.toMap().value("family").toString());
		f->setStyleName(v.toMap().value("style").toString());
		f->setPointSizeF(v.toMap().value("size").toDouble());
		f->setBold(v.toMap().value("bold").toBool());
		f->setItalic(v.toMap().value("italic").toBool());
		f->setUnderline(v.toMap().value("underline").toBool());
		f->setStrikeOut(v.toMap().value("strikeout").toBool());
		return true;
	} else if (t == QDiagramToolkit::LineStyle){
		QDiagramLineStyle* s = reinterpret_cast<QDiagramLineStyle*>(ptr);
		QMapIterator<QString,QVariant> it(v.toMap());
		while(it.hasNext()){
			it.next();
			s->setProperty(it.key(), it.value());
		}
		return s;
	} else if (t == QDiagramToolkit::Int || t == QDiagramToolkit::Percent){
		if (!v.canConvert(QVariant::Bool)){
			return false;
		}
		int* i = reinterpret_cast<int*>(ptr);
		*i = v.toInt();
		return true;
	} else if (t == QDiagramToolkit::Pen){
		if (!v.canConvert(QVariant::Map)){
			return false;
		}
		QPen* p = reinterpret_cast<QPen*>(ptr);
		p->setStyle(toPenStyle(v.toMap().value("style", "solid").toString()));
		p->setColor(QColor(v.toMap().value("color", "#000000").toString()));
		p->setWidthF(v.toMap().value("width", 1.0).toDouble());
		p->setJoinStyle(static_cast<Qt::PenJoinStyle>(v.toMap().value("joinStyle", Qt::BevelJoin).toInt()));
		return true;
	} else if (t == QDiagramToolkit::Point){
		if (!v.canConvert(QVariant::Map)){
			return false;
		}
		QPointF* p = reinterpret_cast<QPointF*>(ptr);
		p->setX(v.toMap().value("x").toDouble());
		p->setY(v.toMap().value("y").toDouble());
		return true;
	} else if (t == QDiagramToolkit::Rect){
		if (!v.canConvert(QVariant::Map)){
			return false;
		}
		QRectF* r = reinterpret_cast<QRectF*>(ptr);
		r->setX(v.toMap().value("x").toDouble());
		r->setY(v.toMap().value("y").toDouble());
		r->setWidth(v.toMap().value("width").toDouble());
		r->setHeight(v.toMap().value("height").toDouble());
		return true;
	} else if (t == QDiagramToolkit::Shadow){
		QDiagramGraphicsItemShadow* s = reinterpret_cast<QDiagramGraphicsItemShadow*>(ptr);
		QMapIterator<QString,QVariant> it(v.toMap());
		while(it.hasNext()){
			it.next();
			s->setProperty(it.key(), it.value());
		}
		return s;
	} else if (t == QDiagramToolkit::String){
		if (!v.canConvert(QVariant::String)){
			return false;
		}
		QString* s = reinterpret_cast<QString*>(ptr);
		*s = v.toString();
		return true;
	}
	return false;
}

QVariant QDiagramProperty::defaultValue(QDiagramToolkit::PropertyType type)
{
	if (type == QDiagramToolkit::Brush){
		return toMap(QBrush(Qt::lightGray, Qt::SolidPattern));
	} else if (type == QDiagramToolkit::Font){
		return toMap(QFont());
	} else if (type == QDiagramToolkit::Pen){
		return toMap(QPen());
	} else if (type == QDiagramToolkit::LineStyle){
		QDiagramLineStyle style;
		QVariantMap m;
		for (int i = 0; i < style.propertyCount(); i++){
			m[style.key(i)] = style.value(i);
		}
		return m;
	} else if (type == QDiagramToolkit::Shadow){
		return toMap(qVariantFromValue(QDiagramGraphicsItemShadow()));
	} else if (type == QDiagramToolkit::TextStyle){
//		return toMap(QDiagramTextStyle());
	}
	return QVariant();
}

QDiagramPropertyEnumMap QDiagramProperty::enumValues(QDiagramToolkit::PropertyType type)
{
	QDiagramPropertyEnumMap m;
	if (type == QDiagramToolkit::BrushStyle){
		m[Qt::NoBrush] = "none";
		m[Qt::SolidPattern] = "solid";
		m[Qt::Dense1Pattern] = "dense1";
		m[Qt::Dense2Pattern] = "dense2";
		m[Qt::Dense3Pattern] = "dense3";
		m[Qt::Dense4Pattern] = "dense4";
		m[Qt::Dense5Pattern] = "dense5";
		m[Qt::Dense6Pattern] = "dense6";
		m[Qt::Dense7Pattern] = "dense7";
		m[Qt::HorPattern] = "horizontal";
		m[Qt::VerPattern] = "vertical";
		m[Qt::CrossPattern] = "cross";
		m[Qt::BDiagPattern] = "bdiag";
		m[Qt::FDiagPattern] = "fdiag";
		m[Qt::DiagCrossPattern] = "diagcross";
		m[Qt::SolidPattern] = "solid";
		m[Qt::ConicalGradientPattern] = "conicalGradient";
		m[Qt::LinearGradientPattern] = "linearGradient";
		m[Qt::RadialGradientPattern] = "radialGradient";
	}
	return m;
}

QVariant QDiagramProperty::fromVariant(QDiagramToolkit::PropertyType type, const QVariant & value)
{
	if (type == QDiagramToolkit::Bool){
		return value.toBool();
	} else if (type == QDiagramToolkit::Brush){
		if (value.canConvert<QBrush>()){
			return toMap(qvariant_cast<QBrush>(value));
		}
	} else if (type == QDiagramToolkit::BrushStyle){
		if (value.type() == QVariant::Int){
			QDiagramPropertyEnumMap m = QDiagramProperty::enumValues(QDiagramToolkit::BrushStyle);
			return m.value(value.toInt());
		}
	} else if (type == QDiagramToolkit::Color){
		if (value.canConvert(QVariant::String)){
			return value.toString();
		} else if (value.canConvert(QVariant::Color)){
			return qvariant_cast<QColor>(value).name();
		}
	} else if (type == QDiagramToolkit::ConnectionPoint){
		if (value.canConvert<QDiagramConnectionPoint>()){
			return toMap(value);
		}
	} else if (type == QDiagramToolkit::Int){
		return value.toInt();
	} else if (type == QDiagramToolkit::LineStyle){
		if (value.canConvert<QDiagramLineStyle>()){
			return toMap(value);
		}
	} else if (type == QDiagramToolkit::Pen){
		if (value.canConvert(QVariant::Pen)){
			return toMap(qvariant_cast<QPen>(value));
		} else if (value.canConvert(QVariant::Map)){
			return value;
		}
	} else if (type == QDiagramToolkit::PenStyle){
		if (value.type() == QVariant::Int){
			return toString(static_cast<Qt::PenStyle>(value.toInt()));
		}
	} else if (type == QDiagramToolkit::Rect){
		if (value.canConvert(QVariant::RectF)){
			return toMap(value.toRectF());
		} else if (value.canConvert(QVariant::Map)){
			return value;
		}
	} else if (type == QDiagramToolkit::Shadow){
		if (value.canConvert<QDiagramGraphicsItemShadow>()){
			return toMap(value);
		}
	} else if (type == QDiagramToolkit::Text){
		return value.toString();
	} else if (type == QDiagramToolkit::TextStyle){
		if (value.canConvert<QDiagramTextStyle>()){
			return toMap(value);
		}
	} else if (value.canConvert(QVariant::Map)){
		return value.toMap();
	}
	return value;
}

bool QDiagramProperty::isValid() const
{
	if (d->diagram && d->index != -1){
		return true;
	}
	return false;
}

QAbstractDiagramGraphicsItem* QDiagramProperty::item() const
{
	if (d->diagram){
		return d->diagram->findItemByUuid(d->uuid);
	}
	return 0;
}

QDiagramMetaData* QDiagramProperty::metaData() const
{
	QAbstractDiagramGraphicsItem* i = item();
	if (i){
		return i->metaData();
	}
	return 0;
}

QDiagramProperty QDiagramProperty::property(const QString & name) const
{
	QAbstractDiagramGraphicsItem* i = item();
	if (i && d->index != -1){
		QDiagramMetaData* m = i->metaData();
		QDiagramMetaProperty p = m->property(d->index);
		int idx = p.indexOf(name);
		if (idx != -1){
			QVariant v = i->propertyValue(d->index);
			return QDiagramProperty(i, d->index, idx);
		}
	}
	return QDiagramProperty();
}

QDiagramPropertyBag QDiagramProperty::propertyBag(QDiagramToolkit::PropertyType type)
{
	QDiagramPropertyBag b;
	if (type == QDiagramToolkit::Brush){
		b["color"] = QDiagramToolkit::Color;
		b["style"] = QDiagramToolkit::BrushStyle;
		b["patternColor"] = QDiagramToolkit::Color;
		b["transparent"] = QDiagramToolkit::Percent;
	} else if (type == QDiagramToolkit::ConnectionPoint){
		b["uuid"] = QDiagramToolkit::UUID;
		b["id"] = QDiagramToolkit::String;
	} else if (type == QDiagramToolkit::Font){
		b["family"] = QDiagramToolkit::FontFamily;
		b["size"] = QDiagramToolkit::Double;
		b["bold"] = QDiagramToolkit::Bool;
		b["italic"] = QDiagramToolkit::Bool;
		b["underline"] = QDiagramToolkit::Bool;
		b["strikeout"] = QDiagramToolkit::Bool;
	} else if (type == QDiagramToolkit::LineStyle){
		b["color"] = QDiagramToolkit::Color;
		b["width"] = QDiagramToolkit::Double;
		b["style"] = QDiagramToolkit::PenStyle;
	} else if (type == QDiagramToolkit::Pen){
		b["color"] = QDiagramToolkit::Color;
		b["joinStyle"] = QDiagramToolkit::PenJoinStyle;
		b["style"] = QDiagramToolkit::PenStyle;
		b["width"] = QDiagramToolkit::Double;
	} else if (type == QDiagramToolkit::Point){
		b["x"] = QDiagramToolkit::Double;
		b["y"] = QDiagramToolkit::Double;
	} else if (type == QDiagramToolkit::Rect){
		b["x"] = QDiagramToolkit::Double;
		b["y"] = QDiagramToolkit::Double;
		b["width"] = QDiagramToolkit::Double;
		b["height"] = QDiagramToolkit::Double;
	} else if (type == QDiagramToolkit::Shadow){
		b["visible"] = QDiagramToolkit::Bool;
		b["color"] = QDiagramToolkit::Color;
		b["blurRadius"] = QDiagramToolkit::Int;
		b["offsetX"] = QDiagramToolkit::Int;
		b["offsetY"] = QDiagramToolkit::Int;
	}
	return b;
}

void QDiagramProperty::setValue(const QVariant & value)
{
	QAbstractDiagramGraphicsItem* i = item();
	if (i){
		if (d->parent == -1){
			i->setProperty(i->metaData()->property(d->index).name(), value);
		} else {
			QDiagramMetaData* m = i->metaData();
			QDiagramMetaProperty p = m->property(d->parent);
			QVariantMap pv = i->propertyValue(d->parent).toMap();
			pv[p.property(d->index).name()] = QDiagramProperty::fromVariant(p.property(d->index).type(), value);
			i->setProperty(p.name(), pv);
		}
	}
}

bool QDiagramProperty::toBool() const
{
	bool v = false;
	QAbstractDiagramGraphicsItem* i = item();
	if (i && d->index != -1){
		QDiagramProperty::cast_helper(value(), QDiagramToolkit::Bool, &v);
	}
	return v;
}

QBrush QDiagramProperty::toBrush() const
{
	QBrush v;
	QAbstractDiagramGraphicsItem* i = item();
	if (i && d->index != -1){
		QDiagramProperty::cast_helper(value(), QDiagramToolkit::Brush, &v);
	}
	return v;
}

Qt::BrushStyle QDiagramProperty::toBrushStyle() const
{
	QDiagramPropertyEnumMap m = QDiagramProperty::enumValues(QDiagramToolkit::BrushStyle);
	return static_cast<Qt::BrushStyle>(m.key(value().toString(), Qt::SolidPattern));
}

QColor QDiagramProperty::toColor() const
{
	QColor v;
	QAbstractDiagramGraphicsItem* i = item();
	if (i && d->index != -1){
		QDiagramProperty::cast_helper(value(), QDiagramToolkit::Color, &v);
	}
	return v;
}

QDiagramConnectionPoint QDiagramProperty::toConnectionPoint() const
{
	QDiagramConnectionPoint v;
	QAbstractDiagramGraphicsItem* i = item();
	QDiagramProperty::cast_helper(value(), QDiagramToolkit::ConnectionPoint, &v);
	return v;
}

double QDiagramProperty::toDouble() const
{
	double v = 0;
	QAbstractDiagramGraphicsItem* i = item();
	if (i && d->index != -1){
		QDiagramProperty::cast_helper(value(), QDiagramToolkit::Double, &v);
	}
	return v;
}

QFont QDiagramProperty::toFont() const
{
	QFont v;
	QAbstractDiagramGraphicsItem* i = item();
	if (i && d->index != -1){
		QDiagramProperty::cast_helper(value(), QDiagramToolkit::Font, &v);
	}
	return v;
}

int QDiagramProperty::toInt() const
{
	int v = 0;
	QAbstractDiagramGraphicsItem* i = item();
	if (i && d->index != -1){
		QDiagramProperty::cast_helper(value(), QDiagramToolkit::Int, &v);
	}
	return v;
}

QDiagramLineStyle QDiagramProperty::toLineStyle() const
{
	QDiagramLineStyle v;
	QAbstractDiagramGraphicsItem* i = item();
	if (i && d->index != -1){
		QDiagramProperty::cast_helper(value(), QDiagramToolkit::LineStyle, &v);
	}
	return v;
}

QVariantMap QDiagramProperty::toMap() const
{
	QAbstractDiagramGraphicsItem* i = item();
	if (i && d->index != -1){
		return value().toMap();
	}
	return QVariantMap();
}

QVariantMap QDiagramProperty::toMap(const QVariant & v)
{
	QVariantMap m;
	if (v.type() == QVariant::Brush){
		QBrush b = qvariant_cast<QBrush>(v);
		m["color"] = b.color().name();
		m["style"] = toString(b.style());
		m["transparent"] = b.color().alpha() * 100 / 255;
		if (b.gradient() == 0){
			m["patternColor"] = b.color().name();
		} else {
			if (b.gradient()->type() == QGradient::LinearGradient){
				m["patternColor"] = b.gradient()->stops().at(1).second.name();
			}
		}
	} else if (v.canConvert<QDiagramConnectionPoint>()){
		QDiagramConnectionPoint c = qvariant_cast<QDiagramConnectionPoint>(v);
		m["uuid"] = c.uuid();
		m["id"] = c.id();
	} else if (v.type() == QVariant::Font){
		QFont f = qvariant_cast<QFont>(v);
		m["family"] = f.family();
		m["style"] = f.styleName();
		m["size"] = f.pointSizeF();
		m["bold"] = f.bold();
		m["italic"] = f.italic();
		m["underline"] = f.underline();
		m["strikeout"] = f.strikeOut();
	} else if (v.canConvert<QDiagramLineStyle>()){
		QDiagramLineStyle s = qvariant_cast<QDiagramLineStyle>(v);
		for (int i = 0; i < s.propertyCount(); i++){
			m[s.key(i)] = s.value(i);
		}
	} else if (v.type() == QVariant::PointF){
		m["x"] = v.toPointF().x();
		m["y"] = v.toPointF().y();
	} else if (v.type() == QVariant::Pen){
		QPen p = qvariant_cast<QPen>(v);
		m["color"] = p.color().name();
		m["width"] = p.widthF();
		m["style"] = QDiagramProperty::toString(p.style());
		m["joinStyle"] = QDiagramMetaEnum::toString(QDiagramToolkit::PenJoinStyle, p.joinStyle());
	} else if (v.type() == QVariant::RectF){
		m["x"] = v.toRectF().x();
		m["y"] = v.toRectF().y();
		m["height"] = v.toRectF().height();
		m["width"] = v.toRectF().width();
	} else if (v.canConvert<QDiagramGraphicsItemShadow>()){
		QDiagramGraphicsItemShadow s = qvariant_cast<QDiagramGraphicsItemShadow>(v);
		for (int i = 0; i < s.propertyCount(); i++){
			m[s.key(i)] = s.value(i);
		}
	}
	return m;
}

QPen QDiagramProperty::toPen() const
{
	QPen v;
	QAbstractDiagramGraphicsItem* i = item();
	if (i && d->index != -1){
		QDiagramProperty::cast_helper(value(), QDiagramToolkit::Pen, &v);
	}
	return v;
}

Qt::PenStyle QDiagramProperty::toPenStyle(const QString & text)
{
	QString t = text.toLower();
	if (t == "dash"){
		return Qt::DashLine;
	} else if (t == "dashdot"){
		return Qt::DashDotLine;
	} else if (t == "dashdotdot"){
		return Qt::DashDotDotLine;
	} else if (t == "solid"){
		return Qt::SolidLine;
	}
	return Qt::NoPen;
}

Qt::PenStyle QDiagramProperty::toPenStyle() const
{
	if (type() == QDiagramToolkit::LineStyle){
		return toLineStyle().penStyle();
	}
	return QDiagramProperty::toPenStyle(toString());
}

QPointF QDiagramProperty::toPointF() const
{
	QPointF v;
	QAbstractDiagramGraphicsItem* i = item();
	if (i && d->index != -1){
		QDiagramProperty::cast_helper(value(), QDiagramToolkit::Point, &v);
	}
	return v;
}

QRectF QDiagramProperty::toRectF() const
{
	QRectF v;
	QAbstractDiagramGraphicsItem* i = item();
	if (i && d->index != -1){
		QDiagramProperty::cast_helper(value(), QDiagramToolkit::Rect, &v);
	}
	return v;
}

QDiagramGraphicsItemShadow QDiagramProperty::toShadow() const
{
	QDiagramGraphicsItemShadow v;
	QAbstractDiagramGraphicsItem* i = item();
	if (i && d->index != -1){
		QDiagramProperty::cast_helper(value(), QDiagramToolkit::Shadow, &v);
	}
	return v;
}

QSizeF QDiagramProperty::toSizeF() const
{
	QSizeF v;
	QAbstractDiagramGraphicsItem* i = item();
	if (i && d->index != -1){
		QDiagramProperty::cast_helper(value(), QDiagramToolkit::Size, &v);
	}
	return v;
}

QString QDiagramProperty::toString() const
{
	QString v;
	QAbstractDiagramGraphicsItem* i = item();
	if (type() == QDiagramToolkit::BrushStyle){
		v = value().toString();
	} else {
		QDiagramProperty::cast_helper(value(), QDiagramToolkit::String, &v);
	}
	return v;
}

Qt::BrushStyle QDiagramProperty::toBrushStyle(const QString & text)
{
    if (text == "none"){
        return Qt::NoBrush;
	} else if (text == "solid"){
        return Qt::SolidPattern;
    } else if (text == "dense1"){
        return Qt::Dense1Pattern;
    } else if (text == "dense2"){
        return Qt::Dense2Pattern;
    } else if (text == "dense3"){
        return Qt::Dense3Pattern;
    } else if (text == "dense4"){
        return Qt::Dense4Pattern;
    } else if (text == "dense5"){
        return Qt::Dense5Pattern;
    } else if (text == "dense6"){
        return Qt::Dense6Pattern;
    } else if (text == "dense7"){
        return Qt::Dense7Pattern;
    } else if (text == "horizontal"){
        return Qt::HorPattern;
    } else if (text == "vertical"){
        return Qt::VerPattern;
    } else if (text == "cross"){
        return Qt::CrossPattern;
    } else if (text == "bdiag"){
        return Qt::BDiagPattern;
    } else if (text == "fdiag"){
        return Qt::FDiagPattern;
    } else if (text == "diagcross"){
        return Qt::DiagCrossPattern;
    } else if (text == "conicalGradient"){
		return Qt::ConicalGradientPattern;
    } else if (text == "linearGradient"){
		return Qt::LinearGradientPattern;
    } else if (text == "radialGradient"){
		return Qt::RadialGradientPattern;
	}
	return Qt::SolidPattern;
}

QString QDiagramProperty::toString(Qt::BrushStyle style)
{
    if (style == Qt::NoBrush){
        return "none";
	} else if (style == Qt::SolidPattern){
        return "solid";
    } else if (style == Qt::Dense1Pattern){
        return "dense1";
    } else if (style == Qt::Dense2Pattern){
        return "dense2";
    } else if (style == Qt::Dense3Pattern){
        return "dense3";
    } else if (style == Qt::Dense4Pattern){
        return "dense4";
    } else if (style == Qt::Dense5Pattern){
        return "dense5";
    } else if (style == Qt::Dense6Pattern){
        return "dense6";
    } else if (style == Qt::Dense7Pattern){
        return "dense7";
    } else if (style == Qt::HorPattern){
        return "horizontal";
    } else if (style == Qt::VerPattern){
        return "vertical";
    } else if (style == Qt::CrossPattern){
        return "cross";
    } else if (style == Qt::BDiagPattern){
        return "bdiag";
    } else if (style == Qt::FDiagPattern){
        return "fdiag";
    } else if (style == Qt::DiagCrossPattern){
        return "diagcross";
	} else if (style == Qt::ConicalGradientPattern){
		return "conicalGradient";
	} else if (style == Qt::LinearGradientPattern){
		return "linearGradient";
	} else if (style == Qt::RadialGradientPattern){
		return "radialGradient";
	}
    return "solid";
}

QString QDiagramProperty::toString(Qt::PenJoinStyle style)
{
	return QString();
}

QString QDiagramProperty::toString(Qt::PenStyle style)
{
	if (style == Qt::DashDotDotLine){
		return "dashdotdot";
	} else if (style == Qt::DashDotLine){
		return "dashdot";
	} else if (style == Qt::DashLine){
		return "dash";
	} else if (style == Qt::SolidLine){
		return "solid";
	}
	return "none";
}

QDiagramToolkit::PropertyType QDiagramProperty::type() const
{
	QAbstractDiagramGraphicsItem* i = item();
	if (i && d->index != -1){
		return i->metaData()->property(d->index).type();
	}
	return QDiagramToolkit::PropertyTypeInvalid;
}

QVariant QDiagramProperty::value() const
{
	if (d->diagram == 0 && d->type != QDiagramToolkit::PropertyTypeInvalid){
		return d->value;
	}
	QAbstractDiagramGraphicsItem* i = item();
	if (i){
		if (d->parent == -1){
			if (d->index != -1){
				return i->propertyValue(d->index);
			}
		} else {
			QDiagramMetaData* m = i->metaData();
			QDiagramMetaProperty p = m->property(d->parent);
			QVariantMap pv = i->propertyValue(d->parent).toMap();
			return pv.value(p.property(d->index).name());
		}
	}
	return QVariant();
}

bool QDiagramProperty::operator==(const QString & other) const
{
    return toString() == other;
}

bool QDiagramProperty::operator!=(const QString & other) const
{
    return toString() != other;
}

QDiagramProperty & QDiagramProperty::operator =( const QDiagramProperty & other )
{
    if (this != &other){
        d = other.d;
    }
    return *this;
}

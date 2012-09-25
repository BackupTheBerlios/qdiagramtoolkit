#include "stdafx.h"
#include "qsysmlaction.h"

QSysMLAction::QSysMLAction(const QMap<QString, QVariant> &properties, QGraphicsItem *parent) :
    QAbstractDiagramShape(properties, parent)
{
    addProperty("actionType", QDiagramGraphicsItemMetaProperty::String, true, properties.value("actionType"));
    addProperty("backgroundColor", QDiagramGraphicsItemMetaProperty::Color, false, QColor("white"));
    addProperty("name", QDiagramGraphicsItemMetaProperty::String, false, "action");

    addSizeGripHandle(new QDiagramShapeSizeGripHandle(QDiagramShapeSizeGripHandle::Left, this));
    addSizeGripHandle(new QDiagramShapeSizeGripHandle(QDiagramShapeSizeGripHandle::Right, this));

    restoreProperties(properties);
}

QRectF QSysMLAction::boundingRect() const
{
    QRectF r = geometry();
    r.moveTo(0, 0);
    return r;
}

void QSysMLAction::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    QColor color = property("backgroundColor").value<QColor>();
    painter->setBrush(QBrush(color, Qt::SolidPattern));

    painter->drawRoundedRect(0, 0, geometry().width(), geometry().height(), 4, 4);
    QRect br = boundingRect().adjusted(-2, -2, -2, -2).toRect();
    painter->drawText(0, 0, geometry().width(), geometry().height(), Qt::AlignCenter | Qt::TextWordWrap, property("name").toString(), &br);
    painter->restore();
    paintFocus(painter, option, widget);
}

QPainterPath QSysMLAction::shape() const
{
    QPainterPath p;
    p.addRoundedRect(0, 0, geometry().width(), geometry().height(), 4, 4);
    return p;
}

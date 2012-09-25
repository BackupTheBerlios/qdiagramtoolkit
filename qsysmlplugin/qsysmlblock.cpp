#include "stdafx.h"
#include "qsysmlblock.h"

QSysMLBlock::QSysMLBlock(const QMap<QString, QVariant> &properties, QGraphicsItem *parent) :
    QAbstractDiagramShape(properties, parent)
{
    addProperty("blockType", QDiagramGraphicsItemMetaProperty::String, true, properties.value("blockType"));
    addProperty("backgroundColor", QDiagramGraphicsItemMetaProperty::Color, false, QColor("white"));
    addProperty("name", QDiagramGraphicsItemMetaProperty::String, false, "action");

    addSizeGripHandle(new QDiagramShapeSizeGripHandle(QDiagramShapeSizeGripHandle::Left, this));
    addSizeGripHandle(new QDiagramShapeSizeGripHandle(QDiagramShapeSizeGripHandle::Right, this));

    restoreProperties(properties);
}

QRectF QSysMLBlock::boundingRect() const
{
    QRectF r = geometry();
    r.moveTo(0, 0);
    return r;
}

void QSysMLBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QColor color = property("backgroundColor").value<QColor>();
    painter->setBrush(QBrush(color, Qt::SolidPattern));
    painter->drawRect(0, 0, geometry().width(), geometry().height());

    QFont font("Arial", 10);
    QFontMetricsF fm(font);
    qreal pixelsWide = fm.width("M");
    qreal pixelsHigh = fm.height();
    // � �
    QRect br = boundingRect().adjusted(-2, -2, -2, -2).toRect();
    QRect r(0, 5, geometry().width(), pixelsHigh);
    painter->drawText(r, Qt::AlignHCenter, "�block�");
    font.setBold(true);
    painter->setFont(font);
    painter->drawText(0, r.height() + fm.leading(), geometry().width(), geometry().height(), Qt::AlignHCenter | Qt::TextWordWrap, property("name").toString(), &br);

    paintFocus(painter, option, widget);
}

QPainterPath QSysMLBlock::shape() const
{
    QPainterPath p;
    p.addRect(0, 0, geometry().width(), geometry().height());
    return p;
}

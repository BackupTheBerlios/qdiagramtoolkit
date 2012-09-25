#include "stdafx.h"
#include "qlogiccircuitflipflopshape.h"

// @MS PMincho

QLogicCircuitFlipFlopShape::QLogicCircuitFlipFlopShape(QGraphicsItem *parent) :
    QAbstractDiagramShape(parent)
{
}

QLogicCircuitFlipFlopShape::QLogicCircuitFlipFlopShape(const QMap<QString,QVariant> & properties, QGraphicsItem* parent) :
    QAbstractDiagramShape(properties, parent)
{
    addProperty("flipflip", QDiagramGraphicsItemMetaProperty::String, true, properties.value("flipflop", "asyncron").toString());
}

QRectF QLogicCircuitFlipFlopShape::boundingRect() const
{
    return QRectF(0, 0, geometry().width(), geometry().height());
}

void QLogicCircuitFlipFlopShape::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setPen(pen());
    painter->setBrush(QBrush(Qt::white, Qt::SolidPattern));
    painter->drawPath(shape());
    QFont font;
    font.setFamily("Arial");
    font.setPixelSize(10);
    painter->setFont(font);

    int offset = geometry().height() / 4;
    if (property("flipflop").toString() == "asyncron"){
        painter->drawText(offset + 4, 4, geometry().width() - offset * 2, geometry().height(), Qt::AlignLeft, "S");
    } else if (property("function").toString() == "computation"){
    }
    paintConnectionPoints(painter, option, widget);
    paintFocus(painter, option, widget);
}

QPainterPath QLogicCircuitFlipFlopShape::shape() const
{
    QPainterPath p;
    int offset = geometry().height() / 4;
    p.addRect(offset, 0, geometry().width() - offset * 2, geometry().height());
    p.moveTo(0, offset);
    p.lineTo(offset, offset);

    p.moveTo(0, geometry().height() - offset);
    p.lineTo(offset, geometry().height() - offset);

    p.moveTo(geometry().width() - offset, offset);
    p.lineTo(geometry().width(), offset);

    p.moveTo(geometry().width() - offset, geometry().height() - offset);
    p.lineTo(geometry().width(), geometry().height() - offset);
    return p;
}

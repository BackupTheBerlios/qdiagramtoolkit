#include "stdafx.h"
#include "qsysmlnode.h"

QSysMLNode::QSysMLNode(const QMap<QString, QVariant> &properties, QGraphicsItem *parent) :
    QAbstractDiagramShape(properties, parent)
{
    addProperty("nodeType", QDiagramGraphicsItemMetaProperty::String, true, properties.value("nodeType"));
    if (nodeType() == "initial"){
//        addConnectionPoint(new QStandardBlockShapeConnectionPoint(this, "c3", QStandardBlockShapeConnectionPoint::Right));
    } else if (property("nodeType") == "fork"){
        QMap<int,QString> pairs;
        pairs[Qt::Horizontal] = "horizontal";
        pairs[Qt::Vertical] = "vertical";
        addProperty("alignment", QDiagramGraphicsItemMetaProperty::Enumeration, pairs, properties.value("alignment", Qt::Horizontal));

        addSizeGripHandle(new QDiagramShapeSizeGripHandle(QDiagramShapeSizeGripHandle::Left, this));
        addSizeGripHandle(new QDiagramShapeSizeGripHandle(QDiagramShapeSizeGripHandle::Right, this));
    }
    restoreProperties(properties);
}

QRectF QSysMLNode::boundingRect() const
{
    QRectF r(geometry());
    r.moveTo(0, 0);
    return r;
}

QString QSysMLNode::nodeType() const
{
    return property("nodeType").toString();
}

void QSysMLNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->save();
    if (nodeType() == "decision"){
        painter->drawPath(shape());
    } else if (property("nodeType") == "initial"){
        QBrush b(Qt::black);
        painter->setBrush(b);
        painter->drawEllipse(13, 13, 13, 13);
    } else if (property("nodeType") == "final.activity"){
        painter->drawEllipse(13, 13, 13, 13);
        QBrush b(Qt::black);
        painter->setBrush(b);
        painter->drawEllipse(16, 16, 7, 7);
    } else if (property("nodeType") == "fork"){
        QBrush b(Qt::black);
        painter->setBrush(b);
        painter->drawRect(0, geometry().height() / 2 - 2, geometry().width(), 4);
    } else if (property("nodeType") == "fork"){
        QBrush b(Qt::black);
        painter->setBrush(b);
        painter->drawRect(0, geometry().height() / 2 - 2, geometry().width(), 4);
    }
    painter->restore();

    paintFocus(painter, option, widget);
}

QPainterPath QSysMLNode::shape() const
{
    QPainterPath p;
    if (nodeType() == "decision"){
        p.moveTo(geometry().width() / 2, geometry().height() / 4);
        p.lineTo(geometry().width() / 4, geometry().height() / 2);
        p.lineTo(geometry().width() / 2, geometry().height() / 4 * 3);
        p.lineTo(geometry().width() / 4 * 3, geometry().height() / 2);
        p.lineTo(geometry().width() / 2, geometry().height() / 4);
    } else if (property("nodeType") == "initial"){
        p.addEllipse(13, 13, 13, 13);
    } else if (property("nodeType") == "final.activity"){
        p.addEllipse(13, 13, 13, 13);
    } else if (property("nodeType") == "fork"){
        if (property("alignment").toInt() == Qt::Horizontal){
            p.addRect(0, geometry().height() / 2 - 2, geometry().width(), 4);
        }
    }

    return p;
}

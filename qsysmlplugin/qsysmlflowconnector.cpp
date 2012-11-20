#include "stdafx.h"
#include "qsysmlflowconnector.h"
#include "qsysmlplugin.h"

#include <qdiagramtoolkit.h>

QSysMLFlowConnector::QSysMLFlowConnector(const QString & itemClass, const QVariantMap & properties) :
    QAbstractDiagramShapeConnector(QSysMLPlugin::staticName(), itemClass, properties)
{
}

QSysMLFlowConnector::~QSysMLFlowConnector()
{
}

QRectF QSysMLFlowConnector::focusRect() const
{
	return boundingRect();
}

QLineF QSysMLFlowConnector::line() const
{
	QLineF l;
	if (startConnectionPoint() && startConnectionPoint()->orientation() == QDiagramToolkit::West){
		l.setP1(QPointF(startPos().x()- startConnectionPoint()->boundingRect().center().x(), startPos().y()));
	} else if (startConnectionPoint() && startConnectionPoint()->orientation() == QDiagramToolkit::South){
		l.setP1(QPointF(startPos().x(), startPos().y() + startConnectionPoint()->boundingRect().center().y()));
	} else {
		l.setP1(startPos());
	}
	if (endConnectionPoint() && endConnectionPoint()->orientation() == QDiagramToolkit::North){
		l.setP2(QPointF(endPos().x(), endPos().y() - endConnectionPoint()->boundingRect().height()));
	} else {
		l.setP2(endPos());
	}
	return l;
}

void QSysMLFlowConnector::paintArrow(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	if (startConnectionPoint() && startConnectionPoint()->id().left(9) == "controlIn"){
	}
}

void QSysMLFlowConnector::paintFocus(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->save();
    if (isSelected()){
        painter->setBrush(Qt::NoBrush);
        QPen p(selectionColor());
		p.setStyle(Qt::DashDotLine);
		p.setWidthF(1);
        painter->setPen(p);
        painter->drawRect(focusRect());
    }
    painter->restore();
}

qreal QSysMLFlowConnector::radius() const
{
	int r = 20;
	if (abs(line().dy()) < 20 || abs(line().dx()) < 20){
		if (abs(line().dy()) < abs(line().dx())){
			r = abs(line().dy());
		} else {
			r = abs(line().dx());
		}
	}
	return r;
}

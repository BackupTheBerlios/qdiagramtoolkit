#ifndef QSYSMLFLOWCONNECTOR_H
#define QSYSMLFLOWCONNECTOR_H

#include <qabstractdiagramshapeconnector.h>

class QSysMLFlowConnector : public QAbstractDiagramShapeConnector
{
public:
	QSysMLFlowConnector(const QString &itemClass, const QVariantMap & properties);
	~QSysMLFlowConnector();

	QRectF focusRect() const;

	QLineF line() const;

	qreal radius() const;
protected:
	void paintArrow(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
	
	void paintFocus(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
};

#endif // QSYSMLFLOWCONNECTOR_H

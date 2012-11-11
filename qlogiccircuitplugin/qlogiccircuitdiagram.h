#ifndef QLOGICCIRCUITDIAGRAM_H
#define QLOGICCIRCUITDIAGRAM_H

#include <qdiagram.h>

class QAbstractDiagramShape;

class QLogicCircuitDiagram : public QDiagram
{
	Q_OBJECT

public:
	QLogicCircuitDiagram(QObject *parent);
	~QLogicCircuitDiagram();

    QString plugin() const;

    QString type() const;

	QString toText() const;
private slots:
	void render();
private:
	QString toText(QAbstractDiagramShape* i) const;
};

#endif // QLOGICCIRCUITDIAGRAM_H

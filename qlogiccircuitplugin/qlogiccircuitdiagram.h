#ifndef QLOGICCIRCUITDIAGRAM_H
#define QLOGICCIRCUITDIAGRAM_H

#include <qdiagram.h>

class QLogicCircuitDiagram : public QDiagram
{
	Q_OBJECT

public:
	QLogicCircuitDiagram(QObject *parent);
	~QLogicCircuitDiagram();

    QString plugin() const;

    QString type() const;
private:
	
};

#endif // QLOGICCIRCUITDIAGRAM_H
